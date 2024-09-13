#include <Arduino.h>
#include "Timer.h"
extern volatile bool timerFlag ;  // Flag déclenché lors de l'interruption


void setup() {
    Serial.begin(115200);
  timer_init(0, 10);
}

void loop() {
  // Si le flag du timer est déclenché, effectuer une action
  if (timerFlag) {
    Serial.println("10 secondes s'est écouler");
    timerFlag = false;  // Réinitialiser le flag
  }

  // Autres tâches possibles dans la boucle principale
}