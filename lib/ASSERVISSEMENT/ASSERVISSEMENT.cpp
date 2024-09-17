#include "ASSERVISSEMENT.h"

double asservissement::calcul_asserv(double consigne, double observation, int resolution_pwm_bits, double coeff_P, double dt, double coeff_D, double coeff_I, double integral_limit)
{
  double sortie;

  resolution_pwm_bits = pow(2, resolution_pwm_bits) - 1;
  double erreur = consigne - observation;

  double proportionnel = erreur * coeff_P;

  double deriver = coeff_D * (erreur - erreur_prec) / dt;

  somme_integral += erreur * dt;
  if (somme_integral > integral_limit)
  {
    somme_integral = integral_limit;
  }
  else if (somme_integral < -integral_limit)
  {
    somme_integral = -integral_limit;
  }
  double integral = coeff_I * somme_integral;

  double commande = proportionnel + deriver + integral;

  erreur_prec = erreur;

  if (commande > 0)
  {
    if (commande > resolution_pwm_bits)
    {
      sortie = resolution_pwm_bits;
    }
  }
  else
  {
    if (commande < -resolution_pwm_bits)
    {
      sortie = -resolution_pwm_bits;
    }
  }

  return sortie;
}
/*  void asservissement(float position)
  {
    float commande;

    commande = 1 * position + 1 * (position - positionAncienne);
    positionAncienne = position;

    if (commande > 0)
    {
      if (commande > PWM)
      {
        commande = PWM;
      }
      ledcWrite(canal_moteur_droit, PWM);
      ledcWrite(canal_moteur_gauche, PWM - commande);
    }
    else
    {
      if (commande < -PWM)
      {
        commande = -PWM;
      }
      ledcWrite(canal_moteur_droit, PWM + commande);
      ledcWrite(canal_moteur_gauche, PWM);
    }
  }
*/