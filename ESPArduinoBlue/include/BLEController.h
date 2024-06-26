#pragma once
#include <Arduino.h>
#include <ArduinoBlue.h>

class BLEController {
 private:
  static ArduinoBlue *ble;  // pass reference of bluetooth object to ArduinoBlue constructor.
  int cruiseControlSpeed;
  void configureHM10();

 public:
  BLEController(int rxPin, int txPin);
  void loopSendBLE();
  void testDislay();
};
