#pragma once
#include <mcp_can.h>
#include "ISender.hpp"
#include "Rtos.hpp"
#include <iostream>
#include <queue>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/*
   //TODO -
   1. check sending thread safe using mutex or queue
      - mutex make the code less complex
      - queue is more sufficient in term of real time os
  2. why the fk couldn't pass ??? mutex method?? for sending message
*/

enum ProccessingState
{
  SENDING,
  FINISHING,
};

struct CanData
{

  unsigned long msgId;
  byte len;
  byte command[4];

  CanData();
  CanData(unsigned long msgID, byte len);
};

class CANController : ISender
{
private:
  MCP_CAN CAN;
  uint8_t CAN_INT;

  QueueHandle_t sendDatas;
  // QueueHandle_t recievedMsgQueue;

public:
  CANController(uint8_t CAN_INT, uint8_t CAN_CS);

  CanData ReadCanMessage();

  // REVIEW - can be improve with type erasure
  void SendMessage(uint8_t msgId, float value) override
  {
    // Create a buffer for the message
    CanData data(msgId, sizeof(float));

    // copy value to the buffer
    memcpy(data.command, &value, sizeof(float));

    // send data to wait in queue
    xQueueSend(sendDatas, (void *)&data, portMAX_DELAY);
  }

  void SendMessage(uint8_t msgId, int value) override
  {
    // Create a buffer for the message
    CanData data(msgId, sizeof(int));
    Serial.println("check 1");

    // copy value to the buffer
    memcpy(data.command, &value, sizeof(int));

    // send data to wait in queue
    xQueueSend(sendDatas, (void *)&data, portMAX_DELAY);
  }

  static void SendMessageTask(void *parameter)
  {
    CANController *controller = static_cast<CANController *>(parameter);
    CanData data;
    while (1)
    {
      if (xQueueReceive(controller->sendDatas, (void *)&data, portMAX_DELAY))
      {
        Serial.print("can begin: ");
        byte sta = controller->CAN.sendMsgBuf(data.msgId, 0, data.len, data.command);

        Serial.print("can send msg: ");
         Serial.println(sta);
        Serial.print("msgId: ");
        Serial.println(data.msgId);
        Serial.print("len: ");
        Serial.println(data.len);
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  }
};