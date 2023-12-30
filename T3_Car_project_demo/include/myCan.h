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
} Timer;

typedef struct
{
    unsigned long mask;
    unsigned long *filter;
    uint8_t len;
} Id_guard;

typedef struct
{
    MCP_CAN *can;
    uint8_t speed;

    unsigned long tx_id;
    unsigned long *rx_filter;
    unsigned long *rx_mask;

    uint8_t filters_len;
    uint8_t masks_len;
} CAN_module;

typedef struct
{
    unsigned long tx_id;
    uint8_t len;
    uint8_t pre_content;
    uint8_t content;
} Message;

typedef struct
{
    unsigned long comp_id;
    uint8_t command;
} Command_t;

int Timer_create(Timer *timer, unsigned long *now, unsigned long interval);
int Id_mask_create(Id_guard *id_guard, unsigned long mask, unsigned long *filters, uint8_t len);
// CAN function declare
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, unsigned long tx_id, unsigned long *rx_mask, uint8_t nofmasks, unsigned long *rx_filter, uint8_t nofilters);
void CAN_setup(CAN_module *can);

// transmitter functions
int CAN_send_message(CAN_module *can, Command_t *command_list, int no);
int Handle_sending_random_signal_comand(CAN_module *can, Command_t *command_list, int cmd_len, Timer *timer);
int Handle_sending_sensor_data(CAN_module *can, Command_t *command_list, int cmd_len, Timer *timer);

// CAN reciever function
int CAN_read_message(CAN_module *can, Message *msg);
int CAN_check_message(Id_guard *id_guard, Message *msg);
int CAN_print_message(Message *msg);



#endif