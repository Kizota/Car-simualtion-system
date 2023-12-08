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
unsigned long rx_masks[NOCM] = {};
unsigned long rx_filters[NOCF] = {};

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
}





