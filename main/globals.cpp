#include "globals.h"

float temperatura = 0;
short hall = 0;
short luz = 0;
short sonido = 0;

bool claveCorrecta = false;
bool sistemaBloqueado = false;
bool botonPresionado = false;
bool intruso = false;
int EEAddress = 0;

short contadorPuertas = 0;
short contadorAlarmas = 0;
short contadorKeypad = 0;
char tecla = 0;
char entradaKeypad[4];
unsigned short aciertos = 0;
unsigned short intentos = 0;

char claveKeypad[4] = {'1', '2', '3', 'A'};
byte validKeyRFID[4] = { 0x0A, 0x32, 0xD6, 0x73 };

Servo myservo;
