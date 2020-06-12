/*  SPI COMS Handler
*	The methods in here should make communicating with the various SPI chips within the BLDC board a little bit easier
*
*	Written by: Camry Mabon
*	Version: 0.1
*/

/*  Device Locations:
*	Tempature chip - Bus 1
*	Driver chip - Bus 1
* 	External SPI Device - Bus 1
* 	SD Card - Bus 2
*/
/*TODO:
 * -Change tempter select pin toggling over to using declarations from msp430f5529.h to make the code more portable
 * -Figure out good way to handle the coast functionality in the driver
 * -Write method for the current sense resistor functions (gain, etc...)
 */


#include "SPIHandler.h"
#include "Energia.h"
#include <SPI.h>
#include <msp430f5529.h>
 
#define P3OUT_REGISTER ((volatile uint8_t * const) 0x222)
#define P3DIR_REGISTER ((volatile uint8_t * const) 0x224)

//Pin decelerations
int _EXTSPIEN = 18;
int _CSTemp = 32;
int _CSSD = 11;
int _CSDRV = 8;

//Variables representing memory of Driver chip
uint16_t _faultStatus1 = 0x0000;
uint16_t _vgsStatus = 0x0000;
uint16_t _driverControl = 0x0000;
uint16_t _gateDriveH = 0x0000;
uint16_t _gateDriveL = 0x0000;
uint16_t _ocpControl = 0x0000;
uint16_t _csaControl = 0x0000;
uint16_t driverSettings[7];

//Register pointers
unsigned char *_pP3OUT = (unsigned char *) 0x000222; //Output control register
unsigned char *_pP3DIR = (unsigned char *) 0x000224; //Direction control register

SPIHandler::SPIHandler(){

	pinMode(_EXTSPIEN, OUTPUT);
	pinMode(_CSTemp, OUTPUT);
	pinMode(_CSSD, OUTPUT);
	pinMode(_CSDRV, OUTPUT);


	//Put bus into inactive state
	digitalWrite(_EXTSPIEN, HIGH);
	digitalWrite(_CSTemp, HIGH);
	digitalWrite(_CSSD, HIGH);
	digitalWrite(_CSDRV, HIGH);

	//Start SPI stuff
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV128); //This can be changed if this bus speed is to fast and cuasing issues
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE1);
	SPI.setModule(0);
} 

int SPIHandler::getTempature(){
	unsigned int rawTemp;
	digitalWrite(_CSTemp, LOW);
	rawTemp = SPI.transfer16(0xFFFF);
	digitalWrite(_CSTemp, HIGH);
	//bool SignBit = bitRead(rawTemp, 15); //Get the sign bit 
	rawTemp = rawTemp & 0x7FFF;	// Clear the sign bit in the number
	rawTemp = rawTemp >> 3; //Clear the extra bits from the transfer (only need bits 15 to 3)
	int convertedTemp = rawTemp * 0.0625; //Convert to temapture in C (value of 0.0625 taken from manufacuter datasheet
	//bitWrite(convertedTemp, 15, SignBit); //Add the sign bit back in so that it can display negative numbers
	return convertedTemp;
}

//Returns a more precise decimal version of the tempature (useful for logging?) 
double SPIHandler::getTempaturePrecise(){
	unsigned int rawTemp;
	*P3DIR_REGISTER = *P3DIR_REGISTER | 0x80; //Set pin 3.7 as an output
	*P3OUT_REGISTER = *P3OUT_REGISTER & 0x7F; //Write pin 3.7 low to engage the chip in communication
	rawTemp = SPI.transfer16(0xFFFF);
	*P3OUT_REGISTER = *P3OUT_REGISTER | 0x80; //Write pin 3.7 high to disengage the chip in communication
	rawTemp = rawTemp & 0x7FFF;
	rawTemp = rawTemp >> 3;
	double convertedTemp = (double) rawTemp * 0.0625;
	return convertedTemp;
}
 
//Reads all the registers on the chip into memory
//Please note that this isn't the fastest operation as it has to make a number of SPI transfers. Use sparingly
void SPIHandler::readDriverChip(){
    //Pull the data in
    P2OUT = P2OUT & 0x7F;
    _faultStatus1 = SPI.transfer(0x80);
    _faultStatus1 = (_faultStatus1 << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _vgsStatus = SPI.transfer(0x88);
    _vgsStatus = (_vgsStatus << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _driverControl = SPI.transfer(0x90);
    _driverControl = (_driverControl << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _gateDriveH = SPI.transfer(0x98);
    _gateDriveH = (_gateDriveH << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _gateDriveL = SPI.transfer(0xA0);
    _gateDriveL = (_gateDriveL << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _ocpControl = SPI.transfer(0xA8);
    _ocpControl = (_ocpControl << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _csaControl = SPI.transfer(0xB0);
    _csaControl = (_csaControl << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    driverSettings[0] = _faultStatus1;
    driverSettings[1] = _vgsStatus;
    driverSettings[2] = _driverControl;
    driverSettings[3] = _gateDriveH;
    driverSettings[4] = _gateDriveL;
    driverSettings[5] = _ocpControl;
    driverSettings[6] = _csaControl;
    //Format the data to get rid of write and don't care bits
    for(int i = 0; i<7; i++){
        driverSettings[i] = driverSettings[i] & 0x07FF;
    }
}


//Reads the fault registers on the driver chip and then returns a pointer to the updated array
uint16_t * SPIHandler::getFaultRegisters(){
    P2OUT = P2OUT & 0x7F;
    _faultStatus1 = SPI.transfer(0x80);
    _faultStatus1 = (_faultStatus1 << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    delayMicroseconds(5);
    P2OUT = P2OUT & 0x7F;
    _vgsStatus = SPI.transfer(0x88);
    _vgsStatus = (_vgsStatus << 8) + SPI.transfer(0x00);
    P2OUT = P2OUT | 0x80;
    return driverSettings;
}

uint16_t * SPIHandler::currentSettings(bool refresh)
{
    if(refresh == true){
        SPIHandler::readDriverChip();
    }
    return driverSettings;
}

//Changes the drive mode of the driver chip to the correct 3X PWM mode
void SPIHandler::setDriverMode3(){
    driverSettings[2] = driverSettings[2] | 0x0010; //Set the corresponding bits in the driver settings register
    driverSettings[2] = driverSettings[2] | 0x1000; //Set write and address bit
    char firstByte = (driverSettings[2] & 0xFF00) > 8; //Split the 16 bit data into two bytes and then write both
    char secondByte = driverSettings[2] & 0x00FF;
    P2OUT = P2OUT & 0x7F;
    SPI.transfer(firstByte);
    SPI.transfer(secondByte);
    P2OUT = P2OUT | 0x80;
}

//Clears a latched fault condition in the driver
void SPIHandler::clearFault(){
    driverSettings[2] = driverSettings[2] | 0x0001;
    driverSettings[2] = driverSettings[2] | 0x1000; //Set write and address bit
    char firstByte = (driverSettings[2] & 0xFF00) > 8; //Split the 16 bit data into two bytes and then write both
    char secondByte = driverSettings[2] & 0x00FF;
    P2OUT = P2OUT & 0x7F;
    SPI.transfer(firstByte);
    SPI.transfer(secondByte);
    P2OUT = P2OUT | 0x80;
    //As the clear fault bit resets in the driver on every clear it should be reflected in the register array
    driverSettings[2] = driverSettings[2] & 0xFFFE;
}

