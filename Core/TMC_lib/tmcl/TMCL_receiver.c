/*
 * TMCL_receiver.c
 *
 *  Created on: Sep 13, 2023
 *      Author: admin
 */
#include "TMCL_receiver.h"
#include <string.h>

#define RX_CMND_BUF_LEN 16

typedef struct
{
	int pos_receive, pos_process;
	char IsCommandDataReceived;
	uint8_t RxBufferFS[RX_CMND_BUF_LEN][10];
	uint8_t CMDLens[RX_CMND_BUF_LEN];
} rx_cmd_buf_t;

static rx_cmd_buf_t rx_cmd_buf;

uint32_t tmcl_receive(uint8_t* Buf, const uint32_t Len)
{
	if(Len > sizeof(rx_cmd_buf.RxBufferFS[0])) {
		return 0;
	}

	int index = rx_cmd_buf.pos_receive;

	rx_cmd_buf.IsCommandDataReceived = 1;
	rx_cmd_buf.CMDLens[index] = Len;
	memcpy(rx_cmd_buf.RxBufferFS[index], Buf, Len);
	index = (index + 1) & (RX_CMND_BUF_LEN - 1);
	rx_cmd_buf.pos_receive = index;
	return Len;
}


uint32_t tmcl_get_data(uint8_t *Buf, uint32_t max_len)
{
	if(rx_cmd_buf.IsCommandDataReceived == 0) {
		return 0;
	}

	int index = rx_cmd_buf.pos_process;
	const uint32_t read_len = rx_cmd_buf.CMDLens[index];
	max_len = (read_len > max_len) ? max_len : read_len;

	memcpy(Buf, rx_cmd_buf.RxBufferFS[index], max_len);
	//testing only. make sure there is ending char in the returned command string
	//check if all data were processed.
	index = (index + 1) & (RX_CMND_BUF_LEN - 1);

	if (index == rx_cmd_buf.pos_receive) {
		rx_cmd_buf.IsCommandDataReceived = 0;
	}
	rx_cmd_buf.pos_process = index;

	//check if all data were processed
	return max_len;
}

// weak function for transmition data
__attribute__((weak)) int tmcl_transmitt(uint8_t* Buf, int Len)
{
	(void)Buf;
	(void)Len;
	return 0;
}
