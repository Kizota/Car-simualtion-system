#include <Arduino.h>
#include <ArduinoBlue.h>
#include <HardwareSerial.h>

#include <string>

// Define the serial object to communicate with the HM-10 module
HardwareSerial hm10Serial(1);   // Use Serial1 or any available hardware serial port on the ESP32
ArduinoBlue phone(hm10Serial);  // pass reference of bluetooth object to ArduinoBlue constructor.

int cruiseControlSpeed = 0;

void configureHM10();

void setup() {
  Serial.begin(9600);        // Initialize Serial for debugging
  hm10Serial.begin(115200);  // Initialize HM-10 communication at 9600 baud rate

  delay(1000);  // Give some time for the HM-10 module to initialize
  Serial.println("HM-10 Setup");
  // Send AT commands to configure the HM-10 module
  configureHM10();
}

void loop() {
  // ID of the button pressed pressed.
  int button = phone.getButton();
  // ID of the slider moved.
  int sliderId = phone.getSliderId();
  // Slider value goes from 0 to 200.
  int sliderVal = phone.getSliderVal();
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
  phone.sendDisplayData(0, String(actualSpeed));
  phone.sendDisplayData(1, String(RPM));
  phone.sendDisplayData(2, String(leftFrontTirePressure));
  phone.sendDisplayData(3, String(rightFrontTirePressure));
  phone.sendDisplayData(4, String(leftRearTirePressure));
  phone.sendDisplayData(5, String(rightRearTirePressure));
  phone.sendDisplayData(6, String(temperature));
}

void configureHM10() {
  // Send AT commands to configure HM-10 module settings
  hm10Serial.println("AT");  // Check if the module responds
  delay(500);                // Delay to allow the module to respond

  if (hm10Serial.available()) {
    String response = hm10Serial.readStringUntil('\n');  // Read the response from HM-10
    Serial.println("Response: " + response);
  }

  // Example: Change the name of the HM-10 module
  hm10Serial.println("Car");  // Replace "NewName" with your desired name
  delay(500);

  // Example: Change the baud rate of the HM-10 module to 115200
  hm10Serial.println("AT+BAUD8");  // 8 represents 115200 baud rate
  delay(500);

  // Example: Set the HM-10 module as a peripheral device (slave)
  hm10Serial.println("AT+ROLE0");  // 0 for peripheral, 1 for central
  delay(500);

  // Example: Restart the module to apply changes
  hm10Serial.println("AT+RESET");
  delay(500);
}
