#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "StateMachineLib.h"
#include "hardware-config.h"
#include "globals.h"

/**
 * @brief Configura la máquina de estados con los estados y transiciones correspondientes.
 * @param sm La instancia de la máquina de estados a configurar.
 */
void setupStateMachine(StateMachine &sm);
// Funciones de ENTRADA

/**
 * @brief Función de entrada para el estado CONFIG - Configuración del sistema.
 */
void onEnterCONFIG();

/**
 * @brief Función de entrada para el estado INICIO - Inicio del sistema.
 */
void onEnterINICIO();

/**
 * @brief Función de entrada para el estado MONITOR_PUERTAS - Monitoreo de puertas.
 */
void onEnterMONITOR_PUERTAS();

/**
 * @brief Función de entrada para el estado GESTION - Gestión del sistema.
 */
void onEnterGESTION();

/**
 * @brief Función de entrada para el estado BLOQUEO - Bloqueo del sistema.
 */
void onEnterBLOQUEO();

/**
 * @brief Función de entrada para el estado MONITOR_AMBIENTAL - Monitoreo ambiental.
 */
void onEnterMONITOR_AMBIENTAL();

/**
 * @brief Función de entrada para el estado ALARMA - Alarma.
 */
void onEnterALARMA();

// Funciones de SALIDA

/**
 * @brief Función de salida para el estado CONFIG - Guardar configuración en EEPROM.
 */
void onLeaveCONFIG();

/**
 * @brief Función de salida para el estado INICIO - Inicio del sistema.
 */
void onLeaveINICIO();

/**
 * @brief Función de salida para el estado MONITOR_PUERTAS - Monitoreo de puertas.
 */
void onLeaveMONITOR_PUERTAS();

/**
 * @brief Función de salida para el estado GESTION - Gestión del sistema.
 */
void onLeaveGESTION();

/**
 * @brief Función de salida para el estado BLOQUEO - Bloqueo del sistema.
 */
void onLeaveBLOQUEO();

/**
 * @brief Función de salida para el estado MONITOR_AMBIENTAL - Monitoreo ambiental.
 */
void onLeaveMONITOR_AMBIENTAL();

/**
 * @brief Función de salida para el estado ALARMA - Alarma.
 */
void onLeaveALARMA();

/**
 * @brief Función para crear un nuevo perfil de usuario a través del estado CONFIG.
 */
void configCrear();

/**
 * @brief Función para mostrar la configuración actual a través del estado CONFIG.
 */
void configMostrar();

#endif