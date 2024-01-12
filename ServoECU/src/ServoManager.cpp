#include "ServoManager.hpp"


ServoManager::ServoManager(uint8_t servoPin) : servo(servoPin)
{
    // create queue for recieving the message
    rcdCmdQueue = xQueueCreate(10, sizeof(CanData));
    xTaskCreate(ServoManager::ReadingCommandTask, "ReaingCommandTask", stackDepth, this, 1, NULL);
}
