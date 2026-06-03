#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>

char readKeypadInput();
char readKeypadGestion();
char getRawKeypad();
void readRFIDInput();
void updateButtonState();

void lcdPrint(const String &message, short fil, short col, char clear);
void cambiarUmbrales();
void cambiarAcceso();  

#endif