#ifndef CANDATAPROCESSION_HPP
#define CANDATAPROCESSION_HPP

#include <mcp_can.h>
#include <iostream>

#include <cstdint>
enum MessageID
{
    VOID_ID = -1,
    NODE_ID_INDICATOR = 0x30,
    NODE_ID_HIGHBEAM = 0x31, // Accepts on or off
    NODE_ID_LEFTBLINKER,         // Accepts on or off
    NODE_ID_RIGHTBLINKER,        // Accepts on or off
    NODE_ID_LOWBEAM,             // Accepts on or off
    NODE_ID_TAILLIGHT,           // Accepts on or off
    NODE_ID_BRAKELIGHT,          // Accepts on or off
    NODE_ID_REVERSELIGHT,        // Accepts on or off
    NODE_ID_MOTOR = 0x38,          // Accepts command type ServoSystemCommand
    NODE_ID_SPEED_FEEDBACK = 0x40, // Sends speed in km/h
    NODE_ID_STEERLEFT,           // Accepts angle in degrees from 0 to 360
    NODE_ID_STEERRIGHT,          // Accepts angle in degrees from 0 to 360
    NODE_ID_SPEED,               // Accepts request
    NODE_ID_RPM,                 // Accepts request
    NODE_ID_TEMPERATURE,         // Accepts request
    NODE_ID_PRESSURE,            // Accepts request
    NODE_ID_DASHBOARD,           // Sends requests and accepts data
    NODE_ID_CRUISECONTROL,       // Accepts on or off
    NODE_ID_CRUISECONTROL_SPEED, // Accepts speed in % from 0 to 100
};

// Commands for nodes over CAN
enum CANCommand
{
    // switch command
    COMMAND_OFF = 0,
    COMMAND_ON = 1,

    // tweaking command
    COMMAND_DOWN = 0,
    COMMAND_UP = 1,

    COMMAND_REQUEST,
};

enum LighSystemCommand
{
    LEFT_IND_ON = 0,
    RIGHT_IND_ON = 1,
    BOTH_OFF = 2
};

enum ServoSystemCommand
{
    SPEED_UP = 0,
    SPEED_DOWN = 1,
    STEER_LEFT = 2,
    STEER_RIGHT = 3,
    SPEED_STOP = 4
};

struct CanData
{

    unsigned long msgId;
    byte len;
    byte command[4];

    CanData();
    CanData(unsigned long msgID, byte len);
};

class ICanSender
{
public:
    virtual void SendMessage(uint8_t msgId, float value) = 0;
    virtual void SendMessage(uint8_t msgId, int value) = 0;
};

class ICanListener
{
public:
    virtual bool RecieveMessage(CanData newData) = 0;
};

#endif