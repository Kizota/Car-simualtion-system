#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "CANController.h"

#define CS_PIN 5
#define INT_PIN 0

CANController *canController;
MCP_CAN CAN(CS_PIN);
void setup()
{
  Serial.begin(9600);
  canController = new CANController(INT_PIN, CS_PIN);
}

void loop()
{
} //
