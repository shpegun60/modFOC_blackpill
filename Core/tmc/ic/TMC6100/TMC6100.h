/*
 * TMC6100.h
 *
 *  Created on: 13.11.2019
 *      Author: ED
 */

#ifndef TMC_IC_TMC6100_H_
#define TMC_IC_TMC6100_H_

#include "API_Header.h"
#include "TMC6100_Constants.h"
#include "TMC6100_Register.h"
#include "TMC6100_Fields.h"
#include "main.h"

typedef struct {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* CS_Port;
	uint16_t CS_Pin;
} tmc6100_t;

int tmc6100_init(tmc6100_t* drv, SPI_HandleTypeDef* hspi, GPIO_TypeDef* CS_Port, uint16_t CS_Pin);
int32_t tmc6100_readInt(tmc6100_t* drv, uint8_t address);
void tmc6100_writeInt(tmc6100_t* drv, uint8_t address, int32_t value);

#endif /* TMC_IC_TMC6630_H_ */
