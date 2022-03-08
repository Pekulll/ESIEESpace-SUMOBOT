/* Cette bibliothèque permet de contrôler le module ultrason HC-SR04.
 * Voici le fonctionnement de ce module à l'aide d'un chronogramme :
 * 
 *               <- 10µs ->                     <- >200µs ->
 * Trigger : ___|¯¯¯¯¯¯¯¯¯|_________________________________|¯¯¯¯¯¯¯¯¯|____
 * Echo    : ___________________|¯¯¯¯¯¯¯¯¯¯¯¯¯¯|___________________________
 *                              <-- distance -->
 *
 * Pour utiliser le module ultrason, il faut envoyer une impulsion de 10µs sur la pin trigger,
 * puis mesurer le temps que la pin echo reste à 1. 
 * Une fois cette mesure effectuée, le calcul pour avoir la distance en centimètres est le suivant : 
 * distance = mesure * 17/1000. Pour plus de détails, voir la datasheet du module.
 */

#include "HCSR04.h"

/****************************************************************************************************************************                                                         
 * Fonction de détection de l'adversaire
 ****************************************************************************************************************************
 * Cette fonction permet de mesurer la distance d'un objet en face du capteur ultrason.
 ****************************************************************************************************************************
 * Valeur de retour : la distance en centimètres. 0 si aucun objet n'est détecté.
 ***************************************************************************************************************************/
long ultrason_distance()
{
  long mesure;
  
  // On envoie le trigger
  digitalWrite(ULTRASON_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASON_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASON_TRIGGER, LOW);
  
  // On mesure le temps à l'état haut de la pin echo
  // On ajoute un timeout de 25ms pour le cas où aucun objet n'est devant le capteur
  mesure = pulseIn(ULTRASON_ECHO, HIGH, 25000); 
  
  // On attend le minimum de 200µs requit par le capteur
  delayMicroseconds(200);
  
  // On effectue la conversion en centimètres
  mesure = mesure * 17/1000;
  
  return mesure;
}
