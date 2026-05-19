#include "StateMachine.h"

enum State
{
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

void onEnterCONFIG()
{
	Serial.println("Estado: CONFIG - Cargando EEPROM");
}

void onEnterINICIO()
{
	Serial.println("Estado: INICIO - Teclado/RFID activo");
	claveCorrecta = false;
	sistemaBloqueado = false;
}

void onEnterMONITOR_PUERTAS()
{
	Serial.println("Estado: MONITOR_PUERTAS - Sensores activos");
	contadorPuertas = 0;
	estadoTiempo = millis();
}

void onEnterGESTION()
{
	Serial.println("Estado: GESTION - Configuración");
}

void onEnterBLOQUEO()
{
	Serial.println("Estado: BLOQUEO - Sistema bloqueado");
	estadoTiempo = millis();
	digitalWrite(LED_RED, HIGH);
}

void onEnterMONITOR_AMBIENTAL()
{
	Serial.println("Estado: MONITOR_AMBIENTAL - Sensores ambientales");
	estadoTiempo = millis();
}

void onEnterALARMA()
{
	Serial.println("Estado: ALARMA - Activada");
	estadoTiempo = millis();
	contadorAlarmas++;
	buzzer.on();
}

void setupStateMachine()
{
	stateMachine.AddTransition(CONFIG, INICIO,
							   []
							   { return botonPresionado; });

	stateMachine.AddTransition(INICIO, CONFIG,
							   []
							   { return botonPresionado; });

	stateMachine.AddTransition(INICIO, MONITOR_AMBIENTAL,
							   []
							   { return claveCorrecta; });

	stateMachine.AddTransition(INICIO, BLOQUEO,
							   []
							   { return sistemaBloqueado; });

	stateMachine.AddTransition(MONITOR_PUERTAS, MONITOR_AMBIENTAL,
							   []
							   { return (millis() - estadoTiempo) >= TIEMPO_2_SEC; });

	stateMachine.AddTransition(MONITOR_PUERTAS, ALARMA,
							   []
							   { return contadorPuertas >= 3; });

	stateMachine.AddTransition(MONITOR_AMBIENTAL, MONITOR_PUERTAS,
							   []
							   { return (millis() - estadoTiempo) >= TIEMPO_5_SEC; });

	stateMachine.AddTransition(MONITOR_AMBIENTAL, ALARMA,
							   []
							   { return (temperatura > 24 || luz > 400); });

	stateMachine.AddTransition(ALARMA, MONITOR_AMBIENTAL,
							   []
							   { return (millis() - estadoTiempo) >= TIEMPO_3_SEC; });

	stateMachine.AddTransition(ALARMA, MONITOR_PUERTAS,
							   []
							   { return (millis() - estadoTiempo) >= TIEMPO_4_SEC; });

	stateMachine.AddTransition(ALARMA, GESTION,
							   []
							   { return contadorAlarmas >= 3; });

	stateMachine.AddTransition(GESTION, INICIO,
							   []
							   { return tecla == '*'; });

	stateMachine.AddTransition(BLOQUEO, INICIO,
							   []
							   { return (millis() - estadoTiempo) >= TIEMPO_7_SEC; });

	stateMachine.SetOnEntering(CONFIG, onEnterCONFIG);
	stateMachine.SetOnEntering(INICIO, onEnterINICIO);
	stateMachine.SetOnEntering(MONITOR_PUERTAS, onEnterMONITOR_PUERTAS);
	stateMachine.SetOnEntering(GESTION, onEnterGESTION);
	stateMachine.SetOnEntering(BLOQUEO, onEnterBLOQUEO);
	stateMachine.SetOnEntering(MONITOR_AMBIENTAL, onEnterMONITOR_AMBIENTAL);
	stateMachine.SetOnEntering(ALARMA, onEnterALARMA);
}

unsigned char readInput()
{
	// Lee la entrada del usuario aquí y devuelve un valor que represente la entrada
	return 0; // Placeholder, reemplaza con la lógica de lectura de entrada
}
