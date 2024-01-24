/**
 * @file distance_sensor.c
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 20 Nov 2023
 * @brief File containing code of ultrasonic distance sensor,
 * implementation, modifications and making measurement
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#include <distance_sensor.h>

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
		distanceSensorDelay(sensor, 1);		//TODO: change to assembly NOP's
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

double distanceSensorGetDistance(distanceSensorStruct* sensor)
{
	return sensor->distnace;
}
