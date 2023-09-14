/*
 * TMCL_receiver.h
 *
 *  Created on: Sep 13, 2023
 *      Author: admin
 */

#ifndef __TMCL_RECEIVER_H__
#define __TMCL_RECEIVER_H__

#include <stdint.h>

// Need use this function for receive data from user interface
uint32_t tmcl_receive(uint8_t* Buf, const uint32_t Len);

// function for getting received data packet
uint32_t tmcl_get_data(uint8_t* Buf, uint32_t max_len);

// weak function for transmitting data (user must realize this)
int tmcl_transmitt(uint8_t* Buf, int Len);

#endif /* __TMCL_RECEIVER_H__ */
