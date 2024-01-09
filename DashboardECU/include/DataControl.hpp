#ifndef DATACONTROL_HPP
#define DATACONTROL_HPP

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Rtos.hpp"
#include "IInfoTracker.hpp"
#include <Arduino.h>

// how to delete the pointer in the task
template <typename T>
class DataControl
{
private:
    //    InfoType type;
    std::string type;
    bool skipOutdate;

    // data storage
    QueueHandle_t dataQueue;
    IDataTracker *tracker;

    // rtos
    RealTime::TaskHandler *handler;

public:
    // DataControl(InfoType type, int numOfLen, bool skipOutdate, IDataTracker *tracker) : type(type), skipOutdate(skipOutdate), tracker(tracker)
    DataControl(std::string type, int numOfLen, bool skipOutdate, IDataTracker *tracker) : type(type), skipOutdate(skipOutdate), tracker(tracker)
    {
        dataQueue = xQueueCreate(numOfLen, sizeof(T));

        std::string name = "data control " + type;
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
            Serial.println("update data-------: ");

            Serial.println(*data);
            // limit string to 2 decimal

            std::string sdata = control->type + "-" + std::to_string(*data);
            // send to the data tracker
            // Serial.println("------- update info task -----");
            // Serial.println(control->type.c_str());
            // Serial.println(sdata.c_str());
            control->tracker->UpdateData(sdata);

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
};

#endif