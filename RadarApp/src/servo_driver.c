
#include "servo_driver.h"

servoDriverStruct servoDriver;

void servoDriverInit()
{

}

void servoDriverStart()
{
	servoDriverStartTimer();
}

void servoDriverStop()
{
	servoDriverStopTimer();
}

__attribute__((weak)) void servoDriverStartTimer()
{
/////////////////////////////////////////////////
}

__attribute__((weak)) void servoDriverStopTimer()
{
/////////////////////////////////////////////////
}


static bool servoDriverAssertDegrees(uint16_t degrees)
{
	if(degrees >= 0 && degrees < 360)
	{
		return true;
	}
	return false;
}

__attribute__((weak)) uint16_t servoDriverDegreesToPulseWidth(uint16_t degrees)
{

}

bool servoDriverSetDegrees(uint16_t degrees)
{
	if(servoDriverAssertDegrees(degrees) == false)
	{
		return false;
	}

	servoDriver.positionInDegrees = degrees;
	servoDriverSetPulseWidth(degrees);
	return true;
}

bool servoDriverSetPulseWidth(uint16_t degrees)
{
	servoDriverDegreesToPulseWidth(degrees);
}

