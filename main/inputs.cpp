#include "inputs.h"
#include "hardware-config.h"
#include "globals.h"
#include <Keypad.h>

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

char readKeypadInput()
{
  char key = keypad.getKey();

  if (key) {
    tecla = key;
    Serial.print(key);
    if (key != '#' && key != '*') {
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
      }
    }

    contadorKeypad = 0;
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
    }
    mfrc522.PICC_HaltA();
  }
}

void updateButtonState()
{
  botonPresionado = (digitalRead(BOTON_CONFIG) == LOW);
}