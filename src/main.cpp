#include <Arduino.h>
#include "MOTEUR.h"
#include "EncoderManager.h"

moteur moteurDroit;
// Définition des pins pour les encodeurs gauche et droit
const int pinRightA = 33;
const int pinRightB = 32;

// Création d'une instance de la classe EncoderManager
EncoderManager roue_droit(pinRightA, pinRightB);

void setup()
{
    Serial.begin(115200);
    moteurDroit.init(19, 18, 0, 19500, 12, "Moteur Droit");

    roue_droit.init("roue droite", 65, 712); // Initialiser les encodeurs
}

void loop()
{
    // Lire et afficher les positions des encodeurs
    // float x = roue_droit.getDistance();
    // Serial.println(x);
    if (roue_droit.getDistance() > 5)
    {
        moteurDroit.stop();
    }
    else
    {
        // moteurDroit.setSpeed(4096);
    }
}
