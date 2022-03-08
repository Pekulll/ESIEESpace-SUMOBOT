/*
 * Projet               : Kit SumoBot 2022
 * Entité               : Association ESIEESPACE : http://www.esieespace.fr/
 * Auteurs              : Benjamin LAMBERT
 *                        Corentin POUPRY
 * Création             : 08/03/2022
 * Dernière mise à jour : 08/03/2022
 * Version              : 5.2.0
 * 
 * --------------------------------------------------------------------------------------------------------------------------
 * 
 * Description          :
 * Ce code est fourni avec le Kit SumoBot et permet de contrôler les différentes parties du robot.
 * Ceci est une base fonctionnelle à améliorer. Pour toute question, visiter le forum SumoBot : http://sumobot.esieespace.fr
 * Pour plus d'informations sur l'utilisation d'Arduino, voir la documentation officielle : http://www.arduino.cc/en/Reference
 * Ce code est du domaine public, il peut donc être modifié et redistribué.
 * 
 * --------------------------------------------------------------------------------------------------------------------------
 * 
 * Révision |    Date    | Description
 * 5.2.0    | 08/03/2022 | Création
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

const unsigned char MAX_DISTANCE = 100; // La distance maximal avec l'ennemi (en cm)

// La vitesse de charge / d'esquive du robot (varie entre 0 et 255)
const unsigned char SPEED = 60; // Cette valeur a été choisi car elle permet au robot de fonctionner parfaitement

// La vitesse de rotation du robot (varie entre 0 et 255)
const unsigned char ROTATION_SPEED = 30; // Une vitesse de rotation plus élevé est déconseiller car cela rend la ddétection moins fiable


/*
* Attends qu'on appuie sur le bouton pour commencer le combat
*/
void wait_start(){
  bool led = false;

  while(digitalRead(PUSH)){
    if(led){
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_D, LOW);
    }
    else{
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_D, HIGH);
    }

    led = !led;
  }

  Serial.println("[ESIEESPACE] EN ATTENTE DE L'ADVERSAIRE (5s)...");
  delay(5000); // Attends les 5 secondes réglementaires après appuie du bouton
  Serial.println("[ESIEESPACE] DÉBUT DU COMBAT !");
}



void setup() {
  Serial.println("[ESIEESPACE] DÉMARRAGE...");
  Serial.begin(9600); // Initialise la communication avec le robot
  delay(1000);
  
  pinMode(TRIG, OUTPUT); // Initialise le pin TRIG comme sortie (capteur utlrason)
  pinMode(ECHO, INPUT);  // Initialise le pin ECHO comme entrée (capteur utlrason)

  pinMode(TCRT_DROIT, INPUT);   // Initialise le pin TCRT_DROIT comme entrée (capteur ligne blanche)
  pinMode(TCRT_GAUCHE, INPUT);  // Initialise le pin TCRT_GAUCHE comme entrée (capteur ligne blanche)

  pinMode(PUSH, INPUT_PULLUP); // Initialise le pin PUSH comme entrée PULLUP (bouton)

  pinMode(MOTEUR_G_PWM, OUTPUT);     // Initalise le pin MOTEUR_G_PWM comme sortie
  pinMode(MOTEUR_G_DIR_1, OUTPUT);   // Idem avec MOTEUR_G_DIR_1
  pinMode(MOTEUR_G_DIR_2, OUTPUT);   // Idem avec MOTEUR_G_DIR_2
  pinMode(MOTEUR_D_PWM, OUTPUT);     // Idem avec MOTEUR_D_PWM
  pinMode(MOTEUR_D_DIR_1, OUTPUT);   // Idem avec MOTEUR_D_DIR_1
  pinMode(MOTEUR_D_DIR_2, OUTPUT);   // Idem avec MOTEUR_D_DIR_2

  Serial.println("[ESIEESPACE] DÉMARRER AVEC SUCCES !");

  // Attend que l'on appuie sur le bouton poussoir pour commencer le combat
  // Cela permet la synchronisation entre les deux robots
  wait_start();
}


