#include <Arduino.h>
#include <SPI.h>
#include "mcp_can.h"

#define ID 0x43
#define MAX_BUFFER 1

#define LED_ON 1
#define LED_OFF 0

#define LED_PIN 22
#define spiCSPin 5

// declare obj and varaible
MCP_CAN CAN(spiCSPin);
 
unsigned char len = 0;
unsigned char content = 2;
unsigned long txId = 0;


// declare functions
uint8_t check_valid_msg(unsigned long txId, unsigned char len);
void CAN_read_msg();
void CAN_handle_led_command(unsigned char command);

void setup()
{
    Serial.begin(9600);

    // CAN setup
    while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ))
    {
        Serial.println("CAN BUS init failed");
        delay(100);
    }

    CAN.setMode(MCP_NORMAL); // can set mode

    Serial.println("Reciever CAN BUS init OK!");

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}

void loop()
{
    CAN_read_msg();
}

uint8_t check_valid_msg(unsigned long txId, unsigned char len)
{
    return txId == ID && len == MAX_BUFFER;
}

void CAN_read_msg()
{
    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&txId, &len, &content);

        if (check_valid_msg(txId, len))
        {
            CAN_handle_led_command(content);
            Serial.print("txId: ");
            Serial.println(txId, HEX);
            Serial.print("len: ");
            Serial.println(len);
            Serial.print("content: ");
            Serial.println(content);
        }
    }
}

unsigned char pre_command = 2;
void CAN_handle_led_command(unsigned char command)
{
    switch (command)
    {
    case LED_OFF:
        if (pre_command != command)
        {
            digitalWrite(LED_PIN, LOW);
            Serial.println("recieved off!\n");
            pre_command = command;
        }
        break;
    case LED_ON:
        if (pre_command != command)
        {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("recieved on!\n");
            pre_command = command;
        }
        break;
    }
}
