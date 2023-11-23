#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define SERVO_MIN_DEGREES 0
#define SERVO_MAX_DEGREES 180

#define SERVO_MIN_PULSE_WIDTH 1000	//1ms = 0 degrees
#define SERVO_MAX_PULSE_WIDTH 2000	//2ms = 180 degrees

typedef struct
{
	uint8_t degreesMin;
	uint8_t degreesMax;

	uint16_t pulseWidthMin;
	uint16_t pulseWidthMax;

	uint16_t positionInDegrees;
	uint16_t positionInPulseWidth;
}servoDriverStruct;

void servoDriverInit(servoDriverStruct *servoDriver, uint8_t dMin, uint8_t dMax, uint16_t pMin, uint16_t pMax);
void servoDriverStart(servoDriverStruct *servoDriver);
void servoDriverStop(servoDriverStruct *servoDriver);

void servoDriverStartTimer(servoDriverStruct *servoDriver);
void servoDriverStopTimer(servoDriverStruct *servoDriver);

bool servoDriverSetDegrees(servoDriverStruct *servoDriver, uint16_t degrees);
uint16_t servoDriverDegreesToPulseWidth(servoDriverStruct *servoDriver, uint16_t degrees);
void servoDriverSetPulseWidth(servoDriverStruct *servoDriver);

#endif
