#include <Arduino.h>
#include <ArduinoBlue.h>
#include <HardwareSerial.h>

#include "BLEController.h"
#include "CANController.h"

#define CS_PIN 5
#define INT_PIN 0

#define RX_PIN 16
#define TX_PIN 17

CANController *canController;
BLEController *bleController;

void setup() {
  bleController = new BLEController(RX_PIN, TX_PIN);
  canController = new CANController(INT_PIN, CS_PIN, (ICanListener *)bleController);
  bleController->addSender((ICanSender *)canController);
  canController->AddIdMask(NODE_ID_SPEED_FEEDBACK);
  canController->AddIdMask(NODE_ID_PRESSURE);
  canController->AddIdMask(NODE_ID_TEMPERATURE);
}

void loop() {
  bleController->loopSendBLE();
}
