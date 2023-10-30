#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include "ronron.hpp"
#include "battement.hpp"
#include "respiration.hpp"
#include "miaulement.hpp"

#define ALLUME_MIAULEMENT_FREQ 30000
#define DANS_LES_MAINS_MIAULEMENT_FREQ 20000

#define DANS_LES_MAINS_TIMER 30000
#define RONRON_TIMER 30000
#define MODE_TIMER 30000
#define ENERVE_TIMER 15000
#define DANS_LES_MAINS_TIMER_MIN 5000

#define ENERVE_THRESHOLD 2.0
#define DANS_LES_MAINS_THRESHOLD 0.5
#define RONRON_THRESHOLD 0.7
#define NO_INTERACTION_THRESHOLD 0.15

typedef enum{
  MODE_ALLUME = (0),
  MODE_DANS_LES_MAINS = (1),
  MODE_RONRON = (2),
  MODE_ENERVE = (3)
} KatouMode;

Adafruit_MMA8451 mma = Adafruit_MMA8451();
int lastMeow = millis();
int modeChanged = millis();
KatouMode currentMode = MODE_ALLUME;
bool hasAngryMeow = false;
bool isRonronning = false;


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

  // Serial.print("acc: ");
  // Serial.print(accelerationGravityless);
  // Serial.println("");
  if(currentMode == MODE_ALLUME){
    stopRonron();
    startBattement();
    if(currentTime-lastMeow > ALLUME_MIAULEMENT_FREQ) {
      myDFPlayer.volume(15);
      myDFPlayer.play(5);
      lastMeow = currentTime;
    }

    if(accelerationGravityless > ENERVE_THRESHOLD){
      currentMode = MODE_ENERVE;
      Serial.println("Mode enervé");
      modeChanged = millis();
    }
    else if(accelerationGravityless > DANS_LES_MAINS_THRESHOLD){
      currentMode = MODE_DANS_LES_MAINS;
      Serial.println("Mode dans les mains");
      modeChanged = millis();
    }
  }
  else if(currentMode == MODE_DANS_LES_MAINS){
    stopRonron();
    startBattement();
    if(currentTime - lastMeow > DANS_LES_MAINS_MIAULEMENT_FREQ){
      myDFPlayer.volume(20);
      myDFPlayer.play(1);
      lastMeow = currentTime;
    }
    if(accelerationGravityless < NO_INTERACTION_THRESHOLD ){
      if(currentTime - modeChanged > DANS_LES_MAINS_TIMER){
        currentMode = MODE_ALLUME;
        Serial.println("Mode allume");
        modeChanged = millis();
      }
    }
    else if(accelerationGravityless > ENERVE_THRESHOLD){
      currentMode = MODE_ENERVE;
      Serial.println("Mode enervé");
      modeChanged = millis();
    }
    else if(accelerationGravityless > RONRON_THRESHOLD && currentTime - modeChanged > DANS_LES_MAINS_TIMER_MIN){
      currentMode = MODE_RONRON;
      Serial.println("Mode ronron");
      modeChanged = millis();
    }
  }
  else if(currentMode == MODE_RONRON){
    stopBattement();
    startRonron();
    if(!isRonronning){
      isRonronning = true;
      Serial.println("Ronronnement");
      myDFPlayer.volume(30);
      myDFPlayer.loop(7);      
    }
    isRonronning = true;
    if(accelerationGravityless > ENERVE_THRESHOLD){
      isRonronning = false;
      currentMode = MODE_ENERVE;
      myDFPlayer.stop();
      Serial.println("Mode enervé");
      modeChanged = millis();
    }
    else if(accelerationGravityless > RONRON_THRESHOLD){
      modeChanged = millis();
    }
    else if(accelerationGravityless < NO_INTERACTION_THRESHOLD && currentTime - modeChanged > RONRON_TIMER ){
      currentMode = MODE_DANS_LES_MAINS;
      isRonronning = false;
      modeChanged = millis();
      myDFPlayer.stop();
      Serial.println("Mode dans les mains");
    }
  }
  else if(currentMode == MODE_ENERVE){
    stopRonron();
    startBattement();
    if(!hasAngryMeow){
      myDFPlayer.volume(25);
      myDFPlayer.play(6);
      hasAngryMeow = true;
    }
    if(currentTime - modeChanged > ENERVE_TIMER ){
      hasAngryMeow = false;
      currentMode = MODE_ALLUME;
      modeChanged = millis();
      Serial.println("Mode allumé");
    }
  }
  

}