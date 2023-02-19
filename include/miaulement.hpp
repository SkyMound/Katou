#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

#define PIN_RX 16
#define PIN_TX 17

DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(PIN_RX, PIN_TX); // RX, TX
bool isAvailable;

void miaulementInit(){
    mySoftwareSerial.begin(9600);
    isAvailable = myDFPlayer.begin(mySoftwareSerial);
    Serial.println(isAvailable);
    myDFPlayer.stop();
}

void playSound(int soundIndex,int volume)
{
    if(myDFPlayer.available()){
        myDFPlayer.volume(volume);
        myDFPlayer.play(soundIndex);
        Serial.println("Sound Played");
    }else
    {
        Serial.println("DF player not available, the sound didnt played");
    }
}