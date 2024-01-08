#ifndef WEBSOCKETMANAGER_HPP
#define WEBSOCKETMANAGER_HPP

#include <iostream>
#include <utility>

#include <WebServer.h>
#include <WebSocketsServer.h>

#include "Rtos.hpp"
#include "WSDataProccession.hpp"

/*
restrict connection to the first client only

*/



class WebSocketManager : IWSSender 
{
private:
    WebServer server;
    WebSocketsServer webSocket;

    QueueHandle_t msgQueue;
    RealTime::TaskHandler *sendingHandler;
    RealTime::TaskHandler *connectionHandler;

public:
    WebSocketManager(void (*webSocketEvent)(uint8_t, WStype_t, uint8_t *, size_t));
   
    // can be extend to store message and send to many client
    bool SendMessage(ClientType client, std::string message) override
    {
        if (client != DASHBOARD)
        {
            return false;
        }

        // packaged the data
        std::pair<ClientType, char *> data;
        data.first = client;
        strcpy(data.second, message.c_str());

        // push the message to queue
        return xQueueSend(msgQueue, (void *)&data, portMAX_DELAY);
    }

private:
    static void ConnectionTask(void *paramater)
    {
        WebSocketManager *manager = static_cast<WebSocketManager *>(paramater);

        while (1)
        {
            // response
            manager->server.handleClient();
            manager->webSocket.loop();
        }
    }

    static void SendingMessagetask(void *parameter)
    {
        WebSocketManager *manager = static_cast<WebSocketManager *>(parameter);
        std::pair<ClientType, char *> data;
        data.first = DASHBOARD;
        data.second = NULL;

        while (1)
        {
            if (xQueueSend(manager->msgQueue, (void *)&data, portMAX_DELAY) == pdTRUE)
            {
                manager->webSocket.sendTXT((uint8_t)data.first, data.second);
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    static void SetupWifi(const char *ssid, const char *password, const char *hostname)
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
};

#endif