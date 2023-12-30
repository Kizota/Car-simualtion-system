#pragma once
#include <mcp_can.h>

enum MessageID {
  NODE_ID_LEFTBLINKER,          // Accepts on or off
  NODE_ID_RIGHTBLINKER,         // Accepts on or off
  NODE_ID_HIGHBEAM,             // Accepts on or off
  NODE_ID_LOWBEAM,              // Accepts on or off
  NODE_ID_TAILLIGHT,            // Accepts on or off
  NODE_ID_BRAKELIGHT,           // Accepts on or off
  NODE_ID_REVERSELIGHT,         // Accepts on or off
  NODE_ID_ACCELERATE,           // Accepts speed in % from 0 to 100
  NODE_ID_DECELERATE,           // Accepts speed in % from 0 to 100
  NODE_ID_STEERLEFT,            // Accepts angle in degrees from 0 to 360
  NODE_ID_STEERRIGHT,           // Accepts angle in degrees from 0 to 360
  NODE_ID_SPEED,                // Accepts request
  NODE_ID_RPM,                  // Accepts request
  NODE_ID_TEMPERATURE,          // Accepts request
  NODE_ID_PRESSURE,             // Accepts request
  NODE_ID_DASHBOARD,            // Sends requests and accepts data
  NODE_ID_CRUISECONTROL,        // Accepts on or off
  NODE_ID_CRUISECONTROL_SPEED,  // Accepts speed in % from 0 to 100
};

// Commands for nodes over CAN
enum CANCommand {
  COMMAND_OFF,
  COMMAND_ON,
  COMMAND_REQUEST,
};

typedef struct
{
  unsigned long *now;
  unsigned long pre_time;
  unsigned long interval;
} Timer;

struct CanData {
  INT32U MessageID;
  INT8U len;
  INT8U command[8];
};

class CANController {
 private:
  MCP_CAN CAN;
  uint8_t CAN_INT;

 public:
  CANController(uint8_t CAN_INT, uint8_t CAN_CS);
  bool SendFloatCanMessage(uint8_t device_id, float value);
  bool SendIntCanMessage(uint8_t device_id, int value);
  CanData ReadCanMessage();

  // int CAN_print_message(CanData *msg);
};