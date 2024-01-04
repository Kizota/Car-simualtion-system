#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "CANController.h"
#include "LSManager.hpp"

#define CS_PIN 5
#define INT_PIN 0

#define LEFT_IND_PIN 21
#define RIGHT_IND_PIN 22
#define HIGH_BM_PIN 15

LSManager *lsManager;
CANController *canController;

void setup()
{
  Serial.begin(9600);
  lsManager = new LSManager(LEFT_IND_PIN,RIGHT_IND_PIN,HIGH_BM_PIN);
  canController = new CANController(INT_PIN, CS_PIN,(LSManager*)lsManager);
  canController->AddIdMask(NODE_ID_INDICATOR);
  canController->AddListener((ICanListener*)lsManager);
}

void loop()
{
  // yield time for the other task
 // canController->ReadMessage();
  vTaskDelay(100 / portTICK_PERIOD_MS);
}
