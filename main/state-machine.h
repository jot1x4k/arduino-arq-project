#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "StateMachineLib.h"
#include "hardware-config.h"
#include "globals.h"

void setupStateMachine(StateMachine &sm);
// Funciones de ENTRADA
void onEnterCONFIG();
void onEnterINICIO();
void onEnterMONITOR_PUERTAS();
void onEnterGESTION();
void onEnterBLOQUEO();
void onEnterMONITOR_AMBIENTAL();
void onEnterALARMA();

// Funciones de SALIDA
void onLeaveCONFIG();
void onLeaveINICIO();
void onLeaveMONITOR_PUERTAS();
void onLeaveGESTION();
void onLeaveBLOQUEO();
void onLeaveMONITOR_AMBIENTAL();
void onLeaveALARMA();

#endif