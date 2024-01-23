/**
 * @file usart_custom.h
 *
 * @brief Header file for custom uart handling, consists of all used
 * variables, enums or functions that are connected directly with uart communication.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */

#ifndef INC_USART_CUSTOM_H_
#define INC_USART_CUSTOM_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UART_RX_DATA_LEN		100		/**< Defines maximum length of single received uart message */
#define MAX_UART_TX_DATA_LEN		30		/**< Defines maximum length of single transmitted uart message */

#define UART_QUEUE_LEN				3		/**< Defines maximum length of uart queue (both tx and rx)*/


#define UART_TX_TRANSMISSION_CMPLT_TIMEOUT 3	/**< Defines timeout after next tx message is allowed to be sent*/

#define COMMAND_START							"START"						/**< String for Radar START command*/
#define COMMAND_STOP							"STOP"						/**< String for Radar STOP command*/
#define COMMAND_GET_RADAR_POSITION				"RADARGETPOS"				/**< String for Radar position getter command*/
#define COMMAND_SET_RADAR_POSITION				"RADARSETPOS"				/**< String for Radar position setter command*/
#define COMMAND_GET_RADAR_STEP					"RADARGETSTEP"				/**< String for Radar servo single step getter command*/
#define COMMAND_SET_RADAR_STEP					"RADARSETSTEP"				/**< String for Radar servo single step setter command*/
#define COMMAND_GET_RADAR_POS_MIN				"RADARGETPOSMIN"			/**< String for Radar min position getter command*/
#define COMMAND_GET_RADAR_POS_MAX				"RADARGETPOSMAX"			/**< String for Radar max position getter command*/
#define COMMAND_SET_RADAR_POS_MIN				"RADARSETPOSMIN"			/**< String for Radar min position setter command*/
#define COMMAND_SET_RADAR_POS_MAX				"RADARSETPOSMAX"			/**< String for Radar min position setter command*/
#define COMMAND_SET_RADAR_SENSOR_PERIOD			"RADARSETSENSORPERIOD"		/**< String for Radar sensor measure period setter command, in ms*/
#define COMMAND_GET_RADAR_SENSOR_PERIOD			"RADARGETSENSORPERIOD"		/**< String for Radar sensor measure period getter command, in ms*/
#define COMMAND_GET_RADAR_SENSOR_MEASURE		"RADARGETSENSORMEASURE"		/**< String for Radar sensor measure getter command*/
#define COMMAND_GET_RADAR_DRAW_SCALE			"RADARGETDRAWSCALE"			/**< String for Radar drawing scale getter command*/
#define COMMAND_SET_RADAR_DRAW_SCALE			"RADARSETDRAWSCALE"			/**< String for Radar drawing scale setter command*/
#define COMMAND_GET_RADAR_DRAW_RULES			"RADARGETDRAWRULES"			/**< String for Radar drawing rules (in centimeters) getter command*/
#define COMMAND_SET_RADAR_DRAW_RULES			"RADARSETDRAWRULES"			/**< String for Radar drawing rules (in centimeters) setter command*/

/**
 * Enumerate for received commands
 */
typedef enum
{
	COMMAND_TYPE_IDLE,
	COMMAND_TYPE_START,
	COMMAND_TYPE_STOP,
	COMMAND_TYPE_SET_POS,
	COMMAND_TYPE_GET_POS,
	COMMAND_TYPE_SET_STEP,
	COMMAND_TYPE_GET_STEP,
	COMMAND_TYPE_GET_POS_MIN,
	COMMAND_TYPE_GET_POS_MAX,
	COMMAND_TYPE_SET_POS_MIN,
	COMMAND_TYPE_SET_POS_MAX,
	COMMAND_TYPE_GET_SENSOR_PERIOD,
	COMMAND_TYPE_SET_SENSOR_PERIOD,
	COMMAND_TYPE_GET_SENSOR_MEASURE,
	COMMAND_TYPE_GET_DRAW_SCALE,
	COMMAND_TYPE_SET_DRAW_SCALE,
	COMMAND_TYPE_GET_DRAW_RULES,
	COMMAND_TYPE_SET_DRAW_RULES,
}CommandType;

/**
 * Enumerate for responses on received commands
 */
typedef enum
{
	RESPONSE_OK,
	RESPONSE_UNKNOW_CMD,
	RESPONSE_ERROR,
}ResponseType;

/**
 * Function for checking if any of uart commands is received
 *
 * @param data pointer to uin8_t type array
 * @param dataLen length of data to decoded
 * @return CommandType which was(or not) decoded
 */
