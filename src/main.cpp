#include <Arduino.h>
#include "ServeurWebGyropode.h"
#include "MPU6050.h"
#include "EncoderManager.h"
#include "MOTEUR.h"
#include "ASSERVISSEMENT.h"

MPU6050 MPU;

EncoderManager encoder_droit(33, 32);
EncoderManager encoder_gauche(26, 25);
moteur moteur_droit;
moteur moteur_gauche;

asservissement asser_droit;
asservissement asser_gauche;
asservissement gyro;

char FlagCalcul = 0;
float Te = 7.5;  // période d'échantillonage en ms
float Tau = 400; // constante de temps du filtre en ms
const int8_t resolution = 8;
//Coefficient moteur Droit
int Kp_droit = 3;
int Kd_droit = 0;
double consigne_droit;
//Coefficient moteur Gauche
int Kp_gauche = 3;
int Kd_gauche = 0;
double theta;
float raccourci_dt;
// coefficient du filtre
float A, B;

void controle(void *parameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        theta = MPU.getPosAngulaireRad();

        //    asser_droit.calcul_asserv_gyro(consigne_droit, theta, resolution, Kp, raccourci_dt, Kd, 0, 0, 0.45);
        // float x = 100 * (asser_droit.calcul_asserv_gyro(consigne_droit, theta, resolution, Kp_droit, raccourci_dt, Kd_droit, 0, 0, 0.45));
        // float y = 100 * (asser_gauche.calcul_asserv_gyro(consigne_droit, theta, resolution, Kp_gauche, raccourci_dt, Kd_gauche, 0, 0, 0.45));

        // Serial.printf(" x %f y %f\n", x,y);
        moteur_droit.setSpeed(100 * (asser_droit.calcul_asserv_gyro(consigne_droit, theta, resolution, Kp_droit, raccourci_dt, Kd_droit, 0, 0, 0.45)));
        moteur_gauche.setSpeed(100 * (asser_gauche.calcul_asserv_gyro(consigne_droit, theta, resolution, Kp_gauche, raccourci_dt, Kd_gauche, 0, 0, 0.45)));
        FlagCalcul = 1;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Te));
    }
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.printf("Bonjour \n\r");
    serveur_web_gyropode();
    encoder_droit.init("encoder droite", 65, 712);  // Initialiser les encodeurs
    encoder_gauche.init("encoder gauche", 65, 712); // Initialiser les encodeurs
    moteur_droit.init(19, 18, 0, 20000, resolution, 50, "moteur droit");
    moteur_gauche.init(16, 17, 2, 20000, resolution, 50, "moteur gauche");

    MPU.init();
    pinMode(27, OUTPUT);
    digitalWrite(27, true);
    delay(20000);
    Serial.printf("Go\n");

    xTaskCreate(
        controle,   // nom de la fonction
        "controle", // nom de la tache que nous venons de vréer
        10000,      // taille de la pile en octet
        NULL,       // parametre
        10,         // tres haut niveau de priorite
        NULL        // descripteur
    );

    // calcul coeff filtre
    A = 1 / (1 + Tau / Te);
    B = Tau / Te * A;
}

void loop()
{
    if (FlagCalcul == 1)
    {
        // Serial.printf("%5.1lf %3.1lf \n", Tau, Te);

        FlagCalcul = 0;
    }
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
void serialEvent()
{
    while (Serial.available() > 0) // tant qu'il y a des caractères à lire
    {
        reception(Serial.read());
    }
}