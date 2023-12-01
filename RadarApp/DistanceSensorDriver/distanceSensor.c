#include "distanceSensor.h"

void distanceSensorInit(distanceSensorStruct* sensor, double ticksToDistanceFactor)
{
	if(ticksToDistanceFactor > 0)
	{
		sensor->factor = ticksToDistanceFactor;
	}
	else
	{
		sensor->factor = 1;
	}

	sensor->ticks = 0;
	sensor->distnace = 0;

	sensor->readyToMeasure = true;
}

__attribute__((weak)) void distanceSensorTrigOn(distanceSensorStruct *sensor)
{
	////////////////////////////////////////////////////////////////////////////
}

__attribute__((weak)) void distanceSensorDelay(distanceSensorStruct *sensor, uint32_t delay)
{
	////////////////////////////////////////////////////////////////////////////
}

__attribute__((weak)) void distanceSensorTrigOff(distanceSensorStruct *sensor)
{
	////////////////////////////////////////////////////////////////////////////
}

void distanceSensorSendTrig(distanceSensorStruct* sensor)
{
	if(sensor->readyToMeasure)
	{
		distanceSensorTrigOn(sensor);
		distanceSensorDelay(sensor, 10);
		distanceSensorTrigOff(sensor);

		sensor->readyToMeasure = false;
	}
}

void distanceSensorGetEcho(distanceSensorStruct* sensor, uint16_t ticks)
{
	sensor->ticks = ticks;
	sensor->distnace = (double)((double)sensor->ticks / (double)sensor->factor);

	sensor->readyToMeasure = true;
}


bool distanceSensorGetReadyToMeasure(distanceSensorStruct* sensor)
{
	return sensor->readyToMeasure;
}

uint16_t distanceSensorGetDistance(distanceSensorStruct* sensor)
{
	return sensor->distnace;
}
