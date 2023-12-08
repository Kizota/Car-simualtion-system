#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "myCan.h"
/*
 //NOTE - Signal ECUs
  - based id: 0x50
  - reciever:
     rx_id      : 0x31
     indicator  : 0x32
*/

#define spiCSPin 5

#define BASED_ID 0x50

#define CAN_MASK 0x3F     // 0x33 -> 0x0011 0011
#define CAN_FILTER_1 0x31 // 0x31 ->0x0011 0001 msg src steering wheel id
#define CAN_FILTER_2 0x32 // 0x032 ->0x0011 0010 msg src remote control id

#define NOCM 1
#define NOCF 2
#define MAX_BUFFER 1

MCP_CAN CAN(spiCSPin);
Message msg = {0, 0, 0};

CAN_module can;
uint8_t rx_masks[NOCM] = {CAN_MASK};
uint8_t rx_filters[NOCF] = {CAN_FILTER_1, CAN_FILTER_2};

void setup()
{
    Serial.begin(9600);
    Serial.println("start setup\n");

    CAN_module_create(&can, &CAN, CAN_500KBPS, BASED_ID, rx_masks, NOCM, rx_filters, NOCF);
    // CAN_setup(&can);

    while (CAN_OK != CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ))
    {
        Serial.println("CAN BUS init failed");
        delay(100);
    }

    CAN.init_Mask(0, 0, 0x3F);
    CAN.init_Filt(0, 0, 0x31);
    CAN.init_Filt(1, 0, 0x32);
}

void loop()
{
    CAN_read_message(&can, &msg);
}
