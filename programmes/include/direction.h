
/**
 * @file direction.h
 * @brief Déclarations liées aux directions du robot.
 */
#ifndef DIRECTION_H
#define DIRECTION_H


/**
 * @enum Direction
 * @brief Directions  du robot.
 */
typedef enum {
    NORD,
    SUD,
    EST,
    OUEST
} direction;

// Définition des types d'ordres possibles
typedef enum { O_AV, O_TD, O_TG } TypeOrdre;

direction direction_oppose(direction courante) ;

direction nouvelle_direction(int id_arrivee, int id_depart, int largeur_ville);

direction char_vers_dir(char direction);

#endif /* DIRECTION_H */
