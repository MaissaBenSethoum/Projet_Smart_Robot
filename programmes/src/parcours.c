/**
 * @file parcours.c
 * @brief Exécution du parcours du robot à partir d’ordres reçus.
 *
 * Ce programme pilote les moteurs et suit une ligne grâce aux capteurs.
 * Les ordres ("AV", "TG", "TD") sont reçus via l’entrée standard (pipe),
 * puis exécutés étape par étape. L’étape finale inclut une détection de rouge
 * pour arrêter le robot si nécessaire.
 *
 * Dépendances matérielles : WiringPi (GPIO/PWM) + capteur couleur TCS34725.
 */
#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "lcd.h"
#include "lcd_shared.h"
#include <stdlib.h>
// définnition des pins pour les capteurs de ligne
#define CAPTEUR_DEVGAUCHE 22 
#define CAPTEUR_DEVDROIT 24 
#define CAPTEUR_GAUCHE 5
#define CAPTEUR_DROIT 19

//définition des pins pour les moteurs
#define IN1 6
#define IN2 18
#define EN1 12
#define IN3 17
#define IN4 16
#define EN2 13

//définition des variables de couleurs
int NOIR=1;
int BLANC=0;
char lcd_ligne1[16] = "               ";
char lcd_ligne2[16] = "               ";
//fonction qui fait la convertion d'un pourcentage à une valeur pour le moteur
/**
 * @brief Convertit un pourcentage en valeur PWM WiringPi.
 *
 *
 * @param val Pourcentage de vitesse (0 à 100).
 * @return Valeur PWM correspondante.
 */
int convertion(int val) {
    return (int)(val/100.0*1024.0);
}

//fonction pour mettre la vitesse des deux moteurs au max
/**
 * @brief Active les deux moteurs (enable).
 *
 * Met les broches EN1 et EN2 à HIGH pour autoriser la commande PWM.
 */
void activer() {
    digitalWrite(EN1, HIGH);
    digitalWrite(EN2, HIGH);
}

//fonction pour faire avancer le robot 
/**
 * @brief Fait avancer le robot avec deux vitesses (gauche/droite).
 *
 * Configure le sens des moteurs pour avancer, puis applique les PWM.
 *
 * @param vitessed Vitesse moteur droit (valeur PWM).
 * @param vitesseg Vitesse moteur gauche (valeur PWM).
 */
void avancer(int vitessed,int vitesseg) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    pwmWrite(EN2, vitesseg);//gauche
    pwmWrite(EN1, vitessed);//droite
}

//fonction qui fait tourner le robot indéfiniment sur la gauche
//rotation sur avec des vitesses différentes pour se replacer correctement
/**
 * @brief Fait tourner le robot vers la gauche (rotation sur place).
 *
 */
void gauche() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    pwmWrite(EN2, convertion(40));//gauche
    pwmWrite(EN1, convertion(45));//droite
}
 
//fonction qui fait tourner le robot indéfiniment sur la droite
//rotation sur avec des vitesses différentes pour se replacer correctement
/**
 * @brief Fait tourner le robot vers la droite (rotation sur place).
 *
 */
void droite() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    pwmWrite(EN2, convertion(45));//gauche
    pwmWrite(EN1, convertion(40));//droite
}

//fonction qui arrète le robot
/**
 * @brief Arrête les moteurs.
 *
 */
void STOP() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    pwmWrite(EN2, convertion(0));//gauche
    pwmWrite(EN1, convertion(0));//droite
}

//fonction pour avancer en ligne droite avec correction
/**
 * @brief Suit la ligne jusqu’à détecter une intersection.
 *
 * Lit les capteurs de ligne et ajuste les vitesses pour corriger la trajectoire.
 * La boucle se termine lorsqu’une intersection est détectée (capteurs gauche/droit).
 * Après dépassement de l’intersection, le robot s’arrête.
 *
 * @param val_devg Valeur du capteur de déviation gauche.
 * @param val_devd Valeur du capteur de déviation droit.
 * @param val_gauche Valeur du capteur gauche.
 * @param val_droite Valeur du capteur droit.
 */
