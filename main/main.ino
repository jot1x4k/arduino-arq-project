#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include "hardware-config.h"
#include "globals.h"
#include "sensors.h"
#include "state-machine.h"
#include "inputs.h"
#include "tasks.h"

StateMachine stateMachine(7, 13);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  EEPROM.get(EEPROM_CURRENT_ID_OFFSET, current_id);
  if (current_id < 0 || current_id > 100) {
    current_id = 0;
  }

  sensorSetup();
  setupStateMachine(stateMachine);
  mfrc522.PCD_Init();
  myservo.attach(PIN_SERVO);
  lcd.begin(16, 2);
  
  stateMachine.SetState(INICIO, false, true);
}

void loop()
{
  stateMachine.Update();
  updateButtonState();
  task_read_RFID.Update();
  task_read_keypad.Update();
  task_read_keypad_gestion.Update();
}