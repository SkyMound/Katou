#include <Arduino.h>
#include <ESP32Servo.h>

#define PIN_SERVO 15

#define INIT_TIME 1000
#define INIT_POS 130
#define SERVO_SLOW 20
#define PAUSE 800
#define AMPLITUDE 30

Servo myServo;
TaskHandle_t* respirationHandler = NULL;

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

void stopBattement(){
    if(respirationHandler != NULL){
    vTaskDelete(respirationHandler);  
  }
}

void respirationInit(){
    myServo.attach(PIN_SERVO);
    myServo.write(INIT_POS);
    delay(INIT_TIME);
}

void respirationTask(void * pvParameters){
    for (int pos = INIT_POS -AMPLITUDE; pos <= INIT_POS +AMPLITUDE; pos += 1) { 
        myServo.write(pos);              
        delay(SERVO_SLOW);                        
    }
    delay(PAUSE);
    for (int pos = INIT_POS +AMPLITUDE; pos >= INIT_POS -AMPLITUDE; pos -= 1) { 
        myServo.write(pos);              
        delay(SERVO_SLOW);                       
    }
   delay(PAUSE);
}