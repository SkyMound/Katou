#include <Arduino.h>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  respirationInit();
  miaulementInit();
  myDFPlayer.volume(10);

}

void loop() {
  // ronronTask();
  // battementTask();
  // respirationTask();
  // printState();
  // myDFPlayer.play(1);
  // delay(1000);
  playSound(1,10);
  delay(1000);
  myDFPlayer.play(3);

  delay(1000);
  // put your main code here, to run repeatedly:
}