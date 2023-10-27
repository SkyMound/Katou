#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include <MadgwickAHRS.h>
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
  
  // Serial.print("acc: "); Serial.print(event.acceleration.x); Serial.print(" ");
  // Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
  // Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");
  // Serial.println("m/s^2 ");
  float accelerationX = event.acceleration.x;
  float accelerationY = event.acceleration.y;
  float accelerationZ = event.acceleration.z;
  float gyroX = event.gyro.x;
  float gyroY = event.gyro.y;
  float gyroZ = event.gyro.z;
  Serial.print("acc: "); Serial.print(event.gyro.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print(" ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print(" ");
  Serial.println("m/s^2 ");
  // filter.updateIMU(gyroX,gyroY,gyroZ,accelerationX, accelerationY, accelerationZ);
  // // print the heading, pitch and roll
  // float roll, pitch, heading;
  // roll = filter.getRoll();
  // pitch = filter.getPitch();
  // heading = filter.getYaw();
  // Serial.print("Orientation: ");
  // Serial.print(heading);
  // Serial.print(" ");
  // Serial.print(pitch);
  // Serial.print(" ");
  // Serial.println(roll);
  // playSound(RONRON_SOUND_1,30);
  delay(100);
  // put your main code here, to run repeatedly:
}