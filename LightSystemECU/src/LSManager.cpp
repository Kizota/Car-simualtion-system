#include "LSManager.hpp"

#define BLINKING_INTERVAL 1000
#define TWEAKING_TOLERANT 40

LSManager::LSManager(uint8_t leftIndPin, uint8_t rightIndPin, uint8_t highBmPin) : leftInd("Left indicator",leftIndPin,BLINKING_INTERVAL), rightInd("Right indicator",rightIndPin,BLINKING_INTERVAL), highBm("High beam",highBmPin)
{
     //configure high beam 
     highBm.SetTolerant(TWEAKING_TOLERANT);

    //create queue for recieving the message 
    rcdCmdQueue = xQueueCreate(10,sizeof(CanData));
}





