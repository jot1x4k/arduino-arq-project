#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

void sensorSetup();
float readTemp();
short readHall();
short readLuz();
short readSonido();
void blinkLed();

#endif
