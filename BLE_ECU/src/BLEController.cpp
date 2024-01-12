#include "BLEController.h"

#include <HardwareSerial.h>

#include "Rtos.hpp"

ArduinoBlue* BLEController::ble = new ArduinoBlue(Serial2);

BLEController::BLEController(int rxPin, int txPin) {
  Serial.begin(9600);  // Initialize Serial for debugging
  // Start UART2 with specific pins
  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);  // Initialize HM-10 communication at 9600 baud rate
  delay(100);
  Serial.println("HM-10 Setup");
  configureHM10();
  rcdCmdQueue = xQueueCreate(10, sizeof(CanData));
  xTaskCreate(BLEController::ReadingCommandTask, "ReaingCommandTask", stackDepth, this, 1, NULL);
}

void BLEController::loopSendBLE() {
  // ID of the button pressed pressed.
  int button = ble->getButton();
  // ID of the slider moved.
  int sliderId = ble->getSliderId();
  // Slider value goes from 0 to 200.
  int sliderVal = ble->getSliderVal();
  if (button != -1) {
    Serial.print("Button: ");
    Serial.println(button);
  }
  if (button == 1) {
    Serial.println("LEFT BLINKER ON");
    can->SendMessage(NODE_ID_INDICATOR, LEFT_IND_ON);
  }
  if (button == 2) {
    Serial.println("LEFT BLINKER OFF");
    can->SendMessage(NODE_ID_INDICATOR, BOTH_OFF);
  }
  if (button == 3) {
    Serial.println("RIGHT BLINKER ON");
    can->SendMessage(NODE_ID_INDICATOR, RIGHT_IND_ON);
  }
  if (button == 4) {
    Serial.println("RIGHT BLINKER OFF");
    can->SendMessage(NODE_ID_INDICATOR, BOTH_OFF);
  }
  if (button == 5) {
    Serial.println("HIGHBEAM ON");
    can->SendMessage(NODE_ID_HIGHBEAM, COMMAND_ON);
  }
  if (button == 6) {
    Serial.println("HIGHBEAM OFF");
    can->SendMessage(NODE_ID_HIGHBEAM, COMMAND_OFF);
  }
  if (button == 7) {
    Serial.println("CRUISECONTROL ON");
    can->SendMessage(NODE_ID_CRUISECONTROL, COMMAND_ON);
  }
  if (button == 8) {
    Serial.println("CRUISECONTROL OFF");
    can->SendMessage(NODE_ID_CRUISECONTROL, COMMAND_OFF);
  }
  // Display slider data when slider moves
  if (sliderId != -1) {
    Serial.print("Slider ID: ");
    Serial.print(sliderId);
    Serial.print("\tValue: ");
    Serial.println(sliderVal);
    int cruiseControlSpeed = sliderVal;
    can->SendMessage(NODE_ID_CRUISECONTROL_SPEED, cruiseControlSpeed);
  }
}

void BLEController::configureHM10() {
  // Send AT commands to configure HM-10 module settings
  Serial2.write("AT");  // Check if the module responds
  delay(500);           // Delay to allow the module to respond
  if (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');  // Read the response from HM-10
    Serial.println("Response: " + response);
  }
  // Set the name of the HM-10 module
  Serial2.write("Car");
  delay(500);
  Serial2.write("AT+BAUD0");  // 0 represents 9200 baud rate
  delay(500);
  // Set the HM-10 module as a peripheral device (slave)
  Serial2.write("AT+ROLE0");  // 0 for peripheral, 1 for central
  delay(500);
  // Restart the module to apply changes
  Serial2.write("AT+RESET");
  delay(500);
}

void BLEController::addSender(ICanSender* Can) {
  can = Can;
}