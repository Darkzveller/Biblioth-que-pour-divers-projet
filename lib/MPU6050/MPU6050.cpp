// EncoderManager.cpp
#include "../ID.h" // Remonte d'un niveau pour atteindre lib
#include "MPU6050.h"

/* variable pour le calcul de l'angle */
float TetaG, TetaGF, TetaW, Teta, Wteta;

extern float Te ;    // période d'échantillonage en ms
extern float Tau; // constante de temps du filtre en ms

// coefficient du filtre
extern float A, B;

// Méthode pour initialiser les encodeurs
void MPU6050::init()
{
    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    A = 1 / (1 + Tau / Te);
    B = Tau / Te * A;
}

double MPU6050::getPosAngulaireRad()
{
double result = 0;
    mpu.getEvent(&a, &g, &temp);
    // Calcul de théta a l'aide de l'accélération mesurer
    // Javais mis un -1 mais la on dirati que ca fonctionne de la meme maniere
    TetaG = (1) * atan2(a.acceleration.y, a.acceleration.x); // Permet de calculer l'angle Théta G avec un angle dans la valeur est entier relatif
                                                              // TetaG = atan2(a.acceleration.x, a.acceleration.y);
    // Calcul du Théta filtrer
    TetaGF = A * TetaG + B * TetaGF;
    // TetaGF = A * TetaG + B * TetaGF;

    // Calcul de théta a l'aide de la valeur mesurer par le gyroscope
    Wteta = g.gyro.z * Tau * 1e-3;
    // Wteta = g.gyro.z;

    // Calcul du thétaW filtrer
    TetaW = A * Wteta + B * TetaW;
    // TetaW = A * (Tau / 1000 * Wteta) + B * TetaW;

    // Calcule de la somme permettant d'avoir un passe bande filtrer
    Teta = TetaGF + TetaW;
    // Teta = TetaGF + TetaW;

    // Serial.print(TetaG);
    // Serial.print(" ");
    // Serial.print(TetaGF);
    // Serial.print(" ");
    // Serial.print(TetaW);
    // Serial.print(" ");
    Serial.print(Teta);
    Serial.println(" radians   ");
    // Serial.println(" ");
     return result =Teta;
}
void MPU6050::getPosAngulaireDegres()
{
    double val_rad = getPosAngulaireRad();
    double degres = val_rad * 180.0 / 3.14;
    Serial.printf(" degres ");
    Serial.print( getPosAngulaireRad());
    // return degres;
}
