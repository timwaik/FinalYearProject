#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

const char *ssid = "Mitkays test"; // The name of the Wi-Fi network that will be created
const char *password = "popolinos95";   // The password required to connect to it, leave blank for an open network
const IPAddress remote_ip(192, 168, 4, 2);

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
      Serial.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      webSocket.sendTXT("Connected. Hello Server 1! Do you have any information for me?");
    }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      webSocket.sendTXT("Thank you client 1. Do you have any other information for me?");
      delay(100);
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to server");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

    Serial.print("Connected to server. IP address:  ");
    Serial.println(WiFi.localIP());


  
  webSocket.begin("192.168.4.1", 81, "ESP8266 Server");    //connecting to server with WebSocket
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}
