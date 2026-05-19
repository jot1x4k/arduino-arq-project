#include "StateMachine.h"

enum State
{
	inicio = 0,
	config = 1,
	bloqueo = 2,
	ambiental = 3,
	puertas = 4,
	alarma = 5,
	gestion = 6
};

enum Input
{
	Unknown = 0,
	Forward = 1,
	Backward = 2,
	Reset = 3,
};

StateMachine stateMachine(7, 13);

void setupStateMachine() {
  // Configura la máquina de estados aquí
}

unsigned char readInput() {
  // Lee la entrada del usuario aquí y devuelve un valor que represente la entrada
  return 0; // Placeholder, reemplaza con la lógica de lectura de entrada
}

