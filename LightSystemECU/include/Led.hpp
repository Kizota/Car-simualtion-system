#ifndef LED_HPP
#define LED_HPP

#include <iostream>
#include <string>
#include <StateControl.hpp>

#include <Arduino.h>
#include "Rtos.hpp"
/*
  //TODO - optimization code
   1. check design for blinking
   2. create inheritance classes
   3. check parameter valid ? using assert?


*/
enum LedState
{
    OFF = LOW,
    ON = HIGH
};

enum BlinkingLedState
{
    OFF_WAIT,
    ON_WAIT
};

enum Tendency
{
    IDLE = 2,
    DECREASE = 0,
    INCREASE = 1

};

class Led
{
private:
    // basic configuration
    std::string name;
    uint8_t pin;
    StateControl<LedState> state;

public:
    Led(std::string name, uint8_t pin);

    // turn the led following the new indicated state without duplication - exp: won't send turn on led 2 times continuously
    void Turn(LedState);

    //
    bool SetBrightness(uint8_t);
};

class BlinkingLed : public Led
{
private:
    // data
    uint16_t interval;
    BlinkingLedState state;

    // rtos
    RealTime::TaskHandler *blinkingHandler;

public:
    BlinkingLed(std::string name, uint8_t pin, uint32_t interval);

    ~BlinkingLed();

    void SetBlinking(RealTime::TaskMode);

    static void Blinking(void *parameter)
    {
        BlinkingLed *led = static_cast<BlinkingLed *>(parameter);

        while (1)
        {
            switch (led->state)
            {
            case OFF_WAIT:
                // turn off led
                led->Turn(OFF);

                // wait for an interval time and switch to ON state
                vTaskDelay(led->interval / portTICK_PERIOD_MS);
                led->state = ON_WAIT;
                break;

            case ON_WAIT:
                // turn on led
                led->Turn(ON);

                // wait for an interval time and switch to ON state
                vTaskDelay(led->interval / portTICK_PERIOD_MS);
                led->state = OFF_WAIT;
                break;
            }
        }
    }
};

// REVIEW - why do I name it tweaking?? isn't that dimming :)
class TweakingLed : public Led
{
private:
    // data
    uint8_t tolerant;
    uint8_t dutyCycle;
    // rtos
    QueueHandle_t tweakingQueue; // queue of tweaking commands( increase or decrease the light level)
    RealTime::TaskHandler *TweakingHandler;
    // rtos
public:
    TweakingLed(std::string, uint8_t);

    ~TweakingLed();

    bool SetTolerant(uint8_t);

    bool AddTweakingCommand(Tendency);

    bool TweakingLightLevel(Tendency);

    static void Tweaking(void *parameter)
    {

        TweakingLed *led = static_cast<TweakingLed *>(parameter);
        Tendency tendency = IDLE;

        while (1)
        {

            if (xQueueReceive(led->tweakingQueue, (void *)&tendency, portMAX_DELAY))
            {
                led->TweakingLightLevel(tendency);
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
};

#endif