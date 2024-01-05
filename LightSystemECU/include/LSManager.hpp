#ifndef LSMANAGER_HPP
#define LSMANAGER_HPP

#include "Led.hpp"
#include "CanDataProcession.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/*
//REVIEW - opitimize code
  1. seperate two task of handling ind and high beam into 2 tasks
  2. why could not pass by &
*/

class LSManager : public ICanListener
{
private:
    // light system component
    BlinkingLed leftInd;  // left indicator
    BlinkingLed rightInd; // right indicator
    TweakingLed highBm;   // high beam

    // rtos
    SemaphoreHandle_t mutex;
    QueueHandle_t rcdCmdQueue; // recieved message queue

public:
    LSManager(uint8_t leftIndPin, uint8_t rightIndPin, uint8_t highBm);

    bool RecieveMessage(CanData newData) override
    {
        return xQueueSend(rcdCmdQueue, (void *)&newData, portMAX_DELAY);
    }

private:
    static void ReadingCommandTask(void *parameter)
    {
        LSManager *lsManager = static_cast<LSManager *>(parameter);
        CanData rcdData;
        while (1)
        {
            if (xQueueReceive(lsManager->rcdCmdQueue, (void *)&rcdData, portMAX_DELAY))
            {
                Serial.println("haha in reading command task");

                switch (rcdData.msgId)
                {
                case NODE_ID_INDICATOR:
                    // lsManager->leftInd.SetBlinking(RealTime::ON);
                    lsManager->rightInd.SetBlinking(RealTime::ON);
                    HandleIndicatorCommand(rcdData.command[0], &lsManager->leftInd, &lsManager->rightInd);
                    break;
                case NODE_ID_HIGHBEAM:
                    Serial.println("recieve command for high beam");
                    HandleHighHBeamCommand(rcdData.command[0], &lsManager->highBm);
                    break;
                }
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    static bool HandleIndicatorCommand(byte command, BlinkingLed *leftInd, BlinkingLed *rightInd)
    {
        // checking if command is out of range
        if (command < LEFT_IND_ON || command > BOTH_OFF)
        {
            return false;
        }

        LighSystemCommand cmd = static_cast<LighSystemCommand>(command);

        // turn on and off indiccators according to the command
        switch (cmd)
        {
        case LEFT_IND_ON:
            leftInd->SetBlinking(RealTime::ON);
            rightInd->SetBlinking(RealTime::OFF);

            break;
        case RIGHT_IND_ON:
            Serial.println("right indicator");

            leftInd->SetBlinking(RealTime::OFF);
            rightInd->SetBlinking(RealTime::ON);

            break;
        case BOTH_OFF:
            leftInd->SetBlinking(RealTime::OFF);
            rightInd->SetBlinking(RealTime::OFF);
            break;
        }

        return true;
    }

    static bool
    HandleHighHBeamCommand(byte command, TweakingLed *highBm)
    {
        if (command < DECREASE || command > INCREASE)
        {
            Serial.println("sth wrong with the command !");
            return false;
        }
        Serial.println("accept command and add the tweaking command");
        // add tweaking cmd to regulate the brightness of the high beam
        Tendency tendency = static_cast<Tendency>(command);
        return highBm->AddTweakingCommand(tendency);
    }
};

#endif