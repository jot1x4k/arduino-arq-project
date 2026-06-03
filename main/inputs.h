#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>

/**
 * @brief Lee la entrada del keypad y verifica la clave en ingreso INICIO.
 * @return La tecla presionada en el keypad.
 */
char readKeypadInput();

/**
 * @brief Lee la entrada del keypad en el modo de gestión.
 * @return La tecla presionada en el keypad.
 */
char readKeypadGestion();

/**
 * @brief Obtiene la tecla presionada en el keypad sin procesar.
 * @return La tecla presionada en el keypad.
 */
char getRawKeypad();

/**
 * @brief Lee la entrada del RFID, verifica el código y actualiza el estado del sistema.
 * @return true si el código es válido, false en caso contrario.
 */
void readRFIDInput();

/**
 * @brief Actualiza el estado del botón de configuración.
 * @return true si el botón está presionado, false en caso contrario.
 */
void updateButtonState();

/**
 * @brief Imprime un mensaje en el LCD en la posición especificada, con opción de limpiar la pantalla.
 * @param message El mensaje a imprimir.
 * @param fil La fila en la que se imprimirá el mensaje.
 * @param col La columna en la que se imprimirá el mensaje.
 * @param clear Si es true, limpia la pantalla antes de imprimir el mensaje.
 */
void lcdPrint(const String &message, short fil, short col, char clear);

/**
 * @brief Permite cambiar los umbrales de temperatura y luz a través del keypad.
 * El usuario ingresa los nuevos valores y el sistema los actualiza.
 */
void cambiarUmbrales();

/**
 * @brief Permite cambiar el acceso a través del keypad.
 * El usuario ingresa los nuevos valores y el sistema los actualiza.
 */
void cambiarAcceso();  

#endif