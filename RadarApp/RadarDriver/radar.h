#ifndef RADAR_H
#define RADAR_H

#define SENSOR_MEASURE_PERIOD_DEFAULT_MS 100

#include "main.h"
#include "servo_driver.h"
#include "distance_sensor.h"

typedef struct {

	servoDriverStruct* servo;
	float position;					// in degrees

	distanceSensorStruct* sensor;
	TIM_HandleTypeDef* sensorTim;
	uint16_t measurePeriodMs;

}radarStruct;

void radarInit(radarStruct* radar, servoDriverStruct* servo, distanceSensorStruct* sensor, TIM_HandleTypeDef* sensor_tim);

////////////// SERVO /////////////////////////////////
bool radarSetPosition(radarStruct* radar, float pos);
float radarGetPosition(radarStruct* radar);
////////////// SERVO /////////////////////////////////

////////////// SENSOR //////////////////////////////////
bool radarSetMeasureFreq(radarStruct* radar, uint16_t periodMs);

void radarTriggerMeasure(radarStruct* radar);
double radarGetMeasure(radarStruct* radar);
////////////// SENSOR //////////////////////////////////

#endif