CommandType CommandDecode(uint8_t *data, uint16_t dataLen);

/**
 * Structure for custom uart handle
 */
typedef struct UART_Custom_HandleTypeDef
{
	UART_HandleTypeDef *huart;						/**< Pointer to HAL uart handle*/
	uint16_t dataRxMaxLen;							/**< Maximum rx uart buffer length*/
	uint16_t dataTxMaxLen;							/**< Maximum tx uart buffer length*/
	volatile uint8_t dataRx[MAX_UART_RX_DATA_LEN];	/**< Array of rx uart data used store recent received data before passing to queue*/
	uint8_t dataTx[MAX_UART_TX_DATA_LEN];			/**< Array of tx uart data used to store data before passing to queue*/
	volatile bool txTransmissionCmplt;				/**< Flag for indicate if data was already send*/
	volatile uint32_t txTransmissionCmpltTime;		/**< Value for measuring time out of last send data frame*/
} UART_Custom_HandleTypeDef;

/**
 * Structure for uart queue
 */
typedef struct UART_Queue
{
	volatile uint8_t dataRx[UART_QUEUE_LEN][MAX_UART_RX_DATA_LEN];		/**< Array of data buffers of rx uart data queue*/
	volatile uint16_t dataRxLength[UART_QUEUE_LEN];						/**< Array of data lengths of rx uart data frames*/
	volatile uint8_t headIxndex;										/**< Index of queue position to write next data frame*/
	volatile uint8_t tailIndex;											/**< Index of queue position to read next data frame*/

	uint8_t dataTx[UART_QUEUE_LEN][MAX_UART_TX_DATA_LEN];				/**< Array of data buffers of tx uart data queue*/
	uint16_t dataTxLength[UART_QUEUE_LEN];								/**< Array of data lengths of tx uart data frames*/
	volatile uint8_t dataIndex;											/**< Index of queue position to write next data frame*/
	volatile uint8_t transmitIndex;										/**< Index of queue position to transmit next data frame*/
} UART_Queue;

/**
 * Function for init uart queue structure
 *
 * @param uartQueue pointer to UART_Queue struct instance
 */
void uartQueueInit(volatile UART_Queue *uartQueue);

/**
 * Function for adding next data to be send via uart queue
 *
 * @param uartQueue pointer to UART_Queue struct instance
 * @param dataTxSource pointer to data array to be added to queue
 * @param msgLen value for added data buffer length
 */
void uartQueueAdd(volatile UART_Queue *uartQueue, uint8_t *dataTxSource, uint16_t msgLen);

/**
 * Function for clearing whole uart structure
 *
 * @param uartQueue pointer to UART_Queue struct instance
 */
void uartQueueClear(volatile UART_Queue *uartQueue);

/**
 * Function for transmitting next data in queue
 *
 * @param uartQueue pointer to UART_Queue struct instance
 * @param huartCustom pointer to UART_Custom_HandleTypeDef struct instance
 */
void uartQueueTransmit(volatile UART_Queue *uartQueue, UART_Custom_HandleTypeDef *huartCustom);

/**
 * Function for receiving next waiting data in queue
 *
 * @param uartQueue pointer to UART_Queue struct instance
 */
void uartQueueReceive(volatile UART_Queue *uartQueue);

/**
 * Function for receiving next waiting data in queue
 *
 * @param huartCustom pointer to UART_Custom_HandleTypeDef struct instance
 * param huart pointer to UART_HandleTypeDef struct instance
 * param rxDataLen value for maximum rx data length
 * param txDataLen value for maximum tx data length
 */
void uartCustomInit(UART_Custom_HandleTypeDef *huartCustom, UART_HandleTypeDef *huart, uint16_t rxDataLen, uint16_t txDataLen);

/**
 * Function for calling receiving process via uart with DMA
 *
 * @param huartCustom pointer to UART_Custom_HandleTypeDef struct instance
 */
HAL_StatusTypeDef uartCustomReceiveDMA(UART_Custom_HandleTypeDef *huartCustom);

/**
 * Function for calling DMA transmit via uart
 *
 * @param huartCustom pointer to UART_Custom_HandleTypeDef struct instance
 * @param msgLen value for maximum tx data length
 */
HAL_StatusTypeDef uartCustomTransmitDMA(UART_Custom_HandleTypeDef *huartCustom, uint16_t msgLen);

#endif /* INC_USART_CUSTOM_H_ */
