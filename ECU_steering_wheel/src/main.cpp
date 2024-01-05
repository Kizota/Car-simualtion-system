#include <Arduino.h>
#include <HardwareSerial.h>

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

#define x_JOYSTICK_SERIAL 0
#define Y_JOYSTICK_SERIAL 2

#define SW_JOYSTICK_PIN 15

#define BUTTON_INTERVAL 50

#define CS_PIN 5
#define INT_PIN 0

Button *indBt;
Button *hgBmBt;

JoyStick *joyStk;
SWManager *swManager;
CANController *canController;

// MCP_CAN CAN(CS_PIN);

// HardwareSerial mySerial0(0); //read x value
// HardwareSerial mySerial2(2); //read y value
void setup()
{
  Serial.begin(9600);

  // //steering wheels components
  indBt = new Button("indicator_button", IND_BUTTON_PIN, BUTTON_INTERVAL);
  hgBmBt = new Button("highbeam_button", HGBM_BUTTON_PIN, BUTTON_INTERVAL);
  joyStk = new JoyStick("JoyStick", x_JOYSTICK_SERIAL, Y_JOYSTICK_SERIAL, SW_JOYSTICK_PIN);
  // manager class
  canController = new CANController(INT_PIN, CS_PIN);
  swManager = new SWManager(indBt, hgBmBt, joyStk);

  // set message sender for the steering wheel manager
  swManager->SetSender((ISender *)canController);

  // vTaskStartScheduler();

  // connect to CAN network
}
int ind = 0;
int preVal = 0;
int val = 0;

const int maxlen = 4;
byte buff[maxlen];

Direction preDirection = UNKNOWN;
void loop()
{
  // test
  // if (mySerial0.available())
  // {
  //   char readX = 0;
  //   mySerial0.readBytes(&readX, 1);
  //   // read byte
  //   Serial.print("read byte: ");
  //   Serial.println((long)readX);

  //   // convert to analog
  //   long result = map((long)readX, 0, 255, 0, 1023);
  //   Serial.print("mapped value x: ");
  //   Serial.println(result);
  // }

  // if (mySerial2.available())
  // {
  //   char readY = 0;
  //   mySerial2.readBytes(&readY, 1);
  //   // read byte
  //   Serial.print("read byte: ");
  //   Serial.println((long)readY);

  //   // convert to analog
  //   long result = map((long)readY, 0, 255, 0, 1023);
  //   Serial.print("mapped value y: ");
  //   Serial.println(result);
  // }

  Direction directiron = joyStk->GetDirection();

  if (directiron != preDirection)
  {
    Serial.print("directiron");
    Serial.println(directiron);
    preDirection = directiron;
  }

  // if (joyStk->IsPressed())
  // {
  //   Serial.println("joystick is pressed");
  // }

  // memcpy((void *)&val, buff, sizeof(int));

  // Serial.println(val);

  // vTaskDelay(1000 / portTICK_PERIOD_MS);
}
