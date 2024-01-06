#include <Arduino.h>

#include "BLEController.h"

#define RX_PIN 16
#define TX_PIN 17

BLEController BLE(RX_PIN, TX_PIN);

void setup() {
}

void loop() {
  BLE.loopSendBLE();
  // BLE.loopRecieveBLE();
}