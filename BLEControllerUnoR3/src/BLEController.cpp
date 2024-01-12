#include "BLEController.h"

#include <HardwareSerial.h>

BLEController::BLEController(int rxPin, int txPin) : ble(Serial2), cruiseControlSpeed(0) {
  Serial.begin(9600);  // Initialize Serial for debugging
  // Start UART2 with specific pins
  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);  // Initialize HM-10 communication at 9600 baud rate
  delay(100);
  Serial.println("HM-10 Setup");
  configureHM10();
}

void BLEController::loopSendBLE() {
  // ID of the button pressed pressed.
  int button = ble.getButton();
  // ID of the slider moved.
  int sliderId = ble.getSliderId();
  // Slider value goes from 0 to 200.
  int sliderVal = ble.getSliderVal();
  if (button != -1) {
    Serial.print("Button: ");
    Serial.println(button);
  }
  if (button == 1) {
    // can.SendIntCanMessage(NODE_ID_LEFTBLINKER, COMMAND_ON);
    Serial.println("LEFT BLINKER ON");
  }
  if (button == 2) {
    // can.SendIntCanMessage(NODE_ID_LEFTBLINKER, COMMAND_OFF);
    Serial.println("LEFT BLINKER OFF");
  }
  if (button == 3) {
    // can.SendIntCanMessage(NODE_ID_RIGHTBLINKER, COMMAND_ON);
    Serial.println("RIGHT BLINKER ON");
  }
  if (button == 4) {
    // can.SendIntCanMessage(NODE_ID_RIGHTBLINKER, COMMAND_OFF);
    Serial.println("RIGHT BLINKER OFF");
  }
  if (button == 5) {
    // can.SendIntCanMessage(NODE_ID_HIGHBEAM, COMMAND_ON);
    Serial.println("HIGHBEAM ON");
  }
  if (button == 6) {
    // can.SendIntCanMessage(NODE_ID_HIGHBEAM, COMMAND_OFF);
    Serial.println("HIGHBEAM OFF");
  }
  if (button == 7) {
    // can.SendIntCanMessage(NODE_ID_CRUISECONTROL, COMMAND_ON);
    Serial.println("CRUISECONTROL ON");
  }
  if (button == 8) {
    // can.SendIntCanMessage(NODE_ID_CRUISECONTROL, COMMAND_OFF);
    Serial.println("CRUISECONTROL OFF");
  }
  // Display slider data when slider moves
  if (sliderId != -1) {
    Serial.print("Slider ID: ");
    Serial.print(sliderId);
    Serial.print("\tValue: ");
    Serial.println(sliderVal);
    cruiseControlSpeed = sliderVal;
    // can.SendIntCanMessage(NODE_ID_CRUISECONTROL_SPEED, cruiseControlSpeed);
  }
}

// // void BLEController::loopRecieveCAN() {
// // // CanData canData = can.ReadCanMessage();
// // if (canData.MessageID == NODE_ID_SPEED) {
// // int actualSpeed = canData.command[0];
// ble.sendDisplayData(0, String(actualSpeed));
// // } else if (canData.MessageID == NODE_ID_RPM) {
// // int RPM = canData.command[0];
// ble.sendDisplayData(1, String(RPM));
// // } else if (canData.MessageID == NODE_ID_PRESSURE) {
// // int leftFrontTirePressure = canData.command[0];
// // int rightFrontTirePressure = canData.command[1];
// // int leftRearTirePressure = canData.command[2];
// // int rightRearTirePressure = canData.command[3];
// ble.sendDisplayData(2, String(leftFrontTirePressure));
// ble.sendDisplayData(3, String(rightFrontTirePressure));
// ble.sendDisplayData(4, String(leftRearTirePressure));
// ble.sendDisplayData(5, String(rightRearTirePressure));
// // } else if (canData.MessageID == NODE_ID_TEMPERATURE) {
// // int temperature = canData.command[0];
// ble.sendDisplayData(6, String(temperature));
// }
// }

void BLEController::configureHM10() {
  // Send AT commands to configure HM-10 module settings
  Serial2.println("AT");  // Check if the module responds
  delay(500);             // Delay to allow the module to respond
  if (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');  // Read the response from HM-10
    Serial.println("Response: " + response);
  }
  // Set the name of the HM-10 module
  Serial2.println("Car");
  delay(500);
  Serial2.println("AT+BAUD8");  // 8 represents 115200 baud rate
  delay(500);
  // Set the HM-10 module as a peripheral device (slave)
  Serial2.println("AT+ROLE0");  // 0 for peripheral, 1 for central
  delay(500);
  // Restart the module to apply changes
  Serial2.println("AT+RESET");
  delay(500);
}
