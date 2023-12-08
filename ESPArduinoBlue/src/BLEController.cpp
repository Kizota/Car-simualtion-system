#include "BLEController.h"

#include <CANController.h>
#include <HardwareSerial.h>

#include <string>

#define BASED_ID 0x30
#define HIGH_BEAM_ID 1
#define LEFTINDICATOR_ID 2
#define RIGHTiNDICATOR_ID 2
#define CRUISECONTROL_ID 3

#define OFF 0
#define ON 1

#define ID_MASK 0x2F
#define NoOfFilters 7

#define ID_FILTER_ACTUALSPEED 0x21
#define ID_FILTER_RPM 0x22
#define ID_FILTER_TEMPERATURE 0x23
#define ID_FILTER_LEFTFRONTTIREPRESSURE 0x24
#define ID_FILTER_RIGHTFRONTTIREPRESSURE 0x25
#define ID_FILTER_LEFTREARTIREPRESSURE 0x26
#define ID_FILTER_RIGHTREARTIREPRESSURE 0x27

Id_guard id_guard;
Message msg = {0, 0, 0, 0};
unsigned long filters[NoOfFilters] = {ID_FILTER_ACTUALSPEED, ID_FILTER_RPM, ID_FILTER_TEMPERATURE, ID_FILTER_LEFTFRONTTIREPRESSURE, ID_FILTER_RIGHTFRONTTIREPRESSURE, ID_FILTER_LEFTREARTIREPRESSURE, ID_FILTER_RIGHTREARTIREPRESSURE};

Command_t commands[] = {{HIGH_BEAM_ID, ON}, {HIGH_BEAM_ID, OFF}, {LEFTINDICATOR_ID, ON}, {LEFTINDICATOR_ID, OFF}, {RIGHTiNDICATOR_ID, ON}, {RIGHTiNDICATOR_ID, OFF}, {CRUISECONTROL_ID, ON}, {CRUISECONTROL_ID, OFF}};

CANController can;

BLEController::BLEController(int rxPin, int txPin) : ble(Serial2), cruiseControlSpeed(0) {
  // Start UART2 with specific pins
  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);  // Initialize HM-10 communication at 9600 baud rate
  delay(100);
  Serial.println("HM-10 Setup");
  configureHM10();
  Id_mask_create(&id_guard, ID_MASK, filters, NoOfFilters);
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
    can.CAN_send_message(commands, LEFTINDICATOR_ON);
    Serial.println("LEFT BLINKER ON");
  }
  if (button == 2) {
    can.CAN_send_message(commands, LEFTINDICATOR_OFF);
    Serial.println("LEFT BLINKER OFF");
  }
  if (button == 3) {
    can.CAN_send_message(commands, RIGHTINDICATOR_ON);
    Serial.println("RIGHT BLINKER ON");
  }
  if (button == 4) {
    can.CAN_send_message(commands, RIGHTINDICATOR_OFF);
    Serial.println("RIGHT BLINKER OFF");
  }
  if (button == 5) {
    can.CAN_send_message(commands, HIGH_BEAM_ON);
    Serial.println("HIGHBEAM ON");
  }
  if (button == 6) {
    can.CAN_send_message(commands, HIGH_BEAM_OFF);
    Serial.println("HIGHBEAM OFF");
  }
  if (button == 7) {
    can.CAN_send_message(commands, CRUISECONTROL_ON);
    Serial.println("CRUISECONTROL ON");
  }
  if (button == 8) {
    can.CAN_send_message(commands, CRUISECONTROL_OFF);
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
  can.CAN_read_message(&msg);
  if (can.CAN_check_message(&id_guard, &msg)) {
    if (*id_guard.filter == ID_FILTER_ACTUALSPEED) {
      actualSpeed = msg.content;
    } else if (*id_guard.filter == ID_FILTER_RPM) {
      RPM = msg.content;
    } else if (*id_guard.filter == ID_FILTER_TEMPERATURE) {
      temperature = msg.content;
    } else if (*id_guard.filter == ID_FILTER_LEFTFRONTTIREPRESSURE) {
      leftFrontTirePressure = msg.content;
    } else if (*id_guard.filter == ID_FILTER_RIGHTFRONTTIREPRESSURE) {
      rightFrontTirePressure = msg.content;
    } else if (*id_guard.filter == ID_FILTER_LEFTREARTIREPRESSURE) {
      leftRearTirePressure = msg.content;
    } else if (*id_guard.filter == ID_FILTER_RIGHTREARTIREPRESSURE) {
      rightRearTirePressure = msg.content;
    }
  }
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
