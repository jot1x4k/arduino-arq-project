#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>

char readKeypadInput();
char readKeypadGestion();
void readRFIDInput();
void updateButtonState();

void lcdPrint(const String &message, char pos, char clear);
void cambiarUmbrales();
void cambiarAcceso();  

#endif