#include <Arduino.h>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(PIN_VIBREUR,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  // respirationInit();
  miaulementInit();
  playSound(6,28);
}

void loop() {
  delay(1000);
  // ronronTask();
  // battementTask();
  //respirationTask();

  //delay(1000);
  // put your main code here, to run repeatedly:
}