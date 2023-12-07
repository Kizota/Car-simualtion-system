#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "mcp_can.h"
#include <stdlib.h>
#include <time.h>

/*
//NOTE - STEERING Wheels ECU
 - transmitter
  based id: 0x30
*/
#define spiCSPin 5

#define BASED_ID 0x30  
#define HIGH_BEAM_ID 1
#define INDICATOR_ID 2

#define NOCF 0
#define NOCM 0
#define MAX_BUFFER 1

#define OFF 0
#define ON 1

#define TIME_INTERVAL 5000
#define LED_PIN 22


typedef enum
{
    HIGH_NEAM_OFF,
    HIGH_BEAM_ON,
    INDICATOR_OFF,
    INDICATOR_ON
} Message_t;


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

typedef struct
{
    uint8_t comp_id;
    uint8_t command;
} Command_t;

MCP_CAN CAN(spiCSPin);
Message msg = {0, 0, 0};

// CAN_module can=

Command_t commands[] = {{HIGH_BEAM_ID, ON}, {HIGH_BEAM_ID, OFF}, {INDICATOR_ID, ON}, {INDICATOR_ID, OFF}};

//CAN function declare
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, uint8_t tx_id, uint8_t *rx_mask, uint8_t *rx_filter);
void CAN_init(CAN_module *can);

// transmitter functions
int CAN_send_message(CAN_module *can, Command_t *command_list, Message_t msg);
// timer
unsigned long now = 0;
unsigned long pre_time = 0;

const int stackDepth = 2000;
void setup()
{
  Serial.begin(9600);

 // CAN_init();

}

void loop()
{
  now = millis();

//  Handle_sending_random_signal_comand(commands);
}

void CAN_init(uint8_t speed)
{
  while (CAN_OK != CAN.begin(MCP_ANY, speed, MCP_16MHZ))
  {
    Serial.println("CAN BUS init failed");
    delay(100);
  }

  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN Bus init ok!\n");
}
/* ----  Transmitter   ---- */
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

int CAN_send_message(CAN_module *can, Command_t *command_list, Message_t msg)
{
    if (can == nullptr || command_list == nullptr)
    {
        return 0;
    }

    unsigned long id = can->tx_id + command_list->comp_id;

    can->can->sendMsgBuf(id,(uint8_t) MAX_BUFFER, &(command_list + msg)->command);

    return 1;
}

// steering wheels - node
void Handle_sending_random_signal_comand(CAN_module *can, Command_t *command_list)
{
  if (now - pre_time > TIME_INTERVAL)
  {
    srand(time(NULL));
    uint8_t no = rand() % 4;

   CAN_send_message(can,command_list,(Message_t) no);
    pre_time = now;
  }
}


/*DASHBOARD*/
// include LED signal
// receiver & transmitter
/*
   give command for sensor

*/

/*
 //NOTE - Sensor ECUs
//transmitter
    base id: 30
    Pressure
    added id: 1
    content: PressVal

    Temperature
    added id: 2
    content: TempVal

*/

/*ENGINE CONTROL*/
// receiver

/*REMOTE CONTROL*/

/*
   1. Node ECUs
      -  Sensor
      -  Signal
      -  steering wheels +
      -  Engine control

      -  Remote control
      -  Dashboard

   2 Message:
      - base id
      - command

   3. Msg sr to des:
    sensor --> dashboard
    sensor --> remote control

    steering wheels --> Led
    sterring wheels --> engine control

*/
