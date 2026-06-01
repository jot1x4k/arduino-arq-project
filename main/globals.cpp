#include "globals.h"

float temperatura = 0;
short hall = 0;
short luz = 0;
short sonido = 0;

bool claveCorrecta = false;
bool sistemaBloqueado = false;
bool botonPresionado = false;
bool intruso = false;
bool alarma = false;
int EEAddress = 0;

int TEMP_HIGH = 23;
int LIGHT_HIGH = 100;

short contadorPuertas = 0;
short contadorAlarmas = 0;
short contadorKeypad = 0;
char tecla = 0;
char entradaKeypad[4];
unsigned short aciertos = 0;
unsigned short intentos = 0;


unsigned short puertaServo = 0;

char claveKeypad[4] = {'1', '2', '3', 'A'};
byte validKeyRFID[4] = { 0x3A, 0xD4, 0xA8, 0x00 }; 

Servo myservo;
