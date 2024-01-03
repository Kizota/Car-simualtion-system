#include "Led.hpp"

Led::Led(std::string name, uint8_t pin) : name(name), pin(pin), state(OFF, OFF)
{
    // configuration pin
    pinMode(pin, OUTPUT);
}

bool Led::TurnLed(LedState newState)
{
    state.UpdateState(newState);

    // method 1:
    if (state.IsNewState())
    //toggle Led following the new set state 
    {
        digitalWrite(pin, state.GetState());
        state.Refresh();
    }

    // // method 2:
    // switch (state.GetState())
    // {
    // case ON:
    //     if (state.IsNewState())
    //     {
    //         // turn on led
    //         digitalWrite(pin, HIGH);
    //         state.Refresh();
    //     }

    //     break;
    // case OFF:
    //     if (state.IsNewState())
    //     {
    //         // turn off led
    //         digitalWrite(pin, HIGH);
    //         state.Refresh();
    //     }
    //     break;
    // }
}
