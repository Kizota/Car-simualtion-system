#include "AnalogReader.hpp"
#include <Arduino.h>

AnalogReader::AnalogReader(uint8_t pin, uint16_t tolerant) : pin(pin), tolerant(tolerant), val(0)
{
     pinMode(pin,INPUT);
}

uint16_t AnalogReader::AnalogRead()
{
   uint16_t newVal = analogRead(pin);
   
    if(newVal < val - tolerant || newVal > val + tolerant )
    {
        val = newVal;
    }
    return newVal;
}

