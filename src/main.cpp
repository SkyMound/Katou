#include <M5StickC.h>


float accX = 0;
float accY = 0;
float accZ = 0;


void setup(void) {
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(WHITE);
  delay(1000);
}


void loop() {
  M5.Lcd.fillScreen(WHITE);
  M5.IMU.getAccelData(&accX,&accY,&accZ);

  if (accX > 1.5 ||  accY > 1.5 ) {
    M5.Lcd.setTextColor(TFT_BLACK);  // Adding a background colour erases previous text automatically
    M5.Lcd.setCursor(10, 30);  
    M5.Lcd.setTextSize(3);
    M5.Lcd.print("Miaou"); 

    delay(5000);
  }

}


