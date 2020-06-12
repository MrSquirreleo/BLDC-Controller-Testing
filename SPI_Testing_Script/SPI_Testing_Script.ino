#include <SPI.h>
#include "SPIHandler.h"

//Variable declerations
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

  //Begin Serial Communication
  Serial.begin(38400);
  SPIHandle = SPIHandler();
  /*
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setModule(0);
  */
}

void loop(){
  uint16_t * data;
  data = SPIHandle.currentSettings(true);
  SPIHandle.setDriverMode3();
  //data = SPIHandle.getFaultRegisters();
  for(int i = 0; i < 7; i++){
      Serial.println(*(data+i), HEX);
  }
  Serial.println("END OF DATA");
  delay(1000);
}
