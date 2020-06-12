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

#ifndef SPIHANDLER_H
#define SPIHANDLER_H

#include "Energia.h"

class SPIHandler
{
	public:
		SPIHandler();
		int getTempature();
		double getTempaturePrecise();
		void readDriverChip();
		void setDriverMode3();
		void clearFault();
		uint16_t * currentSettings(bool refresh);
		uint16_t * getFaultRegisters();
	private:
		int _EXTSPIEN;
		int _CSTemp;
		int _CSSD;
		int _CSDRV;
		unsigned char* _pP3OUT;
		unsigned char* _pP3DIR;
		uint16_t _faultStatus1;
		uint16_t _vgsStatus;
		uint16_t _driverControl;
		uint16_t _gateDriveH;
		uint16_t _gateDriveL;
		uint16_t _ocpControl;
		uint16_t _csaControl;
};

#endif
