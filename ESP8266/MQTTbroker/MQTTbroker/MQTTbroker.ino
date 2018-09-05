/*
 * uMQTTBroker demo for Arduino
 * 
 * The program starts a broker, subscribes to anything and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */

#include <ESP8266WiFi.h>

#include "uMQTTBroker.h"
#define BUILTIN_LED 2


const char *ssid = "Mitkays test"; // The name of the Wi-Fi network that will be created
const char *password = "popolinos95";   // The password required to connect to it, leave blank for an open network


unsigned int mqttPort = 1883;       // the standard MQTT broker port
unsigned int max_subscriptions = 30;
unsigned int max_retained_topics = 30;

void data_callback(uint32_t *client /* we can ignore this */, const char* topic, uint32_t topic_len, const char *data, uint32_t lengh) {
  char topic_str[topic_len+1];
  os_memcpy(topic_str, topic, topic_len);
  topic_str[topic_len] = '\0';

  char data_str[lengh+1];
  os_memcpy(data_str, data, lengh);
  data_str[lengh] = '\0';

  Serial.print(topic_str);
  Serial.print(": ");
  Serial.println(data_str);
  if ((char)data[0] == '2')
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
    /*digitalWrite(LEDRED, LOW);*/
  if ((char)data[0] == '3')
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
}
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer

/*
 * Register the callback
 */
  MQTT_server_onData(data_callback);

/*
 * Start the broker
 */
  Serial.println("Starting MQTT broker");
  MQTT_server_start(mqttPort, max_subscriptions, max_retained_topics);

/*
 * Subscribe to anything
 */
  MQTT_local_subscribe((unsigned char *)"topic2", 0);
  MQTT_local_subscribe((unsigned char *)"laptop1", 0);
  MQTT_local_subscribe((unsigned char *)"inTopic", 0);
  MQTT_local_subscribe((unsigned char *)"inTopic2", 0);
}

int counter0 = 0;
int counter1 = 1;
  String myData0(counter0);
  String myData1(counter1);
int memory = 5;
void loop()
{
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  /*String myData0(counter0);
  String myData1(counter1);
  String woah(memory--);*/
  MQTT_local_publish((unsigned char *)"inTopic", (unsigned char *)myData0.c_str(), myData0.length(), 0, 0);
  delay(250);
  MQTT_local_publish((unsigned char *)"inTopic", (unsigned char *)myData1.c_str(), myData1.length(), 0, 0);
  delay(250);

/*
 * Publish the counter value as String
 */
 /* MQTT_local_publish((unsigned char *)"topic1", (unsigned char *)myData.c_str(), myData.length(), 0, 0);
  Serial.println(); */
/*  MQTT_local_publish((unsigned char *)"topic5", (unsigned char *)woah.c_str(), woah.length(), 0, 0);
  Serial.println();*/
  
  // wait a second

}
