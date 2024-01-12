#include <Arduino.h>

#include "Config.hpp"

#include "DataManager.hpp"
#include "DashBoard.hpp"
#include "CANController.h"
#include "WebSocketManager.hpp"

//--------------------------//

#define INT_PIN 0
#define CS_PIN 5

#define SIZE_MESS 50

Dashboard *dashboard;
DataManager *dataManager;
CANController *canController;
WebSocketManager *wsManager;

void SetupWifi();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

void setup()
{
  Serial.begin(9600);

  // terminal dashboard
  //  dashboard = new Dashboard();
  //  dataManager = new DataManager((IDataTracker *)dashboard);
  SetupWifi();

  // intialize managers/controller
  dataManager = new DataManager();
  canController = new CANController(INT_PIN, CS_PIN, (ICanListener *)dataManager);
  wsManager = new WebSocketManager(webSocketEvent);

  // add relationship betweeen managers and controller
  dataManager->AddSender((IWSSender *)wsManager);

  // //add id mask
  canController->AddIdMask(NODE_ID_SPEED_FEEDBACK);
  canController->AddIdMask(NODE_ID_SPEED);

  canController->AddIdMask(NODE_ID_TEMPERATURE);
  canController->AddIdMask(NODE_ID_PRESSURE);
  // canController->AddIdMask(NODE_ID_INDICATOR);

  // // wifi connection
  // WiFi.begin(ssid, password);

  // // waiting for connect
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(1000);
  //   Serial.println("Waiting for connecting...\n");
  // }

  // Serial.println("connect to " + String(ssid));

  // //websocket connection
  // webSocketsClient.begin(serverAddress,81);
  // webSocketsClient.onEvent(WebSocketEvent);  //callback function
}

unsigned long preTime = 0;

void loop()
{
  // if (millis() - preTime > 1000)
  // {
  //   wsManager->SendMessage(DASHBOARD, "10");
  //   preTime = millis();
  // }
  wsManager->ConnectionHandler();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  Serial.print("web socket event: ");
  Serial.println(type);

  char mess[SIZE_MESS];

  if (type == WStype_TEXT)
  {
    // read message
    for (int i = 0; i < length; i++)
    {
      mess[i] = (char)payload[i];
    }
    mess[length] = '\0';
  }
  Serial.print("node ");
  Serial.print(num);
  Serial.print("said :");
  Serial.println(mess);

  if (num == 0)
  {
    Serial.println("client connected!");
    wsManager->SetClientConnected(true);
  }
  else
  {
    Serial.println("more than 1 client client connected!");

    wsManager->SetClientConnected(false);
  }
}

void SetupWifi()
{
  // connect to wifi
  WiFi.begin(ssid, password);

  // guard the wifi connection
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf("waiting for connecting...\n");
    delay(1000);
  }

  WiFi.setHostname(hostname);
  // print ssid and local ip information
  Serial.printf("server connect to ssid: %s\n", String(ssid));
  Serial.print("local ip address: ");
  Serial.println(WiFi.localIP());
}