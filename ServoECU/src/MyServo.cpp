#include "MyServo.hpp"

int pos = 0;

PServo::PServo(uint8_t pin)
{
    this->pin = pin;
    this->currentSpeed = 0;
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
    pinMode(pin, OUTPUT);
	servo.setPeriodHertz(50);
	servo.attach(pin);
	Serial.println("servo initialized");
	servo.writeMicroseconds(1500);

}
int PServo::mapSpeedToPulse(int speed) 
{
    int pulse;

    if(speed == 0) {
        pulse = 1500;
    } else if(speed < 0) {
        // Map speed -10 to -1 to PWM 1520 to 1720
        pulse = map(speed, -10, -1, 1720, 1550);
    } else {
        // Map speed 1 to 10 to PWM 1280 to 1480
        pulse = map(speed, 1, 10, 1450, 1280);
    }

    return pulse;
}
bool PServo::ChangeSpeed()
{
    Serial.println("changing the speed to " + String(currentSpeed));
    servo.writeMicroseconds(mapSpeedToPulse(currentSpeed));
    return true;
}

