#include "radar.h"

void radarInit(radarStruct* radar, servoDriverStruct* servo, TIM_HandleTypeDef* servo_tim, distanceSensorStruct* sensor, TIM_HandleTypeDef* sensor_tim)
{
	//TODO: assert arguments
	radar->servo = servo;
	radar->position = 0.0f;
	radar->servoTim = servo_tim;
	radar->direction = 1;
	radarSetPositionMin(radar, 0.0f);
	radarSetPositionMax(radar, 180.0f);
	radarSetPositionUpdateStep(radar, 0.5f);
	radarSetPositionUpdatePeriod(radar, 11);

	radar->sensor = sensor;
	radar->sensorTim = sensor_tim;
	radarSetMeasureFreq(radar, SENSOR_MEASURE_PERIOD_DEFAULT_MS);
}


////////////// SERVO //////////////////////////////////
void radarServoUpdate(radarStruct* radar)
{
	float posNew = 0;
	posNew = (radar->direction * radar->positionUpdateStep) + radar->position;

	if(posNew < radar->positionMin)
	{
		posNew = radar->positionMin;
		radar->direction *= -1;
	}
	else if(posNew > radar->positionMax)
	{
		posNew = radar->positionMax;
		radar->direction *= -1;
	}

	radarSetPosition(radar, posNew);
}

bool radarSetPositionMin(radarStruct* radar, float pos)
{
	if(pos >= 0.0 && pos <= 180.0 && pos < radar->positionMax)
	{
		radar->positionMin = pos;
		return true;
	}

	return false;
}

float radarGetPositionMin(radarStruct* radar)
{
	return radar->positionMin;
}

bool radarSetPositionMax(radarStruct* radar, float pos)
{
	if(pos >= 0.0 && pos <= 180.0 && pos > radar->positionMin)
	{
		radar->positionMax = pos;
		return true;
	}

	return false;
}

float radarGetPositionMax(radarStruct* radar)
{
	return radar->positionMax;
}

void radarServoStart(radarStruct* radar)
{
	servoDriverStartTimer(radar->servo);
	HAL_TIM_Base_Start_IT(radar->servoTim);
}

void radarServoStop(radarStruct* radar)
{
	HAL_TIM_Base_Stop_IT(radar->servoTim);
}

bool radarSetPositionUpdatePeriod(radarStruct* radar, uint16_t periodMs)
{
	if(periodMs > 10 && periodMs < UINT16_MAX)
	{
		radar->positionUpdatePeriodMs = periodMs;
		__HAL_TIM_SET_AUTORELOAD(radar->servoTim, periodMs);
		return true;
	}

	return false;
}

bool radarSetPositionUpdateStep(radarStruct* radar, float step)
{
	if(step > 0.001 && step < 90)
	{
		radar->positionUpdateStep = step;
		return true;
	}

	return false;
}

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
	if(periodMs > 10 && periodMs < UINT16_MAX)
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


//PARSING//
bool radarParseSetPosition(radarStruct* radar, uint8_t* data)
{
	float newPos = 0;
	char* dataNew = strnstr((char*) data, COMMAND_SET_RADAR_POSITION, strlen((char*)data));

	if(dataNew == NULL)
	{
		return false;
	}

	dataNew += strlen((char*) COMMAND_SET_RADAR_POSITION);

	if (sscanf(dataNew, "%f", &newPos) != 1)
	{
		return false;
	}

	return radarSetPosition(radar, newPos);
}

bool radarParseSetStep(radarStruct* radar, uint8_t* data)
{
	float newStep = 0;
	char* dataNew = strnstr((char*) data, COMMAND_SET_RADAR_STEP, strlen((char*)data));

	if(dataNew == NULL)
	{
		return false;
	}

	dataNew += strlen((char*) COMMAND_SET_RADAR_STEP);

	if (sscanf(dataNew, "%f", &newStep) != 1)
	{
		return false;
	}

	return radarSetPositionUpdateStep(radar, newStep);
}
