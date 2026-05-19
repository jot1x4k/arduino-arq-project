#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

void sensorSetup();
float readTemp();
short readHall();
short readLuz();
short readSonido();

