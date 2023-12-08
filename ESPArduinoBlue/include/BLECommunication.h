#pragma once
#include <Arduino.h>
#include <ArduinoBlue.h>

class BLECommunication {
 private:
  ArduinoBlue ble;  // pass reference of bluetooth object to ArduinoBlue constructor.
  int cruiseControlSpeed;
  void configureHM10();

 public:
  BLECommunication(int rxPin, int txPin);
  void loopBLE();
};
