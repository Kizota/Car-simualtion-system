#include "CANController.h"

// CANController class definition
CANController::CANController(uint8_t CAN_INT, uint8_t CAN_CS) : CAN(CAN_CS), CAN_INT(CAN_INT) {
  // connect to CAN network
  // while (CAN_OK != CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ)) {
  Serial.println("CAN BUS init failed");
  delay(100);
  // }

  // set CAN mode
  CAN.setMode(MCP_NORMAL);
  pinMode(CAN_INT, INPUT);

  Serial.println("CAN Bus init ok!\n");
}

// REVIEW - Float and int can? be mixed as they both use 4 bytes
bool CANController::SendFloatCanMessage(uint8_t messageID, float value) {
  // Create a buffer for the message
  byte buf[4];
  // Copy the float into the buffer
  memcpy(buf, &value, sizeof(float));
  // Send the message
  return (CAN.sendMsgBuf(messageID, 1, sizeof(float), buf) == CAN_OK);
}

bool CANController::SendIntCanMessage(uint8_t messageID, int value) {
  // Create a buffer for the message
  byte buf[4];
  // Copy the float into the buffer
  memcpy(buf, &value, sizeof(int));
  // Send the message
  return (CAN.sendMsgBuf(messageID, 1, sizeof(int), buf) == CAN_OK);
}

CanData CANController::ReadCanMessage() {
  CanData data;
  if (!digitalRead(CAN_INT)) {
    CAN.readMsgBuf(&data.MessageID, &data.len, data.command);
    return data;
  }
  return CanData();  // Return a default-constructed CanData object if no message is available
}

// print the CAN message
// int CAN_print_message(CanData *msg) {
//   if (msg == nullptr) {
//     return 0;
//   }

//   if (msg->content == msg->pre_content) {
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