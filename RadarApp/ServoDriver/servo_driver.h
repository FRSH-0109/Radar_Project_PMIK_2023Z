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

#define SERVO_MIN_DEGREES 0.0f		/**< Minimum hardware servo position in degrees*/
#define SERVO_MAX_DEGREES 180.0f	/**< Maximum hardware servo position in degrees*/

/**
 * Structure for control servomechanisms with pwm singal,
 */
typedef struct
{
	float degreesMin;				/**< Minimum degrees value for movement*/
	float degreesMax;				/**< Maximum degrees value for movement*/

	uint16_t pulseWidthMin;			/**< Minimum pwm pulse width for movement*/
	uint16_t pulseWidthMax;			/**< Maximum pwm pulse width for movement*/

	float positionInDegrees;		/**< Actual position in degrees*/
	uint16_t positionInPulseWidth;	/**< Actual position in pulse width*/
}servoDriverStruct;


/**
 * Function for initialization of servo structure instance
 *
 * @param servoDriver pointer to servoDriverStruct struct
 * @param dMin value of minimal degrees movement for servo
 * @param dMax value of maximal degrees movement for servo
 * @param pMin value of minimal pulse width corresponding to minimal degrees value
 * @param pMax value of maximal pulse width corresponding to minimal degrees value
 */
void servoDriverInit(servoDriverStruct *servoDriver, float dMin, float dMax, uint16_t pMin, uint16_t pMax);

/**
 * Function for starting servo movement
 *
 * @param servoDriver pointer to servoDriverStruct struct
 */
void servoDriverStart(servoDriverStruct *servoDriver);

/**
 * Function for stopping servo movement
 *
 * @param servoDriver pointer to servoDriverStruct struct
 */
void servoDriverStop(servoDriverStruct *servoDriver);

/**
 * Function for starting timer paired with certain servo struct
 *
 * @param servoDriver pointer to servoDriverStruct struct
 */
void servoDriverStartTimer(servoDriverStruct *servoDriver);

/**
 * Function for stopping timer paired with certain servo struct
 *
 * @param servoDriver pointer to servoDriverStruct struct
 */
void servoDriverStopTimer(servoDriverStruct *servoDriver);

/**
 * Function for setting servo position in degrees
 *
 * @param servoDriver pointer to servoDriverStruct struct
 * @param degrees value of degrees to set
 * @return bool is action was successful
 */
bool servoDriverSetDegrees(servoDriverStruct *servoDriver, float degrees);

/**
 * Function for converting degrees value into pulse width
 *
 * @param servoDriver pointer to servoDriverStruct struct
 * @param degrees value of degrees to convert
 * @return uint16_t value of corresponding pulse width
 */
uint16_t servoDriverDegreesToPulseWidth(servoDriverStruct *servoDriver, float degrees);

/**
 * Function for setting pulse width from struct into timer instance
 *
 * @param servoDriver pointer to servoDriverStruct struct
 */
void servoDriverSetPulseWidth(servoDriverStruct *servoDriver);

#endif
