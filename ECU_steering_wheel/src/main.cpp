#include <Arduino.h>

// #include <freertos/FreeRTOS.h>
// #include <freertos/semphr.h>

/*//REVIEW -
   1. check Timer work correctlly
   2. making an acutator class
*/

#include "Button.hpp"
#include "JoyStick.hpp"

#define IND_BUTTON_PIN 21  // indicator button - on the right
#define HGBM_BUTTON_PIN 22 // high beam button - on the left

#define x_JOYSTICK_PIN 35
#define y_JOYSTICK_PIN 34
#define SW_JOYSTICK_PIN 15

#define BUTTON_INTERVAL 50

Button *indBt;
Button *hgBmBt;

JoyStick *joyStk;

void setup()
{
  Serial.begin(9600);

  indBt = new Button("indicator_button", IND_BUTTON_PIN, BUTTON_INTERVAL);
  hgBmBt = new Button("highbeam_button", HGBM_BUTTON_PIN, BUTTON_INTERVAL);

  joyStk = new JoyStick("JoyStick", x_JOYSTICK_PIN, y_JOYSTICK_PIN, SW_JOYSTICK_PIN);

  // xTaskCreate();
  joyStk->SetReadMode(ON);
}

// testing
Timer testTimer(1000);
bool taskState = false;
Direction preDirection = UNKNOWN;

void loop()
{
  Direction direction = joyStk->GetDirection();

  if (preDirection != direction)
  {
    preDirection = direction;
    Serial.println(direction);
  }
}
