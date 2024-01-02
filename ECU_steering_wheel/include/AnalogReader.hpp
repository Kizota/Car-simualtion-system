#ifndef ANALOGREADER_HPP
#define ANALOGREADER_HPP

#include <cstdint>

struct AnalogReader
{
public:
    uint8_t pin;
    uint16_t val;
    uint16_t tolerant;

    AnalogReader(uint8_t pin,uint16_t tolerant);

    uint16_t AnalogRead();
};

#endif