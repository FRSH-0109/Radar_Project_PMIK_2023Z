/**
 * @file radar_driver.h
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 20 Nov 2023
 * @brief File used to collapse ale used by radar stuff,
 * sensors, servo's etc in one place.
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#ifndef RADAR_H
#define RADAR_H

#define SENSOR_MEASURE_PERIOD_DEFAULT_MS 100 		/**< Default period time for radar's sensor to measure*/

#include "main.h"
#include "stdio.h"
#include "servo_driver.h"
#include "distance_sensor.h"
#include "usart_custom.h"

/**
 * Structure of radar, containing servo and sensor
 * variables needed to control and measure process
 */
typedef struct {
	servoDriverStruct* servo;		/**< Pointer to servo's structure*/
	float position;					/**< Actual position of servo in degrees*/
	TIM_HandleTypeDef* servoTim;	/**< Pointer to timer structure used to control servo's movement*/
	uint16_t positionUpdatePeriodMs;/**< Period in ms, of updating servo's position*/
	float positionUpdateStep;		/**< Value in degrees, of how much position should be updated*/
	float positionMin;				/**< Minimal position of servo's movement in degrees*/
	float positionMax;				/**< Maximal position of servo's movement in degrees*/
	int8_t direction;				/**< Direction of servo's movement indicator (1 or -1)*/

	distanceSensorStruct* sensor;	/**< Pointer to sensors structure*/
	TIM_HandleTypeDef* sensorTim;	/**< POinter to timer structure used to control sensor's measurements*/
	uint16_t measurePeriodMs;		/**< Period in ms, of how often measurement should be triggered*/
}radarStruct;

/**
 * Function for initialization of radar structure instance
 *
 * @param radar pointer to radarStruct struct
 * @param servo pointer to servoDriverStruct struct
 * @param servo_tim pointer to TIM_HandleTypeDef struct
 * @param sensor pointer to distanceSensorStruct struct
 * @param sensor_tim pointer to sensor_tim struct
 */
void radarInit(radarStruct* radar, servoDriverStruct* servo, TIM_HandleTypeDef* servo_tim, distanceSensorStruct* sensor, TIM_HandleTypeDef* sensor_tim);

////////////// SERVO /////////////////////////////////
/**
 * Function for handling servo position update
 *
 * @param radar pointer to radarStruct struct
 */
void radarServoUpdate(radarStruct* radar);

/**
 * Function for setting minimal position of radar
 *
 * @param radar pointer to radarStruct struct
 * @param pos value of position to set
 * @return bool value inidicating if operation was successful
 */
bool radarSetPositionMin(radarStruct* radar, float pos);

/**
 * Function for getter minimal position of radar
 *
 * @param radar pointer to radarStruct struct
 * @return float value of minimal position
 */
float radarGetPositionMin(radarStruct* radar);

/**
 * Function for setting maximal position of radar
 *
 * @param radar pointer to radarStruct struct
 * @param pos value of position to set
 * @return bool value inidicating if operation was successful
 */
bool radarSetPositionMax(radarStruct* radar, float pos);

/**
 * Function for getter maximal position of radar
 *
 * @param radar pointer to radarStruct struct
 * @return float value of maximal position
 */
float radarGetPositionMax(radarStruct* radar);

/**
 * Function for starting servo movement
 */
void radarServoStart(radarStruct* radar);

/**
 * Function for stopping servo movement
 */
void radarServoStop(radarStruct* radar);

/**
 * Function for setting update period of radar's servo movement
 *
 * @param radar pointer to radarStruct struct
 * @param periodMs new value of period
 * @return bool value inidicating if operation was successful
 */
bool radarSetPositionUpdatePeriod(radarStruct* radar, uint16_t periodMs);

/**
 * Function for setting update step of radar's servo movement
 *
 * @param radar pointer to radarStruct struct
 * @param step to be set in structure
 * @return bool value inidicating if operation was successful
 */
bool radarSetPositionUpdateStep(radarStruct* radar, float step);

/**
 * Function for setting position of radar's servo movement
 *
 * @param radar pointer to radarStruct struct
 * @param pos new position to be set
 * @return bool value inidicating if operation was successful
 */
bool radarSetPosition(radarStruct* radar, float pos);

/**
 * Function for getting position of radar's servo movement
 *
 * @param radar pointer to radarStruct struct
 * @return float position value
 */
float radarGetPosition(radarStruct* radar);
////////////// SERVO /////////////////////////////////

////////////// SENSOR //////////////////////////////////
/**
 * Function for starting sensors measurements
 *
 * @param radar pointer to radarStruct struct
 */
void radarMeasureStart(radarStruct* radar);

/**
 * Function for stopping sensors measurements
 *
 * @param radar pointer to radarStruct struct
 */
void radarMeasureStop(radarStruct* radar);

/**
 * Function for setting measure frequency of radar's sensor
 *
 * @param radar pointer to radarStruct struct
 * @param peridoMs new period value
 * @return bool value inidicating if operation was successful
 */
bool radarSetMeasureFreq(radarStruct* radar, uint16_t periodMs);

/**
 * Function for sending trigger signal to radar's signal
 *
 * @param radar pointer to radarStruct struct
 */
void radarTriggerMeasure(radarStruct* radar);

/**
 * Function for getting recent measurement of radar's sensor
 *
 * @param radar pointer to radarStruct struct
 * @return double value of measurements
 */
double radarGetMeasure(radarStruct* radar);
////////////// SENSOR //////////////////////////////////

////////////// DATA PARSING ////////////////////////////
/**
 * Function for parsing data array into position variable
 *
 * @param radar pointer to radarStruct struct
 * @param data pointer
 * @return bool value if operation was successful
 */
bool radarParseSetPosition(radarStruct* radar, uint8_t* data);

/**
 * Function for parsing data array into servo's step variable
 *
 * @param radar pointer to radarStruct struct
 * @param data pointer
 * @return bool value if operation was successful
 */
bool radarParseSetStep(radarStruct* radar, uint8_t* data);

/**
 * Function for parsing data array into posiiton min/max variable
 *
 * @param radar pointer to radarStruct struct
 * @param data pointer
 * @return bool value if operation was successful
 */
bool radarParseSetPositionMinMax(radarStruct* radar, uint8_t* data, bool isMin);

/**
 * Function for parsing data array into measurement period variable
 *
 * @param radar pointer to radarStruct struct
 * @param data pointer
 * @return bool value if operation was successful
 */
bool radarParseSetMeasurePeriod(radarStruct* radar, uint8_t* data);

/**
 * Function for parsing data array into draw scale variable
 *
 * @param radar pointer to radarStruct struct
 * @param data pointer
 * @return bool value if operation was successful
 */
bool radarParseSetDrawScale(radarStruct* radar, uint8_t* data);

/**
 * Function for parsing data array into draw rules variable
 *
 * @param radar pointer to radarStruct struct
 * @param data pointer
 * @return bool value if operation was successful
 */
bool radarParseSetDrawRules(radarStruct* radar, uint8_t* data);

#endif
