/**
 * @file Ville.c
 * @brief Implémentation des fonctions de gestion de la ville (graphe).
 *
 * Ce fichier implémente :
 * - l'ajout de connexions entre cases,
 * - le chargement d'une ville depuis un fichier texte,
 * - l'affichage d'un résumé de la ville.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Ville.h"

/**
 * @brief Ajoute une connexion entre deux cases.
 *
 * Cette fonction met à jour la liste des voisins des deux cases.
 *
 * @param ville Pointeur vers la ville.
 * @param id1 Identifiant de la case 1.
 * @param id2 Identifiant de la case 2.
 */
void ajouter_connexion(Ville *ville, int id1 ,int id2 ){
    Case *c1 = &ville->grille[id1-1];
    Case *c2 = &ville->grille[id2-1];

    c1->id = id1;
    c2->id = id2;


    // Ajout de la connexion 1-2 (exemple)
    if (c1->nb_voisins < Max_Voisins){
        c1->voisins[c1->nb_voisins] = id2;
        c1->nb_voisins++;
    }
    // Ajout de la connexion 2-1 (exemple)
    if (c2->nb_voisins < Max_Voisins){
        c2->voisins[c2->nb_voisins] = id1;
        c2->nb_voisins++;
    }

}
/**
 * @brief Charge une ville depuis un fichier texte.
 *
 * @param nom_fichier nom du fichier de description de la ville.
 * @param maville Structure Ville à remplir.
 * @return 1 si le chargement a réussi, 0 sinon.
 */
int charger_ville(const char* nom_fichier, Ville* maville){
    FILE *f = fopen(nom_fichier,"r");
    if (f == NULL){
        fprintf(stderr,"Erreur : Impossible de lire le fichier. \n");
        return 0;
    }

    
    char ligne[256];
    int taille;    
    char Direction_initiale;
    int Position_initiale;
    int Point_Bip;
    // Initialisation de la ville
    // on mets tout a zero grace a la fontion memset
    memset(maville,0,sizeof(Ville));



    // Lecture et Stockage de la taille de la ville d'aprés le fichier texte.
    if (fscanf(f,"%d",&taille) == 1){
        if (taille * taille > Max_Cases){
            fprintf(stderr, "Erreur : ville trop grande (%d cases max)\n", Max_Cases);
            fclose(f);
            return 0;
        }
        maville->largeur = taille;
        maville->nb_total_case = taille*taille;
    }else{
        printf("Erreur \n");
    }

    // Lecture et Stockage de la position iniitale d'aprés le fichier texte.
    if (fscanf(f,"%d",&Position_initiale) == 1){
        maville->position_initiale = Position_initiale;

    }else{
        printf("Erreur \n");
    }

    // Lecture et Stockage de la direction iniitale d'aprés le fichier texte.

        if (fscanf(f,"%c",&Direction_initiale) == 1){
        maville->direction_initiale = Direction_initiale;
    }else{
        printf("Erreur \n");
    }

    int i = 0;

    while (fgets(ligne,sizeof(ligne),f)!=NULL){
        // printf( "%s\n",ligne);
        
        if (strchr(ligne, '-') == NULL){ // Pas de tiret dans une ligne

            if (sscanf(ligne,"%d",&Point_Bip) == 1){
                maville->grille[Point_Bip -1].est_bip = 1; // On marque que il faut bipper a cette case
                maville->Points_Bip[i] = Point_Bip;

                maville->nb_points_bip++;
                i++;
                
            }
        }else{
            if (strchr(ligne, '-') != NULL) {
                int id1, id2;
                sscanf(ligne, "%d-%d", &id1, &id2);
                ajouter_connexion(maville,id1,id2);
}
        }

    }


    fclose(f);


    return 1;
}
/**
 * @brief Affiche un résumé de la ville.
 *
 * Affiche :
 * - la largeur et le nombre total de cases,
 * - la position/direction initiale du robot,
 * - les connexions entre cases,
 * - la liste des points bip.
 *
 * @param ville Pointeur vers la ville à afficher.
 */
void afficher_resume_ville(Ville* ville){
    printf("Largeur de la ville %d\n",ville->largeur);
    printf("Nombre totale de cases %d\n",ville->nb_total_case);
    printf("Robot départ : %d %c\n",ville->position_initiale,ville->direction_initiale);


    for (int i = 0; i<ville->nb_total_case;i++){
        for (int j = 0; j<ville->grille[i].nb_voisins ;j++){
            printf ("La case %d est connectée à la case %d\n",ville->grille[i].id,ville->grille[i].voisins[j]);
        }
    }

    for (int i=0;i<ville->nb_points_bip;i++){
        printf("Points Bip : %d\n",ville->Points_Bip[i]);
    }
    
}
