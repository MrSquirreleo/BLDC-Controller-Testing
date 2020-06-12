#include "Energia.h"

#line 1 "P:/Uoe/BLDC-Controller-Testing/SPI_Testing_Script/SPI_Testing_Script.ino"
#include <SPI.h>
#include "SPIHandler.h"


  void setup();
void loop();

#line 5
int Enable = 5;
  int FanEnable = 31;
  int EXTSPIEN = 18;
  int CSTemp = 32;
  int CSSD = 11;
  int CSDRV = 8;

  SPIHandler SPIHandle;

void setup(){
  pinMode(Enable, OUTPUT);
  pinMode(FanEnable , OUTPUT);
  pinMode(EXTSPIEN , OUTPUT);
  pinMode(CSTemp , OUTPUT);
  pinMode(CSSD , OUTPUT);
  pinMode(CSDRV , OUTPUT);
  digitalWrite(CSDRV, HIGH);
  digitalWrite(CSSD, HIGH);
  digitalWrite(EXTSPIEN, LOW);
  digitalWrite(FanEnable, LOW);
  digitalWrite(Enable, HIGH);
  digitalWrite(CSTemp, HIGH);
  digitalWrite(2, LOW);

  
  Serial.begin(38400);
  SPIHandle = SPIHandler();
  






}

void loop(){
  uint16_t * data;
  data = SPIHandle.currentSettings(true);
  SPIHandle.setDriverMode3();
  
  for(int i = 0; i < 7; i++){
      Serial.println(*(data+i), HEX);
  }
  Serial.println("END OF DATA");
  delay(1000);
}



