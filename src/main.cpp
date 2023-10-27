#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

Adafruit_MMA8451 mma = Adafruit_MMA8451();
void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  if (! mma.begin()) {
    Serial.println("Couldnt start");
  }
  mma.setRange(MMA8451_RANGE_2_G);
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
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  
  Serial.print("acc: "); Serial.print(event.acceleration.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");
  Serial.println("m/s^2 ");
  // playSound(RONRON_SOUND_1,30);
  delay(100);
  // put your main code here, to run repeatedly:
}