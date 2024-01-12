#ifndef ANALOGREADER_HPP
#define ANALOGREADER_HPP

#include <Arduino.h>

#include <cstdint>
#include <string>
#include <HardwareSerial.h>

#include "Rtos.hpp"
struct AnalogReader
{
public:
    uint8_t pin;
    uint16_t val;
    uint16_t tolerant;

    AnalogReader(uint8_t pin, uint16_t tolerant);

    uint16_t AnalogRead();
};

class HardwareSerialReader
{
private:
    int num;
    HardwareSerial mySerial;
    uint16_t val;
    uint16_t tolerant;

    TaskHandler *handler;

public:
    HardwareSerialReader(int serialNum, uint16_t tolerant);
    ~HardwareSerialReader();

    uint16_t GetValue();

private:
    static void SerialReadingTask(void *parameter)
    {
        HardwareSerialReader *reader = static_cast<HardwareSerialReader *>(parameter);
        while (1)
        {
            if (reader->mySerial.available())
            {
                // read the byte
                char read = 0;
                reader->mySerial.readBytes(&read, 1);

                // convert to analog
                long result = map((long)read, 0, 255, 0, 1023);

                // update valid value
                if (result < reader->val - reader->tolerant || result > reader->val + reader->tolerant)
                {
                    reader->val = result;
                }
            }

            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
};

#endif