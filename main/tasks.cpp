#include "tasks.h"
#include "inputs.h"
#include "sensors.h"

// Tareas de temporizacion
AsyncTask task_100_ms(100, false);
AsyncTask task_200_ms(200, false);
AsyncTask task_300_ms(300, false);
AsyncTask task_700_ms(700, false);

AsyncTask task_2_sec(2000, false);
AsyncTask task_3_sec(3000, false);
AsyncTask task_4_sec(4000, false);
AsyncTask task_5_sec(5000, false);
AsyncTask task_7_sec(7000, false);

// Tarea de lectura de entradas
AsyncTask task_read_RFID(100, true, [](){ readRFIDInput(); });
AsyncTask task_read_keypad(100, true, [](){ readKeypadInput(); });
AsyncTask task_read_keypad_gestion(100, true, [](){ readKeypadGestion(); });
AsyncTask task_read_temp(100, true, [](){ readTemp(); });
AsyncTask task_read_luz(100, true, [](){ readLuz(); });
AsyncTask task_read_sonido(100, true, [](){ readSonido(); });
AsyncTask task_read_hall(100, true, [](){ readHall(); });

//Tareas output
AsyncTask task_blink_led(1000, true, blinkLed);

// Funciones contadoras de tiempo
bool contar100ms() {
  return task_100_ms.GetElapsedTime() >= task_100_ms.Interval;
}

bool contar300ms() {
  return task_300_ms.GetElapsedTime() >= task_300_ms.Interval;
}

bool contar700ms() {
  return task_700_ms.GetElapsedTime() >= task_700_ms.Interval;
}

bool contar2Segundos() {
  return task_2_sec.GetElapsedTime() >= task_2_sec.Interval;
}

bool contar3Segundos() {
  return task_3_sec.GetElapsedTime() >= task_3_sec.Interval;
}

bool contar4Segundos() {
  return task_4_sec.GetElapsedTime() >= task_4_sec.Interval;
}

bool contar5Segundos() {
  return task_5_sec.GetElapsedTime() >= task_5_sec.Interval;
}

bool contar7Segundos() {
  return task_7_sec.GetElapsedTime() >= task_7_sec.Interval;
}
