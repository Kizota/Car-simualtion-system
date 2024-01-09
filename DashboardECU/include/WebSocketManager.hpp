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

    bool isClientConnected;

public:
    WebSocketManager(void (*webSocketEvent)(uint8_t, WStype_t, uint8_t *, size_t));

    void SetClientConnected(bool sta);

    // can be extend to store message and send to many client
    bool SendMessage(ClientType client, std::string message) override
    {
        if (client != DASHBOARD)
        {
            return false;
        }

        // packaged the data
        std::pair<ClientType, std::string *> data;
        data.first = client;
        data.second = new std::string(message);
        Serial.println("store info -------: ");
        Serial.println(message.c_str());
        Serial.println(data.second->c_str());

        Serial.println(":   -------: ");

        bool sta = xQueueSend(msgQueue, (void *)&data, portMAX_DELAY);

        // push the message to queue
        return sta;
    }

    void ConnectionHandler();

private:
    static void SendingMessagetask(void *parameter)
    {
        WebSocketManager *manager = static_cast<WebSocketManager *>(parameter);
        std::pair<ClientType, std::string *> data;
        data.first = TEST;
        data.second = NULL; // ????

        Serial.println("in send message!");

        while (1)
        {
            // if (manager->isClientConnected)
            // {
            if (xQueueReceive(manager->msgQueue, (void *)&data, portMAX_DELAY))
            {
                Serial.println("send message!");
                Serial.println(data.first);
                Serial.println(data.second->c_str());
                manager->webSocket.broadcastTXT(data.second->c_str());

                delete data.second;
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