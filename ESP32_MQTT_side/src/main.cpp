#include <Arduino.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>

#define MQTT_HOST IPAddress(192, 168, 0, 100)
#define MQTT_PORT 1883
#define SSID "RobotAccelerometer01"
#define PASSWORD "Robot1234"
AsyncMqttClient mqttClient;

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID,PASSWORD);
  
  Serial2.begin(500000);
  while (!WiFi.isConnected()){
    delay(100);
  }
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
    mqttClient.publish("rand", 2, true,(char*)msg.c_str());
    flag = false;
    msg = "";
  }
}