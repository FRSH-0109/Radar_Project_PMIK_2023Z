#ifndef DISTANCE_DRIVER_H
#define DISTANCE_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	double factor;

	uint16_t ticks;
	double distnace;

	bool readyToMeasure;
}distanceSensorStruct;

void distanceSensorInit(distanceSensorStruct* sensor, double ticksToDistanceFactor);

void distanceSensorTrigOn(distanceSensorStruct *sensor);
void distanceSensorDelay(distanceSensorStruct *sensor, uint32_t delay);
void distanceSensorTrigOff(distanceSensorStruct *sensor);

void distanceSensorSendTrig(distanceSensorStruct* sensor);
void distanceSensorGetEcho(distanceSensorStruct* sensor, uint16_t ticks);
bool distanceSensorGetReadyToMeasure(distanceSensorStruct* sensor);

#endif
