#ifndef SWMANAGER_HPP
#define SWMANAGER_HPP

#include "Button.hpp"
#include "JoyStick.hpp"

enum CommandState
{
    TRIGGER,
    RUN,
    DONE
};

class SWManager
{
private:
    Button *indBt;  // indicator button
    Button *hgBmBt; // high beam button
    SemaphoreHandle_t commandMutex;

    JoyStick *joyStk;

public:
    SWManager(Button *, Button *, JoyStick *joyStick);

    // handle read configured mode
    static void HandleSettingMode(void *parameter)
    {
        SWManager *sw = static_cast<SWManager *>(parameter);

        while (1)
        {
            if (sw->indBt->IsPressed())
            {
                // call indicator button handling
                //   xTaskCreate();
            }
            else if (sw->hgBmBt->IsPressed())
            {
                
            }
        }
    }

    static void HandleIndicatorSet(void *parameter)
    {
        SWManager *sw = static_cast<SWManager *>(parameter);
        CommandState state = RUN;
        Direction direction = UNKNOWN;

        while (1)
        {
            switch (state)
            {
            case TRIGGER:
                if (xSemaphoreTake(sw->commandMutex, portMAX_DELAY) == pdTRUE)
                {
                    //turn on joystick reading 
                    state = RUN;
                }
           
                break;
            case RUN:
                // detect and send command
                direction = sw->joyStk->GetDirection();
               
                switch (direction)
                {
                case LEFT:
                    // send command to the left
                    break;
                case RIGHT:

                    break;
                }

                if (!sw->indBt->IsPressed())
                {
                    state = DONE;
                }
                break;
            case DONE:
            
               //turn off joystick reading 
                xSemaphoreGive(sw->commandMutex);
                break;
            }
        }
    }
};

#endif