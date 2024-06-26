#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP


#include "CanDataProcession.hpp"
#include "DataControl.hpp"
#include "IInfoTracker.hpp"
#include "StateControl.hpp"


class Dashboard : IDataTracker
{
private:
    // info state
    int speed;
    float temperature;
    float pressure;

    // rtos
    SemaphoreHandle_t displayMutex;
    RealTime::TaskHandler *taskHandler;

public:
    Dashboard();

    ~Dashboard();

    // bool UpdateData(std::pair<InfoType, void *> info) override
    // {
    //     if (info.second == nullptr)
    //     {
    //         return false;
    //     }
  
    //     // initial void pointer and point to update value
    //     void *val = nullptr;
    //     switch (info.first)
    //     {
    //     case SPEED:
    //     Serial.println("speed updating ");
    //         val = &speed;
    //         break;
    //     case TEMP:
    //         val = &temperature;
    //         break;
    //     case PRESSURE:
    //         val = &pressure;
    //         break;
    //     default:
    //         return false;
    //         break;
    //     }
         
        
    //     // update new value
    //     memcpy(val, info.second, sizeof(int));

    //    taskHandler->SetMode(RealTime::ON);
    //     return true;
    // }

private:
    static void DisplayData(void *parameter)
    {
        Dashboard *db = static_cast<Dashboard *>(parameter);
        Serial.println(" ----- DASHBOARD ------");

        if (xSemaphoreTake(db->displayMutex, portMAX_DELAY))
        {
            Serial.println("Speed: ");
            Serial.println(db->speed);
            Serial.println("Pressure: ");
            Serial.println(db->pressure);
            Serial.println("Temperature: ");
            Serial.println(db->temperature);
            xSemaphoreGive(db->displayMutex);
        }

        Serial.println(" -----         ------");

        db->taskHandler->SetMode(RealTime::OFF);
    }

    void UpdateInfoByType();
};

#endif