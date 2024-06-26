#include "CANController.h"

// CANController class definition
CANController::CANController(uint8_t speed, unsigned long tx_id) : speed(speed), tx_id(tx_id)
{
  // connect to CAN network
  while (CAN_OK != can->begin(MCP_STDEXT, speed, MCP_16MHZ))
  {
    Serial.println("CAN BUS init failed");
    delay(100);
  }

  // set CAN mode
  can->setMode(MCP_NORMAL);
  Serial.println("CAN Bus init ok!\n");
}

int CANController::CAN_send_message(Command_t *command_list, Message_t msg) {
  if (can == nullptr || command_list == nullptr) {
    return 0;
  }

  unsigned long id = tx_id + (command_list + msg)->comp_id;

  can->sendMsgBuf(id, (uint8_t)MAX_BUFFER, &(command_list + msg)->command);

  return 1;
}

// steering wheels - node
int CANController::Handle_sending_random_signal_comand(Command_t *command_list, int cmd_len, Timer *timer) {
  if (can == nullptr || command_list == nullptr || timer == nullptr) {
    return 0;
  }

  if (*timer->now - timer->pre_time > timer->interval) {
    srand(time(NULL));
    uint8_t no = rand() % cmd_len;
    Serial.print("send messge: ");
    Serial.println(no);
    CAN_send_message(command_list, (Message_t)no);
    timer->pre_time = *timer->now;
  }

  return 1;
}

// message filter

//  Read the incoming message from the CAN network
int CANController::CAN_read_message(Message *msg) {
  if (msg == nullptr || can == nullptr) {
    return 0;
  }

  if (CAN_MSGAVAIL == can->checkReceive())
  {
    can->readMsgBuf(&msg->tx_id, &msg->len, &msg->content);
  }

  return 1;
}

int CANController::CAN_check_message(Id_guard *id_guard, Message *msg)
{
  if (id_guard == nullptr || msg == nullptr)
  {
    return 0;
  }

  int is_valid = 0;

  unsigned long concerned_bits = msg->tx_id & id_guard->mask;

  // check concern bit with filter
  for (int i = 0; i < id_guard->len && !is_valid; i++)
  {
    if (*(id_guard->filter + i) == concerned_bits)
    {
      is_valid = 1;
    }
  }

  return is_valid;
}

// print the CAN message
int CANController::CAN_print_message(Message *msg)
{
  if (msg == nullptr)
  {
    return 0;
  }

  if (msg->content == msg->pre_content)
  {
    return 0;
  }
  Serial.print("id: ");
  Serial.println(msg->tx_id, HEX);
  Serial.print("len: ");
  Serial.println(msg->len);
  Serial.print("cotent: ");
  Serial.println(msg->content);

  msg->pre_content = msg->content;

  return 1;
}