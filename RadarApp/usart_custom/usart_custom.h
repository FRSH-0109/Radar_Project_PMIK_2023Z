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
#define MAX_UART_TX_DATA_LEN		100

#define UART_QUEUE_LEN		3


#define UART_TX_TRANSMISSION_CMPLT_TIMEOUT 3

typedef enum
{
    PacketType_Request = '?', 		// ?
    PacketType_Response = '$', 		// $
    PacketType_Event = '#'	, 		// #
	PacketType_Notification = '&',
} PacketTypeDef;

typedef enum
{
	RequestID_RESET = 'R', 				// reset inidicator
    RequestID_PING = '0', 				// no arg, ESP: ResponseID_ACK
	RequestID_FEN = 'F',				// no args, ESP request FEN from STM
	RequestID_SOFTWARE_VERSION = 'V',	// no args, STM Response: ResponseID_SOFTWARE_VERSION

	//frames created for STM<->STM IR communication
	RequestID_UID_HASH = 'h',			// followed by slave fen uid hash let master check it
} RequestTypeDef;						// ?

typedef enum
{
    ResponseID_ACK = '0', 				// Ack
	ResponseID_NO_ACK = '1', 			// NoAck
	ResponseID_ACK_FROM_IR = '9',		// ACK received from other half of board
	ResponseID_FEN = 'F',				// STM sends FEN from ACTUAL REAL POSITON(not expected)
	ResponseID_SOFTWARE_VERSION = 'V',	// Used to indicate ESP software version in STM
} ResponseTypeDef; 						// $

typedef enum
{
    ResponseErrorCode_NoError = '0', 			// NoError
	ResponseErrorCode_CRCError = '1', 			// Checksum Error
	ResponseErrorCode_Busy = '2', 				// STM Busy
	ResponseUnhandled_Command_char1 = '2',		// Unhandled_Command
	ResponseUnhandled_Command_char2 = '3',		// Unhandled_Command
	ResponseToo_Short_Command_char1 = '2',		// Too short command (luck of \r\n chars)
	ResponseToo_Short_Command_char2 = '1',		// Too short command (luck of \r\n chars)
} ResponseErrorCodeTypeDef;


// Struct for handling all operations of custom uart
typedef struct UART_Custom_HandleTypeDef
{
	UART_HandleTypeDef *huart;
	uint16_t dataRxMaxLen;
	uint16_t dataTxMaxLen;
	volatile uint8_t dataRx[MAX_UART_RX_DATA_LEN];
	uint8_t dataTx[MAX_UART_RX_DATA_LEN];
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
