#include "LSManager.hpp"

#define BLINKING_INTERVAL 1000
#define TWEAKING_TOLERANT (uint8_t)40

LSManager::LSManager(uint8_t leftIndPin, uint8_t rightIndPin, uint8_t highBmPin) : leftInd("Left indicator", leftIndPin, BLINKING_INTERVAL), rightInd("Right indicator", rightIndPin, BLINKING_INTERVAL), highBm("High beam", highBmPin)
{
  // configure high beam
  Serial.println("ls contstr");

  highBm.SetTolerant(TWEAKING_TOLERANT);

  //   rightInd.SetBlinking(RealTime::ON);
  // create queue for recieving the message
  rcdCmdQueue = xQueueCreate(10, sizeof(CanData));
  xTaskCreate(LSManager::ReadingCommandTask, "ReaingCommandTask", stackDepth, this, 1, NULL);
  Serial.println("ls contstr");
  leftInd.Turn(ON);
  rightInd.Turn(ON);
  highBm.Turn(ON);
}
