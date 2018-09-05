/*This version incorporates the usage of sending multiple "topics" to each corresponding client*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

#define BUILTIN_LED 2

const char *ssid = "Mitkays test"; // The name of the Wi-Fi network that will be created
const char *password = "popolinos95";   // The password required to connect to it, leave blank for an open network

String incomingByte; 

WebSocketsServer webSocket = WebSocketsServer(81);    // create a websocket server on port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        /*webSocket.sendTXT(num, "0");*/
        webSocket.sendTXT(num, "1");
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("Message from client:");
      Serial.printf("[%u] Data received: %s\n", num, payload);
      for (int i = 0; i < lenght; i++) {
        Serial.print((char)payload[i]);
        }
        if ((char)payload[0] == '1'){

          //Turns on LED
          digitalWrite(BUILTIN_LED, LOW);
          
          //This is the initial message from the server
          webSocket.sendTXT(num, "2");
          delay(1);
          }
        
        else if ((char)payload[0] == '2'){

          //Turns off LED
          digitalWrite(BUILTIN_LED, HIGH);

          
          webSocket.sendTXT(num, "1");
          delay(1);
          }
  }
  delay(50);
}

void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // for incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  delay(10);
  Serial.println('\n');
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Print IP address of the ESP8266 to the computer
  
  startWebSocket();            // Start a WebSocket server
}

void loop() { 
  webSocket.loop();
}
