/**
 * @file main.c
 * @brief Programme : calcul du meilleur ordre et génération des commandes.
 *
 * Ce programme :
 * - charge la ville depuis un fichier,
 * - choisit un ordre optimisé de visite des points bip,
 * - calcule les chemins entre objectifs,
 * - traduit les chemins en ordres élémentaires (AV, TG, TD),
 * - écrit les ordres sur stdout (utilisé par parcours.c via pipe).
 */
#include <stdio.h>
#include <string.h>
#include "Ville.h"
#include "Chemin.h"
#include "pilote.h"
#include "direction.h"
/**
 * @brief Point d’entrée du programme calcul.
 *
 * Charge la ville, calcule l’ordre optimal, génère la liste de commandes
 * et les envoie au programme robot via la sortie standard.
 *
 * @return 0 si succès, autre valeur en cas d’erreur.
 */
int main(int argc, char *argv[]){

    char *nom_fichier = "Ville.txt"; 
    Ville ville;
    if(argc > 1){
        nom_fichier = argv[1]; 
    }

    if (charger_ville(nom_fichier, &ville) == 0){
        fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
        return 1;
    } 
    
    // 1. Calcul de l'ordre optimal des Bips
    int depart = ville.position_initiale;
    int Points_Bip_optimale[10];
    optimiser_ordre(&ville, depart, Points_Bip_optimale); // Le retour int n'est pas critique ici
    
    int obj1 = Points_Bip_optimale[0];
    int obj2 = Points_Bip_optimale[1];
    int obj3 = Points_Bip_optimale[2];

    // 2. Calcul des segments (Comme avant)
    int chemin1[Max_Cases], chemin2[Max_Cases], chemin3[Max_Cases], chemin4[Max_Cases];
    
    // Note: On passe -1 ou la contrainte précédente pour éviter les demi-tours immédiats
    int nb1 = chercher_chemin(&ville, depart, obj1, -1, chemin1);
    int nb2 = chercher_chemin(&ville, obj1, obj2, chemin1[nb1-2], chemin2); 
    int nb3 = chercher_chemin(&ville, obj2, obj3, chemin2[nb2-2], chemin3);
    int nb4 = chercher_chemin(&ville, obj3, depart, chemin3[nb3-2], chemin4);

    // 3. FUSION DES CHEMINS (La solution propre à votre problème)
    int grand_chemin[2000];
    int total = 0;

    // Copie du chemin 1 complet
    for(int i = 0; i < nb1; i++) {
        grand_chemin[total++] = chemin1[i];
    }
    // Copie du chemin 2 (On saute la première case car c'est la même que la fin du chemin 1)
    for(int i = 1; i < nb2; i++) {
        grand_chemin[total++] = chemin2[i];
    }
    // Copie du chemin 3 (On saute la première case)
    for(int i = 1; i < nb3; i++) {
        grand_chemin[total++] = chemin3[i];
    }
    // Copie du chemin 4 (On saute la première case)
    for(int i = 1; i < nb4; i++) {
        grand_chemin[total++] = chemin4[i];
    }

    // 4. Traduction en ordres (Une seule fois sur le trajet total)
    // Comme le chemin est continu, la fonction va "sauter" les Bips qui sont en ligne droite
    // et générer un seul "AV" au lieu de "AV, AV".
    TypeOrdre liste_globale[2000];
    int compteur_global = 0;
    direction dir_init = char_vers_dir(ville.direction_initiale);

    traduction_des_ordres(grand_chemin, dir_init, total, &ville, liste_globale, &compteur_global);

    // 5. Affichage final formaté pour le robot
    printf("{");
    for(int i = 0; i < compteur_global; i++){
        switch(liste_globale[i]){
            case O_AV: printf("\"AV\""); break;
            case O_TD: printf("\"TD\""); break;
            case O_TG: printf("\"TG\""); break;
        }
        if(i < compteur_global - 1) printf(",");
    }
    printf("}\n");


    return 0;
}