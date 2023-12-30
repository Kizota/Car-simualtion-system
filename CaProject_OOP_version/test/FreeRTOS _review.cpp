#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

const uint32_t stackDepth = 1024;
const uint8_t queueSize = 10;
QueueHandle_t msgQueue;

int num;

void PrintMessage(void *pvParameter);
void SendMessage(void *pvParameter);

void setup()
{
  Serial.begin(9600);

  msgQueue = xQueueCreate(queueSize, sizeof(int));

  xTaskCreate(PrintMessage, "print message", stackDepth, nullptr, 1, nullptr);
  xTaskCreate(SendMessage, "send message", stackDepth, nullptr, 1, nullptr);
}

void loop()
{
}

void PrintMessage(void *pvParameter)
{
  while (1)
  {
    if (xQueueReceive(msgQueue, (void *)&num, 0) == pdTRUE)
    {
     // Serial.println(num);
    }

    Serial.println(num);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void SendMessage(void *pvParameter)
{
  int i = 0;
  while (1)
  {
    if (xQueueSend(msgQueue, (void *)&i, 10) == pdFALSE)
    {
      Serial.println("qeue full!");
    }

    i++;

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}