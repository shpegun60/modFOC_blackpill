/*
 * tmc_port.h
 *
 *  Created on: Sep 28, 2023
 *      Author: admin
 */

#ifndef TMC_LIB_PORT_H_
#define TMC_LIB_PORT_H_

#include "main.h"
#include "spi.h"

#define MOTION_CTRL_SPI_READ_WRITE(tx_ptr, rx_ptr, size) HAL_SPI_TransmitReceive(&hspi1, (tx_ptr), (rx_ptr), (size), HAL_MAX_DELAY)
#define MOTION_CTRL_CS_LOW() HAL_GPIO_WritePin(CS_CTRL_GPIO_Port, CS_CTRL_Pin, GPIO_PIN_RESET)
#define MOTION_CTRL_CS_HIGH() HAL_GPIO_WritePin(CS_CTRL_GPIO_Port, CS_CTRL_Pin, GPIO_PIN_SET)

#define MOTION_CTRL_ENABLE() HAL_GPIO_WritePin(CTRL_EN_GPIO_Port, CTRL_EN_Pin, GPIO_PIN_SET)
#define MOTION_CTRL_DISABLE() HAL_GPIO_WritePin(CTRL_EN_GPIO_Port, CTRL_EN_Pin, GPIO_PIN_RESET)


#define DRIVER_SPI_READ_WRITE(tx_ptr, rx_ptr, size) HAL_SPI_TransmitReceive(&hspi1, (tx_ptr), (rx_ptr), (size), HAL_MAX_DELAY)
#define DRIVER_CS_LOW() HAL_GPIO_WritePin(CS_DRV_GPIO_Port, CS_DRV_Pin, GPIO_PIN_RESET)
#define DRIVER_CS_HIGH() HAL_GPIO_WritePin(CS_DRV_GPIO_Port, CS_DRV_Pin, GPIO_PIN_SET)


#endif /* TMC_LIB_PORT_H_ */
