#include <Arduino.h>
#include <string.h>
#include <SPI.h>
#define MOSI_PIN 23
#define SCK_PIN  18
#define SS_PIN   5  //cs
#define MISO_PIN 19
#define DATA_PER_PKG 25
#define READING_MAX 65536
#define SCALE_G 16384.0

void Task1code(void* pvParameters);
void Task2code(void* pvParameters);

TaskHandle_t task1;
TaskHandle_t task2;

void setup() {
  Serial2.begin(500000);
  pinMode(SS_PIN, OUTPUT);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    40000,
    NULL,
    1,
    &task1,
    0);

  xTaskCreatePinnedToCore(
    Task2code,
    "Task2",
    40000,
    NULL,
    4,
    &task2,
    1);
    disableCore0WDT();
}
void func(char* *a,char* *b){
  char* temp = *a;
  *a = *b;
  *b = temp;
}

float Trans_value(int in){
  return in < (READING_MAX/2) ? in/SCALE_G : (in-READING_MAX)/SCALE_G;
}

//char *global=(char*)calloc(30*DATA_PER_PKG+2,sizeof(char));
String global;
bool flag = false;

void Task1code(void* pvParameters){
  uint8_t buffer[14];
  uint16_t ax,ay,az;
  uint8_t count;
  //char* msgStr;
  String msgStr;
  unsigned long time_=micros();
  int interval;
  while (true){
    //msgStr= (char*)calloc(30*DATA_PER_PKG+2,sizeof(char));
    for(count=0;count<DATA_PER_PKG;count++){
        while (micros()-time_<1000);
        interval = micros()-time_;
        time_ = micros();
        uint8_t i;
        digitalWrite(SS_PIN,LOW);
        SPI.transfer(0xbb);
        for(i=0;i<6;i++){
          buffer[i] = SPI.transfer(0x00);
        }

        digitalWrite(SS_PIN, HIGH);

        ax = ((buffer[0]) << 8) | buffer[1];
        ay = ((buffer[2]) << 8) | buffer[3];
        az = ((buffer[4]) << 8) | buffer[5];
        //snprintf(msgStr,30*DATA_PER_PKG+2,"%s%d,%+.4f,%+.4f,%+.4f%s",msgStr,interval,Trans_value(ax),Trans_value(ay),Trans_value(az),count==DATA_PER_PKG-1?"TS":"\n");
        msgStr = msgStr + String(interval) + ',' + String(Trans_value(ax),4)+ ',' + String(Trans_value(ay),4)+ ',' + String(Trans_value(az),4) + '\n';
    }
    //func(&global,&msgStr);
    msgStr = msgStr + "S";
    global = msgStr;
    msgStr = "";
    //free(msgStr);
    flag = true;
    //delay(1);
  }
  
  };

void Task2code(void* pvParameters){
  while(true){
      while (true){
        if(flag) {
          Serial2.print(global);
          //free(global);
          //global = (char*)calloc(30*DATA_PER_PKG+2,sizeof(char));
          global = "";
          flag = false;
          }
        else delay(1);
  }
}};

void loop() {
}