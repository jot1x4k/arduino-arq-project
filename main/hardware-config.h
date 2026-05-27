#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#include <Arduino.h>

#define PIN_TEMP A0
#define PIN_HALL A1
#define PIN_LUZ A2
#define PIN_SONIDO A3
#define LED_RED 10
#define BUZZER_PIN 8
#define BOTON_CONFIG 13
#define PIN_SERVO 7

#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

#define RST_PIN	9
#define SS_PIN	6

#define TEMP_HIGH 23
#define TH_HALL 527
#define LIGHT_HIGH 100
#define SOUND_HIGH 40 

enum State {
  CONFIG = 0,
  INICIO = 1,
  MONITOR_PUERTAS = 2,
  GESTION = 3,
  BLOQUEO = 4,
  MONITOR_AMBIENTAL = 5,
  ALARMA = 6
};

#endif