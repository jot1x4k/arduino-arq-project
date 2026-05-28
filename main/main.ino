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

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
StateMachine stateMachine(7, 13);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  sensorSetup();
  setupStateMachine(stateMachine);
  mfrc522.PCD_Init();
  myservo.attach(PIN_SERVO);
  lcd.begin(16, 2);
  
  stateMachine.SetState(INICIO, false, true);
}

void loop()
{
  myservo.write(180);
  stateMachine.Update();
  updateButtonState();
  task_read_RFID.Update();
  task_read_keypad.Update();
}