#include <Arduino.h>

#define RONRON_DUREE_TOTAL 8000
#define RONRON_DUREE 900
#define RONRON_PAUSE 100

#define PIN_VIBREUR 27

TaskHandle_t* ronronHandler = NULL;
void stopRonron();
void ronronInit();
void startRonron();
void oneRonron(bool inspire);
void ronronTask(void * pvParameters);

void ronronInit(){
  pinMode(PIN_VIBREUR,OUTPUT);
}


void startRonron(){
  xTaskCreate(
                  ronronTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
                  "Tache de ronronnement",   //线程名称
                  4096,      // The size of the task stack specified as the number of * bytes.任务堆栈的大小(字节)
                  NULL,      // Pointer that will be used as the parameter for the task * being created.  创建作为任务输入参数的指针
                  1,         // Priority of the task.  任务的优先级
                  ronronHandler
              );
}

void stopRonron(){
  if(ronronHandler != NULL){
    vTaskDelete(ronronHandler);  
  }
}


void oneRonron(bool inspire){
    int tempsDebut = millis();
    int tempsActuel = millis();
    int dureeActuel = 0;
    while(dureeActuel < RONRON_DUREE){
        int ronronFreq = inspire ? 17 + dureeActuel*10/RONRON_DUREE : 21 - dureeActuel*5/RONRON_DUREE;
        digitalWrite(PIN_VIBREUR,LOW);
        delay(ronronFreq);
        digitalWrite(PIN_VIBREUR,HIGH);
        delay(ronronFreq);
        tempsActuel = millis();
        dureeActuel = tempsActuel - tempsDebut;
    }
}

void ronronTask(void * pvParameters){
  while(true){
    int tempsDebut = millis();
    int tempsActuel1 = millis();
    bool inspire = true;
    while(tempsActuel1 - tempsDebut < RONRON_DUREE_TOTAL){
      oneRonron(inspire);
      inspire = !inspire;
      delay(RONRON_PAUSE);
      tempsActuel1 = millis();
    }
  }
}


