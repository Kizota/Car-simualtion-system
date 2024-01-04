#include "Led.hpp"
#define MAX_WRITE_ANALOG 255
#define MIN_WRITE_ANALOG 0

Led::Led(std::string name, uint8_t pin) : name(name), pin(pin), state(OFF, OFF)
{
    // configuration pin
    pinMode(pin, OUTPUT);
}

bool Led::Turn(LedState newState)
{
    state.UpdateState(newState);

    if (state.IsNewState())
    {
        digitalWrite(pin, state.GetState());
        state.Refresh();
    }
}

bool Led::SetBrightness(uint8_t analogVal)
{
    if (analogVal > MAX_WRITE_ANALOG || analogVal < MIN_WRITE_ANALOG)
    {
        return false;
    }

    analogRead(analogVal);
    return true;
}

BlinkingLed::BlinkingLed(std::string name, uint8_t pin, uint32_t interval) : Led(name, pin), interval(interval), state(OFF_WAIT)
{
    blinkingHandler = new RealTime::TaskHandler("Blinking", BlinkingLed::Blinking, this);
}

BlinkingLed::~BlinkingLed()
{
    blinkingHandler->SetMode(RealTime::OFF);
}

void BlinkingLed::SetBlinking(RealTime::TaskMode mode)
{
    blinkingHandler->SetMode(mode);
}

TweakingLed::TweakingLed(std::string name, uint8_t pin) : Led(name, pin), tolerant(0), dutyCycle(0)
{
    // tweakingQueue
    tweakingQueue = xQueueCreate(10, sizeof(Tendency));
    TweakingHandler->SetMode(RealTime::ON);
    TweakingHandler = new RealTime::TaskHandler("Tweaking", TweakingLed::Tweaking, this);
}

TweakingLed::~TweakingLed()
{
    TweakingHandler->SetMode(RealTime::ON);
}

bool TweakingLed::SetTolerant(uint8_t newTolerant)
{
    if (newTolerant > MAX_WRITE_ANALOG || newTolerant < MIN_WRITE_ANALOG)
    {
        return false;
    }

    tolerant = newTolerant;

    return true;
}

bool TweakingLed::AddTweakingCommand(Tendency tendency)
{
    if(tendency < DECREASE || tendency > INCREASE)
    {
        return false;
    }

    return xQueueSend(tweakingQueue, (void *)&tendency, portMAX_DELAY);
}

bool TweakingLed::TweakingLightLevel(Tendency tendency)
{
    // tweaking duty Cycle
    uint8_t newDutyCycle = dutyCycle;
    switch (tendency)
    {
    case INCREASE:
        newDutyCycle += tolerant;
        break;
    case DECREASE:
        newDutyCycle -= tolerant;
        break;
    }

    if (newDutyCycle >= MIN_WRITE_ANALOG && newDutyCycle <= MAX_WRITE_ANALOG)
    // only set the light level when the new dutycycle is valid
    {
        SetBrightness(dutyCycle);
        dutyCycle = newDutyCycle;
    }
}