#include <Arduino.h>

#include "DataManager.hpp"
#include "DashBoard.hpp"
#include "CANController.h"

#define INT_PIN 0
#define CS_PIN 5

Dashboard *dashboard;
DataManager *dataManager;
CANController *canController;

void setup()
{
  Serial.begin(9600);
  dashboard = new Dashboard();
  dataManager = new DataManager((IInfoTracker *)dashboard);
  canController = new CANController(INT_PIN, CS_PIN, (ICanListener *)dataManager);
   
  //add id mask
  canController->AddIdMask(NODE_ID_SPEED_FEEDBACK);
  canController->AddIdMask(NODE_ID_TEMPERATURE);
  canController->AddIdMask(NODE_ID_PRESSURE);
}

void loop()
{

}
