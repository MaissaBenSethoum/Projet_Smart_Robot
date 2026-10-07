/**
 * @file lcd.c
 * @brief Implémentation des fonctions de contrôle du LCD (I2C).
 *
 * Contient les fonctions bas niveau de communication (envoi de commandes/données)
 * ainsi que les fonctions de plus haut niveau (init, clear, print, affichage lignes).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "lcd.h"
#define LCD_COLS 16

int lcd_fd;
/**
 * @brief Affiche un texte sur la première ligne du LCD.
 *
 * Efface la ligne 1 (16 caractères) puis affiche la chaîne passée en paramètre.
 *
 * @param mot Chaîne à afficher.
 */
void afficherPremierLigne(const char* mot) {
    lcd_set_cursor(0, 0);
    lcd_print("                "); 
    lcd_set_cursor(0, 0);
    lcd_print(mot);
}
/**
 * @brief Affiche un texte sur la deuxième ligne du LCD.
 *
 * Efface la ligne 2 (16 caractères) puis affiche la chaîne passée en paramètre.
 *
 * @param mot Chaîne à afficher.
 */
void afficherDeuxiemeLigne(const char* mot) {
    lcd_set_cursor(1, 0);        
    lcd_print("                "); ;          
    lcd_set_cursor(1, 0);
    lcd_print(mot);               
}
/**
 * @brief Écrit un octet sur le bus I2C du LCD.
 *
 * @param val Octet à écrire.
 */
void i2c_write_byte(unsigned char val) {
    write(lcd_fd, &val, 1);
}
/**
 * @brief Génère l’impulsion “Enable” nécessaire à la validation d’un envoi.
 *
 * @param bits Octet contenant les bits de contrôle/données à toggler.
 */
void lcd_toggle_enable(unsigned char bits) {
    usleep(500);
    i2c_write_byte(bits | 0x04);
    usleep(500);
    i2c_write_byte(bits & ~0x04);
    usleep(500);
}
/**
 * @brief Envoie un octet au LCD en mode 4 bits.
 *
 * L’octet est découpé en deux (haut puis bas) puis envoyé
 * en respectant le protocole via le bus I2C.
 *
 * @param bits Octet à envoyer.
 * @param mode 0 pour commande, 1 pour donnée.
 */
void lcd_send_byte(unsigned char bits, int mode) {
    unsigned char bits_high = mode | (bits & 0xF0) | 0x08;
    unsigned char bits_low = mode | ((bits << 4) & 0xF0) | 0x08;
    i2c_write_byte(bits_high);
    lcd_toggle_enable(bits_high);
    i2c_write_byte(bits_low);
    lcd_toggle_enable(bits_low);
}
/**
 * @brief Envoie une commande au LCD.
 *
 * @param cmd Code commande.
 */
void lcd_command(unsigned char cmd) { lcd_send_byte(cmd, 0); }
/**
 * @brief Envoie une donnée (caractère) au LCD.
 *
 * @param data Caractère à afficher.
 */
void lcd_data(unsigned char data)   { lcd_send_byte(data, 1); }
/**
 * @brief Initialise le LCD sur un bus I2C donné.
 *
 * Ouvre /dev/i2c-<bus_number>, sélectionne l’adresse du LCD, puis envoie
 * la séquence d’initialisation et efface l’écran.
 *
 * @param bus_number Numéro du bus I2C (ex: 1 pour /dev/i2c-1).
 */
void lcd_init(int bus_number) {
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", bus_number);
    lcd_fd = open(filename, O_RDWR);
    if (lcd_fd < 0 || ioctl(lcd_fd, I2C_SLAVE, LCD_ADDR) < 0) {
        perror("Erreur LCD"); exit(1);
    }
    lcd_command(0x33); lcd_command(0x32); lcd_command(0x06);
    lcd_command(0x0C); lcd_command(0x28); lcd_clear();
}
/**
 * @brief Efface l’écran du LCD.
 */
void lcd_clear() { lcd_command(0x01); usleep(2000); }
/**
 * @brief Positionne le curseur du LCD.
 *
 * @param row Ligne (0 ou 1).
 * @param col Colonne (0..15).
 */
void lcd_set_cursor(int row, int col) {
    int pos[] = {0x80, 0xC0};
    lcd_command(pos[row] + col);
}
/**
 * @brief Affiche une chaîne à la position courante du curseur.
 *
 * @param str Chaîne terminée par '\\0'.
 */
void lcd_print(const char* str) { while (*str) lcd_data(*str++); }
/**
 * @brief Ferme le descripteur I2C du LCD.
 */
void lcd_close() { close(lcd_fd); }