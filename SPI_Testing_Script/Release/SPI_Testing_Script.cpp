#include "Energia.h"

#line 1 "C:/Users/squir/Desktop/SPI Testing/SPI_Testing_Script/SPI_Testing_Script.ino"
#include <SPI.h>
#include "SPIHandler.h"


  void setup();
void loop();

#line 5
int Enable = 13;
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
  digitalWrite(Enable, LOW);

  
  Serial.begin(38400);
  SPIHandle = SPIHandler();
  






}

void loop(){
  delay(500);
  double value = SPIHandle.getTempaturePrecise();
  Serial.println(value, DEC);
  




}



