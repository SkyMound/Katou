#include <Arduino.h>
#include <ESP32Servo.h>

#define PIN_SERVO 15

#define INIT_TIME 1000
#define INIT_POS 130
#define SERVO_SLOW 20
#define PAUSE 800
#define AMPLITUDE 30

Servo myServo;

void respirationInit(){
    myServo.attach(PIN_SERVO);
    myServo.write(INIT_POS);
    delay(INIT_TIME);
}

void respirationTask(){
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