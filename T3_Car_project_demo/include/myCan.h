#ifndef MYCAN_H
#define MYCAN_H
#include "Arduino.h"
#include <mcp_can.h>

#define MAX_BUFFER 1

typedef struct
{
  unsigned long *now;
  unsigned long pre_time;
  unsigned long interval;
}Timer;

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
    uint8_t speed;

    uint8_t tx_id;
    uint8_t *rx_filter;
    uint8_t *rx_mask;

    uint8_t filters_len;
    uint8_t masks_len;
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

int Timer_create(Timer* timer, unsigned long *now,unsigned long interval);

// CAN function declare
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, uint8_t tx_id, uint8_t *rx_mask, uint8_t nofmasks, uint8_t *rx_filter, uint8_t nofilters);
void CAN_setup(CAN_module *can);

// transmitter functions
int CAN_send_message(CAN_module *can, Command_t *command_list, Message_t msg);
int Handle_sending_random_signal_comand(CAN_module *can, Command_t *command_list, int cmd_len, Timer *timer);

// CAN reciever function
int CAN_read_message(CAN_module *can ,Message *msg);
int CAN_print_message(Message *msg);

#endif