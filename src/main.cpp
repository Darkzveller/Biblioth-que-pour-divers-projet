#include <Arduino.h>
#include "MOTEUR.h"
#include "EncoderManager.h"
float Te = 10;
moteur moteurDroit;
moteur moteurGauche;

// Définition des pins pour les encodeurs gauche et droit
const int pinRightA = 33;
const int pinRightB = 32;

// Création d'une instance de la classe EncoderManager
EncoderManager roue_droit(33, 32);
EncoderManager roue_gauche(26, 25);

void setup()
{
    Serial.begin(115200);
    moteurDroit.init(19, 18, 0, 19500, 12, "Moteur Droit");
    moteurGauche.init(16, 17, 2, 19500, 12, "Moteur Gauche");

    roue_droit.init("roue droite", 65, 712); // Initialiser les encodeurs
        roue_gauche.init("roue gauche", 65, 712); // Initialiser les encodeurs

}

void loop()
{
    moteurGauche.setSpeed(100);
    moteurDroit.setSpeed(100);
    roue_droit.showTickPosition();
    roue_gauche.showTickPosition();
    delay(1000);

}
