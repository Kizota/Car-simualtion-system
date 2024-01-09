#include "DataManager.hpp"

#define QUEUE_MAX_LENGTH 10

DataManager::DataManager() : speed("SPEED", QUEUE_MAX_LENGTH, true, this),
                                                  temperature("TEMP", QUEUE_MAX_LENGTH, true, this),
                                                  pressure("PRESSURE", QUEUE_MAX_LENGTH, true, this), 
                                                  sender(nullptr)
{   
}

bool DataManager::AddSender(IWSSender *sender)
{
    if(sender == nullptr)
    {
        return false;
    }

   this->sender = sender;

    return true;
}