#include "internal_can.hpp"

//InternalCan::InternalCan() : CAN0(4) {}

bool InternalCan::setup_can()
{
    if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    {
        CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.
        pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input
        return true;
    }
    else
    {
        return false;
    }
}
//REVIEW - Float and int can? be mixed as they both use 4 bytes
bool InternalCan::send_float_can_message(uint8_t device_id, float value)
{
    // Create a buffer for the message
    byte buf[4];

    // Copy the float into the buffer
    memcpy(buf, &value, sizeof(float));

    // Send the message
    return (CAN0.sendMsgBuf(device_id, 1, sizeof(float), buf) == CAN_OK);
}
bool InternalCan::send_int_can_message(uint8_t device_id, int value)
{
    // Create a buffer for the message
    byte buf[4];

    // Copy the float into the buffer
    memcpy(buf, &value, sizeof(int));

    // Send the message
    return (CAN0.sendMsgBuf(device_id, 1, sizeof(int), buf) == CAN_OK);
}
//TODO - How to handle the received data from inside the library? Think of a solution
CanData InternalCan::read_can_message_loop()
{
    CanData data;
    if(!digitalRead(CAN0_INT))   {

    CAN0.readMsgBuf(&data.rxId, &data.len, data.rxBuf);

    // Extract device ID from CAN ID
    //byte deviceId = (rxId & 0xFF);
    // Process data based on device ID
    return data;
  }
}

