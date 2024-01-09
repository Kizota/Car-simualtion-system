#include "WebSocketManager.hpp"

#define QUEUE_LENGTH 20
#include "Arduino.h"

WebSocketManager::WebSocketManager(void (*webSocketEvent)(uint8_t, WStype_t, uint8_t *, size_t)) : server(80), webSocket(81), isClientConnected(false)
{
    Serial.println("constructor!");
    // web server
    server.begin();
    webSocket.begin();

    // set callback function
    webSocket.onEvent(webSocketEvent);

    // create a queue of string
    msgQueue = xQueueCreate(QUEUE_LENGTH, sizeof(std::pair<ClientType,std::string *>));

    // rtos
    sendingHandler = new RealTime::TaskHandler("WSSendingMessage", WebSocketManager::SendingMessagetask, this);

    Serial.println("haha!");

    sendingHandler->SetMode(RealTime::ON);

    Serial.println("hoho!");
}

void WebSocketManager::SetClientConnected(bool sta)
{
      isClientConnected = sta;
}

void WebSocketManager::ConnectionHandler()
{
    server.handleClient();
    webSocket.loop();
}
