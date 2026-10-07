/**
 * @file tcs.h
 * @brief fichier du capteur couleur TCS34725 (I2C).
 *
 * Ce fichier fournit une interface simple pour :
 * - initialiser le capteur sur un bus I2C,
 * - activer/configurer le capteur,
 * - lire les mesures brutes (Clear, R, G, B),
 * - fermer la connexion.
 */#ifndef TCS34725_H
#define TCS34725_H

#include <stdint.h>

/** @name Constantes TCS34725 */
/**@{*/
/** Adresse I2C par défaut du capteur. */
#define TCS34725_ADDRESS 0x29
/** Bit de commande avec l’adresse de registre. */
#define TCS34725_COMMAND_BIT 0x80
/**@}*/

/**
 * @struct TCS34725_Data
 * @brief Mesures brutes renvoyées par le capteur.
 */
typedef struct {
    uint16_t r; // Rouge
    uint16_t g; // Vert
    uint16_t b; // Bleu
    uint16_t c; // Clear (Luminosité globale)
} TCS34725_Data;

// Prototypes des fonctions
// Initialise le bus I2C et le capteur. Retourne le file descriptor (ou -1 si erreur)
/**
 * @brief Initialise le bus I2C et le capteur TCS34725.
 *
 * Ouvre /dev/i2c-<bus_number>, sélectionne l’adresse du capteur,
 * puis appelle l’activation/configuration de base.
 *
 * @param bus_number Numéro du bus I2C.
 * @return Descripteur de fichier I2C (>=0) en cas de succès, -1 sinon.
 */
int tcs34725_init(int bus_number);

/**
 * @brief Active le capteur (Power ON) et configure les paramètres.
 *
 * @param fd Descripteur de fichier I2C retourné par tcs34725_init().
 */
void tcs34725_enable(int fd);

/**
 * @brief Lit les valeurs brutes Clear/R/G/B du capteur.
 *
 * @param fd Descripteur de fichier I2C.
 * @return Structure contenant les mesures brutes.
 */
TCS34725_Data tcs34725_get_raw_data(int fd);

// Ferme la connexion I2C
/**
 * @brief Ferme la connexion I2C avec le capteur.
 *
 * @param fd Descripteur de fichier I2C.
 */
void tcs34725_close(int fd);

#endif
