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
 * @file servo_driver.h
 * @author Kami Kośnik, Kacper Radzikowski
 * @date 20 Nov 2023
 * @brief File containing code of servomechanism driver controlled by PWM,
 * which was created for Radar embedded project.
 *
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define SERVO_MIN_DEGREES 0.0f
#define SERVO_MAX_DEGREES 180.0f

#define SERVO_MIN_PULSE_WIDTH 1000	//1ms = 0 degrees
#define SERVO_MAX_PULSE_WIDTH 2000	//2ms = 180 degrees

typedef struct
{
	float degreesMin;
	float degreesMax;

	uint16_t pulseWidthMin;
	uint16_t pulseWidthMax;

	float positionInDegrees;
	uint16_t positionInPulseWidth;
}servoDriverStruct;

void servoDriverInit(servoDriverStruct *servoDriver, float dMin, float dMax, uint16_t pMin, uint16_t pMax);
void servoDriverStart(servoDriverStruct *servoDriver);
void servoDriverStop(servoDriverStruct *servoDriver);

void servoDriverStartTimer(servoDriverStruct *servoDriver);
void servoDriverStopTimer(servoDriverStruct *servoDriver);

bool servoDriverSetDegrees(servoDriverStruct *servoDriver, float degrees);
uint16_t servoDriverDegreesToPulseWidth(servoDriverStruct *servoDriver, float degrees);
void servoDriverSetPulseWidth(servoDriverStruct *servoDriver);

#endif
