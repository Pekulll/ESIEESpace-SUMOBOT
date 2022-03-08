#include "Arduino.h" // bibliothèque pour la gestion de projet Arduino

// Pins provenant du fichier principal
extern const int ULTRASON_ECHO;
extern const int ULTRASON_TRIGGER;   

// Déclarations des fonctions
long ultrason_distance();
