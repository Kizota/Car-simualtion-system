#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "mcp_can.h"

/*
 //NOTE - Signal ECUs
  - based id: 0x50
  - reciever:
     rx_id      : 0x31
     indicator  : 0x32
*/

#define spiCSPin 5

#define CAN_MASK 0x10
#define CAN_FILTER_1 0x10 // msg src steering wheel id
#define CAN_FILTER_2 0x20 // msg src remote control id

#define NOCF 2
#define NOCM 1
#define MAX_BUFFER 1


// struct declaration
typedef struct
{
    MCP_CAN *can;
    unsigned long speed;

    uint8_t tx_id;
    uint8_t *rx_filter;
    uint8_t *rx_mask;
} CAN_module;

typedef struct
{
    unsigned long tx_id;
    uint8_t len;
    uint8_t content;
} Message;



MCP_CAN CAN(spiCSPin);
Message msg = {0, 0, 0};

// CAN functions declare
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, uint8_t tx_id, uint8_t *rx_mask, uint8_t *rx_filter);
void CAN_init(CAN_module *can);

//CAN reciever function
int CAN_read_message(Message *msg);
int CAN_print_message(Message *msg);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
}

// CAN function definition
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, uint8_t tx_id, uint8_t *rx_mask, uint8_t *rx_filter)
{
    if (can == nullptr || CAN == nullptr || rx_mask == nullptr || rx_filter)
    {
        return 0;
    }

    can->can = CAN;
    can->speed = speed;

    can->tx_id = tx_id;
    can->rx_mask = rx_mask;
    can->rx_filter = rx_filter;

    return 1;
}

void CAN_init(CAN_module *can)
{

    // connect to CAN network
    while (CAN_OK != CAN.begin(MCP_ANY, can->speed, MCP_16MHZ))
    {
        Serial.println("CAN BUS init failed");
        delay(100);
    }

    // init CAN mask
    for (int no = 0; no < NOCM; no++)
    {
        CAN.init_Mask(no, *(can->rx_mask + no));
    }

    // init CAN mask
    for (int no = 0; no < NOCF; no++)
    {
        CAN.init_Filt(0, *(can->rx_filter + no));
    }

    // set CAN mode
    CAN.setMode(MCP_NORMAL);

    Serial.println("CAN Bus init ok!\n");
}

//  Read the incoming message from the CAN network
int CAN_read_message(Message *msg)
{
    if (msg == nullptr)
    {
        return 0;
    }

    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&msg->tx_id, &msg->len, &msg->content);
        CAN_print_message(msg);
    }

    return 1;
}

//print the CAN message
int CAN_print_message(Message *msg)
{
    if (msg == nullptr)
    {
        return 0;
    }

    Serial.print("id: ");
    Serial.println(msg->tx_id);
    Serial.print("len: ");
    Serial.println(msg->len);
    Serial.print("cotent: ");
    Serial.println(msg->content);

    return 1;
}