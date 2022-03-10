/*
 * Projet               : Kit SumoBot 2022
 * Entité               : Association ESIEESPACE : http://www.esieespace.fr/
 * Auteurs              : Benjamin LAMBERT
 *                        Corentin POUPRY
 * Création             : 10/03/2022
 * Dernière mise à jour : 19/03/2022
 * Version              : 5.2.0
 */

const unsigned char LED_G = 10;
const unsigned char LED_D = 11;

// Ces pins sont utiliser pour la détection en ultrason
const unsigned char TRIG = 7;  // Assigne la valeur du pin TRIGGER
const unsigned char ECHO = 6;  // Assigne la valeur du pin ECHO

// Les détecteurs TCRT représentent les détecteurs de ligne blanche
const unsigned char TCRT_DROIT = A1;   // Assigne la valeur du pin assigner au détecteur droit
const unsigned char TCRT_GAUCHE = A0;  // Assigne la valeur du pin assigne au détecteur gauche

const unsigned char MOTEUR_G_PWM = 8;      // Assigne la valeur du pin assigner au PWM des moteurs gauches
const unsigned char MOTEUR_G_DIR_1 = 12;   // Assigne la valeur du pin assigner à la direction 1 des moteurs gauches
const unsigned char MOTEUR_G_DIR_2 = 13;   // Assigne la valeur du pin assigner à la direction 2 des moteurs gauches
const unsigned char MOTEUR_D_PWM = 9;      // Assigne la valeur du pin assigner au PWM des moteurs droits
const unsigned char MOTEUR_D_DIR_1 = A3;   // Assigne la valeur du pin assigner à la direction 1 des moteurs droits
const unsigned char MOTEUR_D_DIR_2 = A2;   // Assigne la valeur du pin assigner à la direction 2 des moteurs droits

const unsigned char PUSH = 8; // Assigne la valeur du pin assigner au bouton poussoir