#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <stdlib.h>
#include <time.h>

#include <myCan.h>

/*
//NOTE - STEERING Wheels ECU
 - transmitter
  based id: 0x30
*/
#define spiCSPin 5

#define BASED_ID 0x30
#define HIGH_BEAM_ID 1
#define INDICATOR_ID 2

#define NOCF 0
#define NOCM 0

#define OFF 0
#define ON 1

#define TIME_NOISE_INTERVAL 1000
#define TIME_INTERVAL 5000
#define LED_PIN 22

MCP_CAN CAN(spiCSPin);
Message msg = {0, 0, 0};

CAN_module can;
uint8_t rx_masks[NOCM] = {};
uint8_t rx_filters[NOCF] = {};

Timer timer;
Command_t commands[] = {{HIGH_BEAM_ID, ON}, {HIGH_BEAM_ID, OFF}, {INDICATOR_ID, ON}, {INDICATOR_ID, OFF}};
uint8_t noise_msg = 1;

// timer
unsigned long now = 0;
unsigned long pre_time_noise = 0;
const int stackDepth = 2000;
void setup()
{
    Serial.begin(9600);
    Timer_create(&timer, &now, TIME_INTERVAL);
    CAN_module_create(&can, &CAN, CAN_500KBPS, BASED_ID, rx_masks, NOCM, rx_filters, NOCF);
    CAN_setup(&can);
}

void loop()
{
    now = millis();

    // send random commands for sigal ecus
    Handle_sending_random_signal_comand(&can, commands, 4, &timer);

    // simulate sending noise from other ecus
    if (now - pre_time_noise > TIME_NOISE_INTERVAL)
    {
        Serial.println("send noise messge!\n");

        CAN.sendMsgBuf((unsigned long)0x40, (uint8_t)MAX_BUFFER, &noise_msg);
        pre_time_noise = now;
    }
}

/*DASHBOARD*/
// include LED signal
// receiver & transmitter
/*
   give command for sensor

*/

/*
 //NOTE - Sensor ECUs
//transmitter
    base id: 30
    Pressure
    added id: 1
    content: PressVal

    Temperature
    added id: 2
    content: TempVal

*/

/*ENGINE CONTROL*/
// receiver

/*REMOTE CONTROL*/

/*
   1. Node ECUs
      -  Sensor
      -  Signal
      -  steering wheels +
      -  Engine control

      -  Remote control
      -  Dashboard

   2 Message:
      - base id
      - command

   3. Msg sr to des:
    sensor --> dashboard
    sensor --> remote control

    steering wheels --> Led
    sterring wheels --> engine control

*/
