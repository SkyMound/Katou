#include <Arduino.h>

#define RONRON_DUREE_TOTAL 8000
#define RONRON_DUREE 900
#define RONRON_PAUSE 100

#define PIN_VIBREUR 27

void oneRonron(bool inspire){
    int tempsDebut = millis();
    int tempsActuel = millis();
    int dureeActuel = 0;
    while(dureeActuel < RONRON_DUREE){
        int ronronFreq = inspire ? 17 + dureeActuel*10/RONRON_DUREE : 21 - dureeActuel*5/RONRON_DUREE;
        digitalWrite(PIN_VIBREUR,LOW);
        delay(ronronFreq);
        digitalWrite(PIN_VIBREUR,HIGH);
        delay(ronronFreq);
        tempsActuel = millis();
        dureeActuel = tempsActuel - tempsDebut;
    }
}

void ronronTask(){
//   playSound(7,30);
  int tempsDebut = millis();
  int tempsActuel1 = millis();
  bool inspire = true;
  while(tempsActuel1 - tempsDebut < RONRON_DUREE_TOTAL){
    oneRonron(inspire);
    inspire = !inspire;
    delay(RONRON_PAUSE);
    tempsActuel1 = millis();
  }
//   ronrone=false;
}