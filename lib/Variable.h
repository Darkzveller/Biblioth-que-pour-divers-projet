
#include <Arduino.h>

#ifndef Variable_H
#define Variable_H
const int8_t resolution = 8;
#define COEFF_ROUE_GAUCHE 0.99
#define COEFF_ROUE_DROITE 1.03
double compDroit;
double compGauche;


double Kp_moteur = 6.49;
double Kd_moteur = 0.227;

double consigne_angulaire=-0.0328;
double theta;
float raccourci_dt;
// coefficient du filtre
float A, B;
#endif 
