/**
 * @file Ville.h
 * @brief Définition des structures et fonctions liées à la ville (graphe).
 *
 * Ce fichier définit les TAD représentant une ville
 * sous forme de cases connectées (graphe) et fournit des fonctions pour :
 * - charger une ville depuis un fichier texte,
 * - ajouter des connexions entre cases,
 */

#ifndef VILLE_H
#define VILLE_H

#include "direction.h"

#define Max_Voisins 4   /**< Nombre maximal de voisins par case. */
#define Max_Cases 100   /**< Nombre maximal de cases dans la ville. */

/**
 * @struct Case
 * @brief Représente une case de la ville.
 *
 * Une case possède :
 * - un identifiant (id),
 * - une liste de voisins (connexions),
 * - un indicateur "bip" indiquant si la case contient une pastille verte.
 */
typedef struct {
    int id;                         /**< Numéro (identifiant) de la case. */
    int voisins[Max_Voisins];       /**< Tableau contenant les identifiants des voisins. */
    int nb_voisins;                 /**< Nombre de voisins. */
    int est_bip;                    /**< 1 si la case contient une pastille verte, 0 sinon. */
} Case;

/**
 * @struct Ville
 * @brief Représente la ville.
 *
 * La ville est stockée dans un tableau de cases (grille).
 */
typedef struct {
    int largeur;                    /**< Largeur de la ville. */
    int nb_total_case;              /**< Nombre total de cases . */

    int position_initiale;          /**< Position initiale du robot. */
    char direction_initiale;        /**< Direction initiale du robot. */

    Case grille[Max_Cases];         /**< Liste des cases de la ville. */

    int Points_Bip[Max_Cases];      /**< Liste des cases contenant une pastille verte. */
    int nb_points_bip;              /**< Nombre de points bip. */
} Ville;

/**
 * @brief Ajoute une connexion entre deux cases.
 *
 * Ajoute id2 dans la liste de voisins de id1, et id1 dans la liste de voisins de id2.
 *
 * @param ville Pointeur vers la ville à modifier.
 * @param id1 Identifiant de la première case .
 * @param id2 Identifiant de la seconde case.
 */
void ajouter_connexion(Ville *ville, int id1, int id2);

/**
 * @brief Charge une ville depuis un fichier texte.
 *
 * Le fichier contient:
 * - la taille N,
 * - la position initiale,
 * - la direction initiale,
 * - puis des lignes décrivant soit des connexions "id1-id2", soit des points bip.
 *
 * La structure maville est initialisée à zéro avant remplissage.
 *
 * @param nom_fichier nom du fichier texte qui décrit la ville.
 * @param maville Pointeur vers la structure Ville à remplir.
 * @return 1 si le chargement a réussi, 0 sinon.
 */
int charger_ville(const char* nom_fichier, Ville* maville);

/**
 * @brief Affiche un résumé de la ville .
 *
 * Affiche la taille, le nombre de cases, la position/direction initiale du robot,
 * toutes les connexions et la liste des points bip.
 *
 * @param ville Pointeur vers la ville à afficher.
 */
void afficher_resume_ville(Ville* ville);

#endif /* VILLE_H */