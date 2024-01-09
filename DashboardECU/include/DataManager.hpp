#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "CanDataProcession.hpp"
#include "WSDataProccession.hpp"

#include "DataControl.hpp"
#include "IInfoTracker.hpp"

#define MAX_QUEUES_DATA_LENGTH 20

enum
{
    INT,
    FLOAT
};

class DataManager : public ICanListener, public IDataTracker
{
private:
    DataControl<int> speed;
    DataControl<float> temperature;
    DataControl<float> pressure;

    int latesSpeed;
    int latesTemp;
    int latesPress;

    IWSSender *sender;

public:
    DataManager();

    bool AddSender(IWSSender* sender);

    bool RecieveMessage(CanData data) override
    {
        bool recievedState = false;
        switch (data.msgId)
        {
        case NODE_ID_SPEED_FEEDBACK:
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

    bool UpdateData(std::string info) override
    {
        if(sender == nullptr)
        {
            return false;
        }
         
        Serial.print("send info: -------: ");
    //    Serial.println(info.c_str());
        // update new value
        sender->SendMessage(DASHBOARD, info );
        return true;
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