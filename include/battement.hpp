#include <Arduino.h>

#define BATTEMENT_FREQ 360

#define PIN_VIBREUR 27

void battementTask(){
    digitalWrite(PIN_VIBREUR,LOW);
    delay(BATTEMENT_FREQ);
    digitalWrite(PIN_VIBREUR,HIGH);
    delay(BATTEMENT_FREQ);
}