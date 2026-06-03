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
        lcdPrint("                 ", 1, 0, false);
      }
      lcdPrint("*", 1, contadorKeypad, false);
      if (contadorKeypad < 4) {
        entradaKeypad[contadorKeypad] = key;
        contadorKeypad++;
      }
    }
  }
  
  if (contadorKeypad == 4) {
    bool valid = false;

    aciertos = 0;
    for(int i = 0; i < 4; i++)  {
      if(claveKeypad[i] == entradaKeypad[i]){
        aciertos++;
      }
    }
    if (aciertos == 4) {
      valid = true;
    }

    if (!valid) {
      for (int i = 0; i < current_id; i++) {
        Perfil p;
        EEPROM.get(EEPROM_PROFILES_OFFSET + (i * sizeof(Perfil)), p);
        if (memcmp(p.clave, entradaKeypad, 4) == 0) {
          valid = true;
          break;
        }
      }
    }

    if (valid){
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
        lcdPrint("Clave incorrecta", 0, 0, true);
        lcdPrint("Intentos: " + String(3 - intentos), 1, 0, false);
        
        task_2_sec.Start();
        while(!contar2Segundos()) {
          task_2_sec.Update();
        }
        if (contar2Segundos()) {
          lcdPrint("E: INICIO", 0, 0, true);
          lcdPrint("Dig. Clave/RFID", 1, 0, false);
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
    if (key != '#' && key != '*' && key != 'A' && key != 'B') {
       lcdPrint(String(key), 1, 0, false);
    }
  }
  return key;
}

char getRawKeypad() {
  return keypad.getKey();
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
    
    bool valid = false;

    if (mfrc522.uid.size >= 4) {
      if (memcmp(id, validKeyRFID, 4) == 0) {
        valid = true;
      } else {
        for (int i = 0; i < current_id; i++) {
          Perfil p;
          EEPROM.get(EEPROM_PROFILES_OFFSET + (i * sizeof(Perfil)), p);
          if (memcmp(p.rfid, id, 4) == 0) {
            valid = true;
            break;
          }
        }
      }
    }

    if (valid) {
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
        lcdPrint("RFID no reconocido", 0, 0, true);
        lcdPrint("Intentos: " + String(3 - intentos), 1, 0, false);
        task_2_sec.Start();
        while(!contar2Segundos()) {
          task_2_sec.Update();
        }
        if (contar2Segundos()) {
          lcdPrint("E: INICIO", 0, 0, true);
          lcdPrint("Dig. Clave/RFID", 1, 0, false);
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

void lcdPrint(const String &message, short fil, short col, char clear) {
  if (clear) {
    lcd.clear();
  }
  lcd.setCursor(col, fil);
  lcd.print(message);
}

void cambiarUmbrales() {
  Serial.println("Opción A - Cambiar umbrales");
  Serial.println("Ingrese nuevo umbral de temperatura: ");

  lcdPrint("T act: " + String(TEMP_HIGH), 0, 0, true);
  lcdPrint("T nuevo: ", 1, 0, false);

  String input = "";
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key >= '0' && key <= '9') {
        input += key;
        Serial.print(key);
        lcdPrint("T nuevo: " + input, 1, 0, false);
        
        task_2_sec.Start();
        task_2_sec.Reset();
        
        while (true) {
          task_2_sec.Update();
          
          if (contar2Segundos()) {
            break;
          }
          
          char nextKey = keypad.getKey();
          if (nextKey) {
            if (nextKey >= '0' && nextKey <= '9') {
              input += nextKey;
              Serial.print(nextKey);
              lcdPrint("T nuevo: " + input, 1, 0, false);
              task_2_sec.Start();
              task_2_sec.Reset();
            } else if (nextKey == '#') {
              break;
            } else if (nextKey == '*') {
              input = "";
              break;
            }
          }
        }
        break;
      } else if (key == '*') {
        break;
      }
    }
  }

  if (input.length() > 0) {
    TEMP_HIGH = strtol(input.c_str(), NULL, 10);
    Serial.println("\nNuevo TEMP: " + String(TEMP_HIGH));
  }

  Serial.println("Ingrese nuevo umbral de luz: ");

  lcdPrint("L act: " + String(LIGHT_HIGH), 0, 0, true);
  lcdPrint("L nuevo: ", 1, 0, false);

  input = "";
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key >= '0' && key <= '9') {
        input += key;
        Serial.print(key);
        lcdPrint("L nuevo: " + input, 1, 0, false);
        
        task_2_sec.Start();
        task_2_sec.Reset();
        
        while (true) {
          task_2_sec.Update();
          
          if (contar2Segundos()) {
            break;
          }
          
          char nextKey = keypad.getKey();
          if (nextKey) {
            if (nextKey >= '0' && nextKey <= '9') {
              input += nextKey;
              Serial.print(nextKey);
              lcdPrint("L nuevo: " + input, 1, 0, false);
              task_2_sec.Start();
              task_2_sec.Reset();
            } else if (nextKey == '#') {
              break;
            } else if (nextKey == '*') {
              input = "";
              break;
            }
          }
        }
        break;
      } else if (key == '*') {
        break;
      }
    }
  }

  if (input.length() > 0) {
    LIGHT_HIGH = strtol(input.c_str(), NULL, 10);
    Serial.println("\nNueva LUZ: " + String(LIGHT_HIGH));
  }

  lcdPrint("UMBRAL ACTUALIZADO", 0, 0, true);
  lcdPrint("T: " + String(TEMP_HIGH) + " L: " + String(LIGHT_HIGH), 1, 0, false);

  task_2_sec.Start();
  task_2_sec.Reset();
  while (!contar2Segundos()) {
    task_2_sec.Update();
  }
}

void cambiarAcceso() {
  Serial.println("Opción B - Cambiar acceso");
  Serial.println("Ingrese nueva clave de 4 dígitos:");

  lcdPrint("Ingrese 4 dig", 0, 0, true);
  lcdPrint("                ", 1, 0, false);

  for (int i = 0; i < 4; i++) {
    while (true) {
      char key = keypad.getKey();
      if (key && key >= '0' && key <= '9') {
        claveKeypad[i] = key;
        Serial.print("*");
        lcdPrint("*", 1, i, false);
        break;
      }
    }
  }
  Serial.println("\nClave actualizada");
  lcdPrint("Clave actualizada", 0, 0, true);

  task_2_sec.Start();
  task_2_sec.Reset();
  while (!contar2Segundos()) {
    task_2_sec.Update();
  }
}