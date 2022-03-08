/****************************************************************************************************************************                                                         
 * Projet               : Kit SumoBot 2021 
 * Entité               : Association ESIEESPACE : http://www.esieespace.fr/ 
 * Auteurs              : Jérémie DUTHIL
 *                      : Antoine BESSIERE
 *                      : Louis DESPLANCHE
 * Création             : 05/07/2016
 * Dernière mise à jour : 06/07/2020
 * Version              : 5.2.0
 * --------------------------------------------------------------------------------------------------------------------------
 * Description          :
 * Ce code est fourni avec le Kit SumoBot et permet de contrôler les différentes parties du robot.
 * Ceci est une base fonctionnelle à améliorer. Pour toute question, visiter le forum SumoBot : http://sumobot.esieespace.fr 
 * Pour plus d'informations sur l'utilisation d'Arduino, voir la documentation officielle : http://www.arduino.cc/en/Reference 
 * Ce code est du domaine public, il peut donc être modifié et redistribué.
 * --------------------------------------------------------------------------------------------------------------------------
 * Révision |    Date    | Description
 * 3.0.0    | 02/08/2016 | Création
 * 5.0.0    | 06/01/2020 | Modification
 * 5.2.0    | 06/07/2020 | Modification
 ***************************************************************************************************************************/


#include "Kit_SumoBot.h"
/****************************************************************************************************************************                                                         
 * Initialisation
 ****************************************************************************************************************************
 * Cette fonction permet d'initialiser notre programme en définissant la direction des pins et autres actions 
 * qui ne seront exécuter qu'une seule fois. Cette fonction est appelée après un reset, donc au démarrage de la carte 
 * ou après l'appuie sur le bouton reset de la carte Arduino.
 ***************************************************************************************************************************/
void setup()
{
  #if DEBUG
    // Initialisation du lien série
    Serial.begin(9600); // init liaison UART
    Serial.print("Bonjour, je suis le Kit SumoBot de l'ESIEESPACE version : "+ String(VERSION));
    Serial.println("-----------------------------------------------------------");
    Serial.println("Initialisation");
  #endif
  //
  
  // Initialisation des entrées
  pinMode(BP,                INPUT_PULLUP); // On ajoute une résistance de pullup pour que la pin soit à un niveau HIGH par défaut.
  pinMode(TCRT_AVG,          INPUT);
  pinMode(TCRT_AVD,          INPUT);
  pinMode(ULTRASON_ECHO,     INPUT);
  
  // Initialisation des sorties
  pinMode(LED_1,             OUTPUT);
  pinMode(LED_2,             OUTPUT);
  pinMode(MOTEUR_G_PWM,      OUTPUT);
  pinMode(MOTEUR_G_DIR_1,    OUTPUT);
  pinMode(MOTEUR_G_DIR_2,    OUTPUT);
  pinMode(MOTEUR_D_PWM,      OUTPUT);
  pinMode(MOTEUR_D_DIR_1,    OUTPUT);
  pinMode(MOTEUR_D_DIR_2,    OUTPUT);
  pinMode(ULTRASON_TRIGGER,  OUTPUT);
  Serial.flush();
}

/****************************************************************************************************************************                                                         
 * Fonction principale
 ****************************************************************************************************************************
 * Cette fonction permet l'exécution du code "en boucle". 
 * Si le programme atteint la dernière ligne de cette fonction, l'exécution recommence indéfiniment.
 ***************************************************************************************************************************/
void loop()
{
  // Déclaration des variables
  byte ligne;      // Cette variable servira à mémoriser l'état des capteurs de ligne blanche
  byte ligne_mem;  // Cette variable servira à mémoriser le dernier capteur de ligne activé
  long distance;   // Cette variable servira à mémoriser l'état du capteur ultrason et la distance de l'adversaire
  
  #if DEBUG
    Serial.println("Attente du bouton GO");
  #endif
  
  // Attente du bouton GO
  attente_go();
  
  // Attente des 5sec règlementaires
  delay(5000);
  
  // Début du combat 
  while (digitalRead(BP) == HIGH)                       // Tant qu'on n'appuie pas sur le bouton, on est en combat
  {    
    ligne = ligne_blanche();                            // Détection de la ligne blanche   
    distance = ultrason_distance();                     // Détection de l'adversaire
        
    if (ligne > 0)                                      // Si une ligne blanche est détectée, on esquive.
    {
      esquive_ligne(ligne);
      ligne_mem = ligne;
      moteur_stop();
    }   
    else if (distance < ULTRASON_SEUIL && distance > 0) // Si un object est présent devant le robot et à une distance inférieure au seuil
    {
      #if DEBUG
        Serial.print("Adversaire detecte a ");
        Serial.print(distance);
        Serial.println("cm");
      #endif      
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, HIGH);
      moteur_avance(MOTEUR_100, MOTEUR_100);            // On avance tout droit pour pousser l'adversaire
    }
    else                                                // Sinon, on recherche l'adversaire.
    {      
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      if (ligne_mem == AV_GAUCHE)
      {
        moteur_avance(128, 50);                         // La recherche se fait en tournant légèrement sur la droite pour décrire une spirale
      }
      else
      {
        moteur_avance(50, 128);                         // La recherche se fait en tournant légèrement sur la gauche pour décrire une spirale 
      }
    } 
  
    delay(0);                                           // delais entre chaque cycle
  }
  
  // Sortie du mode combat, on arrête les moteurs et on attend que le bouton soit relâché
  moteur_stop();
  while (digitalRead(BP) == LOW);                       // cette syntaxe permet d'attendre la condition sans exécuter de code spécifique.
  delay(100);
  
  // Arrivée ici, la fonction loop() recommence du début
}

