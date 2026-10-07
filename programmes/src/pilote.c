/**
 * @file pilote.c
 * @brief Implémentation des fonctions de pilotage logique du robot.
 *
 * Ce module implémente la logique de décision du robot :
 * - détection des intersections,
 * - optimisation de l’ordre de visite des objectifs,
 * - traduction d’un chemin en ordres élémentaires (AV, TG, TD).
 *
 */
#include "direction.h"
#include "Ville.h"
#include <stdio.h>
#include "Chemin.h"


/**
 * @brief Détermine si une case est une intersection.
 *
 * Une case est considérée comme une intersection lorsqu’elle a plus que de 2 voisins.
 *
 * @param c Pointeur vers la case à analyser. 
 * @param ville Pointeur vers la structure Ville.
 * @return 1 si la case est une intersection, 0 sinon.
 */
int est_intersection(Case* c, Ville *ville){
    (void)ville;
    if (c->nb_voisins > 2 ){        
        return 1;
    }else{
        return 0;
    }
}



/**
 * @brief Traduit un chemin en une suite d’ordres.
 *
 * À partir d’un chemin (liste d’identifiants de cases), cette fonction génère
 * des ordres de type AV/TG/TD stockés dans tab_ordres.
 * La génération tient compte de la direction d’entrée, met à jour la direction
 * courante et applique une optimisation : enchaîner les AV tant qu’il n’y a pas
 * d’intersection et que la direction reste la même.
 *
 * @param chemins Tableau contenant les ids des cases du chemin.
 * @param direction_entre Direction du robot au début du chemin.
 * @param longueur_chemin Nombre de cases dans le chemin.
 * @param ville Pointeur vers la structure Ville.
 * @param tab_ordres Tableau de sortie contenant les ordres générés.
 * @param nb_ordres Pointeur vers le compteur du nombre d’ordres ajoutés.
 * @return Direction finale du robot après traduction du chemin.
 */
direction traduction_des_ordres(int* chemins, direction direction_entre, int longueur_chemin, Ville* ville, TypeOrdre* tab_ordres, int* nb_ordres){
    Case* case_actuelle;
    Case* case_prochaine;
    direction direction_souhaite;
    direction direction_actuelle = direction_entre;

    int i = 0;
    while (i < longueur_chemin - 1){
        case_actuelle = &ville->grille[chemins[i]-1];
        case_prochaine = &ville->grille[chemins[i+1]-1];

        direction_souhaite = nouvelle_direction(case_prochaine->id, case_actuelle->id, ville->largeur);
        
        // --- Gestion des virages ---
        if (direction_souhaite != direction_actuelle){
            // Virage à DROITE
            if ((direction_actuelle== EST && direction_souhaite==SUD) ||
                (direction_actuelle==NORD && direction_souhaite==EST) ||
                (direction_actuelle== OUEST && direction_souhaite==NORD) ||
                (direction_actuelle== SUD && direction_souhaite==OUEST)){
                
                // STOCKAGE AU LIEU DE PRINTF
                tab_ordres[*nb_ordres] = O_TD;
                (*nb_ordres)++;

                // Mise à jour direction
                if (direction_actuelle == NORD) direction_actuelle = EST;
                else if (direction_actuelle == EST) direction_actuelle = SUD;
                else if (direction_actuelle == SUD) direction_actuelle = OUEST;
                else if (direction_actuelle == OUEST) direction_actuelle = NORD;
            }

            // Virage à GAUCHE
            if ((direction_actuelle== NORD && direction_souhaite==OUEST) ||
                (direction_actuelle== OUEST && direction_souhaite==SUD) ||
                (direction_actuelle== SUD && direction_souhaite==EST) ||
                (direction_actuelle== EST && direction_souhaite==NORD)){
                
                // STOCKAGE AU LIEU DE PRINTF
                tab_ordres[*nb_ordres] = O_TG;
                (*nb_ordres)++;

                // Mise à jour direction
                if (direction_actuelle == NORD) direction_actuelle = OUEST;
                else if (direction_actuelle == OUEST) direction_actuelle = SUD;
                else if (direction_actuelle == SUD) direction_actuelle = EST;
                else if (direction_actuelle == EST) direction_actuelle = NORD;
            }
        }

        // --- Avancer ---
        // STOCKAGE AU LIEU DE PRINTF
        tab_ordres[*nb_ordres] = O_AV;
        (*nb_ordres)++;

        // --- Optimisation du saut (avancer tant qu'il n'y a pas d'intersection) ---
        int j = i + 1;
        direction direction_du_saut = direction_souhaite;

        while(j < longueur_chemin - 1){
            Case* case_test = &ville->grille[chemins[j]-1]; 
            if (est_intersection(case_test, ville)){
                break; 
            }
            Case* case_suivante = &ville->grille[chemins[j+1]-1];
            direction dir_suivante = nouvelle_direction(case_suivante->id, case_test->id, ville->largeur);
            
            if (dir_suivante != direction_du_saut) {
                break; 
            }
            j++;
        }
        i = j;
    }
    return direction_actuelle;
}

