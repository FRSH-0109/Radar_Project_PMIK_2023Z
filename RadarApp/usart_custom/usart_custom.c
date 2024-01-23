/**
 * @file usart_custom.c
 *
 * @brief Source file for custom uart handling, consists of all used
 * variables, enums or functions that are connected directly with uart communication.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 * @see https://github.com/FRSH-0109/Radar_Project_PMIK_2023Z
 */
#include <stdio.h>
#include "usart.h"
#include <stm32g0xx_hal_dma.h>
#include "usart_custom.h"
#include "radar.h"

extern UART_Custom_HandleTypeDef huartPC;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_Queue uartQueuePC;
extern radarStruct radar;
extern drawHelperStruct drawHelper;

void uartCustomInit(UART_Custom_HandleTypeDef *huartCustom, UART_HandleTypeDef *huart, uint16_t rxDataLen, uint16_t txDataLen)
{
	huartCustom->huart = huart;
	huartCustom->txTransmissionCmplt = true;
	huartCustom->txTransmissionCmpltTime = 0;
	huartCustom->dataRxMaxLen = rxDataLen;
	huartCustom->dataTxMaxLen = txDataLen;
}

inline HAL_StatusTypeDef uartCustomReceiveDMA(UART_Custom_HandleTypeDef *huartCustom)
{
	__HAL_UART_SEND_REQ(huartCustom->huart, UART_RXDATA_FLUSH_REQUEST);
	return HAL_UARTEx_ReceiveToIdle_DMA(huartCustom->huart, (uint8_t*) huartCustom->dataRx, huartCustom->dataRxMaxLen);
}

inline HAL_StatusTypeDef uartCustomTransmitDMA(UART_Custom_HandleTypeDef *huartCustom, uint16_t msgLen)
{
	if(huartCustom == &huartPC)
	{
		uartQueueAdd(&uartQueuePC, huartCustom->dataTx, msgLen);
		return HAL_OK;
	}
	return HAL_BUSY;
}

static bool CommandSearchForString(uint8_t *data, uint16_t dataLen, const char* str)
{
	if(dataLen >= strlen(str))
	{
		if(strnstr((const char*)data, str, strlen(str)) != NULL)
		{
			return true;
		}
	}
	return false;
}

static void removeChar(char *str, char c, uint16_t dataLen)
{
    int i, j;
    for (i = j = 0; i < dataLen; i++)
    {
        if(str[i] != c)
        {
            str[j++] = str[i];
        }
    }
    str[j] = 0;
}

CommandType CommandDecode(uint8_t *data, uint16_t dataLen)
{
	removeChar((char *)(data), ' ', dataLen);	//remove spaces from message
	strupr((char *)(data));						//make all chars upper case

	if(CommandSearchForString(data, dataLen, COMMAND_START))
	{
		return COMMAND_TYPE_START;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_STOP))
	{
		return COMMAND_TYPE_STOP;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_POS_MIN))
	{
		return COMMAND_TYPE_GET_POS_MIN;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_POS_MAX))
	{
		return COMMAND_TYPE_GET_POS_MAX;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_POS_MIN))
	{
		return COMMAND_TYPE_SET_POS_MIN;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_POS_MAX))
	{
		return COMMAND_TYPE_SET_POS_MAX;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_POSITION))
	{
		return COMMAND_TYPE_GET_POS;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_POSITION))
	{
		return COMMAND_TYPE_SET_POS;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_STEP))
	{
		return COMMAND_TYPE_GET_STEP;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_STEP))
	{
		return COMMAND_TYPE_SET_STEP;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_SENSOR_PERIOD))
	{
		return COMMAND_TYPE_SET_SENSOR_PERIOD;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_SENSOR_PERIOD))
	{
		return COMMAND_TYPE_GET_SENSOR_PERIOD;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_SENSOR_MEASURE))
	{
		return COMMAND_TYPE_GET_SENSOR_MEASURE;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_DRAW_SCALE))
	{
		return COMMAND_TYPE_GET_DRAW_SCALE;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_DRAW_SCALE))
	{
		return COMMAND_TYPE_SET_DRAW_SCALE;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_GET_RADAR_DRAW_RULES))
	{
		return COMMAND_TYPE_GET_DRAW_RULES;
	}
	else if(CommandSearchForString(data, dataLen, COMMAND_SET_RADAR_DRAW_RULES))
	{
		return COMMAND_TYPE_SET_DRAW_RULES;
	}


	return COMMAND_TYPE_IDLE;
}

