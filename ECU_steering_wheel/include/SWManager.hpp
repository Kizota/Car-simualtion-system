#ifndef SWMANAGER_HPP
#define SWMANAGER_HPP

#include "CANController.h"

#include "Button.hpp"
#include "JoyStick.hpp"
#include "StateControl.hpp"

/*!SECTION
//TODO
1. optimize handle send command to indicator process
2. implement queue as receive a sequence of commands
3. debounce directions result -> stable and less spamming message

*/

enum CommandState
{
    TRIGGER,
    RUN,
    DONE
};

enum HandleSetModeState
{
    WAIT,
    CHECK_SETTING,
};

enum IndicatorType
{
    LEFT_IND,
    RIGHT_IND
};

class SWManager
{
private:
    Button *indBt;    // indicator button
    Button *hgBmBt;   // high beam button
    JoyStick *joyStk; // joystick

    // RTOS
    SemaphoreHandle_t commandMutex;

    // CAN
    ISender *sender;

public:
    SWManager(Button *, Button *, JoyStick *);

    void SetSender(ISender *sender);

private:
    // handle read configured mode
    static void HandleSettingMode(void *parameter)
    {
        SWManager *sw = static_cast<SWManager *>(parameter);
        while (1)
        {

            if (sw->indBt->IsPressed())
            {
                // call indicator button handling
                Serial.println("indicator task is");
                xTaskCreate(SWManager::HandleIndicatorCommand, "Indiciator command", stackDepth, sw, 2, NULL);
            }
            else if (sw->hgBmBt->IsPressed())
            {
            }
            else if (sw->joyStk->IsPressed())
            {
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    static void HandleIndicatorCommand(void *parameter)
    {
        SWManager *sw = static_cast<SWManager *>(parameter);
        CommandState state = TRIGGER;

        Direction preDirect = UNKNOWN;
        Direction direct = UNKNOWN;

        // state of reading command from joystick
        StateControl<Direction> control(UNKNOWN, UNKNOWN);
        while (1)
        {
            switch (state)
            {

            case TRIGGER:

                if (xSemaphoreTake(sw->commandMutex, portMAX_DELAY) == pdTRUE)
                {
                    if (sw->sender != nullptr)
                    {

                        // turn on joystick reading
                        Serial.println("indicator task is trigger!");
                        sw->joyStk->SetReadMode(ON);
                        state = RUN;
                    }
                    else
                    {
                        Serial.println("sender is not available!");
                        state = DONE;
                    }
                }

                break;
            case RUN:
                // detect and send command
                control.UpdateState(sw->joyStk->GetDirection());
                direct = sw->joyStk->GetDirection();
                if (preDirect != direct)
                {
                    Serial.println(direct);
                    preDirect = direct;
                }
                switch (control.GetState(false))
                {
                case LEFT:
                    if (control.IsNewState())
                    /// turn on the left if off
                    {
                        Serial.println("turn left indicator on");
                        sw->sender->SendMessage(NODE_ID_INDICATOR, LEFT_IND_ON);
                        control.Refresh();
                    }
                    break;
                case RIGHT:
                    if (control.IsNewState())
                    // turn onn the right if off
                    {
                        Serial.println("turn right indicator on");
                        sw->sender->SendMessage(NODE_ID_INDICATOR, RIGHT_IND_ON);
                        control.Refresh();
                    }
                case CENTRE:
                    if (control.IsNewState() && sw->joyStk->IsPressed())
                    // turn off both
                    {
                        Serial.println("turn off both indicators");
                        sw->sender->SendMessage(NODE_ID_INDICATOR, BOTH_OFF);

                        control.Refresh();
                    }
                    break;
                }

                // cancel task
                if (!sw->indBt->IsHold())
                {
                    state = DONE;
                }
                break;
            case DONE:
                Serial.println("indicator task is done!");
                // turn off joystick reading
                sw->joyStk->SetReadMode(OFF);
                xSemaphoreGive(sw->commandMutex);
                vTaskDelete(NULL);
                break;
            }
        }
    }

    static void HandleHighBeamIndicator(void *parameter)
    {
        SWManager *sw = static_cast<SWManager *>(parameter);
        CommandState state = TRIGGER;
        Timer timer(500); // interval for changing the light

        StateControl<Direction> control(UNKNOWN, UNKNOWN);
        switch (state)
        {
        case TRIGGER:
            break;
        case RUN:
            break;
        case DONE:
            break;
        }
    }
};

#endif