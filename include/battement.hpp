#include <Arduino.h>

#define BATTEMENT_FREQ 360

#define PIN_VIBREUR 27

TaskHandle_t* battementHandler = NULL;
void battementTask(void * pvParameters);
void startBattement();
void stopBattement();

void startBattement(){
  xTaskCreate(
                  battementTask,     //Function to implement the task.  线程对应函数名称(不能有返回值)
                  "Tache de battement",   //线程名称
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

void battementTask(void * pvParameters){
  while(true){

    digitalWrite(PIN_VIBREUR,LOW);
    delay(BATTEMENT_FREQ);
    digitalWrite(PIN_VIBREUR,HIGH);
    delay(BATTEMENT_FREQ);
  }
}