/**
 * @brief Calcule un ordre optimisé de visite des points bip.
 *
 * Teste toutes les permutations des points bip (3 objectifs) et conserve
 * celle qui minimise la taille totale du parcours :
 * depart -> point1 -> point2 -> point3 -> depart.
 *
 * La fonction utilise `chercher_chemin()` et une contrainte `id_interdit`
 * pour éviter un demi-tour entre deux étapes.s
 * @param ville Pointeur vers la structure Ville.
 * @param depart Identifiant de la case de départ.
 * @param resultat Tableau de sortie contenant l’ordre optimal (3 ids).
 * @return Taille du parcours.
 */
int optimiser_ordre(Ville* ville, int depart, int resultat[3]){

    int point1, point2, point3;
    int min_etapes = 100000; // Augmenté pour être sûr
    int chemin_temp[Max_Cases];

    resultat[0] = ville->Points_Bip[0];
    resultat[1] = ville->Points_Bip[1];
    resultat[2] = ville->Points_Bip[2];

    for (int i = 0; i < ville->nb_points_bip; i++){
        for (int j = 0; j < ville->nb_points_bip; j++){
            if (j == i) continue;
            
            for (int k = 0; k < ville->nb_points_bip; k++){
                if ((k == j) || (k == i)) continue;

                point1 = ville->Points_Bip[i];
                point2 = ville->Points_Bip[j];
                point3 = ville->Points_Bip[k];
                
                int contrainte = -1;
                int possible = 1;

                int nb1 = chercher_chemin(ville, depart, point1, contrainte, chemin_temp);
                if (nb1 == 0) possible = 0; 
                else contrainte = chemin_temp[nb1-2]; 


                int nb2 = 0;
                if (possible) {
                    nb2 = chercher_chemin(ville, point1, point2, contrainte, chemin_temp);
                    if (nb2 == 0) possible = 0;
                    else contrainte = chemin_temp[nb2-2];
                }


                int nb3 = 0;
                if (possible) {
                    nb3 = chercher_chemin(ville, point2, point3, contrainte, chemin_temp);
                    if (nb3 == 0) possible = 0;
                    else contrainte = chemin_temp[nb3-2];
                }



                int nb4 = 0;
                if (possible) {
                    nb4 = chercher_chemin(ville, point3, depart, -1, chemin_temp); 
                    if (nb4 == 0) possible = 0;
                }


                if (possible) {
                    int totale = nb1 + nb2 + nb3 + nb4;
                    if (totale < min_etapes){
                        min_etapes = totale;
                        resultat[0] = point1;
                        resultat[1] = point2;
                        resultat[2] = point3;
                    }
                }
            }
        }
    }
    return min_etapes;
}