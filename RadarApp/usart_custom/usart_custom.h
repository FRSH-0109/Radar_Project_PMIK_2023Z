/*H**********************************************************************
 * FILENAME :        usart_custom.h
 *
 * DESCRIPTION :
 *       Handling uarts transmit/receive operations by DMA.
 *
 * PUBLIC FUNCTIONS :
 *       void     UartCustomInit(UART_Custom_HandleTypeDef *,  UART_HandleTypeDef *)
 *       void     void uartCustomReceiveDMA(UART_Custom_HandleTypeDef *huartCustom)
 *       void     uartCustomTransmitDMA(UART_Custom_HandleTypeDef *huartCustom, uint8_t msgLen)
 *
 * AUTHOR :    Kamil Kośnik        START DATE :    18 July 2022
 *
 *H*/

#ifndef INC_USART_CUSTOM_H_
#define INC_USART_CUSTOM_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UART_RX_DATA_LEN		100
#define MAX_UART_TX_DATA_LEN		30

#define UART_QUEUE_LEN		3


#define UART_TX_TRANSMISSION_CMPLT_TIMEOUT 3

//PARSER
#define COMMAND_START							"START"
#define COMMAND_STOP							"STOP"
#define COMMAND_GET_RADAR_POSITION				"RADARGETPOS"
#define COMMAND_SET_RADAR_POSITION				"RADARSETPOS"
#define COMMAND_GET_RADAR_STEP					"RADARGETSTEP"
#define COMMAND_SET_RADAR_STEP					"RADARSETSTEP"

typedef enum
{
	COMMAND_TYPE_IDLE,
	COMMAND_TYPE_START,
	COMMAND_TYPE_STOP,
	COMMAND_TYPE_SET_POS,
	COMMAND_TYPE_GET_POS,
	COMMAND_TYPE_SET_STEP,
	COMMAND_TYPE_GET_STEP,
}CommandType;

typedef enum
{
	RESPONSE_OK,
	RESPONSE_UNKNOW_CMD,
	RESPONSE_ERROR,
}ResponseType;

CommandType CommandDecode(uint8_t *data, uint16_t dataLen);

// Struct for handling all operations of custom uart
typedef struct UART_Custom_HandleTypeDef
{
	UART_HandleTypeDef *huart;
	uint16_t dataRxMaxLen;
	uint16_t dataTxMaxLen;
	volatile uint8_t dataRx[MAX_UART_RX_DATA_LEN];
	uint8_t dataTx[MAX_UART_TX_DATA_LEN];
	volatile bool txTransmissionCmplt;
	volatile uint32_t txTransmissionCmpltTime;
} UART_Custom_HandleTypeDef;

typedef struct UART_Queue
{
	volatile uint8_t dataRx[UART_QUEUE_LEN][MAX_UART_RX_DATA_LEN];
	volatile uint16_t dataRxLength[UART_QUEUE_LEN];
	volatile uint8_t headIxndex;
	volatile uint8_t tailIndex;

	uint8_t dataTx[UART_QUEUE_LEN][MAX_UART_TX_DATA_LEN];
	uint16_t dataTxLength[UART_QUEUE_LEN];
	volatile uint8_t dataIndex;
	volatile uint8_t transmitIndex;
} UART_Queue;

void uartQueueInit(volatile UART_Queue *uartQueue);
void uartQueueAdd(volatile UART_Queue *uartQueue, uint8_t *dataTxSource, uint16_t msgLen);
void uartQueueClear(volatile UART_Queue *uartQueue);
void uartQueueTransmit(volatile UART_Queue *uartQueue, UART_Custom_HandleTypeDef *huartCustom);
void uartQueueReceive(volatile UART_Queue *uartQueue);

void uartCustomInit(UART_Custom_HandleTypeDef *huartCustom, UART_HandleTypeDef *huart, uint16_t rxDataLen, uint16_t txDataLen);
HAL_StatusTypeDef uartCustomReceiveDMA(UART_Custom_HandleTypeDef *huartCustom);
HAL_StatusTypeDef uartCustomTransmitDMA(UART_Custom_HandleTypeDef *huartCustom, uint16_t msgLen);

#endif /* INC_USART_CUSTOM_H_ */
