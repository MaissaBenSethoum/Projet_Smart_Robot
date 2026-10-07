/**
 * @file tcs.c
 * @brief Implémentation du pilote I2C pour le capteur couleur TCS34725.
 *
 * Fournit des fonctionspour activer et lire les registres de couleur du TCS34725.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "tcs.h"

/** @name Registres internes du TCS34725 */
/**@{*/
/** Registre ENABLE (mise sous tension / activation ADC). */
#define REG_ENABLE  0x00
/** Registre ATIME (temps d'intégration). */
#define REG_ATIME   0x01
/** Registre CONTROL (gain). */
#define REG_CONTROL 0x0F
/** Registre CDATAL (début des données Clear). */
#define REG_CDATAL  0x14
/**@}*/

/**
 * @brief Écrit un octet dans un registre du capteur.
 *
 * @param fd Descripteur I2C.
 * @param reg Adresse du registre.
 * @param value Valeur à écrire.
 */
static void write_reg(int fd, uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[0] = TCS34725_COMMAND_BIT | reg;
    buffer[1] = value;
    if (write(fd, buffer, 2) != 2) {
        perror("TCS34725: Erreur d'écriture");
    }
}
/**
 * @brief Lit un mot (16 bits) depuis un registre.
 *
 * @param fd Descripteur I2C.
 * @param reg Adresse du registre.
 * @return Valeur 16 bits lue.
 */
static uint16_t read_word(int fd, uint8_t reg) {
    uint8_t buffer[1];
    buffer[0] = TCS34725_COMMAND_BIT | reg;
    
    if (write(fd, buffer, 1) != 1) return 0;

    uint8_t data[2];
    if (read(fd, data, 2) != 2) return 0;
    
    return (uint16_t)(data[0] + (data[1] << 8));
}


/**
 * @brief Initialise le capteur TCS34725 sur un bus I2C.
 *
 * Ouvre le périphérique /dev/i2c-<bus_number>, sélectionne l’adresse
 * du capteur puis active/configure le capteur.
 *
 * @param bus_number Numéro du bus I2C.
 * @return fd I2C (>=0) si succès, -1 sinon.
 */
int tcs34725_init(int bus_number) {
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", bus_number);
    
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("Erreur ouverture bus I2C");
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, TCS34725_ADDRESS) < 0) {
        perror("Erreur accès adresse capteur");
        close(fd);
        return -1;
    }
    return fd;
}
/**
 * @brief Active le capteur et applique une configuration par défaut.
 *
 * @param fd Descripteur I2C.
 */
void tcs34725_enable(int fd) {
    // 1. Power ON et ADC Enable (0x03)
    write_reg(fd, REG_ENABLE, 0x03);
    
    // 2. Temps d'intégration (0xC0 = approx 154ms, bon compromis vitesse/précision)
    write_reg(fd, REG_ATIME, 0xC0);
    
    // 3. Gain 4x (0x01)
    write_reg(fd, REG_CONTROL, 0x01);
    
    // Pause pour laisser le capteur s'initialiser
    usleep(200000);
}
/**
 * @brief Lit les données brutes Clear/R/G/B.
 *
 * @param fd Descripteur I2C.
 * @return Données brutes du capteur.
 */
TCS34725_Data tcs34725_get_raw_data(int fd) {
    TCS34725_Data data;
    
    // Les registres sont contigus : Clear, Red, Green, Blue
    // On lit mot par mot
    data.c = read_word(fd, REG_CDATAL);
    data.r = read_word(fd, REG_CDATAL + 2); // RDATA
    data.g = read_word(fd, REG_CDATAL + 4); // GDATA
    data.b = read_word(fd, REG_CDATAL + 6); // BDATA
    
    return data;
}
/**
 * @brief Ferme le descripteur I2C.
 *
 * @param fd Descripteur I2C.
 */
void tcs34725_close(int fd) {
    if (fd >= 0) close(fd);
}
