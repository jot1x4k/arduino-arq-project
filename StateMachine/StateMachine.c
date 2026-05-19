#include "StateMachine.h"

enum State {
	CONFIG = 0,
	INICIO = 1,
	MONITOR_PUERTAS = 2,
	GESTION = 3,
	BLOQUEO = 4,
	MONITOR_AMBIENTAL = 5,
	ALARMA = 6
};

unsigned long estadoTiempo;
bool claveCorrecta = false;
bool sistemaBloqueado = false;
bool botonPresionado = false;
float temperatura = 0;
short luz = 0;
short contadorPuertas = 0;
short contadorAlarmas = 0;
char tecla = '\0';

const unsigned short TIEMPO_2_SEC = 2000;
const unsigned short TIEMPO_3_SEC = 3000;
const unsigned short TIEMPO_4_SEC = 4000;
const unsigned short TIEMPO_5_SEC = 5000;
const unsigned short TIEMPO_7_SEC = 7000;

StateMachine stateMachine(7, 13);

void setupStateMachine() {
	stateMachine.addTransition(INICIO, CONFIG, fnc_boton);
	stateMachine.addTransition(INICIO, BLOQUEO, fnc_bloqueo);
	stateMachine.addTransition(INICIO, MONITOR_AMBIENTAL, fnc_clave);

	stateMachine.AddTransition(CONFIG, INICIO, []() { return input == Backward });

	stateMachine.addTrasition(BLOQUEO, INICIO, []() { return input == Backward });

	stateMachine.addTrasition(MONITOR_AMBIENTAL, MONITOR_PUERTAS, []() { return input == Forward });
	stateMachine.addTrasition(MONITOR_AMBIENTAL, ALARMA, []() { return input == Forward });

	stateMachine.addTrasition(MONITOR_PUERTAS, MONITOR_AMBIENTAL, []() { return input == Backward });
	stateMachine.addTrasition(MONITOR_PUERTAS, ALARMA, []() { return input == Forward });

	stateMachine.addTrasition(ALARMA, MONITOR_PUERTAS, []() { return input == Backward });
	stateMachine.addTrasition(ALARMA, MONITOR_AMBIENTAL, []() { return input == Backward });
	stateMachine.addTrasition(ALARMA, GESTION, []() { return input == Forward });

	stateMachine.addTrasition(GESTION, INICIO, []() { return input == Forward });

	stateMachine.SetOnEntering(INICIO, taskInInicio);
	stateMachine.SetOnEntering(CONFIG, taskInConfig);
	stateMachine.SetOnEntering(BLOQUEO, taskInBloqueo);
	stateMachine.SetOnEntering(MONITOR_AMBIENTAL, taskInAmbiental);
	stateMachine.SetOnEntering(MONITOR_PUERTAS, taskInPuertas);
	stateMachine.SetOnEntering(ALARMA, taskInAlarma);
	stateMachine.SetOnEntering(GESTION, taskInGestion);

	stateMachine.SetOnLeaving(INICIO, taskOutInicio);
	stateMachine.SetOnLeaving(CONFIG, taskOutConfig);
	stateMachine.SetOnLeaving(BLOQUEO, taskOutBloqueo);
	stateMachine.SetOnLeaving(MONITOR_AMBIENTAL, taskOutAmbiental);
	stateMachine.SetOnLeaving(MONITOR_PUERTAS, taskOutPuertas);
	stateMachine.SetOnLeaving(ALARMA, taskOutAlarma);
	stateMachine.SetOnLeaving(GESTION, taskOutGestion);
}

unsigned char readInput() {
  // Lee la entrada del usuario aquí y devuelve un valor que represente la entrada
  return 0; // Placeholder, reemplaza con la lógica de lectura de entrada
}

