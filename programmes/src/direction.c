/**
 * @file direction.c
 * @brief Gestion des directions du robot.
 *
 * Ce fichier fournit des fonctions permettant de :
 * - convertir des caractères en directions internes,
 * - calculer la nouvelle direction ,
 * - déterminer la direction opposée.
 */
#include "direction.h"

/**
 * @brief Retourne la direction opposée.
 *
 * @param dir Une direction .
 * @return Direction opposée.
 */
direction direction_oppose(direction courante){
    switch(courante) {
        case NORD :
            return SUD ;
        case SUD :
            return NORD ;
        case EST :
            return OUEST ;
        case OUEST :
            return EST ;  
        default:
            return courante;          
    }
}
/**
 * @brief Convertit un caractère en direction.
 *
 * Les caractères possibles sont :
 * - 'N', 'E', 'S', 'O' (Nord, Est, Sud, Ouest).
 *
 * @param dir Caractère représentant la direction.
 * @return Direction.
 */
direction char_vers_dir(char dir){
    if (dir == 'N'){
        return NORD;
    }else if( dir == 'S'){
        return SUD;
    }else if (dir == 'O'){
        return OUEST;
    }else if(dir == 'E'){
        return EST;
    } return NORD; // pour eviter le warning
}

/**
 * @brief Calcule la nouvelle direction.
 *
 * @param id_arrive L'identifiant de la case de départ.
 * @param id_depart L'identfiant de la case d'arrivée
 * @param largeur_ville Largeur de lvaille
 * @return direction.
 */
direction nouvelle_direction(int id_arrivee, int id_depart, int largeur_ville) {
    int diff = id_arrivee - id_depart;
    if (diff == 1) return EST;
    if (diff == -1) return OUEST;
    if (diff == largeur_ville) return SUD;
    if (diff == -largeur_ville) return NORD;
    return NORD; // Valeur par défaut
}