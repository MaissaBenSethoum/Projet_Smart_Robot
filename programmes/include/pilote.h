/**
 * @file pilote.h
 * @brief Fonctions de pilotage logique du robot.
 *
 * Ce fichier contient les fonctions permettant :
 * - de détecter si une case correspond à une intersection,
 * - d’optimiser l’ordre de visite des objectifs,
 * - de traduire un chemin en une suite d’ordres exploitables par le robot.
 */
#ifndef PILOTE_H
#define PILOTE_H

#include "direction.h" // C'est lui qui contient la définition de TypeOrdre
#include "Ville.h"

/**
 * @brief Calcule un ordre optimisé de visite des objectifs(points bip).
 *
 * À partir d’une position de départ, cette fonction détermine un ordre
 * optimisé de visite des objectifs.
 *
 * @param ville Pointeur vers la structure Ville.
 * @param depart Identifiant de la case de départ.
 * @param resultat Tableau de sortie contenant l’ordre des 3 cases optimisé (3 éléments).
 * @return Taille du parcours optimisé.
 */
int optimiser_ordre(Ville* ville, int depart, int resultat[3]);

/**
 * @brief Traduit un chemin en une suite d’ordres pour le robot.
 *
 * À partir d’un chemin représenté par une liste d’identifiants de cases,
 * cette fonction génère une suite d’ordres élémentaires (AV, TG, TD).
 *
 * La traduction dépend de la direction d’entrée du robot sur le chemin
 * et met à jour la direction finale après exécution.
 *
 * @param chemins Tableau contenant les identifiants des cases du chemin.
 * @param direction_entre Direction du robot au début du chemin.
 * @param longueur_chemin Nombre de cases dans le chemin.
 * @param ville Pointeur vers la structure Ville.
 * @param tab_ordres Tableau de sortie contenant les ordres générés.
 * @param nb_ordres Pointeur vers le nombre d’ordres générés.
 * @return Direction finale du robot après exécution du chemin.
 */
direction traduction_des_ordres(int* chemins, direction direction_entre, int longueur_chemin, Ville* ville, TypeOrdre* tab_ordres, int* nb_ordres);

/**
 * @brief Indique si une case correspond à une intersection.
 *
 * Une case est considérée comme une intersection lorsqu’elle a plus de 2 voisins.
 *
 * @param c Pointeur vers la case.
 * @param ville Pointeur vers la structure Ville.
 * @return 1 si la case est une intersection, 0 sinon.
 */
int est_intersection(Case* c, Ville *ville);


#endif
