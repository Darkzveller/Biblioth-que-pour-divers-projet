#include <Arduino.h>
#include "MPU6050.h"
#include "EncoderManager.h"

MPU6050 jsp;
EncoderManager roue_droit(33, 32);
char FlagCalcul = 0;
float Te = 5;    // période d'échantillonage en ms
float Tau = 100; // constante de temps du filtre en ms

float A, B;

void controle(void *parameters)
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while (1)
  {
   float x= jsp.getPosAngulaireRad();
    //roue_droit.showTickPosition();
    FlagCalcul = 1;
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Te));
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("Bonjour \n\r");
    roue_droit.init("roue droite", 65, 712); // Initialiser les encodeurs
  // Try to initialize!
  jsp.init();
  xTaskCreate(
      controle,   // nom de la fonction
      "controle", // nom de la tache que nous venons de vréer
      10000,      // taille de la pile en octet
      NULL,       // parametre
      10,         // tres haut niveau de priorite
      NULL        // descripteur
  );

 
}

void reception(char ch)
{

  static int i = 0;
  static String chaine = "";
  String commande;
  String valeur;
  int index, length;

  if ((ch == 13) or (ch == 10))
  {
    index = chaine.indexOf(' ');
    length = chaine.length();
    if (index == -1)
    {
      commande = chaine;
      valeur = "";
    }
    else
    {
      commande = chaine.substring(0, index);
      valeur = chaine.substring(index + 1, length);
    }

    if (commande == "Tau")
    {
      Tau = valeur.toInt();
      // calcul coeff filtre
      A = 1 / (1 + Tau / Te);
      B = Tau / Te * A;
    }
    if (commande == "Te")
    {
      Te = valeur.toInt();
    }

    chaine = "";
  }
  else
  {
    chaine += ch;
  }
}

void loop()
{
  if (FlagCalcul == 1)
  {
  //    int i=0;
  //  Serial.print(i++);

    FlagCalcul = 0;
  }
}

 void serialEvent()
{
  while (Serial.available() > 0) // tant qu'il y a des caractères à lire
  {
    reception(Serial.read());
  }
}