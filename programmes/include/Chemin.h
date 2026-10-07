/**
 * @file Chemin.h
 * @brief Déclarations des fonctions de recherche de chemin.
 *
 * Ce fichier définit la fonction qui permet trouver le plus court chemin et 
 * retourne une liste d'identifiants de cases
 */
#ifndef CHEMIN_H
#define CHEMIN_H


#include "Ville.h"


void inverser_tableau(int* tableau,int taille);
/**
 * @brief Cherche un chemin entre deux cases dans la ville (BFS).
 *
 * @param maville Pointeur vers la structure Ville
 * @param depart Identifiant de la case de départ
 * @param arrive Identifiant de la case d'arrivée
 * @param id_interdit Identifiant de la case à ignorer (ou -1 si aucune)
 * @param chemin Tableau de sortie contenant le chemin
 * @return Longueur du chemin si trouvé, 0 sinon
 */
int chercher_chemin(Ville* maville, int depart, int arrive, int id_dejavu,int* chemin);


#endif /* CHEMIN_H */