#ifndef LSMANAGER_HPP
#define LSMANAGER_HPP

#include "Led.hpp"
#include "CanDataProcession.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"




class LSManager : public ICanListener
{
private:
    // light system component
    Led leftInd;  // left indicator
    Led rightInd; // right indicator
    Led highBm;   // high beam

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
                switch (rcdData.msgId)
                {
                case NODE_ID_INDICATOR:

                    break;

                }
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }


    static void HandleHighbeamIndicator(void *paramter)
    {
    }
};

#endif