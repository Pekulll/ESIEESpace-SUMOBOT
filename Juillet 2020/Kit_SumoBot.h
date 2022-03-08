/****************************************************************************************************************************                                                         
 * Inclusion des bibliothèques
 ****************************************************************************************************************************
 * Cette partie permet d'inclure des bibliothèques renfermant des fonctions utiles.
 ***************************************************************************************************************************/
// bibliothèque pour la gestion de projet Arduino
#include "Arduino.h" 

// Cette bibliothèque fournie par Arduino permet d'utiliser la liaison série et d'envoyer des messages à l'ordinateur
#include <Wire.h>
//Bibliothèque de dialogue avec le bluetooth
#include <SoftwareSerial.h>
// Cette bibliothèque fournie par ESIEESPACE permet de piloter les 2 moteurs avec des PWM.
#include "moteur.h"
// Cette bibliothèque fournie par ESIEESPACE permet de piloter le module ultrason.
#include "HCSR04.h"


/****************************************************************************************************************************                                                         
 * Déclaration des constantes
 ***************************************************************************************************************************/
#define VERSION "5.2.0"

/* Cette constante permet d'utiliser la liaison série pour émettre des messages d'informations. 
 * valeur 1 : Activer
 * valeur 0 : Désactiver
 */
#define DEBUG 1  

#define NUCLEO
// #define ARDUINO

/* Cette constante permet de préciser le seuil de détection du capteur à ultrason. 
 * La valeur est en centimètres et représente la valeur maximale de détection.
 * Si un objet est détecté après cette distance, il n'est pas considéré comme le robot adverse.
 * Cela est utile pour ne pas détecter "trop loin" et croire que le public ou autre est l'adversaire.
 */
#define ULTRASON_SEUIL 40

/* Ces constantes permettent de définir des noms pour les valeurs de retour des fonctions.
 * Cela permet d'utiliser les noms au lieu des valeurs, ce qui est plus parlant.
 */
#define AV_GAUCHE  1
#define AV_DROITE  2
#define AV_CENTRE  3
#define AR_GAUCHE  4
#define AR_DROITE  5
#define AR_CENTRE  6

/****************************************************************************************************************************                                                         
 * Déclaration des pins
 ****************************************************************************************************************************
 * Cette partie permet de lier le numéro d'une pin de la carte Arduino à une variable.
 * On pourra donc utiliser cette variable plutôt que le numéro de pin, ce qui est plus simple.
 ***************************************************************************************************************************/
// Entrées
const int BP                  = 2;  // bouton poussoir
const int TCRT_AVG            = A0;  // TCRT avant gauche
const int TCRT_AVD            = A1; // TCRT avant droit


const int ULTRASON_ECHO    = 3; // Pin ECHO du module ultrason HC-SR04

// Sorties
#ifdef NUCLEO
const int LED_1            = A4;  // LED verte 1
const int LED_2            = A5; // LED verte 2
#else
const int LED_1            = 7;  // LED verte 1
const int LED_2            = 8; // LED verte 2
#endif
const int MOTEUR_G_PWM     = 5;  // PWM moteur gauche
const int MOTEUR_G_DIR_1   = 9;  // Direction 1 moteur gauche
const int MOTEUR_G_DIR_2   = 10;  // Direction 2 moteur gauche
const int MOTEUR_D_PWM     = 6;  // PWM moteur droit
const int MOTEUR_D_DIR_1   = A3;  // Direction 1 moteur droit
const int MOTEUR_D_DIR_2   = A2; // Direction 2 moteur droit
const int ULTRASON_TRIGGER = 4; // Pin TRIGGER du module ultrason HC-SR04

/****************************************************************************************************************************                                                         
 * Déclarations des fonctions
 ****************************************************************************************************************************
 * Cette partie permet de déclarer les fonctions que l'on va utiliser dans le fichier principal.
 * les fonctions setup() et loop() sont déjà déclarer dans la bibliothèque Arduino.h
 ***************************************************************************************************************************/
void attente_go();
byte ligne_blanche();
void esquive_ligne(byte sens);
