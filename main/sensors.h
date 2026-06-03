#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

/**
 * @brief Configura los pines de los sensores y actuadores.
 */
void sensorSetup();

/**
 * @brief Lee la temperatura del sensor y la muestra en el LCD.
 * @return La temperatura leída en grados Celsius.
 */
float readTemp();

/**
 * @brief Lee el valor del sensor de efecto Hall y lo muestra en el LCD.
 * @return El valor leído del sensor de efecto Hall.
 */
short readHall();

/**
 * @brief Lee el valor del sensor de luz y lo muestra en el LCD.
 * @return El valor leído del sensor de luz.
 */
short readLuz();

/**
 * @brief Lee el valor del sensor de sonido y lo muestra en el LCD.
 * @return El valor leído del sensor de sonido.
 */
short readSonido();

/**
 * @brief Hace parpadear el LED.
 */
void blinkLed();

/**
 * @brief Rota el servo en la dirección especificada.
 * @param direction La dirección en la que se desea rotar el servo.
 */
void rotateServo(unsigned short direction);

/**
 * @brief Emite una señal acústica desde el buzzer.
 */
void buzzAlarma();

#endif
