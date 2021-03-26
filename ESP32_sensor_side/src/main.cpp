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
#define FREQ 1000

void Task1code(void* pvParameters);
void Task2code(void* pvParameters);

TaskHandle_t task1;
TaskHandle_t task2;


void setup() {
  Serial2.begin(250000);
  pinMode(SS_PIN, OUTPUT);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);

   // FIFO Mode
  digitalWrite(SS_PIN,LOW);
  SPI.transfer(0x1A);
  SPI.transfer(0x00);
  digitalWrite(SS_PIN,HIGH);

  // Bypass DLPF to lift the sample rate limitation from 1khz to 4khz.
  digitalWrite(SS_PIN,LOW);
  SPI.transfer(0x1D);
  SPI.transfer(0x08);
  digitalWrite(SS_PIN,HIGH);

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


float Trans_value(int in){
  return in < (READING_MAX/2) ? in/SCALE_G : (in-READING_MAX)/SCALE_G;
}


String global;
bool newData = false;

void Task1code(void* pvParameters){
  uint8_t buffer[14];
  uint16_t ax,ay,az;
  uint8_t count;
  String msgStr;
  unsigned long time_=micros();

  while (true){
    for(count=0;count<DATA_PER_PKG;count++){
        while (micros()-time_< (pow10(6)/FREQ));
        time_ = micros();

        digitalWrite(SS_PIN,LOW);
        SPI.transfer(0xbb);
        for(uint8_t i=0;i<6;i++){
          buffer[i] = SPI.transfer(0x00);
        }
        digitalWrite(SS_PIN, HIGH);

        ax = ((buffer[0]) << 8) | buffer[1];
        ay = ((buffer[2]) << 8) | buffer[3];
        az = ((buffer[4]) << 8) | buffer[5];
        
        // msg format : csv -> x,y,z
        msgStr = msgStr + String(Trans_value(ax),4)+ ',' + String(Trans_value(ay),4)+ ',' + String(Trans_value(az),4) + '\n';
    }

    msgStr = msgStr + "S";
    global = msgStr;
    msgStr = "";
    newData = true;
  }
  
  };

void Task2code(void* pvParameters){
  while(true){
      while (true){
        if(newData) {
          Serial2.print(global);
          global = "";
          newData = false;
          }
        else delay(1);
  }
}};

void loop() {
}