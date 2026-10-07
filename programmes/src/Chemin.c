/**
 * @file Chemin.c
 * @brief Calcul et reconstruction d’un chemin entre deux cases.
 *
 * Ce fichier contient une recherche de parcours en largeur
 * dans la ville représentée comme un graphe.
 *
 * La fonction principale `chercher_chemin()` calcule un chemin depuis une case
 * de départ vers une case d'arrivée, en pouvant ignorer une case interdite (pour éviter le demi tour).
 * Elle reconstruit ensuite le chemin final dans un tableau d’identifiants de cases.
 */
#include "Chemin.h"
#include "Ville.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief Inverse un Tableau d'entiers.
 *
 * Utilisé pour remettre le chemin reconstruit dans le bon ordre
 * (car la reconstruction se fait dans l'ordre inverse).
 *
 * @param tableau Tableau à inverser
 * @param taille taille du Tableau
 */
void inverser_tableau(int* tableau,int taille){
    int debut = 0;
    int fin = taille-1;
    while(debut<fin){
        int temp = tableau[debut];
        tableau[debut] = tableau[fin];
        tableau[fin] = temp;
        debut++;
        fin = fin -1;
    }

}

/**
 * @brief Cherche un chemin entre deux cases dans la ville.
 *
 * Effectue un parcours en largeur (BFS) depuis `depart` jusqu’à `arrive`.
 * La recherche ignore  une case `id_interdit` (pour éviter de faire demi tour).
 *
 * Si un chemin est trouvé, il est reconstruit à l'aide du tableau des prédécesseurs,
 *
 * @param maville Pointeur vers la structure Ville 
 * @param depart Identifiant de la case de départ 
 * @param arrive Identifiant de la case d'arrivée 
 * @param id_interdit Identifiant d’une case à ignorer (ou -1 si aucune)
 * @param chemin Tableau de sortie contenant le chemin (suite d’ids de cases)
 * @return Longueur du chemin (nombre de cases) si trouvé, 0 sinon
 */
int chercher_chemin(Ville* maville, int depart, int arrive, int id_interdit,int* chemin){
    int file[Max_Cases];
    int queue = 0 ;
    int tete=0 ;
    int trouve = 0;
    int indice_depart;
    int indice_arrive;

    int predecesseurs[Max_Cases];
    int deja_vu[Max_Cases];

    // initialisation des valeurs dans les listes 
    for(int i = 0; i<Max_Cases;i++){
        predecesseurs[i]=-1;
        deja_vu[i]=0;
    }

    indice_arrive = arrive-1;
    indice_depart = depart -1;

    file[queue] = indice_depart; // on enfile 
    queue++;

    while(tete<queue){ // pour parcourir toutes les cases
        int actuel = file[tete];
        tete++;
        if(actuel==indice_arrive){
            trouve = 1;
            break;
        }

        Case* c =&maville->grille[actuel];

        for(int i = 0;i< c->nb_voisins;i++){

            int id_voisin = c->voisins[i];
            if (id_voisin == id_interdit){
                continue; // quand le programme rencontre cette instruction, il passe à la prochaine itération ( on saute la case indiqué)
            }

            int indice_voisin = id_voisin - 1;

            if (deja_vu[indice_voisin]==0){
                deja_vu[indice_voisin]=1;
                predecesseurs[indice_voisin]= actuel;
                file[queue]=indice_voisin;
                queue++;
            }
        }

    }

    if (!trouve){ // pas de chemin trouvé
        return 0;
    }

    int longueur = 0;
    int courant = indice_arrive;

    while(courant!=-1){  // tant que on a pas trouvé le points de départ
        chemin[longueur] = maville->grille[courant].id;
        longueur++;
        if (courant == indice_depart){
            break;
        }
        courant = predecesseurs[courant];

    } 

    inverser_tableau(chemin,longueur);

    return longueur;
}