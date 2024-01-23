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

void distanceSensorInit(distanceSensorStruct* sensor, double ticksToDistanceFactor);

void distanceSensorTrigOn(distanceSensorStruct *sensor);
void distanceSensorDelay(distanceSensorStruct *sensor, uint32_t delay);
void distanceSensorTrigOff(distanceSensorStruct *sensor);

void distanceSensorSendTrig(distanceSensorStruct* sensor);
void distanceSensorGetEcho(distanceSensorStruct* sensor, uint16_t ticks);
bool distanceSensorGetReadyToMeasure(distanceSensorStruct* sensor);

double distanceSensorGetDistance(distanceSensorStruct* sensor);

#endif
