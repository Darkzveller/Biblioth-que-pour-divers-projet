#include <Arduino.h>
#include <MOTEUR.h>

moteur moteurDroit;

void setup() {

Serial.begin(115200);

moteurDroit.init(19,18,0,19500,12,"Moteur Droit");

}

void loop() {
    moteurDroit.setSpeed(3050);
    delay(5000);
    moteurDroit.setSpeed(4096);
    delay(5000);
    moteurDroit.stop();
    delay(5000);

}

