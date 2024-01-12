#include <Arduino.h>
#include <ArduinoBlue.h>
#include <HardwareSerial.h>

#include "BLEController.h"

#define RX_PIN 16
#define TX_PIN 17

BLEController ble(RX_PIN, TX_PIN);

void setup() {
}

void loop() {
  ble.loopSendBLE();
  // BLE.loopRecieveBLE();
}
