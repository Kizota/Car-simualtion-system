#ifndef CANMESSAGER_HPP
#define CAMMESSAGER_HPP

#include <mcp_can.h>

#define MAX_BUFFER 1

/*
TODO: 
1. fix Can id guard
2. fix msg sending mechanis

*/

typedef enum
{
  HIGH_NEAM_OFF,
  HIGH_BEAM_ON,
  INDICATOR_OFF,
  INDICATOR_ON
} Message_t;



typedef struct
{
  unsigned long mask;
  unsigned long *filter;
  uint8_t len;
} Id_guard;

int Id_mask_create(Id_guard *id_guard, unsigned long mask, unsigned long *filters, uint8_t len);


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


class CanMessager
{
private:
  MCP_CAN *can;
  unsigned long tx_id;
  uint8_t speed;

public:
  CanMessager(uint8_t speed, unsigned long tx_id);
  int SendMessage(Command_t *command_list, Message_t msg);

  int ReadMessage(Message *msg);
  int CAN_check_message(Id_guard *id_guard, Message *msg);
  int CAN_print_message(Message *msg);
};

#endif