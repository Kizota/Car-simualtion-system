#ifndef SERVOMANAGER_HPP
#define SERVOMANAGER_HPP

#include "MyServo.hpp"
#include "CanDataProcession.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class ServoManager : public ICanListener
{
private:
    PServo servo;

    SemaphoreHandle_t mutex;
    QueueHandle_t rcdCmdQueue;

public:
    ServoManager(uint8_t servoPin);

    bool RecieveMessage(CanData newData) override
    {
        return xQueueSend(rcdCmdQueue, (void *)&newData, portMAX_DELAY);
    }
    int GetSpeed()
    {
        return servo.currentSpeed;
    }


private:
    static void ReadingCommandTask(void *parameter)
    {
        Serial.println("interpreting command for servo");
        ServoManager *servoManager = static_cast<ServoManager *>(parameter);
        CanData rcdData;
        while (1)
        {
            if (xQueueReceive(servoManager->rcdCmdQueue, (void *)&rcdData, portMAX_DELAY))
            {
                if(rcdData.msgId != NODE_ID_MOTOR)
                {
                    //Serial.println("message not for servo");
                }
                else
                {
                    HandleSpeedCommand(rcdData.command[0], &servoManager->servo);
                }

                
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }


    static bool HandleSpeedCommand(byte command, PServo *servo)
    {

        if(command == SPEED_UP)
        {
            if(servo->currentSpeed < 10)
            {
            servo->currentSpeed++;
            }
        }
        else if(command == SPEED_DOWN)
        {
            if(servo->currentSpeed > -10)
            {
            servo->currentSpeed--;
            }
        }
        else if(command == SPEED_STOP)
        {
            servo->currentSpeed = 0;
        }
        else
        {
            return false;
        }
        return servo->ChangeSpeed();
    }
};
#endif