/****************************************************************************************************************************                                                         
 * Fonction d'attente du bouton go
 ****************************************************************************************************************************
 * Cette fonction permet d'attendre une pression sur le bouton poussoir.
 * Pendant cette attente, les leds clignotent.
 ***************************************************************************************************************************/
void attente_go()
{
  bool led = false;              // On créer une variable pour l'état des leds
  
  while(digitalRead(BP) == HIGH) // Tant que le bouton poussoir est à l'état haut (grâce à la fonction PULLUP)
  {    
    if (led)                     // On fait clignoter les 2 leds pour montrer qu'on attend le go
    {
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
    }
    else
    {
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
    }
    
    led = !led;                  // on inverse la variable pour inverser l'état des leds
    
    delay(100);                  // On répète l'opération toutes les 200ms, c'est ce qui créer le clignotement
  }
  
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  
  // Quand le bouton est pressé, on quitte la boucle While et on sort de la fonction attente_go()
  
  #if DEBUG
    Serial.println("Go !");
  #endif
}

/****************************************************************************************************************************                                                         
 * Fonction de détection de la ligne blanche
 ****************************************************************************************************************************
 * Cette fonction permet d'utiliser les capteurs de ligne blanche (TCRT5000) pour détecter le bord de l'arène.
 ****************************************************************************************************************************
 * Valeur de retour : 0         pour aucune ligne détectée
 *                    AV_GAUCHE pour une ligne détectée à l'avant gauche
 *                    AV_DROITE pour une ligne détectée à l'avant droit
 ***************************************************************************************************************************/
byte ligne_blanche()
{
  if (digitalRead(TCRT_AVG))                      // si le capteur de ligne blanche avant gauche est activé
  {
    #if DEBUG
      Serial.println("Ligne blanche detectee a l'avant gauche !");
    #endif     
    return AV_GAUCHE;                             // alors on renvoie la valeur associée à ce capteur, définie dans Kit_SumoBot.h
  }
  else if (digitalRead(TCRT_AVD))                 // si le capteur de ligne blanche avant droit est activé
  {
    #if DEBUG
      Serial.println("Ligne blanche detectee a l'avant droit !");
    #endif
    return AV_DROITE;                             // alors on renvoie la valeur associée à ce capteur 
  }
  else                                            // Sinon, aucun capteur d'activé, pas de ligne blanche détectée 
  {
    return 0;
  } 
}

/****************************************************************************************************************************                                                         
 * Fonction d'esquive de la ligne blanche
 ****************************************************************************************************************************
 * Cette fonction permet d'éviter de franchir la ligne blanche en effectuant une equive sur le côté.
 * Une esquive est considérée ici comme une manoeuvre de recule puis d'une rotation d'une durée aléatoire. 
 ****************************************************************************************************************************
 * Paramètre sens : détermine le sens de l'esquive. Valeurs définies dans le fichier "Kit_SumoBot.h".
 ***************************************************************************************************************************/
void esquive_ligne(byte sens)
{   
  long aleatoire;
  
  moteur_recule(MOTEUR_100, MOTEUR_100);            // le robot recule a pleine vitesse
  delay(300);                                       // temps où le robot recule. 300ms
  
  if (sens == AV_GAUCHE)
  {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    moteur_rotation_droite(MOTEUR_100, MOTEUR_100); // le robot tourne vers la droite à pleine vitesse
  }
  else
  {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    moteur_rotation_gauche(MOTEUR_100, MOTEUR_100); // le robot tourne vers la gauche à pleine vitesse
  }

  aleatoire = random(200, 500);
  #if DEBUG
    Serial.print("Pour esquiver la ligne blanche, je tourne pendant ");
    Serial.print(aleatoire);
    Serial.println("ms");
  #endif
  delay(aleatoire);                                // temps où le robot tourne vers la droite. Temps aléatoire entre 800ms et 2000ms
}
