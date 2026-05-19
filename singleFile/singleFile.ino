// ============================================================
//  INCLUDES Y DEPENDENCIAS
// ============================================================
#include "StateMachineLib.h"
#include <Keypad.h>
#include "AsyncTaskLib.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

// ============================================================
//  DEFINICIONES DE HARDWARE
// ============================================================
#define PIN_TEMP A0

#define PIN_HALL A1

#define PIN_LUZ A2

#define PIN_SONIDO A3

#define LED_RED 10

#define BUZZER_PIN 11

#define BOTON_CONFIG 12

#define rs 12
#define en 11
#define d4 5
#define d5 4
#define d6 3
#define d7 2

#define RST_PIN	9
#define SS_PIN	53

#define TEMP_HIGH 23
#define TH_HALL 527
#define LIGHT_HIGH 100

// ============================================================
//  CONSTANTES DE TIEMPO
// ============================================================
const unsigned short TIEMPO_2_SEC = 2000;
const unsigned short TIEMPO_3_SEC = 3000;
const unsigned short TIEMPO_4_SEC = 4000;
const unsigned short TIEMPO_5_SEC = 5000;
const unsigned short TIEMPO_7_SEC = 7000;

// ============================================================
//  VARIABLES GLOBALES DE ESTADO
// ============================================================
enum State {
  CONFIG = 0,
  INICIO = 1,
  MONITOR_PUERTAS = 2,
  GESTION = 3,
  BLOQUEO = 4,
  MONITOR_AMBIENTAL = 5,
  ALARMA = 6
};

bool claveCorrecta = false;
bool sistemaBloqueado = false;
bool botonPresionado = false;

// Lecturas de sensores
float temperatura = 0;
short hall = 0;
short luz = 0;
short sonido = 0;

// Contadores y entrada
short contadorPuertas = 0;
short contadorAlarmas = 0;
short contadorKeypad = 0;
char tecla = '\0';
char entradaKeypad[4];
unsigned short aciertos = 0;
unsigned short intentos = 0;
int EEAddress = 0;

// Claves de acceso
char claveKeypad[4] = {'1', '2', '3', 'A'};
byte validKeyRFID[4] = { 0x0A, 0x32, 0xD6, 0x73 };

MFRC522 mfrc522(SS_PIN, RST_PIN);

// ============================================================
//  KEYPAD
// ============================================================
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// ============================================================
//  LECTURA DE SENSORES
// ============================================================
float readTemp() {
  int Vo = analogRead(PIN_TEMP);
  int R1 = 10000;
  float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  float logR2 = log(R2);
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
  temperatura = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  temperatura = temperatura - 273.15;
  return temperatura;
}

short readHall() { return analogRead(PIN_HALL); }
short readLuz() { return analogRead(PIN_LUZ); }
short readSonido() { return analogRead(PIN_SONIDO); }

void sensorSetup() {
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_HALL, INPUT);
  pinMode(PIN_LUZ, INPUT);
  pinMode(PIN_SONIDO, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BOTON_CONFIG, INPUT_PULLUP);
  digitalWrite(LED_RED, LOW);
}


// ============================================================
//  TAREAS ASÍNCRONAS
// ============================================================

// Tareas de temporizacion
AsyncTask task_2_sec(TIEMPO_2_SEC, false);
AsyncTask task_3_sec(TIEMPO_3_SEC, false);
AsyncTask task_4_sec(TIEMPO_4_SEC, false);
AsyncTask task_5_sec(TIEMPO_5_SEC, false);
AsyncTask task_7_sec(TIEMPO_7_SEC, false);

// Tarea de lectura de entradas
AsyncTask task_read_RFID(500, true, readRFIDInput);
AsyncTask task_read_keypad(100, true, readKeypadInput);


// ============================================================
//  MÁQUINA DE ESTADOS - CALLBACKS
// ============================================================
void onEnterCONFIG() { 
  Serial.println("Estado: CONFIG - Cargando EEPROM"); 
}

void onEnterINICIO() {
  Serial.println("Estado: INICIO - Teclado/RFID activo");
  claveCorrecta = false;
  sistemaBloqueado = false;
}

void onEnterMONITOR_PUERTAS() {
  Serial.println("Estado: MONITOR_PUERTAS - Sensores activos");
  contadorPuertas = 0;
  // estadoTiempo = millis();
}

void onEnterGESTION() { 
  Serial.println("Estado: GESTION - Configuración"); 
}

void onEnterBLOQUEO()
{
  Serial.println("Estado: BLOQUEO - Sistema bloqueado");
  //estadoTiempo = millis();
  digitalWrite(LED_RED, HIGH);
}

void onEnterMONITOR_AMBIENTAL()
{
  Serial.println("Estado: MONITOR_AMBIENTAL - Sensores ambientales");
  //estadoTiempo = millis();
}

