#include "Rtos.hpp"
#include <Arduino.h>

TaskHandler::TaskHandler(std::string name, TaskFunction_t task, void *param) : name(name), task(task), param(param), mode(VOID), preMode(VOID)
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
            xTaskCreate(task, name.c_str(), stackDepth, param, 1, &handler);
            Refresh();
        }

        break;
    case OFF:
        if (IsNewMode())
        // turn off task
        {
            vTaskDelete(handler);
            handler = NULL;
            Refresh();
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