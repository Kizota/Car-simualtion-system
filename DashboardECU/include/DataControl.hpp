#ifndef DATACONTROL_HPP
#define DATACONTROL_HPP

#include <iostream>
#include "Rtos.hpp"
#include "IInfoTracker.hpp"
#include <Arduino.h>

//how to delete the pointer in the task
template <typename T>
class DataControl
{
private:
    InfoType type;
    bool skipOutdate;

    // data storage
    QueueHandle_t dataQueue;
    IInfoTracker *tracker;

    // rtos
    RealTime::TaskHandler *handler;

public:
    DataControl(InfoType type, int numOfLen, bool skipOutdate, IInfoTracker *tracker) : type(type), skipOutdate(skipOutdate), tracker(tracker)
    {
        dataQueue = xQueueCreate(numOfLen, sizeof(T));

        std::string name = "data control " + std::to_string(type);
        handler = new RealTime::TaskHandler(name, DataControl::UpdateInfoTask, this);
        handler->SetMode(RealTime::ON);
    }

    ~DataControl()
    {
        handler->SetMode(RealTime::OFF);
        vQueueDelete(dataQueue);
    }

    bool ReceiveData(T data)
    {
        T *test = (T *)operator new(sizeof(T));
        if (uxQueueSpacesAvailable(dataQueue) == 0 && skipOutdate)
        {
            test = GiveData();
        }

        return xQueueSend(dataQueue, (void *)&data, portMAX_DELAY);
    }

    T *GiveData()
    {
        T *data = (T *)operator new(sizeof(T));
        xQueueReceive(dataQueue, (void *)data, portMAX_DELAY);

        return data;
    }

private:
    static void UpdateInfoTask(void *parameter)
    {
        DataControl *control = static_cast<DataControl *>(parameter);
        T *data = nullptr;

        while (1)
        {
            // get data from queue
            T *data = control->GiveData();

            // package the data with meaningful info type
            std::pair<InfoType, void *> info;
            info.first = control->type;
            info.second = (void *)data;

            // send to the data tracker
            control->tracker->UpdateInfo(info);

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }

    }
};

#endif