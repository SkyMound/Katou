#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

#define ALLUME_MIAULEMENT_FREQ 300000
#define DANS_LES_MAINS_MIAULEMENT_FREQ 30000

#define DANS_LES_MAINS_TIMER 60000
#define RONRON_TIMER 60000


typedef enum{
  MODE_ALLUME = (0),
  MODE_DANS_LES_MAINS = (1),
  MODE_RONRON = (2),
  MODE_ENERVE = (3)
} KatouMode;

Adafruit_MMA8451 mma = Adafruit_MMA8451();
int lastMeow = millis();
int modeChanged = millis();
KatouMode currentMode;
bool hasAngryMeow = false;


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
  // myDFPlayer.play();
  // 1 - demande ronron
  // 2 - pas content
  // 3 - mimi 
  // 4 - mimi aussi
  // 5 - normal
  // 6 - venere de ouf
  // 7 - ronron (5,6sec)
  // 8 - ronron2 (5,6sec)
}


void loop() {
  int currentTime = millis();

  mma.read();
  float accelerationNorm = sqrt(pow(mma.x_g,2)+pow(mma.y_g,2)+pow(mma.z_g,2));
  float accelerationGravityless = abs(accelerationNorm-1);


  if(accelerationGravityless < 0.1 && currentTime - lastMeow > RONRON_TIMER){
    currentMode = MODE_ALLUME;
    modeChanged = millis();
    Serial.println("Mode allume");
  }
  else if(accelerationGravityless < 0.25 && currentTime - lastMeow > RONRON_TIMER){
    Serial.println("Mode dans les mains");
    modeChanged = millis();
    currentMode = MODE_DANS_LES_MAINS;
  }
  else if(accelerationGravityless < 0.5 && currentMode == MODE_DANS_LES_MAINS && currentTime - lastMeow > RONRON_TIMER){
    Serial.println("Mode ronron");
    modeChanged = millis();
    currentMode = MODE_RONRON;
  }
  else if(accelerationGravityless > 1.25){
    Serial.println("Mode enerve");
    modeChanged = millis();
    currentMode = MODE_ENERVE;
  }



  if(currentMode = MODE_ALLUME){
    startBattement();
    stopRonron();
    if(currentTime-lastMeow > ALLUME_MIAULEMENT_FREQ) {
      myDFPlayer.volume(15);
      myDFPlayer.play(5);
      lastMeow = currentTime;
    }
  }
  else if(currentMode = MODE_DANS_LES_MAINS){
    startBattement();
    stopRonron();
    if(currentTime - lastMeow > DANS_LES_MAINS_MIAULEMENT_FREQ){
      myDFPlayer.volume(10);
      myDFPlayer.play(1);
      lastMeow = currentTime;
    }
    if(currentTime - modeChanged > DANS_LES_MAINS_TIMER ){
      currentMode = MODE_ALLUME;
      modeChanged = millis();
      Serial.println("Mode allume");

    }
  }
  else if(currentMode = MODE_RONRON){
    stopBattement();
    startRonron();
    if(myDFPlayer.available()){
      myDFPlayer.volume(25);
      myDFPlayer.play(8);      
    }
    if(currentTime - modeChanged > RONRON_TIMER ){
      currentMode = MODE_DANS_LES_MAINS;
      modeChanged = millis();
      Serial.println("Mode dans les mains");

    }
  }
  else if(currentMode = MODE_ENERVE){
    startBattement();
    stopRonron();
    if(!hasAngryMeow){
      myDFPlayer.volume(25);
      myDFPlayer.play(6);
      hasAngryMeow = true;
    }
  }
  

}