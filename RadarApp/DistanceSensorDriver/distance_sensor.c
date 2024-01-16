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
 * @file distnace_sensor.c
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 25 Nov 2023
 * @brief File containing code of distance sensor driver (ex. hc-sr04),
 * which was created for Radar embedded project.
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#include <distance_sensor.h>

void distanceSensorInit(distanceSensorStruct* sensor, double ticksToDistanceFactor)
{
	if(ticksToDistanceFactor > 0)
	{
		sensor->factor = ticksToDistanceFactor;
	}
	else
	{
		sensor->factor = 1;
	}

	sensor->ticks = 0;
	sensor->distnace = 0;

	sensor->readyToMeasure = true;
}

__attribute__((weak)) void distanceSensorTrigOn(distanceSensorStruct *sensor)
{
	////////////////////////////////////////////////////////////////////////////
}

__attribute__((weak)) void distanceSensorDelay(distanceSensorStruct *sensor, uint32_t delay)
{
	////////////////////////////////////////////////////////////////////////////
}

__attribute__((weak)) void distanceSensorTrigOff(distanceSensorStruct *sensor)
{
	////////////////////////////////////////////////////////////////////////////
}

void distanceSensorSendTrig(distanceSensorStruct* sensor)
{
	if(sensor->readyToMeasure)
	{
		distanceSensorTrigOn(sensor);
		distanceSensorDelay(sensor, 1);		//TODO: change to assembly NOP's
		distanceSensorTrigOff(sensor);

		sensor->readyToMeasure = false;
	}
}

void distanceSensorGetEcho(distanceSensorStruct* sensor, uint16_t ticks)
{
	sensor->ticks = ticks;
	sensor->distnace = (double)((double)sensor->ticks / (double)sensor->factor);

	sensor->readyToMeasure = true;
}


bool distanceSensorGetReadyToMeasure(distanceSensorStruct* sensor)
{
	return sensor->readyToMeasure;
}

double distanceSensorGetDistance(distanceSensorStruct* sensor)
{
	return sensor->distnace;
}
