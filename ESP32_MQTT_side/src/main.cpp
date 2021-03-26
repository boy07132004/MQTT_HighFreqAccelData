#include <WiFi.h>
#include <sstream>
#include <string.h>
#include <Arduino.h>
#include <AsyncMqttClient.h>

#define MQTT_HOST IPAddress(192, 168, 0, 250)
#define SSID "asus"
#define PASSWORD "0989212698"
#define MQTT_PORT 1883
// #define MQTTUSER "user"
// #define MQTTPSWD "password"

#define THRESHOLD 30

HardwareSerial sensorSerial(1);
AsyncMqttClient mqttClient;
TaskHandle_t task1;
TaskHandle_t task2;


void publishTask(void* pvParameters);
void subscribeTask(void* pvParameters);
void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void publish();


void setup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID,PASSWORD);
  Serial.begin(115200);
  Serial.println("Connect to WiFi....");
  while (!WiFi.isConnected()) delay(100);
  Serial.println(WiFi.localIP());
  sensorSerial.begin(250000,SERIAL_8N1,16,17);
  delay(10);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setKeepAlive(3);
  // mqttClient.setCredentials(MQTTUSER,MQTTPSWD);
  mqttClient.connect();
  Serial.println(WiFi.macAddress());
  delay(1000);
  mqttClient.subscribe("ctValue", 0);
  mqttClient.onMessage(onMqttMessage);

  xTaskCreatePinnedToCore(
    publishTask,
    "publishNode",
    40000,
    NULL,
    -1,
    &task1,
    tskNO_AFFINITY);
    
  xTaskCreatePinnedToCore(
    subscribeTask,
    "subscribeNode",
    40000,
    NULL,
    1,
    &task2,
    tskNO_AFFINITY);
}

bool POWERON = false;
bool newData = false;
String Message;

void publishTask(void* pvParameters){
  while(true){
    if(sensorSerial.available()){
      Message += sensorSerial.readStringUntil('S');
      newData = true;
    }
    if (newData){
      Message[Message.length()-1] = '\0';
      if (!mqttClient.connected()) ESP.restart();
      if (POWERON)
        mqttClient.publish("vibration",0,true,(char*)Message.c_str());
      newData = false;
      Message.clear();
    }

    delay(1);
  }
}


void subscribeTask (void* pvParameters){
  vTaskDelete(NULL);
}

void loop(){
}


void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  static int ctValue;
  std::stringstream msg(payload);
  if (msg >> ctValue);
  else Serial.println("Error msg");
  if (ctValue > THRESHOLD) POWERON = true;
  else POWERON = false;
}
