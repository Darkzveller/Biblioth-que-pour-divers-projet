#include <Arduino.h>
#include "Timer.h"
extern volatile bool timerFlag ;  // Flag déclenché lors de l'interruption


void setup() {
    Serial.begin(115200);
  timer_init(0, 0.0025);
}

void loop() {
  // Si le flag du timer est déclenché, effectuer une action
  if (timerFlag) {
static int i =0;
i++;
Serial.println(i);    timerFlag = false;  // Réinitialiser le flag
  }

  // Autres tâches possibles dans la boucle principale
}