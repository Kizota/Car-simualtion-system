#include "AnalogReader.hpp"
#include <Arduino.h>

AnalogReader::AnalogReader(uint8_t pin, uint16_t tolerant) : pin(pin), tolerant(tolerant), val(0)
{

    pinMode(pin, INPUT);
}

uint16_t AnalogReader::AnalogRead()
{
    uint16_t newVal = analogRead(pin);

    if (newVal < val - tolerant || newVal > val + tolerant)
    {
        val = newVal;
    }
    return val;
}

HardwareSerialReader::HardwareSerialReader(int serialNum, uint16_t tolerant) : num(serialNum), mySerial(serialNum), tolerant(tolerant)
{
    mySerial.begin(9600);

    std::string name = "Serial reader " + std::to_string(serialNum);
    handler = new TaskHandler(name, HardwareSerialReader::SerialReadingTask, this);
    handler->SetMode(ON);

    Serial.println(serialNum);
}

HardwareSerialReader::~HardwareSerialReader()
{
    handler->SetMode(OFF);
}

uint16_t HardwareSerialReader::GetValue()
{

    return val;
}