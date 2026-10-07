/**
 * @file lcd.h
 * @brief Interface d’affichage sur LCD (I2C) pour le robot.
 *
 * Ce module fournit les fonctions nécessaires pour :
 * - initialiser/fermer le LCD,
 * - afficher du texte et positionner le curseur,
 * - effacer l’écran,
 * - afficher des lignes (helpers).
 */
#ifndef LCD_I2C_H
#define LCD_I2C_H

// Adresse I2C de l'écran (souvent 0x27 ou 0x3F)
#define LCD_ADDR 0x27 
/**
 * @brief Initialise le LCD.
 * @return 0 si succès, -1 sinon.
 */
void lcd_init(int bus_number);
/**
 * @brief Efface l’écran LCD.
 */
void lcd_clear();
/**
 * @brief Positionne le curseur du LCD.
 * @param row Ligne (0 ou 1).
 * @param col Colonne (0..15).
 */
void lcd_set_cursor(int row, int col);
/**
 * @brief Affiche une chaîne à la position courante du curseur.
 * @param str Chaîne à afficher (terminée par '\0').
 */
void lcd_print(const char* str);
/**
 * @brief Ferme/arrête proprement le LCD.
 */
void lcd_close();
/**
 * @brief Affiche une première ligne (16 caractères max).
 * @param ligne Chaîne à afficher.
 */
void afficherPremierLigne(const char* mot) ;
/**
 * @brief Affiche une deuxième ligne (16 caractères max).
 * @param ligne Chaîne à afficher.
 */
void afficherDeuxiemeLigne(const char* mot) ;
#endif