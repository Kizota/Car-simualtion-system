#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <Arduino.h>
#include "config.hpp"
#include "Rtos.hpp"
#include "AnalogReader.hpp"

#include "Button.hpp"

#define MAX_ANALOG_VAL 1023
#define MID_ANALOG_VAL 510
#define MIN_ANALOG_VAL 0

const uint16_t tolerant = 20;

/*
  //TODO - improve code
      1. check read threadsafe with mutex or binary / how to remove and delete mutex

*/

enum Axis_t
{
    X,
    Y
};

enum AxisState
{
    UNDETECTED = -1,
    START,
    MIDDLE,
    END,

};

enum Direction
{
    UNKNOWN = -1,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    CENTRE,
};

class JoyStick
{
private:
    std::string name;
    // readed signal
    HardwareSerialReader xReader;
    HardwareSerialReader yReader;

    // AnalogReader xReader;
    // AnalogReader yReader;
    Button *swBt;

    Direction direction;

    SemaphoreHandle_t readMutex;
    TaskHandler *handler;
    ;

public:
    JoyStick(std::string, int, int, int);

    ~JoyStick();

    bool IsPressed();

    Direction GetDirection();

    AxisState GetAxisState(Axis_t);

    void SetReadMode(TaskMode);

private:
    static void ReadSignals(void *parameter)
    {
        while (1)
        {
            JoyStick *js = static_cast<JoyStick *>(parameter);

            if (xSemaphoreTake(js->readMutex, portMAX_DELAY))
            {

                uint16_t xReading = js->xReader.GetValue();
                uint16_t yReading = js->yReader.GetValue();
                xSemaphoreGive(js->readMutex);
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
};

#endif