void onEnterALARMA()
{
  Serial.println("Estado: ALARMA - Activada");
  //estadoTiempo = millis();
  contadorAlarmas++;
  tone(BUZZER_PIN, 1000, 500);
}

// ============================================================
//  CONFIGURACIÓN DE TRANSICIONES
// ============================================================
void setupStateMachine(StateMachine &sm)
{
  sm.AddTransition(CONFIG, INICIO, [] { return botonPresionado; });

  sm.AddTransition(INICIO, CONFIG, [] { return tecla == '#'; });
  sm.AddTransition(INICIO, MONITOR_AMBIENTAL, [] { return claveCorrecta; });
  sm.AddTransition(INICIO, BLOQUEO, [] { return sistemaBloqueado; });

  sm.AddTransition(MONITOR_PUERTAS, MONITOR_AMBIENTAL,[] { return task_2_sec.IsExpired(); });
  sm.AddTransition(MONITOR_PUERTAS, ALARMA, [] { return contadorPuertas >= 3; });

  sm.AddTransition(MONITOR_AMBIENTAL, MONITOR_PUERTAS, [] { return task_5_sec.IsExpired(); });
  sm.AddTransition(MONITOR_AMBIENTAL, ALARMA, [] { return (temperatura > 24 || luz > 400); });

  sm.AddTransition(ALARMA, MONITOR_AMBIENTAL, [] { return task_3_sec.IsExpired(); });
  sm.AddTransition(ALARMA, MONITOR_PUERTAS, [] { return task_4_sec.IsExpired(); });
  sm.AddTransition(ALARMA, GESTION, [] { return contadorAlarmas >= 3; });

  sm.AddTransition(GESTION, INICIO, [] { return tecla == '*'; });
  
  sm.AddTransition(BLOQUEO, INICIO, [] { return task_7_sec.IsExpired(); });

  sm.SetOnEntering(CONFIG, onEnterCONFIG);
  sm.SetOnEntering(INICIO, onEnterINICIO);
  sm.SetOnEntering(MONITOR_PUERTAS, onEnterMONITOR_PUERTAS);
  sm.SetOnEntering(GESTION, onEnterGESTION);
  sm.SetOnEntering(BLOQUEO, onEnterBLOQUEO);
  sm.SetOnEntering(MONITOR_AMBIENTAL, onEnterMONITOR_AMBIENTAL);
  sm.SetOnEntering(ALARMA, onEnterALARMA);
}

// ============================================================
//  LECTURA DE ENTRADAS (única definición)
// ============================================================
char readKeypadInput()
{
  char key = keypad.getKey();

    if (key) {
      if (contadorKeypad < 4) {
        entradaKeypad[contadorKeypad] = key;
        contadorKeypad++;
      }
    }

    if (contadorKeypad == 4) {
      aciertos = 0;

      for(int i = 0; i < 4; i++)  {
        if(claveKeypad[i] == entradaKeypad[i]){
          aciertos++;
        }
      }

      if (aciertos == 4){
        Serial.println("Clave correcta");
        intentos = 0;
        claveCorrecta = true;
      } 
      else {
        intentos++;
        if(intentos >= 3) {
          Serial.println("SIS. BLOQUEADO");
          sistemaBloqueado = true;
          intentos = 0; 
        } else {
          Serial.println("Clave incorrecta");
        }
      }

      contadorKeypad = 0;
      Serial.println("Ingrese Clave:");
    }
  return key;
}

void readRFIDInput()
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte* id = mfrc522.uid.uidByte;
    if (memcmp(id, validKeyRFID, 4) == 0) {
      Serial.println("RFID válido");
      claveCorrecta = true;
    } else {
      Serial.println("RFID no reconocido");
    }
    mfrc522.PICC_HaltA();
  }
}

void updateButtonState()
{
  botonPresionado = (digitalRead(BOTON_CONFIG) == LOW);
}

// ============================================================
//  SETUP Y LOOP PRINCIPAL
// ============================================================
StateMachine stateMachine(7, 13);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(9600);
  sensorSetup();
  setupStateMachine(stateMachine);
  stateMachine.SetState(CONFIG, false, true);
  SPI.begin();
	mfrc522.PCD_Init();
  lcd.begin(16, 2);
}

void loop()
{
  readKeypadInput();
  updateButtonState();

  temperatura = readTemp();
  hall = readHall();
  luz = readLuz();
  sonido = readSonido();

  if (hall < TH_HALL)
    contadorPuertas++;

  stateMachine.Update();

  if (stateMachine.GetState() != ALARMA)
  {
    noTone(BUZZER_PIN);
    digitalWrite(LED_RED, LOW);
  }
}