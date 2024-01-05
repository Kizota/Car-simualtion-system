#include <Arduino.h>

// #include <freertos/FreeRTOS.h>
// #include <freertos/semphr.h>

#define LEFT_BUTTON_PIN 21
#define RIGHT_BUTTON_PIN 22

#define x_JOYSTICK_PIN 2
#define y_JOYSTICK_PIN 4

void setup()
{
  Serial.begin(9600);

  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);

  pinMode(x_JOYSTICK_PIN, INPUT);
  pinMode(y_JOYSTICK_PIN, INPUT);
}

int preLeftSignal = -1;
int preRightSignal = -1;

uint16_t preXParam = 0;
uint16_t preYParam = 0;

void loop()
{
  // read button signal
  int leftSingal = digitalRead(LEFT_BUTTON_PIN);
  int rightSignal = digitalRead(RIGHT_BUTTON_PIN);

  // test component
  if (leftSingal != preLeftSignal)
  {
    Serial.print("Left signal");
    Serial.println(leftSingal);
    preLeftSignal = leftSingal;
  }

  if (rightSignal != preRightSignal)
  {
    Serial.print("right signal");
    Serial.println(rightSignal);
    preRightSignal = rightSignal;
  }

  uint16_t xParam = analogRead(x_JOYSTICK_PIN);
  uint16_t yParam = analogRead(y_JOYSTICK_PIN);

  if (xParam > preXParam + 100 || xParam < preXParam - 100)
  {
    Serial.print("x :");
    Serial.println(xParam);
    preXParam = xParam;
  }

  if (yParam > preYParam + 100 || yParam < preYParam - 100)
  {
    Serial.print("y: ");
    Serial.println(yParam);
    preYParam = yParam;
  }
}


// if (numOfByte)
//   {
//     for (int i = numOfByte; i >= 0; i--)
//     {
//       Serial.println("numOfByte");
//       Serial.println(numOfByte);
//       buff[i] = mySerial2.read();

//       Serial.print("byte ");
//       Serial.print(i);
//       Serial.print(": ");
//       Serial.println(buff[i]);
//     }

//     memcpy((void *)&val, buff, sizeof(int));
//     Serial.println(val);
//   }
