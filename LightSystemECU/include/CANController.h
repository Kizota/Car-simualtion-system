#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <iostream>
#include <list>
#include <queue>

#include "CanDataProcession.hpp"
#include "Rtos.hpp"

/*
   //TODO -
  1. why the fk couldn't pass ??? mutex method?? for sending message
*/

class CANController : ICanSender {
 private:
  // CAN data
  MCP_CAN CAN;
  uint8_t CAN_INT;
  std::list<unsigned long> idMasks;

  // Listener
  ICanListener *listener;

  // RTOS
  QueueHandle_t sendDatas;
  QueueHandle_t rcdDatas;  // recieved message queue

 public:
  CANController(uint8_t CAN_INT, uint8_t CAN_CS, ICanListener *listener);

  bool AddListener(ICanListener *);

  bool AddIdMask(unsigned long idMask);

  bool IsMessageIdValid(unsigned long msgId);

  CanData ReadMessage();

  // REVIEW - can be improve with type erasure
  void SendMessage(uint8_t msgId, float value) override {
    // Create a buffer for the message
    CanData data(msgId, sizeof(float));

    // copy value to the buffer
    memcpy(data.command, &value, sizeof(float));

    // send data to wait in queue
    xQueueSend(sendDatas, (void *)&data, portMAX_DELAY);
  }

  void SendMessage(uint8_t msgId, int value) override {
    // Create a buffer for the message
    CanData data(msgId, sizeof(int));

    // copy value to the buffer
    memcpy(data.command, &value, sizeof(int));

    // send data to wait in queue
    xQueueSend(sendDatas, (void *)&data, portMAX_DELAY);
  }

 private:
  static void SendMessageTask(void *parameter) {
    CANController *controller = static_cast<CANController *>(parameter);
    CanData data;
    while (1) {
      if (xQueueReceive(controller->sendDatas, (void *)&data, portMAX_DELAY))
      // sending message waiting in sending message queue
      {
        byte sta = controller->CAN.sendMsgBuf(data.msgId, 0, data.len, data.command);
      }

      // yield time for the other task - stop trigger dog - try taskScheduler
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }

  // REVIEW - check optimize the code
  static void ReadMessageTask(void *parameter) {
    CANController *controller = static_cast<CANController *>(parameter);
    if (controller->listener == nullptr)
    // cancelling the task if listener is not
    {
      vTaskDelete(NULL);
      return;
    }

    while (1) {
      CanData newData = controller->ReadMessage();
      if (newData.msgId != 0 && controller->IsMessageIdValid(newData.msgId))
      // only call listener to handle the message, when it is valid
      {
        Serial.print("id valid: ");
        Serial.println(newData.msgId);
        controller->listener->RecieveMessage(newData);
      }

      // yield time for the other task - stop trigger dog - try taskScheduler
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }
};

//  if (newData.msgId != 0)
//       // only call listener to handle the message, when it is valid
//       {
//         Serial.println(newData.msgId);
//         Serial.println(newData.command[0]);
//         // controller->listener->RecieveMessage(newData);
//       }