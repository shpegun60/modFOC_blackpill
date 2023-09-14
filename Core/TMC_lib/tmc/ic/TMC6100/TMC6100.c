/*
 * TMC6100.c
 *
 *  Created on: 13.11.2019
 *      Author: ED
 */

#include "TMC6100.h"
#include "spi.h"
#include "main.h"

#define TMC6100_SPI_READ_WRITE(tx_ptr, rx_ptr, size) HAL_SPI_TransmitReceive(&hspi1, (tx_ptr), (rx_ptr), (size), HAL_MAX_DELAY)
#define TMC6100_CS_LOW() HAL_GPIO_WritePin(CS_DRV_GPIO_Port, CS_DRV_Pin, GPIO_PIN_RESET)
#define TMC6100_CS_HIGH() HAL_GPIO_WritePin(CS_DRV_GPIO_Port, CS_DRV_Pin, GPIO_PIN_SET)

// => SPI wrapper
//extern uint8_t tmc6100_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer);
// <= SPI wrapper

// spi access
int tmc6100_readInt(uint8_t motor, uint8_t address)
{

	int32_t writeBuff = 0;
	int32_t readBuff= 0;

	// clear write bit
	address = TMC_ADDRESS(address);

	TMC6100_CS_LOW();
	TMC6100_SPI_READ_WRITE((uint8_t*)&address, 		(uint8_t*)&readBuff, sizeof(address));  // write address
	TMC6100_SPI_READ_WRITE((uint8_t*)&writeBuff, 	(uint8_t*)&readBuff, sizeof(readBuff)); // read data
	TMC6100_CS_HIGH();
	readBuff = __builtin_bswap32(readBuff);
	return readBuff;

//	// clear write bit
//	address = TMC_ADDRESS(address);
//
//	// write address
//	tmc6100_readwriteByte(motor, address, false);
//
//
//	int value = tmc6100_readwriteByte(motor, 0, false);
//	value <<= 8;
//	value |= tmc6100_readwriteByte(motor, 0, false);
//	value <<= 8;
//	value |= tmc6100_readwriteByte(motor, 0, false);
//	value <<= 8;
//	value |= tmc6100_readwriteByte(motor, 0, true);
//
//	return value;
}

void tmc6100_writeInt(uint8_t motor, uint8_t address, int value)
{
	int32_t readBuff = 0;

	// set address
	address = address | TMC6100_WRITE_BIT;

	// write value
	value = __builtin_bswap32(value);
	TMC6100_CS_LOW();
	TMC6100_SPI_READ_WRITE((uint8_t*)&address, 	(uint8_t*)&readBuff, sizeof(address));  	// write address
	TMC6100_SPI_READ_WRITE((uint8_t*)&value, 	(uint8_t*)&readBuff, sizeof(value)  ); 	// write value
	TMC6100_CS_HIGH();

//	// write address
//	tmc6100_readwriteByte(motor, address | TMC6100_WRITE_BIT, false);
//
//	// write value
//	tmc6100_readwriteByte(motor, 0xFF & (value>>24), false);
//	tmc6100_readwriteByte(motor, 0xFF & (value>>16), false);
//	tmc6100_readwriteByte(motor, 0xFF & (value>>8), false);
//	tmc6100_readwriteByte(motor, 0xFF & (value>>0), true);
}
