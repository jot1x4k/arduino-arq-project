#include "state-machine.h"
#include "inputs.h"
#include "sensors.h"
#include "tasks.h"
#include <EEPROM.h>

// Funciones de ENTRADA
void onEnterCONFIG() { 
  Serial.println("Estado: CONFIG - Cargando EEPROM"); 
}

void onEnterINICIO() {
  tecla = 0;
  contadorKeypad = 0;
  Serial.println("Estado: INICIO - Teclado/RFID activo");
  claveCorrecta = false;
  sistemaBloqueado = false;

  task_read_RFID.Start();
  task_read_keypad.Start();

  Serial.print("Ingrese clave: ");
}

void onEnterMONITOR_PUERTAS() {
  Serial.println("Estado: MONITOR_PUERTAS - Sensores activos");
  task_2_sec.Start();
  task_read_hall.Start();
  task_read_sonido.Start();
  contadorPuertas++;
  Serial.println("Se ha entrado a puertas " + String(contadorPuertas) + " veces");

  while (!contar2Segundos()) {
    task_read_hall.Update();
    task_read_sonido.Update();

    if (contadorPuertas > 3 && sonido > SOUND_HIGH) {
      intruso = true;
      break;
    }
  }

}

void onEnterGESTION() { 
  Serial.println("Estado: GESTION - Configuración"); 
  tecla = 0;
  task_read_keypad.Start();
}

void onEnterBLOQUEO()
{
  Serial.println("Estado: BLOQUEO - Sistema bloqueado");
  task_blink_led.Start();
  task_7_sec.Start();

  while(!contar7Segundos()) {
    task_blink_led.Update();
  }
}

void onEnterMONITOR_AMBIENTAL()
{
  Serial.println("Estado: MONITOR_AMBIENTAL - Sensores ambientales");
  task_5_sec.Start();
  task_read_temp.Start();
  task_read_luz.Start();

  while (!contar5Segundos()) {
    task_read_luz.Update();
    task_read_temp.Update();
  }
}

void onEnterALARMA()
{
  Serial.println("Estado: ALARMA - Activada");
  contadorPuertas = 0;
  contadorAlarmas++;
  Serial.println("INTRUSO ES: " + String(intruso));
  if (intruso) {
    task_4_sec.Start();
    Serial.println("Viniendo de PUERTAS - CONTAR 4 SEGUNDOS");
  } else {
    task_3_sec.Start();
    Serial.println("Viniendo de AMBIENTAL - CONTAR 3 SEGUNDOS");
  }
  tone(BUZZER_PIN, 1000, 500);
  task_blink_led.Start();
}

// Funciones de SALIDA
void onLeaveCONFIG() { 
  Serial.println("Saliendo de CONFIG - Guardando en EEPROM"); 
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEAddress + i, claveKeypad[i]);
  }
}

void onLeaveINICIO() {
  contadorKeypad = 0;
  Serial.println("Saliendo de INICIO - Desactivando entradas");
  task_read_RFID.Stop();
  task_read_keypad.Stop();
}

void onLeaveMONITOR_PUERTAS() { 
  Serial.println("Saliendo de MONITOR_PUERTAS"); 
  task_read_hall.Stop();
  task_read_sonido.Stop();
}

void onLeaveGESTION() { 
  Serial.println("Saliendo de GESTION"); 
  task_read_keypad.Stop();
}

void onLeaveBLOQUEO() { 
  Serial.println("Saliendo de BLOQUEO - Reiniciando sistema"); 
  sistemaBloqueado = false;
}
void onLeaveMONITOR_AMBIENTAL() { 
  Serial.println("Saliendo de MONITOR_AMBIENTAL"); 
  task_read_temp.Stop();
  task_read_luz.Stop();
}

void onLeaveALARMA() { 
  Serial.println("Saliendo de ALARMA - Desactivando alarma"); 
  intruso = false;
  noTone(BUZZER_PIN);
}

// Configuración
void setupStateMachine(StateMachine &sm)
{
  sm.AddTransition(CONFIG, INICIO, [] { return botonPresionado; });

  sm.AddTransition(INICIO, CONFIG, [] {
    if (tecla == '#') {
      tecla = 0;
      return true;
    }
    return false;
  });
  sm.AddTransition(INICIO, MONITOR_AMBIENTAL, [] { return claveCorrecta; });
  sm.AddTransition(INICIO, BLOQUEO, [] { return sistemaBloqueado; });

  sm.AddTransition(MONITOR_PUERTAS, ALARMA, [] { return intruso; }); 
  sm.AddTransition(MONITOR_PUERTAS, MONITOR_AMBIENTAL,[] { return contar2Segundos(); });

  sm.AddTransition(MONITOR_AMBIENTAL, ALARMA, [] { return (temperatura > 24 && luz < 800); });
  sm.AddTransition(MONITOR_AMBIENTAL, MONITOR_PUERTAS, [] { return contar5Segundos(); });

  sm.AddTransition(ALARMA, MONITOR_AMBIENTAL, [] { return !intruso && contar3Segundos(); });
  sm.AddTransition(ALARMA, MONITOR_PUERTAS, [] { return intruso && contar4Segundos(); });
  sm.AddTransition(ALARMA, GESTION, [] { return contadorAlarmas >= 3; });

  sm.AddTransition(GESTION, INICIO, [] {
    if (tecla == '*') {
      tecla = 0;
      return true;
    }
    return false;
  });
  
  sm.AddTransition(BLOQUEO, INICIO, [] { return contar7Segundos(); });

  sm.SetOnEntering(CONFIG, onEnterCONFIG);
  sm.SetOnEntering(INICIO, onEnterINICIO);
  sm.SetOnEntering(MONITOR_PUERTAS, onEnterMONITOR_PUERTAS);
  sm.SetOnEntering(GESTION, onEnterGESTION);
  sm.SetOnEntering(BLOQUEO, onEnterBLOQUEO);
  sm.SetOnEntering(MONITOR_AMBIENTAL, onEnterMONITOR_AMBIENTAL);
  sm.SetOnEntering(ALARMA, onEnterALARMA);

  sm.SetOnLeaving(CONFIG, onLeaveCONFIG);
  sm.SetOnLeaving(INICIO, onLeaveINICIO);
  sm.SetOnLeaving(MONITOR_PUERTAS, onLeaveMONITOR_PUERTAS);
  sm.SetOnLeaving(GESTION, onLeaveGESTION);
  sm.SetOnLeaving(BLOQUEO, onLeaveBLOQUEO);
  sm.SetOnLeaving(MONITOR_AMBIENTAL, onLeaveMONITOR_AMBIENTAL);
  sm.SetOnLeaving(ALARMA, onLeaveALARMA);
}
