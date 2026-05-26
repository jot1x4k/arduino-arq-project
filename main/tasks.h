#ifndef TASKS_H
#define TASKS_H

#include "AsyncTaskLib.h"

extern AsyncTask task_100_ms;
extern AsyncTask task_200_ms;
extern AsyncTask task_300_ms;
extern AsyncTask task_700_ms;

extern AsyncTask task_2_sec;
extern AsyncTask task_3_sec;
extern AsyncTask task_4_sec;
extern AsyncTask task_5_sec;
extern AsyncTask task_7_sec;

extern AsyncTask task_read_RFID;
extern AsyncTask task_read_keypad;
extern AsyncTask task_read_temp;
extern AsyncTask task_read_luz;
extern AsyncTask task_read_sonido;
extern AsyncTask task_read_hall;

extern AsyncTask task_blink_led;

bool contar300ms();
bool contar700ms();
bool contar2Segundos();
bool contar3Segundos();
bool contar4Segundos();
bool contar5Segundos();
bool contar7Segundos();

#endif