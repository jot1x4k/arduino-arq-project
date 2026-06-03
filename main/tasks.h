#ifndef TASKS_H
#define TASKS_H

#include "AsyncTaskLib.h"

/**
 * @brief Tarea asíncrona para ejecutar cada 100 ms.
 */
extern AsyncTask task_100_ms;

/**
 * @brief Tarea asíncrona para ejecutar cada 200 ms.
 */
extern AsyncTask task_200_ms;

/**
 * @brief Tarea asíncrona para ejecutar cada 300 ms.
 */
extern AsyncTask task_300_ms;

/**
 * @brief Tarea asíncrona para ejecutar cada 700 ms.
 */
extern AsyncTask task_700_ms;

/**
 * @brief Tarea asíncrona para ejecutar cada 2 segundos.
 */
extern AsyncTask task_2_sec;
/**
 * @brief Tarea asíncrona para ejecutar cada 3 segundos.
 */
extern AsyncTask task_3_sec;
/**
 * @brief Tarea asíncrona para ejecutar cada 4 segundos.
 */
extern AsyncTask task_4_sec;
/**
 * @brief Tarea asíncrona para ejecutar cada 5 segundos.
 */
extern AsyncTask task_5_sec;
/**
 * @brief Tarea asíncrona para ejecutar cada 7 segundos.
 */
extern AsyncTask task_7_sec;

/**
 * @brief Tarea asíncrona para leer el RFID.
 */
extern AsyncTask task_read_RFID;
/**
 * @brief Tarea asíncrona para leer el teclado.
 */
extern AsyncTask task_read_keypad;
/**
 * @brief Tarea asíncrona para leer el teclado de gestión.
 */
extern AsyncTask task_read_keypad_gestion;
/**
 * @brief Tarea asíncrona para leer la temperatura.
 */
extern AsyncTask task_read_temp;
/**
 * @brief Tarea asíncrona para leer la luz.
 */
extern AsyncTask task_read_luz;
/**
 * @brief Tarea asíncrona para leer el sonido.
 */
extern AsyncTask task_read_sonido;
/**
 * @brief Tarea asíncrona para leer el sensor de puerta.
 */
extern AsyncTask task_read_hall;

/**
 * @brief Tarea asíncrona para parpadear el LED.
 */
extern AsyncTask task_blink_led;
/**
 * @brief Tarea asíncrona para emitir una alarma.
 */
extern AsyncTask task_buzz_alarma;  

/**
 * @brief Funcion para contar intervalo de 100 ms.
 * @return true si la tarea de contar 100 ms ha terminado, false en caso contrario.
 */
bool contar100ms();
/**
 * @brief Funcion para contar intervalo de 200 ms.
 * @return true si la tarea de contar 200 ms ha terminado, false en caso contrario
 */
bool contar200ms();
/**
 * @brief Funcion para contar intervalo de 300 ms.
 * @return true si la tarea de contar 300 ms ha terminado, false en caso contrario
 */
bool contar300ms();
/**
 * @brief Funcion para contar intervalo de 700 ms.
 * @return true si la tarea de contar 700 ms ha terminado, false en caso contrario
 */
bool contar700ms();
/**
 * @brief Funcion para contar intervalo de 2 segundos.
 * @return true si la tarea de contar 2 segundos ha terminado, false en caso contrario
 */
bool contar2Segundos();
/**
 * @brief Funcion para contar intervalo de 3 segundos.
 * @return true si la tarea de contar 3 segundos ha terminado, false en caso contrario
 */
bool contar3Segundos();
/**
 * @brief Funcion para contar intervalo de 4 segundos.
 * @return true si la tarea de contar 4 segundos ha terminado, false en caso contrario
 */
bool contar4Segundos();
/**
 * @brief Funcion para contar intervalo de 5 segundos.
 * @return true si la tarea de contar 5 segundos ha terminado, false en caso contrario
 */
bool contar5Segundos();
/**
 * @brief Funcion para contar intervalo de 7 segundos.
 * @return true si la tarea de contar 7 segundos ha terminado, false en caso contrario
 */
bool contar7Segundos(); 

#endif