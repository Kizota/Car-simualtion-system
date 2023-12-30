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

#define CAN_MASK 0x003F0000     // 0x33 -> 0x0011 0011
#define CAN_FILTER_1 0x00310000 // 0x31 ->0x0011 0001 msg src steering wheel id
#define CAN_FILTER_2 0x00320000 // 0x032 ->0x0011 0010 msg src remote control id

#define NoOfFilters 2
#define ID_MASK 0x3F     // 0x33 -> 0x0011 0011
#define ID_FILTER_1 0x31 // 0x33 -> 0x0011 0011
#define ID_FILTER_2 0x32 // 0x33 -> 0x0011 0011

#define NOCM 1
#define NOCF 2
#define MAX_BUFFER 1

MCP_CAN CAN(spiCSPin);
Message msg = {0, 0, 0, 0};

CAN_module can;
Id_guard id_guard;

unsigned long rx_masks[NOCM] = {CAN_MASK};
unsigned long rx_filters[NOCF] = {CAN_FILTER_1, CAN_FILTER_2};
unsigned long filters[NoOfFilters] = {ID_FILTER_1, ID_FILTER_2};

void setup()
{
    Serial.begin(9600);
    Serial.println("start setup\n");

    CAN_module_create(&can, &CAN, CAN_500KBPS, BASED_ID, rx_masks, NOCM, rx_filters, NOCF);
    CAN_setup(&can);

    Id_mask_create(&id_guard, ID_MASK, filters, NoOfFilters);
}

void loop()
{
    CAN_read_message(&can, &msg);

    // only print concerned messages
    if (CAN_check_message(&id_guard, &msg))
    {

        CAN_print_message(&msg);
    }
}
