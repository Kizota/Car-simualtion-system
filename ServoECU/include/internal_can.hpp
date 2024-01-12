#pragma once
#include "SPI.h"
#include "mcp_can.h"

#define CAN0_INT 2 // Set INT to pin 2
#define CAN0_CS 4  // Set CS to pin 4
// CAN Ids for nodes
enum DeviceId
{
    NODE_ID_LEFTBLINKER = 0x101,    // Accepts on or off
    NODE_ID_RIGHTBLINKER = 0x102,   // Accepts on or off
    NODE_ID_HIGHBEAM = 0x103,   // Accepts on or off
    NODE_ID_LOWBEAM = 0x104, // Accepts on or off
    NODE_ID_TAILLIGHT = 0x105,  // Accepts on or off
    NODE_ID_BRAKELIGHT = 0x106, // Accepts on or off
    NODE_ID_REVERSELIGHT = 0x106,   // Accepts on or off
    NODE_ID_ACCELERATE = 0x107, // Accepts speed in % from 0 to 100
    NODE_ID_DECELERATE = 0x108, // Accepts speed in % from 0 to 100
    NODE_ID_STEERLEFT = 0x109,  // Accepts angle in degrees from 0 to 360
    NODE_ID_STEERRIGHT = 0x10A, // Accepts angle in degrees from 0 to 360
    NODE_ID_SPEED = 0x10B,  // Accepts request
    NODE_ID_RPM = 0x10C,    // Accepts request
    NODE_ID_PRESSURE = 0x10D, // Accepts request
    NODE_ID_DASHBOARD = 0x10E // Sends requests and accepts data
};
// Commands for nodes over CAN
enum CANCommand
{
    COMMAND_OFF = 0x00,
    COMMAND_ON = 0x01,
    COMMAND_REQUEST = 0x02,

};
struct CanData {
    long unsigned int rxId;
    uint8_t len;
    uint8_t rxBuf[8];
};

class InternalCan
{
    public:
        InternalCan() : CAN0(CAN0_CS) // Initialize CAN0 in the constructor
        {
            
        };

        bool setup_can();
        bool send_float_can_message(uint8_t device_id, float value);
        bool send_int_can_message(uint8_t device_id, int value);
        CanData read_can_message_loop();

    private:
        MCP_CAN CAN0;
};