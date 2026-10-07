/**
 * @file thread.c
 * @brief Implémentation du thread de détection de couleur (TCS34725) et buzzer/LCD.
 *
 * Ce fichier :
 * - lit les données brutes du capteur TCS34725,
 * - détermine une couleur lisible (texte),
 * - met à jour l'affichage LCD via les fonctions lcd,
 * - peut déclencher un buzzer selon les conditions.
 *
 * Le thread tourne en boucle avec une temporisation (ex : 250 ms).
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "tcs.h"
#include "lcd.h"
#include <wiringPi.h>
#define BUZZER_PIN 25
#include "lcd_shared.h"

static int sensor_fd;
static char buffer_line1[17];
static char buffer_line2[17];
/**
 * @brief Convertit des mesures TCS34725 en une couleur lisible.
 *
 * @param d Mesures brutes du capteur.
 * @return Nom de couleur (chaîne constante).
 */
const char* detecter_couleur(TCS34725_Data d) {
    if (d.c < 20) return "Noir / Vide";

    float total = (float)(d.r + d.g + d.b);
    if (total == 0) return "Inconnu";

    float r_ratio = d.r / total;
    float g_ratio = d.g / total;
    float b_ratio = d.b / total;

    if (r_ratio > 0.40 && g_ratio < 0.3 && b_ratio < 0.29) return "Rouge";
    //g_ratio > 0.32 && r_ratio < 0.38 && b_ratio < 0.30
    
    if (g_ratio > 0.345 && r_ratio < g_ratio && b_ratio < g_ratio) return "Vert";


    return "Indefini";
}
/**
 * @brief Indique si les mesures correspondent à la couleur rouge.
 *
 * @param d Mesures brutes du capteur.
 * @return 1 si rouge détecté, 0 sinon.
 */
int detecter_rouge(TCS34725_Data d) {

    float total = (float)(d.r + d.g + d.b);

    float r_ratio = d.r / total;
    float g_ratio = d.g / total;
    float b_ratio = d.b / total;

    if (r_ratio > 0.40 && g_ratio < 0.3 && b_ratio < 0.29){
        return 1;
    }else {
        return 0;
    }
}
/**
 * @brief Fonction exécutée par le thread de détection de couleur.
 *
 * Boucle principale :
 * - lit les données du capteur,
 * - détermine la couleur,
 * - met à jour le LCD.
 *
 * @return Toujours 0 (selon convention piThread).
 */
PI_THREAD(thread_detection_couleur) {
    while (1) {
        TCS34725_Data data = tcs34725_get_raw_data(sensor_fd);
        const char* nom_couleur = detecter_couleur(data);

        snprintf(lcd_ligne2, sizeof(lcd_ligne2), "Couleur:%s", nom_couleur);

        if (strcmp(nom_couleur, "Vert") == 0) {
            digitalWrite(BUZZER_PIN, HIGH);
        }
        else {
            digitalWrite(BUZZER_PIN, LOW);
        }

   
        // --- Affichage LCD ---
        //afficherPremierLigne(buffer_line1);   // ligne 1 dynamique
        afficherDeuxiemeLigne(buffer_line2);  // ligne 2 = couleur

        // --- Affichage console ---
        //printf("\33[2K\r%s | %s", buffer_line2);
        //fflush(stdout);

        usleep(250000); // 250 ms
    }

}

/**
 * @brief Initialise le capteur couleur, le buzzer, le LCD et démarre le thread.
 *
 * @return 0 si succès, -1 si échec (capteur ou création du thread).
 */
int initDetectionCouleur(void) {

    wiringPiSetup();
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    sensor_fd = tcs34725_init(1);
    if (sensor_fd < 0) {
        printf("Erreur init capteur couleur\n");
        return -1;
    }
    tcs34725_enable(sensor_fd);

    lcd_init(1);
    lcd_clear();

    int success = piThreadCreate(thread_detection_couleur);
    if (success != 0) {
        printf("Erreur creation thread couleur\n");
        return -1;
    }

    printf("Thread detection couleur lancé\n");
    return 0;
}
