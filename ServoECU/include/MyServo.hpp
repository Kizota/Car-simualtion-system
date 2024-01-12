#ifndef MYSERVO_HPP
#define MYSERVO_HPP

#include <iostream>
#include <string>

#include <Arduino.h>
#include "Rtos.hpp"
#include <ESP32Servo.h>

class PServo
{
private:
    // basic configuration
    Servo servo;
    uint8_t pin;
    static int mapSpeedToPulse(int speed);

public:
    short currentSpeed;
    PServo(uint8_t pin);
    
    bool ChangeSpeed();
};

#endif