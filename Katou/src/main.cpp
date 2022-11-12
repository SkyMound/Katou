#include <Arduino.h>
#include <ESP32Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <Adafruit_MMA8451.h>
#include <math.h>

#define ronronDureeTotal 8000
#define ronronDuree 1200
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
bool ronrone = false;

TaskHandle_t* respirationHandler;
TaskHandle_t* ronronHandler;
TaskHandle_t* battementHandler;

int valAccFort = 2200; 
int valAccMoyenne = 500;
int valAccFaible = 50;

int tempsDebutPresence = 0;
int tempsDebutPasContent = 0;
int tempsDebutRonron = 0;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

// DFPlayer

void playSound(int soundIndex,int volume)
{
  
  if(myDFPlayer.available()){
    myDFPlayer.volume(volume);
    myDFPlayer.play(soundIndex);
    Serial.println("Sound Played");
  }else
  {
    Serial.println("DF player not available, the sound didnt played");
  }
}

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
//void * pvParameters
void ronronTask(){
  playSound(7,30);
  int tempsDebut = millis();
  int tempsActuel1 = millis();
  bool inspire = true;
  while(tempsActuel1 - tempsDebut < ronronDureeTotal){
    oneRonron(inspire);
    inspire = !inspire;
    delay(ronronInterDuree);
    tempsActuel1 = millis();
  }
  ronrone=false;
}

//---Parti battement de coeur---
void battementTask(void * pvParameters){
  while(true){
    if(ronrone){
      ronronTask();
  }
    else{
      digitalWrite(pinVibreur,LOW);
      delay(200);
      digitalWrite(pinVibreur,HIGH);
      delay(200);
    }
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
  Serial.println("Task deleted 1");
  if(battementHandler != NULL){
    Serial.println("Task deleted");
    vTaskDelete(battementHandler);  
  }
}




// void startRonron(){
//   xTaskCreate(
//                   ronronTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
//                   "Tache de respiration",   //线程名称
//                   4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
//                   NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
//                   1,         // Priority of the task.  任务的优先级
//                   ronronHandler
//               );
// }




//---Parti respiration--- 
void respirationTask(void * pvParameters){
  while(estEnAttente){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(10);                       // waits 15 ms for the servo to reach the position
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
    Serial.println("Task deleted");
    vTaskDelete(respirationHandler);  
  }
}


void setup() {
  respirationHandler = NULL;
  ronronHandler = NULL;
  battementHandler = NULL;

  //Gyroscope
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1){
      delay(20000)
    };
  }

  mma.setRange(MMA8451_RANGE_4_G);
  
  //Vibreur
  pinMode(pinVibreur,OUTPUT);

  //Servo Moteur
  myServo.attach(15);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  boolean test = myDFPlayer.begin(mySoftwareSerial);
  Serial.println(test);
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
  myDFPlayer.stop();
  playSound(5,15);
  startRespiration();
  startBattement();
}

void loop() {
 int tempsActuel= millis();


  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  //Serial.println(event.acceleration.x); 
  int testValue=abs(mma.x);
  Serial.println(testValue);
 
  //Mode détection de présence
  if(testValue > valAccFaible && testValue < valAccMoyenne && !isPresent){
    Serial.println("Sent une présence !");
    tempsDebutPresence = millis();
    isPresent = true;
    playSound(2,10);
  }

  //Le mode présence ne peut s'activer qu'une fois toute les minutes
  if((tempsDebutPresence != 0) && ((tempsActuel - tempsDebutPresence) > 60000)){
    isPresent=false;
  }


  //Mode pas content
  if((testValue > valAccFort && !isPasContent)){
    Serial.println("Katou est pas content");
    isPasContent = true;
    tempsDebutPasContent = millis();
    playSound(6,15);
  }

  if((tempsDebutPasContent != 0) && ((tempsActuel - tempsDebutPasContent) > 5000)){
    isPasContent=false;
  }

  // Mode Ronronnement
  if(testValue > valAccMoyenne && testValue < valAccFort && !ronrone && !isPasContent){
    Serial.println("Katou ronrone !");
    ronrone = true;
    tempsDebutRonron = millis();
    //startRonron();
  }

  if((tempsDebutRonron != 0) && ((tempsActuel - tempsDebutRonron) > ronronDureeTotal)){
    ronrone=false;
  }

  // if(!isPasContent && !ronrone && !isPresent){
  //   //startBattement();
  // }
  delay(150);
}
