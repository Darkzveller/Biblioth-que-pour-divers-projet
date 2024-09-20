#include "../ID.h" // Remonte d'un niveau pour atteindre lib
#include <math.h>
#include "MOTEUR.h"

int pwmManager[12]{false};

// Définition de la méthode init
void moteur::init(int PINA, int PINB, int channel_pwm, int frequence, int resolution_bits,double saturation_pwm_pourcen, String Name_Motor)
{
    saturation_pwm_pourcent=saturation_pwm_pourcen;
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

    if (DEBUG_MOTOR_INIT)
    {
        Serial.println("Le " + nameMotor + " a été initialisé");
        Serial.println();
    }
}

void moteur::setSpeed(int motorSpeed)// En pourcentage
{
    if (motorSpeed >= saturation_pwm_pourcent)
    {
        motorSpeed = saturation_pwm_pourcent;
    }
    if (motorSpeed <= -saturation_pwm_pourcent)
    {
        motorSpeed = saturation_pwm_pourcent;
    }
   
    speed = motorSpeed * (pow(2, resolution) - 1) / saturation_pwm_pourcent;

    // speed = motorSpeed; // Met à jour la vitesse
#ifdef COMMANDE_UNIPOLAIRE

    deltaSpeed = (pow(2, resolution) - 1) - speed;
    ledcWrite(channelA, speed);
    ledcWrite(channelB, deltaSpeed);

#endif

#ifdef COMMANDE_BIPOLAIRE
    ledcWrite(channelA, speed); // Placeholder pour un traitement spécifique

#endif

    if (DEBUG_MOTOR_VITESSE)
    {
        Serial.printf("Le %s pwm = %d Motorspeed %d", nameMotor.c_str(), speed,motorSpeed);
        Serial.println();
    }
}
void moteur::stop()
{

#ifdef COMMANDE_UNIPOLAIRE
    speed = pow(2, resolution) / 2; // Met à jour la vitesse
    deltaSpeed = pow(2, resolution) - speed;
    ledcWrite(channelA, speed);
    ledcWrite(channelB, deltaSpeed);

#endif

#ifdef COMMANDE_BIPOLAIRE
    speed = 0;
    ledcWrite(channelA, speed); // Placeholder pour un traitement spécifique

#endif

    if (DEBUG_MOTOR_STOP)
    {
        Serial.printf("Le %s s'est arrerter", nameMotor.c_str());
        Serial.printf(" La pwm de %d et le delta est %d", speed, deltaSpeed);
        Serial.println();
    }
}