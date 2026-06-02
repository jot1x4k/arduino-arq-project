#include "state-machine.h"
#include "inputs.h"
#include "sensors.h"
#include "tasks.h"
#include <EEPROM.h>
#include <stdlib.h>

// Funciones de ENTRADA
void onEnterCONFIG() { 
  Serial.println("Estado: CONFIG - Cargando EEPROM"); 
  lcdPrint("E: CONFIG", 0, 0, true);

  Serial.println("A. Crear perfil");
  Serial.println("B. Mostrar perfiles");

  lcdPrint("A: Crear ", 1, 0, false);
  lcdPrint("B: Mostrar", 1, 10, false);

  char key = 0;
  while (true) {
    key = readKeypadGestion();
    if (key) {
      break;
    }
  }

  switch (key)
  {
    case 'A':
      configCrear();
    break;
    case 'B':
      configMostrar();
    break;

    default:
      Serial.println("Opción no válida");
      stateMachine.SetState(CONFIG, false, true);
      break;
  }
  
}

void onEnterINICIO() {
  tecla = 0;
  contadorKeypad = 0;

  claveCorrecta = false;
  sistemaBloqueado = false;

  task_read_RFID.Start();
  task_read_keypad.Start();

  Serial.println("Estado: INICIO - Teclado/RFID activo");
  lcdPrint("E: INICIO", 0, 0, true);
  lcdPrint("Dig. Clave/RFID", 1, 0, false);
}

void onEnterMONITOR_PUERTAS() {
  task_2_sec.Start();
  task_read_hall.Start();
  task_read_sonido.Start();
  contadorPuertas++;

  Serial.println("Estado: MONITOR_PUERTAS - Sensores activos");
  lcdPrint("E: MONITOR_PUERTAS", 0, 0, true);

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
  contadorAlarmas = 0;

  Serial.println("A. Cambiar Umbrales");
  Serial.println("B. Cambiar Acceso");
  Serial.println("*. Volver a INICIO");
  Serial.println("Ingrese opción: ");

  lcdPrint("E: GESTION A:Umbrales", 0, 0, true);
  lcdPrint("B:Acceso *:Inicio", 1, 0, false);

  task_read_keypad_gestion.Start();
  char key = 0;
  while (true) {
    key = readKeypadGestion();
    if (key) {
      break;
    }
  }
  tecla = key;
  Serial.println();

  switch (tecla)
  {
  case 'A':
      cambiarUmbrales();
      stateMachine.SetState(GESTION, false, true);
    break;

  case 'B':
      cambiarAcceso();
      stateMachine.SetState(GESTION, false, true);
    break;
  
  case '*':
    Serial.println("Volviendo a INICIO");
    break;
  
  default:
    Serial.println("Opción no válida");
    stateMachine.SetState(GESTION, false, true);
    break;
  }
}

void onEnterBLOQUEO()
{
  Serial.println("Estado: BLOQUEO - Sistema bloqueado");
  lcdPrint("E: BLOQUEO", 0, 0, true);
  lcdPrint("Espere...", 1, 0, false);

  task_blink_led.Start();
  task_7_sec.Start();
  rotateServo(180);

  while(!contar7Segundos()) {
    task_blink_led.Update();
  }
}

void onEnterMONITOR_AMBIENTAL()
{
  Serial.println("Estado: MONITOR_AMBIENTAL - Sensores ambientales");
  lcdPrint("E: MONITOR AMBIENTAL", 0, 0, true);

  task_5_sec.Start();
  task_read_temp.Start();
  task_read_luz.Start();


  if(puertaServo == 0) {
    rotateServo(0);
  }

  task_3_sec.Start();


  while (!contar5Segundos()) {
    task_read_luz.Update();
    task_read_temp.Update();
    if(contar3Segundos() && puertaServo == 0) {
    rotateServo(180);
    rotateServo(180);
    puertaServo++;
    }
  }
}

