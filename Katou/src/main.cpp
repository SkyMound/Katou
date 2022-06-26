#include <Arduino.h>
#include <ESP32Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <Adafruit_MMA8451.h>


#define ronronDuree 1500
#define ronronInterDuree 300

SoftwareSerial mySoftwareSerial(32, 14); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

float accX = 0;
float accY = 0;
float accZ = 0;

Servo myServo;
int pos;

int pinVibreur = 27;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

void ronron(bool inspire){
  int tempsDebut = millis();
  int tempsActuel = millis();

  while(tempsActuel - tempsDebut < (inspire ? ronronDuree -200 : ronronDuree)){
    digitalWrite(pinVibreur,LOW);
    inspire ? delay(15) : delay(18);
    digitalWrite(pinVibreur,HIGH);
    inspire ? delay(15) : delay(18);
    tempsActuel = millis();
  }
}

void respirationTask(void * pvParameters){
  while(1){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  }
}

void miaulementTask(void * pvParameters){
    
      myDFPlayer.volume(30);  //Set volume value. From 0 to 30
      myDFPlayer.play(1); 
}

void setup() {
  //Gyroscope
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }

  mma.setRange(MMA8451_RANGE_4_G);
  

  //Vibreur
  pinMode(pinVibreur,OUTPUT);

  //Servo Moteur
  myServo.attach(15);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  myDFPlayer.begin(mySoftwareSerial);
  // if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
  //       Serial.println(F("Unable to begin:"));
  //       Serial.println(F("1.Please recheck the connection!"));
  //       Serial.println(F("2.Please insert the SD card!"));
  //       while(true){
  //         Serial.println("yoo");
  //         delay(0); // Code to compatible with ESP8266 watch dog.
  //       }
  // }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  // myDFPlayer.play(2); 
  

  xTaskCreatePinnedToCore(
                  respirationTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
                  "Tache de respiration",   //线程名称
                  4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
                  NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
                  1,         // Priority of the task.  任务的优先级
                  NULL,      // Task handler.  任务句柄
                  0);
  
//   xTaskCreatePinnedToCore(
//                   miaulementTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
//                   "Tache de miaulement",   //线程名称
//                   4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
//                   NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
//                   1,         // Priority of the task.  任务的优先级
//                   NULL,      // Task handler.  任务句柄
//                   0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  mma.read();
  if(mma.x>1){
    myDFPlayer.play(2);
    for(int i = 0 ; i < 20 ; i++){
      ronron(i%2==0);
      delay(ronronInterDuree);
    }
  }

  
}





void playMiaou() {
   myDFPlayer.play(1);  //Play the first mp3
   delay(2000);
}

void playRonron() {
   myDFPlayer.play(2);  //Play the second mp3
   delay(5000);
}

