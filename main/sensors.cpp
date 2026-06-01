#include "sensors.h"
#include "hardware-config.h"
#include "globals.h"
#include "tasks.h" 
#include "inputs.h"

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

float readTemp() {
  Serial.println("Temperatura: ");
  int Vo = analogRead(PIN_TEMP);
  int R1 = 10000;
  float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  float logR2 = log(R2);
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
  temperatura = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  temperatura = temperatura - 273.15;
  Serial.println(temperatura);
  
  lcdPrint("T: " + String(temperatura) + "C", 1, 0, false);

  return temperatura;
}

short readHall() { 
  Serial.println("Hall: ");
  hall = analogRead(PIN_HALL); 
  Serial.println(hall);

  lcdPrint("H: " + String(hall), 1, 0, false);
  
  return hall; 
}

short readLuz() { 
  Serial.println("Luz: ");
  luz =  analogRead(PIN_LUZ); 
  Serial.println(luz);

  lcdPrint("L: " + String(luz), 1, 9, false);

  return luz; 
}

short readSonido() { 
  Serial.println("Sonido: ");
  sonido = analogRead(PIN_SONIDO); 
  Serial.println(sonido);

  lcdPrint("S: " + String(sonido), 1, 9, false);

  return sonido; 
}

void blinkLed() {
  task_300_ms.Start();
  while (!contar300ms()) {
    digitalWrite(LED_RED, HIGH);
    Serial.println("LED ON");
  }
  
  task_700_ms.Start();
  while (!contar700ms()) {
    digitalWrite(LED_RED, LOW);
    Serial.println("LED OFF");
  }
}

void rotateServo(unsigned short direction) {
  task_100_ms.Start();
  if(direction == 0) {
    myservo.write(0);
  }

  if (direction == 180)
  {
    myservo.write(180);
  } 
  
  task_300_ms.Start();
  while (!contar300ms())
  {
    if(contar100ms()) {
      myservo.write(90);
    }
  }
}