void suivit_ligne_avancer(int val_devg,int val_devd,int val_gauche,int val_droite){
    //tant qu'il n'y a pas d'intersection
    while (val_gauche == BLANC && val_droite == BLANC) {
        //lire les capteurs
        val_devg = digitalRead(CAPTEUR_DEVGAUCHE);
        val_devd = digitalRead(CAPTEUR_DEVDROIT);
        val_droite = digitalRead(CAPTEUR_DROIT);
        val_gauche = digitalRead(CAPTEUR_GAUCHE);
        //corriger la trajectoire
        if (val_devg == NOIR && val_devd == BLANC) {//tourner légèrement à gauche
            avancer(convertion(35), convertion(50));
        } else if (val_devg == BLANC && val_devd == NOIR) {//tourner légèrement à droite
            avancer(convertion(50), convertion(35));
        } else if (val_devg == NOIR && val_devd == NOIR) {//avancer tout droit
            avancer(convertion(60), convertion(60));
        } else {//les deux capteurs sont sur le blanc ou le vert 
            avancer(convertion(50), convertion(50));
        }  
    }
    //pour depasser l'intersection
    delay(170);
    //une fois l'intersection détectée et dépassée, s'arrêter
    STOP();
}

//fonction pour avancer en ligne droite avec correction et détection de rouge utile à la fin du parcours
/**
 * @brief Suit la ligne jusqu’à intersection avec détection de rouge.
 *
 * Identique à suivit_ligne_avancer(), mais interrompt l’exécution si une
 * couleur rouge est détectée via le capteur (TCS34725).
 *
 * @param val_devg Valeur du capteur de déviation gauche.
 * @param val_devd Valeur du capteur de déviation droit.
 * @param val_gauche Valeur du capteur gauche.
 * @param val_droite Valeur du capteur droit.
 */
void suivit_ligne_avancer_detection_rouge(int val_devg,int val_devd,int val_gauche,int val_droite){
    //tant qu'il n'y a pas d'intersection
    int sensor_fd = tcs34725_init(1);

    while (val_gauche == BLANC && val_droite == BLANC) {
        //lire les capteurs
        val_devg = digitalRead(CAPTEUR_DEVGAUCHE);
        val_devd = digitalRead(CAPTEUR_DEVDROIT);
        val_droite = digitalRead(CAPTEUR_DROIT);
        val_gauche = digitalRead(CAPTEUR_GAUCHE);

        TCS34725_Data data = tcs34725_get_raw_data(sensor_fd);
        int rouge_detecte = detecter_rouge(data);
        if (rouge_detecte) {
            STOP();
            printf("Couleur rouge détectée ! Arrêt du robot.\n");
            break;
        }
        //corriger la trajectoire
        if (val_devg == NOIR && val_devd == BLANC) {//tourner légèrement à gauche
            avancer(convertion(35), convertion(50));
        } else if (val_devg == BLANC && val_devd == NOIR) {//tourner légèrement à droite
            avancer(convertion(50), convertion(35));
        } else if (val_devg == NOIR && val_devd == NOIR) {//avancer tout droit
            avancer(convertion(50), convertion(50));
        } else {//les deux capteurs sont sur le blanc ou le vert 
            avancer(convertion(45), convertion(45));
        }  
    }
    //pour depasser l'intersection
    delay(200);
    //une fois l'intersection détectée et dépassée, s'arrêter
    STOP();
}


//fonction qui fait tourner le robot à droite jusqu'à la ligne
/**
 * @brief Effectue un virage à droite jusqu’à retrouver la ligne.
 *
 * Tourne à droite, attend de ne plus être sur la ligne, puis recherche la ligne
 * en lisant les capteurs de déviation. Termine par un léger repositionnement.
 *
 * @param val_devg Valeur du capteur de déviation gauche.
 * @param val_devd Valeur du capteur de déviation droit.
 */
void tourner_Droite(int val_devg,int val_devd){
    //tourner à droite
    droite();
    //faire tourner pour ne pas rester sur la ligne
    delay(450);
    //attendre de retrouver la ligne
    while (val_devd == BLANC || val_devg==BLANC) {
        delay(1);
        val_devg = digitalRead(CAPTEUR_DEVGAUCHE);
        val_devd = digitalRead(CAPTEUR_DEVDROIT);
    }
    //tourner dans l'autre sens pour se repositionner
    gauche();
    delay(100);
    //s'arrêter
    STOP();
}

//fonction qui fait tourner le robot à gauche jusqu'à la ligne
/**
 * @brief Effectue un virage à gauche jusqu’à retrouver la ligne.
 *
 * Tourne à gauche, attend de ne plus être sur la ligne, puis recherche la ligne
 * en lisant les capteurs de déviation. Termine par un léger repositionnement.
 *
 * @param val_devg Valeur du capteur de déviation gauche.
 * @param val_devd Valeur du capteur de déviation droit.
 */
