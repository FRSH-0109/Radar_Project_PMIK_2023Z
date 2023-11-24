/*
 * ILI9341.c
 *
 *  Created on: Apr 6, 2021
 *      Author: kamil
 */

#include "main.h"
#include "GFX_Color.h"
#include "ILI9341.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include <math.h>

SPI_HandleTypeDef *Tft_hspi;

static const uint8_t initcmd[] =
{ 0xEF, 3, 0x03, 0x80, 0x02, 0xCF, 3, 0x00, 0xC1, 0x30, 0xED, 4, 0x64, 0x03, 0x12, 0x81, 0xE8, 3, 0x85, 0x00, 0x78, 0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02, 0xF7, 1, 0x20, 0xEA, 2, 0x00, 0x00,
ILI9341_PWCTR1, 1, 0x23,             // Power control VRH[5:0]
ILI9341_PWCTR2, 1, 0x10,             // Power control SAP[2:0];BT[3:0]
ILI9341_VMCTR1, 2, 0x3e, 0x28,       // VCM control
ILI9341_VMCTR2, 1, 0x86,             // VCM control2
ILI9341_MADCTL, 1, 0x48,             // Memory Access Control
ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
ILI9341_PIXFMT, 1, 0x55,
ILI9341_FRMCTR1, 2, 0x00, 0x18,
ILI9341_DFUNCTR, 3, 0x08, 0x82, 0x27,    // Display Function Control
0xF2, 1, 0x00,                         // 3Gamma Function Disable
ILI9341_GAMMASET, 1, 0x01,             // Gamma curve selected
ILI9341_GMCTRP1, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,    // Set Gamma
0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
ILI9341_GMCTRN1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,    // Set Gamma
0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
ILI9341_SLPOUT, 0x80,                // Exit Sleep
ILI9341_DISPON, 0x80,                // Display on
0x00                                   // End of list
};

void ILI9341_Init(SPI_HandleTypeDef *hspi)
{
	Tft_hspi = hspi;
	uint8_t cmd, x, numArgs;
	const uint8_t *addr = initcmd;

	__HAL_SPI_ENABLE(Tft_hspi);

	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	ILI9341_SendCommand(ILI9341_SWRESET);
	HAL_Delay(150);

	while((cmd = *(addr++)) > 0)
	{
		x = *(addr++);
		numArgs = x & 0x7F;

		ILI9341_SendCommandAndData(cmd, (uint8_t*) addr, numArgs);

		addr += numArgs;

		if(x & 0x80)
		{
			HAL_Delay(150);
		}
	}

	ILI9341_SetRotation(ILI9341_ROTATION);
}

void ILI9341_SetRotation(uint8_t m)
{
	m %= 4;    // can't be higher than 3
	switch(m)
	{
		case 0:
			m = (MADCTL_MX | MADCTL_BGR);
		break;
		case 1:
			m = (MADCTL_MV | MADCTL_BGR);
		break;
		case 2:
			m = (MADCTL_MY | MADCTL_BGR);
		break;
		case 3:
			m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
		break;
	}

	ILI9341_SendCommandAndData(ILI9341_MADCTL, &m, 1);
}

void ILI9341_SendToTFT(uint8_t *Byte, uint32_t Length)
{
	while(Length > 0U)
	{
		/* Wait until TXE flag is set to send data */
		if(__HAL_SPI_GET_FLAG(Tft_hspi, SPI_FLAG_TXE))
		{
			*((__IO uint8_t*) &Tft_hspi->Instance->DR) = *Byte;
			Byte++;
			Length--;
		}
	}

	while(__HAL_SPI_GET_FLAG(Tft_hspi, SPI_FLAG_BSY) != RESET)
	{

	}

	//HAL_SPI_Transmit(Tft_hspi, Byte, Length, ILI9341_SPI_TIMEOUT);
}

void ILI9341_SendCommand(uint8_t Command)
{
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);	//CS to LOW

	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);	//DC to LOW

	ILI9341_SendToTFT(&Command, 1);    //Command and length in bytes

	//HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);	//DC to HIGH

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);		//CS to HIGH
}

void ILI9341_SendCommandAndData(uint8_t Command, uint8_t *Data, uint16_t Length)
{
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);	//CS to LOW
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);	//DC to LOW

	ILI9341_SendToTFT(&Command, 1);    //Command and length in bytes

	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);    //DC to HIGH

	ILI9341_SendToTFT(Data, Length);	//Command and length in bytes

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);		//CS to HIGH

}

void ILI9341_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
	uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
	uint8_t DataToTransmit[4];

	DataToTransmit[0] = x1 >> 8;
	DataToTransmit[1] = x1 & 0xFF;
	DataToTransmit[2] = x2 >> 8;
	DataToTransmit[3] = x2 & 0xFF;

	ILI9341_SendCommandAndData(ILI9341_CASET, DataToTransmit, 4);

	DataToTransmit[0] = y1 >> 8;
	DataToTransmit[1] = y1 & 0xFF;
	DataToTransmit[2] = y2 >> 8;
	DataToTransmit[3] = y2 & 0xFF;

	ILI9341_SendCommandAndData(ILI9341_PASET, DataToTransmit, 4);

}

void ILI9341_SendData16(uint16_t Data)
{
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);	//CS to LOW

	uint8_t tmp[2];
	tmp[0] = (Data >> 8);
	tmp[1] = Data & 0xFF;

	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);    //DC to HIGH

	ILI9341_SendToTFT(tmp, 2);

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);		//CS to HIGH
}

void ILI9341_WritePixel(uint16_t x, uint16_t y, uint16_t color)
{
	if((x >= 0) && (x < ILI9341_TFTWIDTH) && (y >= 0) && (y < ILI9341_TFTHEIGHT))
	{
		uint8_t DataToTransmit[2];

		ILI9341_SetAddrWindow(x, y, 1, 1);

		DataToTransmit[0] = color >> 8;
		DataToTransmit[1] = color & 0xFF;

		ILI9341_SendCommandAndData(ILI9341_RAMWR, DataToTransmit, 2);
	}
}

void ILI9341_DrawImage(int x, int y, const uint8_t *img, uint16_t w, uint16_t h)
{
	if((x >= 0) && ((x + w) <= ILI9341_TFTWIDTH) && (y >= 0) && ((y + h) <= ILI9341_TFTHEIGHT))
	{
		ILI9341_SetAddrWindow(x, y, w, h);

		ILI9341_SendCommandAndData(ILI9341_RAMWR, (uint8_t*) img, (w * h * 2));
	}
}

void ILI9341_WriteScreen(uint16_t color)
{
	uint32_t Length = ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT;

	ILI9341_SetAddrWindow(0, 0, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT);

	ILI9341_SendCommand(ILI9341_RAMWR);

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);	//CS to LOW

	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);    //DC to HIGH

	while(Length > 0U)
	{
		/* Wait until TXE flag is set to send data */
		if(__HAL_SPI_GET_FLAG(Tft_hspi, SPI_FLAG_TXE))
		{
			*((__IO uint8_t*) &Tft_hspi->Instance->DR) = color >> 8;

			while(__HAL_SPI_GET_FLAG(Tft_hspi, SPI_FLAG_TXE) != SET)
			{
			}

			*((__IO uint8_t*) &Tft_hspi->Instance->DR) = color & 0xFF;

			Length--;
		}
	}

	while(__HAL_SPI_GET_FLAG(Tft_hspi, SPI_FLAG_BSY) != RESET)
	{

	}

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);    //CS to LOW
}
