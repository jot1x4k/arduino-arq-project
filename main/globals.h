#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <MFRC522.h>
#include "StateMachineLib.h"
#include "AsyncTaskLib.h"

extern LiquidCrystal lcd;
extern Servo myservo;
extern MFRC522 mfrc522;
extern StateMachine stateMachine;

extern float temperatura;
extern short hall;
extern short luz;
extern short sonido;

extern bool claveCorrecta;
extern bool sistemaBloqueado;
extern bool botonPresionado;
extern bool intruso;
extern int EEAddress;

extern short contadorPuertas;
extern short contadorAlarmas;
extern short contadorKeypad;
extern char tecla;
extern char entradaKeypad[4];
extern unsigned short aciertos;
extern unsigned short intentos;

extern unsigned short puertaServo;

extern char claveKeypad[4];
extern byte validKeyRFID[4];

#endif
