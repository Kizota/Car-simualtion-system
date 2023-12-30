#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <stdlib.h>
#include <time.h>

#include <myCan.h>

#define spiCSPin 5

// Message id
#define BASED_ID 0x30
#define HIGH_BEAM_ID 1
#define INDICATOR_ID 2

#define NUM_OF_COMMAND 4

// filters and mask
#define NOCF 0
#define NOCM 0

// Led state
#define OFF 0
#define ON 1

// Timer
#define TIME_NOISE_INTERVAL 1000
#define TIME_INTERVAL 5000

typedef enum
{
    HIGH_NEAM_OFF,
    HIGH_BEAM_ON,
    INDICATOR_OFF,
    INDICATOR_ON
} Message_t;

MCP_CAN CAN(spiCSPin);
Message msg = {0, 0, 0};

CAN_module can;
unsigned long rx_masks[NOCM] = {};
unsigned long rx_filters[NOCF] = {};

Timer timer;
Command_t commands[NUM_OF_COMMAND] = {{HIGH_BEAM_ID, OFF}, {HIGH_BEAM_ID, ON}, {INDICATOR_ID, ON}, {INDICATOR_ID, OFF}};
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
    Handle_sending_random_signal_comand(&can, commands, NUM_OF_COMMAND, &timer);
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
