/**
 * @file servo_driver.c
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 20 Nov 2023
 * @brief File containing code of servomechanism driver controlled by PWM,
 * which was created for Radar embedded project.
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#include "servo_driver.h"

void servoDriverInit(servoDriverStruct *servoDriver, float dMin, float dMax, uint16_t pMin, uint16_t pMax)
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

static inline bool servoDriverAssertDegrees(servoDriverStruct *servoDriver, float degrees)
{
	return (degrees >= servoDriver->degreesMin && degrees <= servoDriver->degreesMax);
}

bool servoDriverSetDegrees(servoDriverStruct *servoDriver, float degrees)
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

uint16_t servoDriverDegreesToPulseWidth(servoDriverStruct *servoDriver, float degrees)
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

