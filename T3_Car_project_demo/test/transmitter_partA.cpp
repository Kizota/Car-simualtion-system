#include <Arduino.h>
#include <SPI.h>
#include "mcp_can.h"

#define RECIEVER_ID 0x43
#define OFF 0
#define ON 1

#define spiCSpin  5


unsigned char led_command[] = {0, 1};

MCP_CAN CAN(spiCSpin);

String ReadMessage();
void handle_led_command();
uint8_t CAN_send_led_command(MCP_CAN *Can, uint8_t com_id);
void testing_led();


void setup()
{
  Serial.begin(9600);

  // wait until can is connected
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ))
  {
    Serial.println("CAN BUS intit Failed");
    delay(100);
  }

  CAN.setMode(MCP_NORMAL); // Change to normal mode to allow messages to be transmitted

  Serial.println("Transmitter CAN BUS init succesfully");
}

void loop()
{
  handle_led_command();

  testing_led();

}

String ReadMessage()
{
  String message = "";

  if (Serial.available() > 0)
  {
    message = Serial.readStringUntil('\r');
    Serial.print(message);
  }

  return message;
}

// -- how about a my_CAN library dividing message and CAN
void handle_led_command()
{
  String msg = ReadMessage();

  if (msg == "ON")
  {
    CAN_send_led_command(&CAN, ON);
    Serial.println("send on!\n");
  }
  else if (msg == "OFF")
  {
    CAN_send_led_command(&CAN, OFF);
     Serial.println("send off!\n");
  }
}

uint8_t CAN_send_led_command(MCP_CAN *Can, uint8_t com_id)
{
  if (Can == NULL)
  {
    return 0;
  }

  Can->sendMsgBuf(RECIEVER_ID, 1, &led_command[com_id]);
  Serial.print("led_command: "); Serial.println(led_command[com_id]);
  return 1;
}

void testing_led()
{
      CAN_send_led_command(&CAN, ON);
          Serial.println("send on!\n");

    delay(500);
    CAN_send_led_command(&CAN, OFF);
        Serial.println("send off!\n");

    delay(500);
}
