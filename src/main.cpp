#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include <MadgwickAHRS.h>
#include <cmath>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

Adafruit_MMA8451 mma = Adafruit_MMA8451();
Madgwick filter;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  if (! mma.begin()) {
    Serial.println("Couldnt start");
  }
  mma.setRange(MMA8451_RANGE_2_G);
  filter.begin(10);
  //miaulementInit();
  mySoftwareSerial.begin(9600, SWSERIAL_8N1, PIN_RX, PIN_TX, false, 256);
  Serial.println(mySoftwareSerial.available());
  myDFPlayer.begin(mySoftwareSerial);
  Serial.println(myDFPlayer.available());
  ronronInit();
  myDFPlayer.volume(10);
  //startRonron();
  //startBattement();
  //delay(5000);
  //stopBattement();
  //myDFPlayer.play(RONRON_SOUND_2);
}

void loop() {
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  float norm = sqrt(pow(mma.x_g,2)+pow(mma.y_g,2)+pow(mma.z_g,2));

  float godAcceleration = abs(norm-1);
  // Serial.print("acc: "); Serial.print(abs(abs(event.acceleration.x+event.acceleration.y+event.acceleration.z)-9.8f));Serial.println(" m/s^2 ");
  Serial.print("acc: "); Serial.print(godAcceleration);Serial.print(" "); Serial.print(event.acceleration.y);Serial.print(" "); Serial.print(event.acceleration.z);Serial.println(" m/s^2 ");
  
  // Serial.print("x_g: "); Serial.print(mma.x_g); Serial.print(" ");
  // Serial.print("Y_g: "); Serial.print(mma.y_g); Serial.print(" ");
  // Serial.print("Z_g: "); Serial.print(mma.z_g); Serial.print(" ");
  // //Serial.print("angle: "); Serial.print(angle_degres); Serial.print(" ");
  // Serial.println("m/s^2 ");


  // Serial.print("Orientation: ");
  // Serial.print(event.acceleration.heading);
  // Serial.print(" ");
  // Serial.print(event.acceleration.pitch);
  // Serial.print(" ");
  // Serial.println(event.acceleration.roll);
  // playSound(RONRON_SOUND_1,30);
  delay(500);
  // put your main code here, to run repeatedly:
}