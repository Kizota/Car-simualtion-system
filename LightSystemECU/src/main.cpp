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
   lsManager = new LSManager(LEFT_IND_PIN, RIGHT_IND_PIN, HIGH_BM_PIN);
   canController = new CANController(INT_PIN, CS_PIN, (LSManager *)lsManager);

  // // add message id mask
   canController->AddIdMask(NODE_ID_INDICATOR);
   canController->AddIdMask(NODE_ID_HIGHBEAM);

  // pinMode(15,OUTPUT);
  // analogWrite(15,100);
}

int speed = 0;
float temp = 12.4;
float press = 34.2;

void loop()
{
  // test send data to dashboard

  if (speed != 100)
  {
    speed += 10;
    temp += 0.2;
    press += 0.2;
  }
  else
  {
    temp = 12.4;
    press = 34.2;
    speed = 0;
  }

  canController->SendMessage(NODE_ID_SPEED_FEEDBACK, speed);
  // canController->SendMessage(NODE_ID_TEMPERATURE, temp);
  // canController->SendMessage(NODE_ID_PRESSURE, press);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
