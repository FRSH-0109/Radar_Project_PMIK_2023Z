#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include "stdint.h"

typedef struct
{
	uint16_t positionInDegrees;
	uint16_t positionInPulseWidth;
}servoDriverStruct;

void servoDriverInit();
void servoDriverStart();
void servoDriverStop();

void servoDriverStartTimer();
void servoDriverStopTimer();

#endif
