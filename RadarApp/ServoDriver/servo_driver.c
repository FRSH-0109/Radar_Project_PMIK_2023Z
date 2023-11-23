
#include "servo_driver.h"

void servoDriverInit(servoDriverStruct *servoDriver, uint8_t dMin, uint8_t dMax, uint16_t pMin, uint16_t pMax)
{
	servoDriver->degreesMin = dMin;
	servoDriver->degreesMax = dMax;
	servoDriver->pulseWidthMin = pMin;
	servoDriver->pulseWidthMax = pMax;
}

void servoDriverStart(servoDriverStruct *servoDriver)
{
	servoDriverStartTimer(servoDriver);
}

void servoDriverStop(servoDriverStruct *servoDriver)
{
	servoDriverStopTimer(servoDriver);
}

static inline bool servoDriverAssertPulseWidth(servoDriverStruct *servoDriver, uint16_t pulseWidth)
{
	return (pulseWidth >= servoDriver->pulseWidthMin && pulseWidth <= servoDriver->pulseWidthMax);
}

static inline bool servoDriverAssertDegrees(servoDriverStruct *servoDriver, uint16_t degrees)
{
	return (degrees >= servoDriver->degreesMin && degrees <= servoDriver->degreesMax);
}

bool servoDriverSetDegrees(servoDriverStruct *servoDriver, uint16_t degrees)
{
	if(servoDriverAssertDegrees(servoDriver, degrees) == false)
	{
		return false;
	}
	servoDriver->positionInDegrees = degrees;

	uint16_t tmpPulseWidth = servoDriverDegreesToPulseWidth(servoDriver, degrees);
	if(servoDriverAssertPulseWidth(servoDriver, tmpPulseWidth) == false)
	{
		return false;
	}
	servoDriver->positionInPulseWidth = tmpPulseWidth;

	servoDriverSetPulseWidth(servoDriver);
	return true;
}

uint16_t servoDriverDegreesToPulseWidth(servoDriverStruct *servoDriver, uint16_t degrees)
{
	uint16_t tmpPulseWidth = (((degrees - servoDriver->degreesMin) * (servoDriver->pulseWidthMax - servoDriver->pulseWidthMin) ) / (servoDriver->degreesMax - servoDriver->degreesMin)) + servoDriver->pulseWidthMin;
	return tmpPulseWidth;
}


__attribute__((weak)) void servoDriverStartTimer(servoDriverStruct *servoDriver)
{
/////////////////////////////////////////////////
}

__attribute__((weak)) void servoDriverStopTimer(servoDriverStruct *servoDriver)
{
/////////////////////////////////////////////////
}

__attribute__((weak)) void servoDriverSetPulseWidth(servoDriverStruct *servoDriver)
{
	/////////////////////////////////////////////
}

