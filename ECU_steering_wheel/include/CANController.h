#pragma once
#include <mcp_can.h>
#include "ISender.hpp"




struct CanData
{
  INT32U MessageID;
  INT8U len;
  INT8U command[8];
};

class CANController : ISender 
{
private:
  MCP_CAN CAN;
  uint8_t CAN_INT;

public:
  CANController(uint8_t CAN_INT, uint8_t CAN_CS);

  CanData ReadCanMessage();

  // REVIEW - can be improve with type erasure
  bool SendMessage(uint8_t msgId, float value) override 
  {
    // Create a buffer for the message
    byte buf[4];
    // Copy the float into the buffer
    memcpy(buf, &value, sizeof(float));
    // Send the message
    return (CAN.sendMsgBuf(msgId, 1, sizeof(float), buf) == CAN_OK);
  }

  bool SendMessage(uint8_t msgId, int value) override 
  {
    // Create a buffer for the message
    byte buf[4];
    // Copy the float into the buffer
    memcpy(buf, &value, sizeof(int));
    // Send the message
    return (CAN.sendMsgBuf(msgId, 1, sizeof(int), buf) == CAN_OK);
  }

  // int CAN_print_message(CanData *msg);
};