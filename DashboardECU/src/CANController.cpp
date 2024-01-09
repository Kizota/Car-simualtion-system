#include "CANController.h"
#include <algorithm>

// CANController constructor
CANController::CANController(uint8_t CAN_INT, uint8_t CAN_CS, ICanListener* listener) : CAN(CAN_CS), CAN_INT(CAN_INT), listener(listener)
{
  Serial.println("can controll constructor");
  // connect to CAN network
  while (CAN_OK != CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ))
  {
    Serial.println("CAN BUS init failed");
    delay(100);
  }

  // set CAN mode
  CAN.setMode(MCP_NORMAL);
  pinMode(CAN_INT, INPUT);

  Serial.println("CAN Bus init ok!\n");

  // create queue to store and task to send the messages
  sendDatas = xQueueCreate(10, sizeof(CanData));
  xTaskCreate(CANController::SendMessageTask, "SendMessageTask", stackDepth, this, 1, NULL);
  xTaskCreate(CANController::ReadMessageTask, "ReadMessageTask", stackDepth, this, 1, NULL);
}

bool CANController::AddListener(ICanListener *listener)
{
  if (listener != nullptr)
  {
    return false;
  }

  listener = listener;
  return true;
}

bool CANController::AddIdMask(unsigned long idMask)
{
  // check and terminate if mask is already exist
  auto result = std::find(idMasks.begin(), idMasks.end(), idMask);

  if (result != idMasks.end())
  {
    return false;
  }

  // add new idMask
  idMasks.push_back(idMask);
  return true;
}

bool CANController::IsMessageIdValid(unsigned long msgId)
{
  bool isValid = true;

  auto result = std::find(idMasks.begin(), idMasks.end(), msgId);

  if (result == idMasks.end())
  {
    isValid = false;
  }

  return isValid;
}

CanData CANController::ReadMessage()
{
  CanData data;
  // if (!digitalRead(CAN_INT))
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&data.msgId, &data.len, data.command);
    // Serial.print("msg id:");
    // Serial.println(data.msgId);
    // Serial.print("msg len:");
    // Serial.println(data.len);
    // Serial.print("msg command:");
    // Serial.println(data.command[0]);

    return data;
  }
  return CanData(); // Return a default-constructed CanData object if no message is available
}
