#include "DataManager.hpp"

#define QUEUE_MAX_LENGTH 10

DataManager::DataManager(IInfoTracker *tracker) : speed(SPEED, QUEUE_MAX_LENGTH, true, tracker),
                                                  temperature(TEMP, QUEUE_MAX_LENGTH, true, tracker),
                                                  pressure(PRESSURE, QUEUE_MAX_LENGTH, true, tracker)
{   
}