#include "WebSocketManager.hpp"

#define QUEUE_LENGTH 20


WebSocketManager::WebSocketManager(void (*webSocketEvent)(uint8_t, WStype_t, uint8_t *, size_t)) : server(80), webSocket(81)
{
    // web server
    server.begin();
    webSocket.begin();

    // set callback function
    webSocket.onEvent(webSocketEvent);

    // create a queue of string
    msgQueue = xQueueCreate(QUEUE_LENGTH, sizeof(std::pair<ClientType, char *>));

    // rtos
    sendingHandler = new RealTime::TaskHandler("WSSendingMessage", WebSocketManager::SendingMessagetask, this);
    connectionHandler = new RealTime::TaskHandler("WSConnection", WebSocketManager::ConnectionTask, this);

    sendingHandler->SetMode(RealTime::ON);
    connectionHandler->SetMode(RealTime::ON);
}