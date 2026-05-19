#include "Keypad.h"

#define PIN_TEMP A0;
#define PIN_HALL A1;
#define PIN_LUZ A2;
#define PIN_SONIDO A3;

#define TEMP_HIGH 23
#define TH_HALL 527
#define LIGHT_HIGH 100

int Vo;
float R1 = 10000;
float logR2, R2, temp;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;

short hall;
short luz = 0; 
short sonido;

void sensorSetup() {
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_HALL, INPUT);
  pinMode(PIN_LUZ, INPUT);
  pinMode(PIN_SONIDO, INPUT);
}

float readTemp() {
  Vo = analogRead(PIN_TEMP);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  temp = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)) - 273.15;
  return temp;
}

short readHall() {
  hall = analogRead(PIN_HALL);
  return hall;
}

short readLuz() {
  luz = analogRead(PIN_LUZ);
  return luz;
}

short readSonido() {
  sonido = analogRead(PIN_SONIDO);
  return sonido;
}