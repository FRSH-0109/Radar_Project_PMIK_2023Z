/****************************************************************************
 * Copyright (C) 2012 by Matteo Franchin                                    *
 *                                                                          *
 * This file is part of Box.                                                *
 *                                                                          *
 *   Box is free software: you can redistribute it and/or modify it         *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Box is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file distnace_sensor.h
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 25 Nov 2023
 * @brief File containing code of distance sensor driver (ex. hc-sr04),
 * which was created for Radar embedded project.
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

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

double distanceSensorGetDistance(distanceSensorStruct* sensor);

#endif
