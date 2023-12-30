#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <stdlib.h>
#include <time.h>

#include <myCan.h>

/*
//NOTE - Sensor ECU
 - transmitter
  based id: 0x40
*/
#define spiCSPin 5

//message id 
#define BASED_ID 0x40
#define TEMP_ID 1
#define PRESSURER_ID 2

#define NUM_OF_COMMAND 2

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
Command_t commands[NUM_OF_COMMAND] = {{TEMP_ID, 0},{PRESSURER_ID, 0}};
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
    
    //simulate sensor value sending from  
    Handle_sending_sensor_data(&can, commands, NUM_OF_COMMAND, &timer);
}





