#include <Arduino.h>
#include <SoftwareSerial.h>

#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1

#define TX2_PIN 13
#define RX2_PIN 12

#define TX0_PIN 11
#define RX0_PIN 10

#define SIGNAL_PIN 9
#define JT_BT_PIN 8

#define JT_INTERVAL 500

SoftwareSerial mySerial0(RX0_PIN, TX0_PIN);
SoftwareSerial mySerial2(RX2_PIN, TX2_PIN);

int button_signal = 0;
unsigned long preTime = 0;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial0.begin(9600);
  mySerial2.begin(9600);

  pinMode(JT_BT_PIN, INPUT);
  pinMode(SIGNAL_PIN, OUTPUT);
}

int preRead = 0;

void loop()
{
  // put your main code here, to run repeatedly:

  if (millis() - preTime > JT_INTERVAL)
  {
    int xParam = analogRead(PIN_ANALOG_X);
    int yParam = analogRead(PIN_ANALOG_Y);

    // Serial.print("xParam: ");
    // Serial.println(xParam);

    // Serial.print("yParam: ");
    // Serial.println(yParam);

    // // convert x
    long resultx = map(xParam, 0, 1023, 0, 255);
    Serial.print("mapped value x: ");
    Serial.println(resultx);
     mySerial0.print((char)resultx);

    // // convert y
    long resulty = map(yParam, 0, 1023, 0, 255);
    Serial.print("mapped value y: ");
    Serial.println(resulty);
     mySerial2.print((char)resulty);

    preTime = millis();
  }

  int readingBt = digitalRead(JT_BT_PIN);



  digitalWrite(SIGNAL_PIN, readingBt);
}

// put function definitions here:
