#include "radar.h"

void radarInit(radarStruct* radar, servoDriverStruct* servo, distanceSensorStruct* sensor, TIM_HandleTypeDef* sensor_tim)
{
	radar->servo = servo;
	radar->position = 0.0f;
	servoDriverStartTimer(servo);

	radar->sensor = sensor;
	radar->sensorTim = sensor_tim;
	radarSetMeasureFreq(radar, SENSOR_MEASURE_PERIOD_DEFAULT_MS);
}


////////////// SERVO //////////////////////////////////
bool radarSetPosition(radarStruct* radar, float pos)
{
	if(servoDriverSetDegrees(radar->servo, pos))
	{
		radar->position = pos;
		return true;
	}
	return false;
}

float radarGetPosition(radarStruct* radar)
{
	return radar->position;
}
////////////// SERVO //////////////////////////////////



////////////// SENSOR //////////////////////////////////
void radarMeasureStart(radarStruct* radar)
{
	HAL_TIM_Base_Start_IT(radar->sensorTim);
}

void radarMeasureStop(radarStruct* radar)
{
	HAL_TIM_Base_Stop_IT(radar->sensorTim);
}

bool radarSetMeasureFreq(radarStruct* radar, uint16_t periodMs)
{
	if(periodMs > 0 && periodMs < 1000)
	{
		radar->measurePeriodMs = periodMs;
		__HAL_TIM_SET_AUTORELOAD(radar->sensorTim, periodMs);
		return true;
	}

	return false;
}

inline void radarTriggerMeasure(radarStruct* radar)
{
	distanceSensorSendTrig(radar->sensor);
}

double radarGetMeasure(radarStruct* radar)
{
	return distanceSensorGetDistance(radar->sensor);
}


////////////// SENSOR //////////////////////////////////

////////////// DISPLAY //////////////////////////////////
void radarDrawMeasure(radarStruct* radar)
{

}
////////////// DISPLAY //////////////////////////////////

