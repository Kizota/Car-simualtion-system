#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "CANController.h"
#include "ServoManager.hpp"

#define CS_PIN 4
#define INT_PIN 2
#define SERVO_PIN 22



ServoManager *servoManager;
CANController *canController;

void setup()
{
  Serial.begin(9600);

  servoManager = new ServoManager(SERVO_PIN);
  canController = new CANController(INT_PIN, CS_PIN, (ServoManager *)servoManager);
  //canController->AddListener((ICanListener *)servoManager);


  canController->AddIdMask(NODE_ID_MOTOR);
  Serial.println("setup done");

}
void loop()
{
  canController->SendMessage(NODE_ID_SPEED_FEEDBACK, servoManager->GetSpeed());
  vTaskDelay(5000 / portTICK_PERIOD_MS);
}