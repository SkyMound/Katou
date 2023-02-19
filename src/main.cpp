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
  ronronInit();
  myDFPlayer.volume(10);
  startRespiration();
  startRonron();
  myDFPlayer.volume(30);
  myDFPlayer.play(RONRON_SOUND_2);
}

void loop() {
  // playSound(RONRON_SOUND_1,30);
  delay(1000);
  // put your main code here, to run repeatedly:
}