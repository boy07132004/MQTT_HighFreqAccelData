#include <Arduino.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>

#define MQTT_HOST IPAddress(192, 168, 0, 100)
#define MQTT_PORT 1883
#define SSID "SSID"
#define PASSWORD "PASSWORD"
AsyncMqttClient mqttClient;

void setup(){
  delay(10);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID,PASSWORD);
  Serial2.begin(500000);
  while (!WiFi.isConnected()){
    delay(100);
  }
  delay(10);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.connect();
  
}
#include <string.h>
bool flag = false;
String msg ="";

void loop(){
  if(Serial2.available()){
    msg += Serial2.readStringUntil('S');
    flag = true;
  }
  if (flag){
    msg[msg.length()-1]='\0';
    if (!mqttClient.connected()) ESP.restart();
    mqttClient.publish("rand", 2, true,(char*)msg.c_str());
    flag = false;
    msg = "";
  }
}