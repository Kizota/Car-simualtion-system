#include <Arduino.h>

#include "BLEController.h"

BLEController bleController(16, 17);

void setup() {
}

void loop() {
  bleController.loopSendBLE();
}