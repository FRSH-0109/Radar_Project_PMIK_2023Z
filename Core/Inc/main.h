/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define DEGREES_TO_RADIANS M_PI / 180.0;
#define DRAW_SCALE_DEFAULT 2
#define MEASURE_SCALES_NUMBER_DEFAULT 3

typedef struct {
	double drawScale;
	uint8_t measureScalesNumber;
	uint16_t* measureScaleY;
	uint16_t* measureScaleCm;
	int16_t xp, yp;
}drawHelperStruct;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void drawClear();
void drawSetup();
bool drawSetupUpdate(double drawScale, uint8_t measureScalesNumber);
void drawMeasure();
void drawMeasureScale();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SENSOR_TRIG_Pin GPIO_PIN_1
#define SENSOR_TRIG_GPIO_Port GPIOA
#define SENSOR_ECHO_Pin GPIO_PIN_2
#define SENSOR_ECHO_GPIO_Port GPIOA
#define SENSOR_ECHO_EXTI_IRQn EXTI2_3_IRQn
#define SERVO_PWM_Pin GPIO_PIN_6
#define SERVO_PWM_GPIO_Port GPIOA
#define TFT_CS_Pin GPIO_PIN_1
#define TFT_CS_GPIO_Port GPIOD
#define TFT_RESET_Pin GPIO_PIN_2
#define TFT_RESET_GPIO_Port GPIOD
#define TFT_DC_Pin GPIO_PIN_3
#define TFT_DC_GPIO_Port GPIOD
#define TT_LED_Pin GPIO_PIN_6
#define TT_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
