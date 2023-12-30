#include "JoyStick.hpp"

JoyStick::JoyStick(std::string name, int xPin, int yPin, int swPin) : name(name), xPin(xPin), yPin(yPin), xParam(0), yParam(0)
{
    // configure pin mode
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);

    swBt = new Button("sw_joystick", swPin, 50);

    // RTOS
    handler = new TaskHandler(name, JoyStick::ReadSignals, this);
}

JoyStick::~JoyStick()
{

    // turn off task
}

AxisState JoyStick::GetAxisState(Axis_t type)
{
    uint16_t *param = nullptr;
    AxisState state = UNDETECTED;

    // selected read axis param
    switch (type)
    {
    case X:
        param = &xParam;
        break;
    case Y:
        param = &yParam;
        break;
    }

    // detecting state based on the range analog range
    if (*param < MIN_ANALOG_VAL + tolerant)
    {
        state = START;
    }
    else if (*param > MAX_ANALOG_VAL - tolerant)
    {
        state = END;
    }
    else if (*param > MID_ANALOG_VAL - tolerant && *param < MID_ANALOG_VAL + tolerant)
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

    Direction direction = UNKNOWN;

    //  Serial.println(xState);
    // Serial.println(yState);

    if (xState == START && (yState == MIDDLE || yState == END))
    // left
    {
        direction = LEFT;
    }
    else if (xState == END && (yState == MIDDLE || yState == END ))
    // right
    {
        direction = RIGHT;
    }
    else if ((xState == MIDDLE || xState == END) && yState == START)
    // UP
    {
        direction = UP;
    }
    else if ((xState == MIDDLE || xState == END) && yState == END)
    // down
    {
        direction = DOWN;
    }
    else if ((xState == MIDDLE || xState == END) && yState == MIDDLE)
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