static void uartTransmitResponse(volatile UART_Queue *uartQueue, ResponseType respType)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	switch(respType)
	{
		case RESPONSE_OK:
			memcpy(data, "OK\r\n", strlen("OK\r\n"));
			break;
		case RESPONSE_UNKNOW_CMD:
			memcpy(data,"UNKNOW CMD\r\n", strlen("UNKNOW CMD\r\n"));
			break;
		case RESPONSE_ERROR:
			memcpy(data, "ERROR\r\n", strlen("ERROR\r\n"));
			break;
		default:
			return;
			break;
	}

	dataLen = strlen((char* )data);
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitPosition(volatile UART_Queue *uartQueue)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	dataLen = sprintf((char* )data, "Position: %.2f\r\n", radar.position);
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitStep(volatile UART_Queue *uartQueue)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	dataLen = sprintf((char* )data, "Step: %.2f\r\n", radar.positionUpdateStep);
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitPositionMinMax(volatile UART_Queue *uartQueue, bool isMin)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	if(isMin)
	{
		dataLen = sprintf((char* )data, "Position Min: %.2f\r\n", radar.positionMin);
	}
	else
	{
		dataLen = sprintf((char* )data, "Position Max: %.2f\r\n", radar.positionMax);
	}
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitSensorMeasure(volatile UART_Queue *uartQueue)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	dataLen = sprintf((char* )data, "Measure: %.2f\r\n", radarGetMeasure(&radar));
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitSensorPeriod(volatile UART_Queue *uartQueue)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	dataLen = sprintf((char* )data, "Measure Period: %d ms\r\n", radar.measurePeriodMs);
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitDrawScale(volatile UART_Queue *uartQueue)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	dataLen = sprintf((char* )data, "Draw Scale: %.2f\r\n", drawHelper.drawScale);
	uartQueueAdd(uartQueue, data, dataLen);
}

static void uartTransmitDrawRules(volatile UART_Queue *uartQueue)
{
	uint8_t data[MAX_UART_TX_DATA_LEN] = {0};
	uint8_t dataLen = 0;

	dataLen = sprintf((char* )data, "Draw rules number: %d\r\n", drawHelper.measureScalesNumber);
	uartQueueAdd(uartQueue, data, dataLen);
}

inline void uartQueueReceive(volatile UART_Queue *uartQueue)
{
	if(uartQueue->tailIndex != uartQueue->headIxndex && uartQueue->dataRx[uartQueue->tailIndex][0] != 0)
	{
		CommandType commandType = CommandDecode((uint8_t *)uartQueue->dataRx[uartQueue->tailIndex], uartQueue->dataRxLength[uartQueue->tailIndex]);
		switch(commandType)
		{
			case COMMAND_TYPE_IDLE:
				uartTransmitResponse(uartQueue, RESPONSE_UNKNOW_CMD);
				break;
			case COMMAND_TYPE_START:
				radarMeasureStart(&radar);
				radarServoStart(&radar);
				uartTransmitResponse(uartQueue, RESPONSE_OK);
				break;
			case COMMAND_TYPE_STOP:
				radarMeasureStop(&radar);
				radarServoStop(&radar);
				uartTransmitResponse(uartQueue, RESPONSE_OK);
				break;
			case COMMAND_TYPE_GET_POS:
				uartTransmitPosition(uartQueue);
				break;
			case COMMAND_TYPE_SET_POS:
				if(radarParseSetPosition(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex]))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
				break;

			case COMMAND_TYPE_GET_STEP:
				uartTransmitStep(uartQueue);
				break;
			case COMMAND_TYPE_SET_STEP:
				if(radarParseSetStep(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex]))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
				break;
			case COMMAND_TYPE_GET_POS_MIN:
				uartTransmitPositionMinMax(uartQueue, true);
			break;

			case COMMAND_TYPE_GET_POS_MAX:
				uartTransmitPositionMinMax(uartQueue, false);
			break;

			case COMMAND_TYPE_SET_POS_MIN:
				if(radarParseSetPositionMinMax(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex], true))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
			break;

			case COMMAND_TYPE_SET_POS_MAX:
				if(radarParseSetPositionMinMax(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex], false))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
			break;

			case COMMAND_TYPE_GET_SENSOR_MEASURE:
				uartTransmitSensorMeasure(uartQueue);
			break;

			case COMMAND_TYPE_SET_SENSOR_PERIOD:
				if(radarParseSetMeasurePeriod(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex]))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
			break;

			case COMMAND_TYPE_GET_SENSOR_PERIOD:
				uartTransmitSensorPeriod(uartQueue);
			break;

			case COMMAND_TYPE_GET_DRAW_SCALE:
				uartTransmitDrawScale(uartQueue);
			break;

			case COMMAND_TYPE_GET_DRAW_RULES:
				uartTransmitDrawRules(uartQueue);
			break;

			case COMMAND_TYPE_SET_DRAW_SCALE:
				if(radarParseSetDrawScale(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex]))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
			break;

			case COMMAND_TYPE_SET_DRAW_RULES:
				if(radarParseSetDrawRules(&radar, (uint8_t*)uartQueue->dataRx[uartQueue->tailIndex]))
				{
					uartTransmitResponse(uartQueue, RESPONSE_OK);
				}
				else
				{
					uartTransmitResponse(uartQueue, RESPONSE_ERROR);
				}
			break;

			default:
				uartTransmitResponse(uartQueue, RESPONSE_UNKNOW_CMD);
			break;
		}

		uartQueue->dataRx[uartQueue->tailIndex][0] = 0;
		uartQueue->dataRxLength[uartQueue->tailIndex] = 0;

		++uartQueue->tailIndex;
		if(uartQueue->tailIndex == UART_QUEUE_LEN)
		{
			uartQueue->tailIndex = 0;
		}
	}
}

