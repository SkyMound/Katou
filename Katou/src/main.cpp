#include <Arduino.h>
#include <ESP32Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <Adafruit_MMA8451.h>

#define ronronDureeTotal 30000
#define ronronDuree 1500
#define ronronInterDuree 200

SoftwareSerial mySoftwareSerial(32, 14); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

float accX = 0;
float accY = 0;
float accZ = 0;

Servo myServo;
int pos;
int pinVibreur = 27;


bool isPasContent =false;
bool estEnAttente = true;
bool isPresent = false;

TaskHandle_t* respirationHandler = NULL;
TaskHandle_t* ronronHandler = NULL;
TaskHandle_t* battementHandler = NULL;

int valAccFort = 1000; 
int valAccMoyenne = 100;
int valAccFaible = 10;

int tempsDebutPresence = 0;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

//---Parti ronronnement---
void oneRonron(bool inspire){
  int tempsDebut = millis();
  int tempsActuel = millis();

  while(tempsActuel - tempsDebut < (inspire ? ronronDuree -200 : ronronDuree)){
    digitalWrite(pinVibreur,LOW);
    inspire ? delay(15) : delay(17);
    digitalWrite(pinVibreur,HIGH);
    inspire ? delay(15) : delay(17);
    tempsActuel = millis();
  }
}

void ronronTask(void * pvParameters){
  int tempsDebut = millis();
  int tempsActuel1 = millis();
  bool inspire = true;
  while(tempsActuel1 - tempsDebut < ronronDureeTotal){
    oneRonron(inspire);
    inspire = !inspire;
    delay(ronronInterDuree);
    tempsActuel1 = millis();
  }
  startBattement();
}

void startRonron(){
  xTaskCreate(
                  ronronTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
                  "Tache de respiration",   //线程名称
                  4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
                  NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
                  1,         // Priority of the task.  任务的优先级
                  ronronHandler
              );
}

//---Parti battement de coeur---
void battementTask(void * pvParameters){
  while(estEnAttente){
    digitalWrite(pinVibreur,LOW);
    delay(200);
    digitalWrite(pinVibreur,HIGH);
    delay(200);
  }
}

void startBattement(){
  xTaskCreate(
                    battementTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
                    "Tache de battement de coeur",   //线程名称
                    4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
                    NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
                    1,         // Priority of the task.  任务的优先级
                    battementHandler
                );
}

void stopBattement(){
  if(battementHandler != NULL){
    vTaskDelete(battementHandler);  
  }
}


//---Parti respiration--- 
void respirationTask(void * pvParameters){
  while(estEnAttente){
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

void startRespiration(){
  xTaskCreate(
                  respirationTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
                  "Tache de respiration",   //线程名称
                  4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
                  NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
                  1,         // Priority of the task.  任务的优先级
                  respirationHandler
              );
}

void stopRespiration(){
  if(respirationHandler != NULL){
    vTaskDelete(respirationHandler);  
  }
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
  
  startRespiration();
  startBattement();
}

void loop() {
 int tempsActuel= millis();



  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  Serial.println(event.acceleration.x); 


 
//Mode détection de présence
  if((mma.x > valAccFaible) && !isPresent){
    tempsDebutPresence = millis();
    isPresent = true;
    myDFPlayer.volume(10);  //Set volume value. From 0 to 30
    myDFPlayer.play((rand() % 6));
  }

  //Le mode présence ne peut s'activer qu'une fois toute les minutes
  if((tempsDebutPresence != 0) && ((tempsActuel - tempsDebutPresence) > 60000)){
    isPresent=false;
  }


//Mode pas content
  if((mma.x > valAccFort)){
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
    myDFPlayer.play(6);
  }


  // Mode Ronronnement
  if(mma.x > valAccMoyenne && mma.x < valAccFort){
    stopBattement();
    startRonron();
  }
}
