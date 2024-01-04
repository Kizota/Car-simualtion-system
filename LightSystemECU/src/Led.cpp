#include "Led.hpp"
#define MAX_WRITE_ANALOG (uint8_t)255
#define MIN_WRITE_ANALOG (uint8_t)0

Led::Led(std::string name, uint8_t pin) : name(name), pin(pin), state(OFF, OFF)
{
    // configuration pin
    pinMode(pin, OUTPUT);
    // digitalWrite(pin,HIGH);
}

void Led::Turn(LedState newState)
{
    state.UpdateState(newState);
    Serial.println("turn led");

    if (state.IsNewState())
    {
        Serial.println("new state: ");
        Serial.println(state.GetState());
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
    if (mode == RealTime::OFF)
    {
        Turn(OFF);
    }

    blinkingHandler->SetMode(mode);
}

TweakingLed::TweakingLed(std::string name, uint8_t pin) : Led(name, pin), tolerant(0), dutyCycle(0)
{
    // tweakingQueue
    tweakingQueue = xQueueCreate(10, sizeof(Tendency));
    TweakingHandler = new RealTime::TaskHandler("Tweaking", TweakingLed::Tweaking, this);
    TweakingHandler->SetMode(RealTime::ON);
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
    if (tendency < DECREASE || tendency > INCREASE)
    {
        return false;
    }

    return xQueueSend(tweakingQueue, (void *)&tendency, portMAX_DELAY);
}

bool TweakingLed::TweakingLightLevel(Tendency tendency)
{
    if (tendency < DECREASE || tendency > INCREASE)
    {
        return false;
    }

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

    bool isTweaked = false;
    if (newDutyCycle >= MIN_WRITE_ANALOG && newDutyCycle <= MAX_WRITE_ANALOG)
    // only set the light level when the new dutycycle is valid
    {
        SetBrightness(dutyCycle);
        dutyCycle = newDutyCycle;
        isTweaked = true;
    }

    return isTweaked;
}