void uartQueueInit(volatile UART_Queue *uartQueue)
{
	uartQueue->dataIndex = 0;
	uartQueue->transmitIndex = 0;

	uartQueue->headIxndex = 0;
	uartQueue->tailIndex = 0;
}

inline void uartQueueAdd(volatile UART_Queue *uartQueue, uint8_t *dataTxSource, uint16_t msgLen)
{
	memcpy((uint8_t *)uartQueue->dataTx[uartQueue->dataIndex], dataTxSource, msgLen);
	uartQueue->dataTxLength[uartQueue->dataIndex] = msgLen;
	uartQueue->dataIndex = (++uartQueue->dataIndex) % UART_QUEUE_LEN;
}

inline void uartQueueClear(volatile UART_Queue *uartQueue)
{
	for(uint8_t i = 0; i < UART_QUEUE_LEN; ++i)
	{
		uartQueue->dataTx[i][0] = 0;
	}

	uartQueue->transmitIndex = 0;
	uartQueue->dataIndex = 0;
}

inline void uartQueueTransmit(volatile UART_Queue *uartQueue, UART_Custom_HandleTypeDef *huartCustom)
{
	if(uartQueue == &uartQueuePC)
	{
		if(huartCustom->txTransmissionCmplt && (HAL_GetTick() - huartCustom->txTransmissionCmpltTime) > UART_TX_TRANSMISSION_CMPLT_TIMEOUT)
		{
			if(uartQueue->dataTx[uartQueue->transmitIndex][0] != 0)
			{
				huartCustom->txTransmissionCmplt = false;
				if(uartQueue->dataTxLength[uartQueue->transmitIndex] > huartCustom->dataTxMaxLen)
				{
					uartQueue->dataTxLength[uartQueue->transmitIndex] = huartCustom->dataTxMaxLen;
				}
				HAL_UART_Transmit_DMA(huartCustom->huart,(uint8_t *) uartQueue->dataTx[uartQueue->transmitIndex], uartQueue->dataTxLength[uartQueue->transmitIndex]);
				__HAL_DMA_DISABLE_IT(&hdma_usart1_tx, DMA_IT_HT);

				uartQueue->dataTx[uartQueue->transmitIndex][0] = 0;
				uartQueue->dataTxLength[uartQueue->transmitIndex] = 0;
				uartQueue->transmitIndex = (++uartQueue->transmitIndex) % UART_QUEUE_LEN;
			}
		}
	}
}

//////////////////////Interrupt callbacks///////////////////////////////

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == huartPC.huart->Instance)
	{
		memcpy((void *)uartQueuePC.dataRx[uartQueuePC.headIxndex], (void *)huartPC.dataRx, Size);
		uartQueuePC.dataRxLength[uartQueuePC.headIxndex] = Size;

		++uartQueuePC.headIxndex;
		if(uartQueuePC.headIxndex == UART_QUEUE_LEN)
		{
			uartQueuePC.headIxndex = 0;
		}

		uartCustomReceiveDMA(&huartPC);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huartPC.huart->Instance)
	{
		huartPC.txTransmissionCmplt = true;
		huartPC.txTransmissionCmpltTime = HAL_GetTick();
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huartPC.huart->Instance)
	{
		__HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
		huart->ErrorCode = 0;
		HAL_DMA_Abort(huart->hdmarx); 			// Disable the DMA channel
		HAL_UART_Receive_DMA(huart, NULL, 0); 	// Flush the UART RX buffer
		HAL_DMA_Init(huart->hdmarx);			// Reconfigure the DMA channel
		uartCustomReceiveDMA(&huartPC);			// Restart the DMA transfer
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}
