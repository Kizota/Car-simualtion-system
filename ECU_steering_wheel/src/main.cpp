#include <Arduino.h>
#include "CanController.h"
// #include <freertos/FreeRTOS.h>
// #include <freertos/semphr.h>

/*//REVIEW -
   1. check Timer work correctlly
   2. making an acutator class
*/

#include "Button.hpp"
#include "JoyStick.hpp"
#include "SWManager.hpp"

#define IND_BUTTON_PIN 21  // indicator button - on the right
#define HGBM_BUTTON_PIN 22 // high beam button - on the left

#define x_JOYSTICK_PIN 35
#define y_JOYSTICK_PIN 34
#define SW_JOYSTICK_PIN 15

#define BUTTON_INTERVAL 50

#define CS_PIN 5
#define INT_PIN 0

Button *indBt;
Button *hgBmBt;

JoyStick *joyStk;
SWManager *swManager;
CANController *canController;

MCP_CAN CAN(CS_PIN);

void setup()
{
  Serial.begin(9600);

  // //steering wheels components
  indBt = new Button("indicator_button", IND_BUTTON_PIN, BUTTON_INTERVAL);
  hgBmBt = new Button("highbeam_button", HGBM_BUTTON_PIN, BUTTON_INTERVAL);
  joyStk = new JoyStick("JoyStick", x_JOYSTICK_PIN, y_JOYSTICK_PIN, SW_JOYSTICK_PIN);

  // manager class
  canController = new CANController(INT_PIN, CS_PIN);
  swManager = new SWManager(indBt, hgBmBt, joyStk);

  // //set message sender for the steering wheel manager
   swManager->SetSender((ISender*)canController);

  // vTaskStartScheduler();

  // connect to CAN network
}
void loop()
{
  // test

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
