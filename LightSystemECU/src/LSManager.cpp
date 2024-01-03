#include "LSManager.hpp"

LSManager::LSManager(uint8_t leftIndPin, uint8_t rightIndPin, uint8_t highBmPin) : leftInd("Left indicator",leftIndPin), rightInd("Right indicator",rightIndPin), highBm("High beam",highBmPin)
{
    //create queue for recieving the message 
    rcdCmdQueue = xQueueCreate(10,sizeof(CanData));
}





