#include "BLEController.h"

#include <HardwareSerial.h>

#include <string>

#include "CANController.h"

#define OFF 0
#define ON 1

#define NOCF 0
unsigned long rx_filters[NOCF] = {};
#define NOCM 0
unsigned long rx_masks[NOCM] = {};

CANController can(CAN_500KBPS, rx_masks, NOCM, rx_filters, NOCF);

BLEController::BLEController(int rxPin, int txPin) : ble(Serial2), cruiseControlSpeed(0) {
  // Start UART2 with specific pins
  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);  // Initialize HM-10 communication at 9600 baud rate
  delay(100);
  Serial.println("HM-10 Setup");
  configureHM10();
}

void BLEController::loopBLE() {
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
    Serial.println("LEFT BLINKER ON");
  }
  if (button == 2) {
    Serial.println("LEFT BLINKER OFF");
  }
  if (button == 3) {
    Serial.println("RIGHT BLINKER ON");
  }
  if (button == 4) {
    Serial.println("RIGHT BLINKER OFF");
  }
  if (button == 5) {
    Serial.println("HIGHBEAM ON");
  }
  if (button == 6) {
    Serial.println("HIGHBEAM OFF");
  }
  if (button == 7) {
    Serial.println("CRUISECONTROL ON");
  }
  if (button == 8) {
    Serial.println("CRUISECONTROL OFF");
  }
  // Display slider data when slider moves
  if (sliderId != -1) {
    Serial.print("Slider ID: ");
    Serial.print(sliderId);
    Serial.print("\tValue: ");
    Serial.println(sliderVal);
    cruiseControlSpeed = sliderVal;
  }
  int actualSpeed, RPM, temperature;
  int leftFrontTirePressure, rightFrontTirePressure, leftRearTirePressure, rightRearTirePressure;
  ble.sendDisplayData(0, String(actualSpeed));
  ble.sendDisplayData(1, String(RPM));
  ble.sendDisplayData(2, String(leftFrontTirePressure));
  ble.sendDisplayData(3, String(rightFrontTirePressure));
  ble.sendDisplayData(4, String(leftRearTirePressure));
  ble.sendDisplayData(5, String(rightRearTirePressure));
  ble.sendDisplayData(6, String(temperature));
}

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
