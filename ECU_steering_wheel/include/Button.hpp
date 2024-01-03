#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <string>

#include <Arduino.h>
#include "Rtos.hpp"
#include "config.hpp"
#include "Timer.hpp"


// REVIEW
/*
   1. implement thread safe with queue
     - but new update into queue 
     - read gonna read after that 
*/

class Button
{
private:
    std::string name;
    uint8_t pin;

    // read data
    uint8_t preState;
    uint8_t state;
    bool isPressed;
    
    //timer for debouncing
    Timer *timer;
    
    //RTOS
    SemaphoreHandle_t readMutex;
    TaskHandler *readTaskHandler;
public:
    Button(std::string name, int pin, uint32_t interval);

    ~Button();
    
    bool IsPressed();

    bool IsHold();

    // on will turn on the read task, off will delete the task
    void SetReadMode(TaskMode mode);
    // delete task

    static void ReadSignal(void *paramater)
    {
        while (1)
        {
            Button *button = static_cast<Button *>(paramater);

            int reading = digitalRead(button->pin);

            // denoise from button ciruit
            if (reading != button->preState)
            {

                button->timer->ReFresh();
            }
        
            // debounce
            if (button->timer->IsTimeOut())
            {
                if (button->state != reading)
                {
                    // record the button state
                    if (xSemaphoreTake(button->readMutex, portMAX_DELAY))
                    // update state with thread safe
                    {
                        button->state = reading;
                        if(reading == LOW)
                        {
                            button->isPressed = true;
                        }
                        xSemaphoreGive(button->readMutex);
                    }
                }
            }

            button->preState = reading;
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
};

#endif