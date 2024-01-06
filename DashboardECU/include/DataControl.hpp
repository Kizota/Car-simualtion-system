#ifndef DATACONTROL_HPP
#define DATACONTROL_HPP

#include <iostream>
#include "Rtos.hpp"
#include "IInfoTracker.hpp"

//

template <typename T>
class DataControl
{
private:
    InfoType type;
    bool skipOutdate;

    QueueHandle_t dataQueue;

    IInfoTracker *tracker;

public:
    DataControl(InfoType, int, bool, IInfoTracker *);

    ~DataControl();

    bool ReceiveData(T);

    T *GiveData();

private:
    static void UpdateInfoTask(void *parameter)
    {
        DataControl *control = static_cast<DataControl *>(parameter);

        // get data from queue
        T *data = control->GiveData();

        // package the data with meaningful info type
        std::pair<InfoType, void *> info;
        info.first = control->type;
        info.second = (void *)data;

        //send to the data tracker
        control->tracker->UpdateInfo(info);

        operator delete(data);
        vTaskDelete(NULL);
    }
};

#endif