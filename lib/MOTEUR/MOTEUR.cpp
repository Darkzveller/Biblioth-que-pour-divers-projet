#include "../ID.h" // Remonte d'un niveau pour atteindre lib
#include <math.h>
#include "MOTEUR.h"

int pwmManager[12]{false};

// Définition de la méthode init
void moteur::init(int PINA, int PINB, int channel_pwm, int frequence, int resolution_bits, String Name_Motor)
{
    pinA = PINA;
    pinB = PINB;
    channelA = channel_pwm;
    nameMotor = Name_Motor;
    resolution = resolution_bits;

#ifdef COMMANDE_UNIPOLAIRE
    ledcSetup(channelA, frequence, resolution);
    ledcAttachPin(pinA, channelA);
    ledcWrite(channelA, speed);
    channelB = channelA + 1;
    ledcAttachPin(pinB, channelB);
    ledcWrite(channelA, deltaSpeed);

#endif
#ifdef COMMANDE_BIPOLAIRE

    ledcSetup(channelA, frequence, resolution);
    ledcAttachPin(pinA, channelA);
    ledcWrite(channelA, speed);

    pinMode(pinB, OUTPUT);
    digitalWrite(pinB, false);
#endif

    if (DEBUG_MOTOR)
    {
        Serial.println("Le " + nameMotor + " a été initialisé");
        Serial.println();
    }
}

void moteur::setSpeed(int motorSpeed)
{
    speed = motorSpeed; // Met à jour la vitesse
    deltaSpeed = pow(2, 10) - speed;
#ifdef COMMANDE_UNIPOLAIRE
    ledcWrite(channelA, speed);
    ledcWrite(channelB, deltaSpeed);

#endif

#ifdef COMMANDE_BIPOLAIRE
    ledcWrite(channelA, speed); // Placeholder pour un traitement spécifique

#endif

    if (DEBUG_MOTOR)
    {
        Serial.printf("Le %s a une pwm de %d", nameMotor.c_str(), speed);
        Serial.println();
    }
}
void moteur::stop()
{

#ifdef COMMANDE_UNIPOLAIRE
    speed = pow(2, 10) / 2; // Met à jour la vitesse
    deltaSpeed = pow(2, 10) - speed;
    ledcWrite(channelA, 0);
    ledcWrite(channelB, 4095);
    

#endif

#ifdef COMMANDE_BIPOLAIRE
    speed = 0;
    ledcWrite(channelA, speed); // Placeholder pour un traitement spécifique

#endif

    if (DEBUG_MOTOR)
    {
        Serial.printf("Le %s s'est arrerter", nameMotor.c_str());
        Serial.printf(" La pwm de %d et le delta est ", speed,deltaSpeed);
        Serial.println();
    }
}