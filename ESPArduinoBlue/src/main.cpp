#include <Arduino.h>

#include "BLECommunication.h"

#define RX_PIN 16
#define TX_PIN 17

BLECommunication bleCommunication(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);  // Initialize Serial for debugging
}

void loop() {
  bleCommunication.loopBLE();
}