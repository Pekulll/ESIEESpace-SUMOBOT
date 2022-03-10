/*
 * Projet               : Kit SumoBot 2022
 * Entité               : Association ESIEESPACE : http://www.esieespace.fr/
 * Auteurs              : Benjamin LAMBERT
 *                        Corentin POUPRY
 * Création             : 10/03/2022
 * Dernière mise à jour : 19/03/2022
 * Version              : 5.2.0
 */

const unsigned char MAX_DISTANCE = 100; // La distance maximal avec l'ennemi (en cm)

// La vitesse de charge / d'esquive du robot (varie entre 0 et 255)
const unsigned char SPEED = 60; // Cette valeur a été choisi car elle permet au robot de fonctionner parfaitement

// La vitesse de rotation du robot (varie entre 0 et 255)
const unsigned char ROTATION_SPEED = 30; // Une vitesse de rotation plus élevé est déconseiller car cela rend la ddétection moins fiable