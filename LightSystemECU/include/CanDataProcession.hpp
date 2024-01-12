#ifndef CANDATAPROCESSION_HPP
#define CANDATAPROCESSION_HPP

#include <mcp_can.h>

#include <cstdint>

#define NO_MESSGAGE 1000

enum MessageID
{VOID_ID = -1,
 
  NODE_ID_INDICATOR,
  NODE_ID_HIGHBEAM ,  // Accepts on or off

  NODE_ID_ACCELERATE,  // Accepts speed in % from 0 to 100
  NODE_ID_DECELERATE,  // Accepts speed in % from 0 to 100

  NODE_ID_SPEED,           // Accepts request
  NODE_ID_SPEED_FEEDBACK,  // Accepts request

  NODE_ID_TEMPERATURE,  // Accepts request
  NODE_ID_PRESSURE,     // Accepts request

  NODE_ID_STEERRIGHT,           // Accepts angle in degrees from 0 to 360
  NODE_ID_RPM,                  // Accepts request
  NODE_ID_DASHBOARD,            // Sends requests and accepts data
  NODE_ID_CRUISECONTROL,        // Accepts on or off
  NODE_ID_CRUISECONTROL_SPEED,  //
};

// Commands for nodes over CAN
enum CANCommand {
  // switch command
  COMMAND_OFF = 0,
  COMMAND_ON = 1,

  // tweaking command
  COMMAND_DOWN = 0,
  COMMAND_UP = 1,

  COMMAND_REQUEST,
};

enum LighSystemCommand {
  LEFT_IND_ON = 0,
  RIGHT_IND_ON = 1,
  BOTH_OFF = 2
};

struct CanData {
  unsigned long msgId;
  byte len;
  byte command[4];

  CanData();
  CanData(unsigned long msgID, byte len);
};

class ICanSender {
 public:
  virtual void SendMessage(uint8_t msgId, float value) = 0;
  virtual void SendMessage(uint8_t msgId, int value) = 0;
};

class ICanListener {
 public:
  virtual bool RecieveMessage(CanData newData) = 0;
};

#endif