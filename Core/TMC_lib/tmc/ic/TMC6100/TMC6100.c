/*******************************************************************************
* Copyright © 2019 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2023 Analog Devices Inc. All Rights Reserved. This software is
* proprietary & confidential to Analog Devices, Inc. and its licensors.
*******************************************************************************/


#include "TMC6100.h"
#include "tmc_port.h"

// => SPI wrapper
extern uint8_t tmc6100_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer);
// <= SPI wrapper

// spi access
int32_t tmc6100_readInt(uint8_t motor, uint8_t address)
{
	int32_t writeBuff = 0;
	int32_t readBuff= 0;

	// clear write bit
	address = TMC_ADDRESS(address);

	DRIVER_CS_LOW();
	DRIVER_SPI_READ_WRITE((uint8_t*)&address, 		(uint8_t*)&readBuff, sizeof(address));  // write address
	DRIVER_SPI_READ_WRITE((uint8_t*)&writeBuff, 	(uint8_t*)&readBuff, sizeof(readBuff)); // read data
	DRIVER_CS_HIGH();
	readBuff = __builtin_bswap32(readBuff);
	return readBuff;

//	// clear write bit
//	address = TMC_ADDRESS(address);
//
//	// write address
//	tmc6100_readwriteByte(motor, address, false);
//
//	// read data
//	int32_t value = tmc6100_readwriteByte(motor, 0, false);
//	value <<= 8;
//	value |= tmc6100_readwriteByte(motor, 0, false);
//	value <<= 8;
//	value |= tmc6100_readwriteByte(motor, 0, false);
//	value <<= 8;
//	value |= tmc6100_readwriteByte(motor, 0, true);
//
//	return value;
}

void tmc6100_writeInt(uint8_t motor, uint8_t address, int32_t value)
{
	int32_t readBuff = 0;

	// set address
	address = address | TMC6100_WRITE_BIT;

	// write value
	value = __builtin_bswap32(value);
	DRIVER_CS_LOW();
	DRIVER_SPI_READ_WRITE((uint8_t*)&address, 	(uint8_t*)&readBuff, sizeof(address));  	// write address
	DRIVER_SPI_READ_WRITE((uint8_t*)&value, 	(uint8_t*)&readBuff, sizeof(value)  ); 	// write value
	DRIVER_CS_HIGH();

//	// write address
//	tmc6100_readwriteByte(motor, address | TMC6100_WRITE_BIT, false);
//
//	// write value
//	tmc6100_readwriteByte(motor, 0xFF & (value>>24), false);
//	tmc6100_readwriteByte(motor, 0xFF & (value>>16), false);
//	tmc6100_readwriteByte(motor, 0xFF & (value>>8), false);
//	tmc6100_readwriteByte(motor, 0xFF & (value>>0), true);


}