void onEnterALARMA()
{
  Serial.println("Estado: ALARMA - Activada");
  lcdPrint("E: ALARMA", 0, 0, true);
  contadorPuertas = 0;
  contadorAlarmas++;
  alarma = true;

  lcdPrint("ALAR: " + String(contadorAlarmas), 1, 0, false);

  task_buzz_alarma.Start();
  task_blink_led.Start();

  if (intruso) {
    task_4_sec.Start();
    Serial.println("Viniendo de PUERTAS - CONTAR 4 SEGUNDOS");
    while (!contar4Segundos()) {
      task_buzz_alarma.Update();
      task_blink_led.Update();
    }
  } else {
    task_3_sec.Start();
    Serial.println("Viniendo de AMBIENTAL - CONTAR 3 SEGUNDOS");
    while (!contar3Segundos()) {
      task_buzz_alarma.Update();
      task_blink_led.Update();
    }
  }
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
  task_read_keypad_gestion.Stop();
}

void onLeaveBLOQUEO() { 
  Serial.println("Saliendo de BLOQUEO - Reiniciando sistema"); 
  sistemaBloqueado = false;

  rotateServo(0);
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

  sm.AddTransition(MONITOR_AMBIENTAL, ALARMA, [] { return (temperatura < TEMP_HIGH && luz > LIGHT_HIGH); });
  sm.AddTransition(MONITOR_AMBIENTAL, MONITOR_PUERTAS, [] { return contar5Segundos(); });

  sm.AddTransition(ALARMA, GESTION, [] { return contadorAlarmas >= 3; });
  sm.AddTransition(ALARMA, MONITOR_AMBIENTAL, [] { return !intruso && contar3Segundos() && contadorAlarmas < 3; });
  sm.AddTransition(ALARMA, MONITOR_PUERTAS, [] { return intruso && contar4Segundos() && contadorAlarmas < 3; });

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

void configCrear() {
  Serial.println("Función configCrear - Crear nuevo perfil");
  Serial.println("Ingrese clave de 4 dígitos: ");
  char clave[4];
  int idx = 0;
  while (idx < 4) {
    char key = readKeypadGestion();
    if (key) {
      if (key >= '0' && key <= '9') {
        clave[idx] = key;
        Serial.print(key);
        lcdPrint("Clave: ****", 1, 0, false);
        idx++;
      }
    }
  }

  Serial.println("\nAcerque RFID para registrar: ");
  byte rfid[4];
  while (true) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        rfid[i] = mfrc522.uid.uidByte[i];
        Serial.print(rfid[i], HEX);
        Serial.print(" ");
      }
      break;
    }
  }

  Serial.println("\nIngrese horario permitido (HHMM-HHMM): ");
  unsigned short horario[2];
  for (int i = 0; i < 2; i++) {
    char timeStr[5] = {0};
    int j = 0;
    while (j < 4) {
      char key = readKeypadGestion();
      if (key) {
        if (key >= '0' && key <= '9') {
          timeStr[j] = key;
          Serial.print(key);
          lcdPrint((i == 0 ? "Inicio: " : "Fin: ") + String(timeStr), 1, 0, false);
          j++;
        }
      }
    }
    horario[i] = atoi(timeStr);
  }

  crearPerfil(clave, rfid, horario);
}

void configMostrar() {
  Serial.println("Función configMostrar - Mostrar perfiles registrados");
  int numPerfiles = current_id; 
  for (int i = 0; i < numPerfiles; i++) {
    Perfil p;
    int address = i * sizeof(Perfil);
    EEPROM.get(address, p);
    Serial.println("ID: " + String(p.id) + " Nombre: " + p.nombre);
    Serial.print("Clave: ");
    for (int j = 0; j < 4; j++) {
      Serial.print(p.clave[j]);
    }
    Serial.println();
    Serial.print("RFID: ");
    for (int j = 0; j < 4; j++) {
      Serial.print(p.rfid[j], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println("Horario: " + String(p.horario[0]) + "-" + String(p.horario[1]));
    Serial.println("-----------------------");
  }
}