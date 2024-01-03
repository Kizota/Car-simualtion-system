#include "CANController.h"

#define MAX_SIZE_MESSAGE 4 // 4 byte

CanData::CanData() : msgId(0), len(0)
{
  command[8] = {0};
}

CanData::CanData(unsigned long msgId, byte len) : msgId(msgId), len(len)
{
  command[8] = {0};
}


// CANController class definition
CANController::CANController(uint8_t CAN_INT, uint8_t CAN_CS) : CAN(CAN_CS), CAN_INT(CAN_INT)
{
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

  //create mutex for sending the message one by one 
   sendDatas = xQueueCreate(10,sizeof(CanData));
   xTaskCreate(CANController::SendMessageTask,"SendingMessage",stackDepth,this,1,NULL);
  }

CanData CANController::ReadCanMessage()
{
  CanData data;
  if (!digitalRead(CAN_INT))
  {
    CAN.readMsgBuf(&data.msgId, &data.len, data.command);
    return data;
  }
  return CanData(); // Return a default-constructed CanData object if no message is available
}

// print the CAN message
// int CAN_print_message(CanData *msg) {
//   if (msg == nullptr) {
//     return 0;
//   }

//   if (msg->data == msg->pre_content) {
//     return 0;
//   }
//   Serial.print("id: ");
//   Serial.println(msg->messageID, HEX);
//   Serial.print("len: ");
//   Serial.println(msg->len);
//   Serial.print("cotent: ");
//   Serial.println(msg->content);

//   msg->pre_content = msg->content;

//   return 1;
// }