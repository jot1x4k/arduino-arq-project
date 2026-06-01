#include "inputs.h"
#include "hardware-config.h"
#include "globals.h"
#include <Keypad.h>
#include "tasks.h"

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {27, 29, 31, 33}; 
byte colPins[COLS] = {35, 37, 39, 41}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

char readKeypadInput()
{
  char key = keypad.getKey();

  if (key) {
    tecla = key;
    Serial.print(key);
    if (key != '#') {
      if(contadorKeypad < 1) {
        lcdPrint("                 ", 1, false);
      }
      lcd.setCursor(contadorKeypad, 1);
      lcd.print("*");
      if (contadorKeypad < 4) {
        entradaKeypad[contadorKeypad] = key;
        contadorKeypad++;
      }
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
        lcdPrint("Clave incorrecta", 0, true);
        lcdPrint("Intentos: " + String(3 - intentos), 1, false);
        
        task_2_sec.Start();
        while(!contar2Segundos()) {
          task_2_sec.Update();
        }
        if (contar2Segundos()) {
          lcdPrint("E: INICIO", 0, true);
          lcdPrint("Dig. Clave/RFID", 1, false);
        }
      }
    }

    contadorKeypad = 0;
  }
  return key;
}

char readKeypadGestion() {
  char key = keypad.getKey();

  if (key) {
    tecla = key;
    Serial.print(key);
    lcdPrint(String(key), 1, false);
  }
  return key;
}

void readRFIDInput()
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print(F("[RFID] Código UID detectado:"));
    byte* id = mfrc522.uid.uidByte;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    if (mfrc522.uid.size >= 4 && memcmp(id, validKeyRFID, 4) == 0) {
      Serial.println(F("RFID válido"));
      claveCorrecta = true;
    } else {
      Serial.println(F("RFID no reconocido"));
      intentos++;


      if(intentos >= 3) {
        Serial.println("SIS. BLOQUEADO");
        sistemaBloqueado = true;
        intentos = 0;
      } else {
        lcdPrint("RFID no reconocido", 0, true);
        lcdPrint("Intentos: " + String(3 - intentos), 1, false);
        task_2_sec.Start();
        while(!contar2Segundos()) {
          task_2_sec.Update();
        }
        if (contar2Segundos()) {
          lcdPrint("E: INICIO", 0, true);
          lcdPrint("Dig. Clave/RFID", 1, false);
        }
      }
    }
    mfrc522.PICC_HaltA();
  }
}

void updateButtonState()
{
  botonPresionado = (digitalRead(BOTON_CONFIG) == LOW);
}

// Funciones auxiliares

void lcdPrint(const String &message, char pos, char clear) {
  if (clear) {
    lcd.clear();
  }
  lcd.setCursor(0, pos);
  lcd.print(message);
}

void cambiarUmbrales() {
  Serial.println("Opción A - Cambiar umbrales");
  Serial.println("Ingrese nuevo umbral de temperatura: ");

  lcdPrint("TEMP act: " + String(TEMP_HIGH), 0, true);
  lcdPrint("TEMP nuevo: ", 1, false);

  while (true) {
    char key = readKeypadGestion();
    if (key && key != '#' && key != '*') {
      Serial.print(key);
      task_2_sec.Start();
      String input = "";
      while (!contar2Segundos)
      {
        input += key;
      }
      
      if(contar2Segundos()) {
        TEMP_HIGH = strtol(input.c_str(), NULL, 10);
        Serial.println("Nuevo TEMP: " + String(TEMP_HIGH));
        break;
      }
      break;
    }
  }

  Serial.println("Ingrese nuevo umbral de luz: ");

  lcdPrint("Luz act: " + String(LIGHT_HIGH), 0, true);
  lcdPrint("Luz nuevo: ", 1, false);
  
  while (true) {
    char key = readKeypadGestion();
    if (key && key != '#' && key != '*') {
      Serial.print(key);
      task_2_sec.Start();
      String input = "";
      while (!contar2Segundos)
      {
        input += key;
      }
      
      if(contar2Segundos()) {
        LIGHT_HIGH = strtol(input.c_str(), NULL, 10);
        Serial.println("Nueva LUZ: " + String(LIGHT_HIGH));
        break;
      }
      break;
    }
  }

  lcdPrint("UMBRAL ACTUALIZADO", 0, true);
  lcdPrint("TEMP: " + String(TEMP_HIGH) + " LUZ: " + String(LIGHT_HIGH), 1, false);
}

void cambiarAcceso() {
  Serial.println("Opción B - Cambiar acceso");
  Serial.println("Ingrese nueva clave de 4 dígitos:");

  lcdPrint("Ingrese 4 dígitos", 0, true);

  for (int i = 0; i < 4; i++) {
    while (true) {
      char key = readKeypadGestion();
      if (key && key != '#' && key != '*') {
        claveKeypad[i] = key;
        Serial.print("*");
        lcdPrint("*", 1, false);
        break;
      }
    }
  }
  Serial.println("\nClave actualizada");
  lcdPrint("Clave actualizada", 0, true);
}