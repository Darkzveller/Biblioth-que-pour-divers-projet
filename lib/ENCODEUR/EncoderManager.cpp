// EncoderManager.cpp
#include "../ID.h" // Remonte d'un niveau pour atteindre lib
#include "EncoderManager.h"

// Constructeur avec les pins des encodeurs gauche et droit
EncoderManager::EncoderManager(int pinA, int pinB)
    : pinA(pinA), pinB(pinB) {}

// Méthode pour initialiser les encodeurs
void EncoderManager::init(String nameEncoder, float wheel_size_mm, float TIC_ONE_TOUR)
{
    nameEncodeur = nameEncoder;
    TIC_UN_TOUR = TIC_ONE_TOUR;
    ESP32Encoder::useInternalWeakPullResistors = UP; // Utilise les résistances internes
    encoder.attachHalfQuad(pinA, pinB);
    encoder.clearCount();

    if (DEBUG_ENCODEUR)
    {
        // Serial.printf(" %s à été initialisé", nameEncodeur.c_str());
        // Serial.println();
    }
}

// Méthode pour obtenir la position de l'encodeur gauche
long EncoderManager::getTickPosition()
{
    long val_encodeur = encoder.getCount();
    if (DEBUG_ENCODEUR_TICK)
    {
        Serial.printf("Le %s a pour valeur actuelle %d", nameEncodeur.c_str(), val_encodeur);
        Serial.println();
    }
    return val_encodeur;
}

float EncoderManager::getDistance()
{
    long val_tick = EncoderManager::getTickPosition();
    float number_tour = val_tick * 1.0 / TIC_UN_TOUR;
    if (DEBUG_ENCODEUR_TOUR)
    {
        Serial.printf("Le %s a fait %f tour", nameEncodeur.c_str(), number_tour);
        Serial.println();
    }
    return number_tour;
}

// Méthode pour réinitialiser la position de l'encodeur gauche
void EncoderManager::resetPosition()
{
    encoder.clearCount();
    if (DEBUG_ENCODEUR)
    {
        Serial.printf(" %s à été reset", nameEncodeur.c_str());
        Serial.println();
    }
}
