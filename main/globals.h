#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <MFRC522.h>
#include "StateMachineLib.h"
#include "AsyncTaskLib.h"
#include <EEPROM.h>

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
extern bool alarma;
extern int EEAddress;

extern short contadorPuertas;
extern short contadorAlarmas;
extern short contadorKeypad;
extern char tecla;
extern char entradaKeypad[4];
extern unsigned short aciertos;
extern unsigned short intentos;


extern int TEMP_HIGH;
extern int LIGHT_HIGH;

extern unsigned short puertaServo;

extern char claveKeypad[4];
extern byte validKeyRFID[4];

extern short current_id;

struct Perfil
{
    short id;
    String nombre;
    char clave[4];
    byte rfid[4];
    unsigned short horario[2];
};

extern void crearPerfil(const char clave[], const byte rfid[], const unsigned short horario[]);


#endif
