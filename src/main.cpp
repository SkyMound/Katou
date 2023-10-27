#include <Arduino.h>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  //miaulementInit();
  mySoftwareSerial.begin(9600, SWSERIAL_8N1, PIN_RX, PIN_TX, false, 256);
  Serial.println(mySoftwareSerial.available());
  myDFPlayer.begin(mySoftwareSerial);
  Serial.println(myDFPlayer.available());
  ronronInit();
  myDFPlayer.volume(10);
  //startRonron();
  startBattement();
  delay(5000);
  stopBattement();
  //myDFPlayer.play(RONRON_SOUND_2);
}

void loop() {
  // playSound(RONRON_SOUND_1,30);
  delay(1000);
  // put your main code here, to run repeatedly:
}