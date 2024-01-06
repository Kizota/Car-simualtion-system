#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "CanDataProcession.hpp"
#include "DataControl.hpp"
#include "IInfoTracker.hpp"

#define MAX_QUEUES_DATA_LENGTH 20

enum
{
    INT,
    FLOAT
};

class DataManager : public ICanListener
{
private:
    DataControl<int> speed;
    DataControl<float> temperature;
    DataControl<float> pressure;


public:
    DataManager(IInfoTracker *);

    bool RecieveMessage(CanData data) override
    {
        bool recievedState = false;
        switch (data.msgId)
        {
        case NODE_ID_SPEED:
            recievedState = speed.ReceiveData(data.command[0]);
            break;

        case NODE_ID_TEMPERATURE:
            recievedState = temperature.ReceiveData(ConvertBuffToFloat(data.command));
            break;

        case NODE_ID_PRESSURE:
        
            recievedState = pressure.ReceiveData(ConvertBuffToFloat(data.command));
            break;
        }

        return recievedState;
    }

private:
    static float ConvertBuffToFloat(byte *buff)
    {
        float result = 0;
        memcpy((void *)&result, (void *)buff, sizeof(float));
        return result;
    }
};

#endif