/**
 * @file thread.h
 * @brief Détection de couleur en tâche de fond (thread) + fonctions de décision.
 *
 * Ce module gère :
 * - l'initialisation du capteur couleur (TCS34725),
 * - le lancement d'un thread de détection,
 * - des fonctions d'analyse des mesures (couleur détectée, rouge).
 */
#ifndef DETECTION_COULEUR_H
#define DETECTION_COULEUR_H

#include "tcs.h"
/**
 * @brief Initialise la détection de couleur et lance le thread associé.
 *
 * Initialise WiringPi (buzzer), ouvre le capteur TCS34725, initialise le LCD,
 * puis crée un thread qui lit régulièrement les données et met à jour l'affichage.
 *
 * @return 0 si succès, -1 en cas d'erreur.
 */
void initDetectionCouleur(void);
/**
 * @brief Détermine une couleur (texte) à partir des mesures brutes du capteur.
 *
 * Utilise les valeurs R/G/B/C pour estimer la couleur dominante et renvoie
 * une chaîne constante (ex : "Rouge", "Vert", etc.).
 *
 * @param d Mesures brutes (Clear/R/G/B).
 * @return Chaîne constante décrivant la couleur détectée.
 */
const char* detecter_couleur(TCS34725_Data d);
/**
 * @brief Détecte si la couleur rouge est présente.
 *
 * Critère basé sur les mesures brutes : renvoie 1 si rouge détecté.
 *
 * @param d Mesures brutes (Clear/R/G/B).
 * @return 1 si rouge détecté, 0 sinon.
 */
int detecter_rouge(TCS34725_Data d);

#endif
