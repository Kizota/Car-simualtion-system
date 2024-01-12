#pragma once
#include <Arduino.h>
#include <ArduinoBlue.h>

#include "CanDataProcession.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class BLEController : public ICanListener {
 private:
  static ArduinoBlue *ble;
  void configureHM10();
  SemaphoreHandle_t mutex;
  QueueHandle_t rcdCmdQueue;  // recieved message queue
  ICanSender *can;

 public:
  BLEController(int rxPin, int txPin);
  void addSender(ICanSender *Can);
  void loopSendBLE();
  // static void loopRecieveCAN(CanData rcdData);
  bool RecieveMessage(CanData newData) override {
    return xQueueSend(rcdCmdQueue, (void *)&newData, portMAX_DELAY);
  }
  static void RecieveCAN(CanData rcdData) {
    int actualSpeed, RPM, leftFrontTirePressure, rightFrontTirePressure, leftRearTirePressure, rightRearTirePressure, temperature;

    switch (rcdData.msgId) {
      case NODE_ID_SPEED:
        actualSpeed = rcdData.command[0];
        ble->sendDisplayData(1, String(actualSpeed));
        Serial.println("actual speed: " + String(actualSpeed));
        break;
      case NODE_ID_RPM:
        RPM = rcdData.command[0];
        ble->sendDisplayData(2, String(RPM));
        Serial.println("RPM: " + String(RPM));
        break;
      case NODE_ID_PRESSURE:
        leftFrontTirePressure = rcdData.command[0];
        rightFrontTirePressure = rcdData.command[1];
        leftRearTirePressure = rcdData.command[2];
        rightRearTirePressure = rcdData.command[3];
        ble->sendDisplayData(3, String(leftFrontTirePressure));
        ble->sendDisplayData(4, String(rightFrontTirePressure));
        ble->sendDisplayData(5, String(leftRearTirePressure));
        ble->sendDisplayData(6, String(rightRearTirePressure));
        Serial.println("left front tire pressure: " + String(leftFrontTirePressure));
        Serial.println("right front tire pressure: " + String(rightFrontTirePressure));
        Serial.println("left rear tire pressure: " + String(leftRearTirePressure));
        Serial.println("right rear tire pressure: " + String(rightRearTirePressure));
        break;
      case NODE_ID_TEMPERATURE:
        temperature = rcdData.command[0];
        ble->sendDisplayData(7, String(temperature));
        Serial.println("temperature: " + String(temperature));
        break;
    }
  }
  static void ReadingCommandTask(void *parameter) {
    BLEController *bleController = static_cast<BLEController *>(parameter);
    CanData rcdData;
    while (1) {
      if (xQueueReceive(bleController->rcdCmdQueue, (void *)&rcdData, portMAX_DELAY)) {
        Serial.println("Reading command task");

        RecieveCAN(rcdData);
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  }
  // static void SendingBLETask(void *parameter) {
  //   BLEController *bleController = static_cast<BLEController *>(parameter);
  //   CanData rcdData;
  //   while (1) {
  //     if (xQueueReceive(bleController->rcdSendQueue, (void *)&rcdData, portMAX_DELAY)) {
  //       Serial.println("Sending ble task");
  //       bleController->loopSendBLE();
  //       vTaskDelay(10 / portTICK_PERIOD_MS);
  //     }
  //   }
  // }
};
