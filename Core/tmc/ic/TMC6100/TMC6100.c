/*
 * TMC6100.c
 *
 *  Created on: 13.11.2019
 *      Author: ED
 */

#include "TMC6100.h"

// => SPI wrapper
//extern uint8_t tmc6100_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer);
// <= SPI wrapper

int tmc6100_init(tmc6100_t* drv, SPI_HandleTypeDef* hspi, GPIO_TypeDef* CS_Port, uint16_t CS_Pin)
{
	if(drv == NULL || hspi == NULL || CS_Port == NULL) {
		return 1;
	}

	drv->CS_Pin 	= CS_Pin;
	drv->CS_Port 	= CS_Port;
	drv->hspi 		= hspi;
	HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET); // write CS to High
	return 0;
}

// spi access
int32_t tmc6100_readInt(tmc6100_t* drv, uint8_t address)
{
	int32_t writeBuff = 0;
	int32_t readBuff= 0;

	// clear write bit
	address = TMC_ADDRESS(address);

	// read data
	HAL_GPIO_WritePin(drv->CS_Port, drv->CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(drv->hspi, (uint8_t*)&address, (uint8_t*)&readBuff, sizeof(address), HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(drv->hspi, (uint8_t*)&writeBuff, (uint8_t*)&readBuff, sizeof(readBuff), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(drv->CS_Port, drv->CS_Pin, GPIO_PIN_SET);
	readBuff = __builtin_bswap32(readBuff);
	return readBuff;
}

void tmc6100_writeInt(tmc6100_t* drv, uint8_t address, int32_t value)
{
	int32_t readBuff = 0;

	// set address
	address = address | TMC6100_WRITE_BIT;

	// write value
	value = __builtin_bswap32(value);
	HAL_GPIO_WritePin(drv->CS_Port, drv->CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(drv->hspi, (uint8_t*)&address, (uint8_t*)&readBuff, sizeof(address), HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(drv->hspi, (uint8_t*)&value, (uint8_t*)&readBuff, sizeof(readBuff), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(drv->CS_Port, drv->CS_Pin, GPIO_PIN_SET);
}
