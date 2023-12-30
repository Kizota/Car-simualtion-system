#ifndef RTOS_HPP
#define RTOS_HPP

#include <iostream>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

static const int stackDepth = 2000;

enum TaskMode
{
    VOID,
    ON,
    OFF
};

// TODO - using template instead of void
class TaskHandler
{
private:
    TaskMode mode;
    TaskMode preMode;

    std::string name;
    void *param; //
    TaskFunction_t task;
    TaskHandle_t handler;

public:
    TaskHandler(std::string name, TaskFunction_t, void *param);

    void SetMode(TaskMode);

    bool IsNewMode();

    void Refresh();
};

#endif