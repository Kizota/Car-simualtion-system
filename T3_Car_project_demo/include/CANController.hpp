#pragma once
#include <mcp_can.h>
#include <IMessager.hpp>

#define MAX_BUFFER 1
using namespace message;

typedef enum
{
  HIGH_NEAM_OFF,
  HIGH_BEAM_ON,
  INDICATOR_OFF,
  INDICATOR_ON
} Message_t;

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
  unsigned long tx_id;
  uint8_t len;
  uint8_t content;
  uint8_t pre_content;

} Message;

typedef struct
{
  unsigned long comp_id;
  uint8_t command;
} Command_t;

int Timer_create(Timer *timer, unsigned long *now, unsigned long interval);
int Id_mask_create(Id_guard *id_guard, unsigned long mask, unsigned long *filters, uint8_t len);

class CANController : ISender
{
private:
  MCP_CAN *can;
  unsigned long tx_id;
  uint8_t speed;

public:
  CANController(uint8_t speed, unsigned long tx_id);
  int SendMessage(Command_t *command_list, Message_t msg);
  int Handle_sending_random_signal_comand(Command_t *command_list, int cmd_len, Timer *timer);

  int ReadMessage(Message *msg);
  int CAN_check_message(Id_guard *id_guard, Message *msg);
  int CAN_print_message(Message *msg);
};