/* Cette bibliothèque permet de contrôler les moteurs du robot.
 * Les moteurs sont connectés à un pont en H : le composant SN754410.
 * Pour chaque moteur, il y a 3 pins à piloter : EN, PWM_1 et PWM_2.
 * Voici leur fonction :
 *
 * PWM    | DIR_1 | DIR_2 ||| Moteur     | Fonction associée
 *     0% |     X |     X ||| Roue libre | moteur_libre()
 *   100% |     0 |     0 ||| Arrêt      | moteur_stop()
 * 1-100% |     1 |     0 ||| Avance     | moteur_avance()
 * 1-100% |     0 |     1 ||| Recule     | moteur_recule()
 *
 * Quand les 2 moteurs tournent en sens opposé, le robot effectue alors une rotation.
 * La rotation est associer aux 2 fonctions moteur_rotation_droite() et moteur_rotation_gauche()
 */

#include "moteur.h"

/****************************************************************************************************************************                                                         
 * Fonction d'avance du robot
 ****************************************************************************************************************************
 * Cette fonction permet de faire avancer le robot.
 ****************************************************************************************************************************
 * Paramètre val_gauche : détermine la valeur de la PWM sur le moteur gauche. de 0 à 255 pour une vitesse de 0% à 100%.
 * Paramètre val_droite : détermine la valeur de la PWM sur le moteur droit. de 0 à 255 pour une vitesse de 0% à 100%.
 ***************************************************************************************************************************/
void moteur_avance(byte val_gauche, byte val_droite)
{
  digitalWrite(MOTEUR_G_DIR_1, 1);
  digitalWrite(MOTEUR_G_DIR_2, 0);
  digitalWrite(MOTEUR_D_DIR_1, 1);
  digitalWrite(MOTEUR_D_DIR_2, 0);
  analogWrite(MOTEUR_G_PWM, val_gauche);
  analogWrite(MOTEUR_D_PWM, val_droite);
}

/****************************************************************************************************************************                                                         
 * Fonction de recule du robot
 ****************************************************************************************************************************
 * Cette fonction permet de faire reculer le robot.
 ****************************************************************************************************************************
 * Paramètre val_gauche : détermine la valeur de la PWM sur le moteur gauche. de 0 à 255 pour une vitesse de 0% à 100%.
 * Paramètre val_droite : détermine la valeur de la PWM sur le moteur droit. de 0 à 255 pour une vitesse de 0% à 100%.
 ***************************************************************************************************************************/
void moteur_recule(byte val_gauche, byte val_droite)
{
  digitalWrite(MOTEUR_G_DIR_1, 0);
  digitalWrite(MOTEUR_G_DIR_2, 1);
  digitalWrite(MOTEUR_D_DIR_1, 0);
  digitalWrite(MOTEUR_D_DIR_2, 1);
  analogWrite(MOTEUR_G_PWM, val_gauche);
  analogWrite(MOTEUR_D_PWM, val_droite);
}

/****************************************************************************************************************************                                                         
 * Fonction de rotation à droite du robot
 ****************************************************************************************************************************
 * Cette fonction permet de faire tourner le robot sur lui-même vers sa droite.
 ****************************************************************************************************************************
 * Paramètre val_gauche : détermine la valeur de la PWM sur le moteur gauche. de 0 à 255 pour une vitesse de 0% à 100%.
 * Paramètre val_droite : détermine la valeur de la PWM sur le moteur droit. de 0 à 255 pour une vitesse de 0% à 100%.
 ***************************************************************************************************************************/
void moteur_rotation_droite(byte val_gauche, byte val_droite)
{
  digitalWrite(MOTEUR_G_DIR_1, 1);
  digitalWrite(MOTEUR_G_DIR_2, 0);
  digitalWrite(MOTEUR_D_DIR_1, 0);
  digitalWrite(MOTEUR_D_DIR_2, 1);
  analogWrite(MOTEUR_G_PWM, val_gauche);
  analogWrite(MOTEUR_D_PWM, val_droite);
}

/****************************************************************************************************************************                                                         
 * Fonction de rotation à gauche du robot
 ****************************************************************************************************************************
 * Cette fonction permet de faire tourner le robot sur lui-même vers sa gauche.
 ****************************************************************************************************************************
 * Paramètre val_gauche : détermine la valeur de la PWM sur le moteur gauche. de 0 à 255 pour une vitesse de 0% à 100%.
 * Paramètre val_droite : détermine la valeur de la PWM sur le moteur droit. de 0 à 255 pour une vitesse de 0% à 100%.
 ***************************************************************************************************************************/
void moteur_rotation_gauche(byte val_gauche, byte val_droite)
{
  digitalWrite(MOTEUR_G_DIR_1, 0);
  digitalWrite(MOTEUR_G_DIR_2, 1);
  digitalWrite(MOTEUR_D_DIR_1, 1);
  digitalWrite(MOTEUR_D_DIR_2, 0);
  analogWrite(MOTEUR_G_PWM, val_gauche);
  analogWrite(MOTEUR_D_PWM, val_droite);
}

/****************************************************************************************************************************                                                         
 * Fonction de blocage du robot
 ****************************************************************************************************************************
 * Cette fonction permet d'arrêter le robot. Cela n'éteint pas les moteurs, au contraire, 
 * les moteurs vont consommer de l'énergie pour s'opposer à tout mouvement.
 ***************************************************************************************************************************/
void moteur_bloque()
{
  digitalWrite(MOTEUR_G_DIR_1, 0);
  digitalWrite(MOTEUR_G_DIR_2, 0);
  digitalWrite(MOTEUR_D_DIR_1, 0);
  digitalWrite(MOTEUR_D_DIR_2, 0);
  analogWrite(MOTEUR_G_PWM, MOTEUR_100);
  analogWrite(MOTEUR_D_PWM, MOTEUR_100);
}

/****************************************************************************************************************************                                                         
 * Fonction d'arrêt du robot
 ****************************************************************************************************************************
 * Cette fonction permet de couper les moteurs, le robot est alors en roue libre. 
 ***************************************************************************************************************************/
void moteur_stop()
{
  digitalWrite(MOTEUR_G_DIR_1, 0);
  digitalWrite(MOTEUR_G_DIR_2, 0);
  digitalWrite(MOTEUR_D_DIR_1, 0);
  digitalWrite(MOTEUR_D_DIR_2, 0);
  analogWrite(MOTEUR_G_PWM, MOTEUR_0);
  analogWrite(MOTEUR_D_PWM, MOTEUR_0);
}
