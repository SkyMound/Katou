#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

#define PIN_RX 16
#define PIN_TX 17

DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(PIN_RX, PIN_TX); // RX, TX
void printDetail(uint8_t type, int value);

void miaulementInit(){
    mySoftwareSerial.begin(9600);//, SWSERIAL_8N1, PIN_RX, PIN_TX, false, 256);
    Serial.println(mySoftwareSerial.available());
    myDFPlayer.begin(mySoftwareSerial);
    Serial.println(myDFPlayer.available());
}

void playSound(int soundIndex,int volume)
{
    if(myDFPlayer.available()){
        
        Serial.println("Sound Played");
        printDetail(myDFPlayer.readType(), myDFPlayer.read()); 
    }else
    {
        Serial.println("DF player not available, the sound didnt played");
        printDetail(myDFPlayer.readType(), myDFPlayer.read()); 
    }
}


void printState(){
    Serial.println(myDFPlayer.waitAvailable());
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}