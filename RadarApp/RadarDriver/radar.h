#ifndef RADAR_H
#define RADAR_H

#define SENSOR_MEASURE_PERIOD_DEFAULT_MS 100

#include "main.h"
#include "stdio.h"
#include "servo_driver.h"
#include "distance_sensor.h"
#include "usart_custom.h"

typedef struct {

	servoDriverStruct* servo;
	float position;					// in degrees
	TIM_HandleTypeDef* servoTim;
	uint16_t positionUpdatePeriodMs;
	float positionUpdateStep;
	float positionMin;				// in degrees
	float positionMax;				// in degrees
	int8_t direction;

	distanceSensorStruct* sensor;
	TIM_HandleTypeDef* sensorTim;
	uint16_t measurePeriodMs;

}radarStruct;

void radarInit(radarStruct* radar, servoDriverStruct* servo, TIM_HandleTypeDef* servo_tim, distanceSensorStruct* sensor, TIM_HandleTypeDef* sensor_tim);

////////////// SERVO /////////////////////////////////
void radarServoUpdate(radarStruct* radar);
bool radarSetPositionMin(radarStruct* radar, float pos);
float radarGetPositionMin(radarStruct* radar);
bool radarSetPositionMax(radarStruct* radar, float pos);
float radarGetPositionMax(radarStruct* radar);

void radarServoStart(radarStruct* radar);
void radarServoStop(radarStruct* radar);

bool radarSetPositionUpdatePeriod(radarStruct* radar, uint16_t periodMs);
bool radarSetPositionUpdateStep(radarStruct* radar, float step);
bool radarSetPosition(radarStruct* radar, float pos);
float radarGetPosition(radarStruct* radar);
////////////// SERVO /////////////////////////////////

////////////// SENSOR //////////////////////////////////
void radarMeasureStart(radarStruct* radar);
void radarMeasureStop(radarStruct* radar);

bool radarSetMeasureFreq(radarStruct* radar, uint16_t periodMs);

void radarTriggerMeasure(radarStruct* radar);
double radarGetMeasure(radarStruct* radar);
////////////// SENSOR //////////////////////////////////

//DATA PARSING//
bool radarParseSetPosition(radarStruct* radar, uint8_t* data);
bool radarParseSetStep(radarStruct* radar, uint8_t* data);

#endif
