#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <Arduino.h>
#include "config.hpp"
#include "Rtos.hpp"

#include "Button.hpp"

#define MAX_ANALOG_VAL 4095
#define MID_ANALOG_VAL  3100
#define MIN_ANALOG_VAL 0

const uint16_t tolerant = 150;

/*
  //TODO - improve code
    1. applied modulal read for analog
    2. debounce direction result 

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
    uint8_t xPin;
    uint8_t yPin;

    // readed signal
    uint16_t xParam;
    uint16_t yParam;

    Button *swBt;

    TaskHandler *handler;
    ;

public:
    JoyStick(std::string, int, int, int);

    ~JoyStick();

    bool IsPressed();

    Direction GetDirection();

    AxisState GetAxisState(Axis_t);

    void SetReadMode(TaskMode);

    // turn on read task
    // turn off read task

    // TODO use modulo for the reading analog value
    static void ReadSignals(void *parameter)
    {
        while(1)
        {
            JoyStick *js = static_cast<JoyStick *>(parameter);

            //
            js->xParam = analogRead(js->xPin);
            js->yParam = analogRead(js->yPin);
         

            vTaskDelay(10/portTICK_PERIOD_MS);
        }

    }
};

#endif