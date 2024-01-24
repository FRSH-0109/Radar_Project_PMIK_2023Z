/**
 * @file distance_sensor.h
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 20 Nov 2023
 * @brief File containing code of ultrasonic distance sensor,
 * implementation, modifications and making measurement
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#ifndef DISTANCE_DRIVER_H
#define DISTANCE_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Structure for control certain sensor
 */
typedef struct
{
	double factor;			/**< Factor for calculation ticks amount to distance*/

	uint16_t ticks;			/**< Signal length in ticks from distance sensor measurement*/
	double distnace;		/**< Calculated distance from sensor*/

	bool readyToMeasure;	/**< Flag for controlling sensor behavior wile measurements*/
}distanceSensorStruct;

/**
 * Function used to once init sensor structure
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 * @param ticksToDistanceFactor used to set factor param in sensor's structure
 */
void distanceSensorInit(distanceSensorStruct* sensor, double ticksToDistanceFactor);

/**
 * Function used to turn on trigger signal to the sensor
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 */
void distanceSensorTrigOn(distanceSensorStruct *sensor);

/**
 * Function used to implement custom time delay of 1-2ms
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 * @param delay time in ms to wait
 */
void distanceSensorDelay(distanceSensorStruct *sensor, uint32_t delay);

/**
 * Function used to turn off trigger signal to the sensor
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 */
void distanceSensorTrigOff(distanceSensorStruct *sensor);

/**
 * Function used to send trigger signal to the sensor
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 */

void distanceSensorSendTrig(distanceSensorStruct* sensor);

/**
 * Function used to receive echo signal from sensor
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 * @param ticks value of how many ticks length echo signal was
 */
void distanceSensorGetEcho(distanceSensorStruct* sensor, uint16_t ticks);

/**
 * Function used to get state of readyToMeasure flag
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 * @return bool value corresponding to readyToMeasureFlag
 */
bool distanceSensorGetReadyToMeasure(distanceSensorStruct* sensor);

/**
 * Function used to get distance from sensor's structure
 *
 * @param sensor pointer to distanceSensorStruct instance of sensor to be initialized
 * @return double value of distance contained in sensor's struct
 */
double distanceSensorGetDistance(distanceSensorStruct* sensor);

#endif
