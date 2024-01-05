#include "JoyStick.hpp"
#define ANALOGREAD_TOLERANT 150

JoyStick::JoyStick(std::string name, int xSerial, int ySerial, int swPin) : name(name), xReader(xSerial, ANALOGREAD_TOLERANT), yReader(ySerial, ANALOGREAD_TOLERANT), direction(UNKNOWN)
{
    swBt = new Button("sw_joystick", swPin, 50);
    
    // RTOS
    readMutex = xSemaphoreCreateBinary();
    handler = new TaskHandler(name, JoyStick::ReadSignals, this);
    handler->AddMutex(readMutex);
}

JoyStick::~JoyStick()
{
    // turn off task
    handler->SetMode(OFF);
}

bool JoyStick::IsPressed()
{
    return swBt->IsPressed();
}

AxisState JoyStick::GetAxisState(Axis_t type)
{
    uint16_t param = 0;
    AxisState state = UNDETECTED;

    // selected read axis param
    switch (type)
    {
    case X:
        param = xReader.GetValue();
        break;
    case Y:
        param = yReader.GetValue();
        break;
    }

    // detecting state based on the range analog range
    if (param < MIN_ANALOG_VAL + tolerant)
    {
        state = START;
    }
    else if (param > MAX_ANALOG_VAL - tolerant)
    {
        state = END;
    }
    else if (param > MID_ANALOG_VAL - tolerant && param < MID_ANALOG_VAL + tolerant)
    {
        state = MIDDLE;
    }

    return state;
}

// component suck
Direction JoyStick::GetDirection()
{

    AxisState xState = GetAxisState(X);
    AxisState yState = GetAxisState(Y);

    if (xState == START && yState == MIDDLE)
    // left
    {
        direction = LEFT;
    }
    else if (xState == END && yState == MIDDLE)
    // right
    {
        direction = RIGHT;
    }
    else if (xState == MIDDLE && yState == END)
    // UP
    {
        direction = UP;
    }
    else if (xState == MIDDLE && yState == START)
    // down
    {
        direction = DOWN;
    }
    else if (xState == MIDDLE && yState == MIDDLE)
    // centre
    {
        direction = CENTRE;
    }

    return direction;
}

void JoyStick::SetReadMode(TaskMode mode)
{

    // turn on read task
    handler->SetMode(mode);
    swBt->SetReadMode(mode);
}
