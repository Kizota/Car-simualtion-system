#include "Rtos.hpp"
#include <Arduino.h>

namespace RealTime
{
    TaskHandler::TaskHandler(std::string name, TaskFunction_t task, void *param) : name(name), task(task), param(param), handler(NULL), mode(VOID), preMode(VOID)
    {
    }

    void TaskHandler::SetMode(TaskMode newMode)
    {
        mode = newMode;
        switch (mode)
        {
        case ON:
            if (IsNewMode())
            // turn on task only one time prevent many task run at a time
            {
                Refresh();
                xTaskCreate(task, name.c_str(), stackDepth, param, 1, &handler);
            }

            break;
        case OFF:
            if (IsNewMode() && handler != NULL)
            // turn off task
            {
                handler = NULL;

                Refresh();
                FreeAllMutex();
                vTaskDelete(handler);
            }

            break;
        }
    }

    bool TaskHandler::IsNewMode()
    {
        return mode != preMode;
    }

    void TaskHandler::Refresh()
    {
        preMode = mode;
    }

    void TaskHandler::AddMutex(SemaphoreHandle_t newMutex)
    {
        mutexes.push_back(newMutex);
    }

    void TaskHandler::FreeAllMutex()
    {
        // give all mutex
        for (auto mutex : mutexes)
        {
            // only free
            if (uxSemaphoreGetCount(mutex) == 0)
                xSemaphoreGive(mutex);
        }
    }
}