void tourner_Gauche(int val_devg,int val_devd){
    //tourner à gauche
    gauche();
    //faire tourner pour ne pas rester sur la ligne
    delay(600);
    //attendre de retrouver la ligne
    while (val_devd == BLANC || val_devg==BLANC) {
        delay(1);
        val_devg = digitalRead(CAPTEUR_DEVGAUCHE);
        val_devd = digitalRead(CAPTEUR_DEVDROIT);
    }
    //tourner dans l'autre sens pour se repositionner
    droite();
    delay(250);
    //s'arrêter
    STOP();
}

//fonction pour lire les ordres reçus via stdin
#define MAX_COMMANDES 500
#define BUFFER_TAILLE 4096

//fonction pour lire la liste des commandes reçues via stdin
/**
 * @brief Lit les ordres reçus via stdin.
 *
 * Attend une ligne sur l’entrée standard (pipe) au format de type :
 * {"AV","TG","AV"}.
 *
 * @param chemin_dynamique Tableau de sortie contenant les chaînes ("AV","TG","TD").
 * @return Nombre d’ordres lus (0 si échec de lecture).
 */
int lire_commandes(char *chemin_dynamique[]) {
    char buffer[BUFFER_TAILLE];
    
    printf("En attente des ordres du cerveau...\n");
    // Lire la ligne depuis stdin (le pipe)
    if (fgets(buffer, BUFFER_TAILLE, stdin) == NULL) {
        return 0;
    }

    // Le format est {"AV","TG","AV"}
    // On utilise strtok pour découper selon les délimiteurs : { } " , et retour ligne
    const char *delimiteurs = "{}\",\n\r ";
    char *token = strtok(buffer, delimiteurs);
    
    int count = 0;
    while (token != NULL && count < MAX_COMMANDES) {
        // On stocke la commande (AV, TG, TD)
        chemin_dynamique[count] = strdup(token); // Allocation mémoire pour la chaîne
        count++;
        token = strtok(NULL, delimiteurs);
    }
    return count;
}


//programme principal
/**
 * @brief Programme principal d’exécution du robot.
 *
 * Initialise les GPIO/PWM, lance le thread de détection couleur,
 * lit les ordres produits , puis exécute chaque étape :
 * - "AV" : suivi de ligne jusqu’à intersection
 * - "TD"/"TG" : virage jusqu’à retrouver la ligne
 * La dernière étape utilise la détection de rouge.
 *
 * @return 0 en fin normale.
 */
int main(void) {
    // Initialisation de la bibliothèque WiringPi
    wiringPiSetupGpio();
    //Initialisation des modes des pins
    pinMode(CAPTEUR_GAUCHE, INPUT);
    pinMode(CAPTEUR_DROIT, INPUT);
    pinMode(CAPTEUR_DEVGAUCHE, INPUT);
    pinMode(CAPTEUR_DEVDROIT, INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(EN1, PWM_OUTPUT);
    pinMode(EN2, PWM_OUTPUT);

    // Initialisation des variables de lecture des capteurs
    int val_devg= BLANC;
    int val_devd = BLANC;
    int val_droite=BLANC;
    int val_gauche=BLANC;

    //Appel du thread de détection de couleur
    initDetectionCouleur();
    // définition du parcours
    char *chemin[MAX_COMMANDES]; 
    int etape_actuelle = 0;
    int nb_etapes = lire_commandes(chemin);    
    while(1) {
        //si on a terminé toutes les étapes
        if (etape_actuelle >= nb_etapes) {
            STOP();
            printf("Parcours termine.\n");
            break; 
        }
        // Affichage de l'étape actuelle sur le LCD
        snprintf(lcd_ligne1, 17, "Etape: %-10s", chemin[etape_actuelle]);
        afficherPremierLigne(lcd_ligne1);
        afficherDeuxiemeLigne(lcd_ligne2);
        //si on est a la dernière étape
        if (etape_actuelle == nb_etapes - 1) {
            suivit_ligne_avancer_detection_rouge(val_devg, val_devd, val_gauche, val_droite);
            etape_actuelle++; 
        }
        else {
            //si on doit avancer
            if (strcmp(chemin[etape_actuelle], "AV") == 0) {
                
                suivit_ligne_avancer(val_devg, val_devd, val_gauche, val_droite);
                etape_actuelle++; // On passe à l'ordre suivant
            }
            
            //si on doit tourner à droite
            else if (strcmp(chemin[etape_actuelle], "TD") == 0) {
                // STOP();
                delay(100);
                tourner_Droite(val_devg,val_devd);
                etape_actuelle++;
            }
            //si on doit tourner à gauche
            else if (strcmp(chemin[etape_actuelle], "TG") == 0) {
                // STOP();
                delay(100);
                tourner_Gauche(val_devg,val_devd);
                etape_actuelle++;
            }
        }
    }
    return 0;
}