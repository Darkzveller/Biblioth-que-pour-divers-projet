// EncoderManager.cpp
#include "../ID.h" // Remonte d'un niveau pour atteindre lib
#include "MPU6050.h"

/* variable pour le calcul de l'angle */
float TetaG, TetaGF, TetaW, Teta, Wteta;

char FlagCalcul = 0;
float Te = 10;    // période d'échantillonage en ms
float Tau = 1000; // constante de temps du filtre en ms

// coefficient du filtre
float A, B;

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

void MPU6050::TEST()
{
    mpu.getEvent(&a, &g, &temp);
    TetaG = atan2(a.acceleration.x, a.acceleration.y);
    TetaGF = A * TetaG + B * TetaGF;
    Wteta = g.gyro.z;
    TetaW = A * (Tau / 1000 * Wteta) + B * TetaW;
    Teta = TetaGF + TetaW;
    // Serial.print(a.acceleration.x);
    // Serial.print(" ");
    // Serial.print(a.acceleration.x);
    // Serial.print(" ");
    // Serial.print(g.gyro.z);
    // Serial.println();

     Serial.print(TetaG);
    Serial.print(" ");
    Serial.print(TetaGF);
    Serial.print(" ");
    Serial.print(TetaW);
    Serial.print(" ");
    Serial.print(Teta);
    Serial.println(" ");
}
