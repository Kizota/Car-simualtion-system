#include "DataControl.hpp"

// template <typename T>
// DataControl<T>::DataControl(InfoType type, int numOfLen, bool skipOutdate, IInfoTracker *tracker) : type(type), skipOutdate(skipOutdate), tracker(tracker)
// {
//     dataQueue = xQueueCreate(numOfLen, sizeof(T));

//     std::string name = "data control " + std::to_string(type);
//     handler = new RealTime::TaskHandler(name, DataControl::UpdateInfoTask, this);
//     handler->SetMode(RealTime::ON);
// }

// template <typename T>
// DataControl<T>::~DataControl()
// {
//     handler->SetMode(RealTime::OFF);
//     vQueueDelete(dataQueue);
// }

// template <typename T>
// bool DataControl<T>::ReceiveData(T data)
// {
//     T *test = operator new(sizeof(T));
//     if (uxQueueSpacesAvailable(dataQueue) == 0 && skipOutdate)
//     {
//         GiveData(test);
//     }

//     return xQueueSend(dataQueue, (void *)&data, portMAX_DELAY);
// }

// template <typename T>
// T *DataControl<T>::GiveData()
// {
//     T *data = operator new(sizeof(T));
//     xQueueReceive(dataQueue, (void *)data, portMAX_DELAY);

//     return data;
// }
