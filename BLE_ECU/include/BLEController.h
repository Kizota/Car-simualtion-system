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

 private:
  bool RecieveMessage(CanData newData) override {
    return xQueueSend(rcdCmdQueue, (void *)&newData, portMAX_DELAY);
  }
  static float ConvertBuffToFloat(byte *buff) {
    float result = 0;
    memcpy((void *)&result, (void *)buff, sizeof(float));
    return result;
  }
  static void RecieveCAN(CanData rcdData) {
    int Speed;
    float leftFrontTirePressure, rightFrontTirePressure, leftRearTirePressure, rightRearTirePressure, temperature;

    switch (rcdData.msgId) {
      case NODE_ID_SPEED_FEEDBACK:
        memcpy(&Speed, &rcdData.command[0], sizeof(int));
        ble->sendDisplayData(1, String(Speed));
        Serial.println("speed: " + String(Speed));
        break;
      case NODE_ID_PRESSURE:
        leftFrontTirePressure = ConvertBuffToFloat(rcdData.command);
        rightFrontTirePressure = leftFrontTirePressure + 0.1;
        leftRearTirePressure = leftFrontTirePressure - 0.1;
        rightRearTirePressure = leftFrontTirePressure;

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
        memcpy(&temperature, &rcdData.command[0], sizeof(float));
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
};