void loop() {
  char back = digitalRead(TCRT_GAUCHE);   // Récupère la valeur du capteur de ligne arrière
  char forward = digitalRead(TCRT_DROIT); // Récupère la valeur du capteur de ligne avant

  /*
   * Vérifie si un des deux capteurs de ligne à detecter une ligne (1)
   * Si c'est le cas, il repart dans la direction opposée à la ligne
   * pendant une demi seconde, puis s'arrête et cherche l'ennemi
   */
  if(back){
    Serial.println("[ESIEESPACE] Ligne blanche derrière !");
    rightEngine(SPEED, 1);
    leftEngine(SPEED, 1);
    delay(500);
    stopEngine();
    return;
  }
  else if(forward){
    Serial.println("[ESIEESPACE] Ligne blanche devant !");
    rightEngine(SPEED, 0);
    leftEngine(SPEED, 0);
    delay(500);
    stopEngine();
    return;
  }

  float distance = detect(); // Récupère la distance le séparant de l'objet le plus proche

  /*
   * Vérifie si la distance est une valeur cohérente (entre 0 et MAX_DISTANCE)
   * Si la valeur est cohérente, alors il considère que l'objet détecté est un ennemi,
   * puis engage une charge contre l'ennemi.
   * Si l'objet n'est pas un ennemi (valeur de la distance incohérente), alors il tourne sur
   * lui-même afin de rechercher un ennemi potentiel.
   * Si l'ennemi est perdu de vue pendant la charge, le robot se met à le chercher en tournant
   * sur lui-même.
   */
  if(distance <= MAX_DISTANCE && distance > 0){
    rightEngine(SPEED, 1);
    leftEngine(SPEED, 1);
    Serial.println("[ESIEESPACE] ATTAQUE...");
  }
  else{
    rightEngine(ROTATION_SPEED, 0);
    leftEngine(ROTATION_SPEED, 1);
    Serial.println("[ESIEESPACE] RECHERCHE...");
  }
}


/*
 * Permet la détection d'un objet et de calculer la distance
 * entre le robot et l'objet détecté.
 * @return La distance entre le robot et l'objet
 */
float detect() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG, LOW);

  unsigned short timeToTarget = pulseIn(ECHO, HIGH);
  float distance = (float)(timeToTarget * 17) / 1000.0;

  /*
   * Si le temps écoulé avant réception du signal est supérieur 
   * à 2000, on considère que la valeur de distance est incohérente
   * Ce constat est venu après des expérimentations où la distance dépassée
   * ce seuil de 2000 µs était incohérente par rapport à la distance réelle
   * observée
   */
  if(timeToTarget >= 2000){
    distance = -1;
  }

  return distance;
}


/*
 * Permet le contrôle total des moteurs droits uniquement
 * Paramètres :
 * speed : vitesse choisis par l'utilisateur (de 0 à 255)
 * direction : direction choisis par l'utilisateur (0 ou 1), 1 représente l'avant et 0 l'arrière
 */
void rightEngine(unsigned char speed, unsigned char direction){
  digitalWrite(MOTEUR_D_DIR_1, direction);   // On met la direction avant des moteurs droits à direction
  digitalWrite(MOTEUR_D_DIR_2, !direction);  // On met la direction arrière des moteurs droits à l'opposée de la direction
  analogWrite(MOTEUR_D_PWM, speed);          // On applique la vitesse choisis par l'utilisateur
}

/*
 * Permet le contrôle total des moteurs gauches uniquement
 * Paramètres :
 * speed : vitesse choisis par l'utilisateur (de 0 à 255)
 * direction : direction choisis par l'utilisateur (0 ou 1), 1 représente l'avant et 0 l'arrière
 */
void leftEngine(unsigned char speed, unsigned char direction) {
  digitalWrite(MOTEUR_G_DIR_1, direction);  // On met la direction avant des moteurs droits à direction
  digitalWrite(MOTEUR_G_DIR_2, !direction); // On met la direction arrière des moteurs droits à l'opposée de la direction
  analogWrite(MOTEUR_G_PWM, speed);         // On applique la vitesse choisis par l'utilisateur
}

/*
 * Permet de stopper les moteurs en bloquant les roues
 */
void stopEngine(){
  digitalWrite(MOTEUR_D_DIR_1, 0);
  digitalWrite(MOTEUR_D_DIR_2, 0);
  digitalWrite(MOTEUR_G_DIR_1, 0);
  digitalWrite(MOTEUR_G_DIR_2, 0);

  // La vitesse est mise au maximum pour bloquer les roues
  analogWrite(MOTEUR_G_PWM, 255);
  analogWrite(MOTEUR_D_PWM, 255);
}
