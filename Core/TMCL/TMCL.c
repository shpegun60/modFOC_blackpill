/*
 * TMCL_handler.c
 *
 *  Created on: Sep 13, 2023
 *      Author: Shpegun60
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>


#include "TMCL.h"
#include "TMCL-Variables.h"
#include "TMCL-Defines.h"
#include "TMCL_receiver.h"
#include "TMCL_cmds.h"

#include "TMC4671.h"
#include "TMC4671_Constants.h"


static void tmcl_executeActualCommand()
{
	// prepare reply command
	TMCL_handle.ActualReply.Opcode = TMCL_handle.ActualCommand.Opcode;
	TMCL_handle.ActualReply.Status = REPLY_OK;
	TMCL_handle.ActualReply.Value.Int32 = TMCL_handle.ActualCommand.Value.Int32;

	//printf("Opcode: %d\n", ActualCommand.Opcode);

	switch (TMCL_handle.ActualCommand.Opcode)
	{
	case TMCL_GetVersion: {
		tmcl_getVersion();
		break;}

	case TMCL_GIO: {
		tmcl_getInput();
		break;}

	case TMCL_readRegisterChannel_1: {
		if (TMCL_handle.ActualCommand.Motor == 0) {
			TMCL_handle.ActualReply.Value.Int32 = tmc4671_readInt(DEFAULT_MC, TMCL_handle.ActualCommand.Type);
		}
		break;}

	case TMCL_writeRegisterChannel_1: {
		if (TMCL_handle.ActualCommand.Motor == 0) {
			tmc4671_writeInt(DEFAULT_MC, TMCL_handle.ActualCommand.Type, TMCL_handle.ActualCommand.Value.Int32);
		}
		break;}

	case TMCL_GAP: {
		TMCL_handle.ActualReply.Value.Int32 = tmc4671_getActualPosition(DEFAULT_MC);
		break;}

	default: {
	break;}

	}
}


void tmcl_processCommand()
{
	static uint8_t TMCLCommandState = TCS_IDLE;
	uint32_t i;
	uint32_t CMDLen;

	uint8_t USBCmd[9];
	uint8_t USBReply[9];

	/* send reply for last TMCL request */
	if(TMCLCommandState == TCS_USB) { // reply via USB
		if(TMCL_handle.TMCLReplyFormat == RF_STANDARD) {
			uint8_t Checksum = HOST_ADDRESS + MODULE_ADDRESS + TMCL_handle.ActualReply.Status + TMCL_handle.ActualReply.Opcode + TMCL_handle.ActualReply.Value.Byte[3] + TMCL_handle.ActualReply.Value.Byte[2] + TMCL_handle.ActualReply.Value.Byte[1] + TMCL_handle.ActualReply.Value.Byte[0];

			USBReply[0] = HOST_ADDRESS;
			USBReply[1] = MODULE_ADDRESS;
			USBReply[2] = TMCL_handle.ActualReply.Status;
			USBReply[3] = TMCL_handle.ActualReply.Opcode;
			USBReply[4] = TMCL_handle.ActualReply.Value.Byte[3];
			USBReply[5] = TMCL_handle.ActualReply.Value.Byte[2];
			USBReply[6] = TMCL_handle.ActualReply.Value.Byte[1];
			USBReply[7] = TMCL_handle.ActualReply.Value.Byte[0];
			USBReply[8] = Checksum;
		} else if(TMCL_handle.TMCLReplyFormat == RF_SPECIAL) {
			memcpy(USBReply, TMCL_handle.SpecialReply, sizeof(USBReply));
		}
		tmcl_transmitt(USBReply, sizeof(USBReply));
	} else if(TMCLCommandState == TCS_USB_ERROR) { // last command had a wrong checksum
		TMCL_handle.ActualReply.Opcode = 0;
		TMCL_handle.ActualReply.Status = REPLY_CHKERR;
		TMCL_handle.ActualReply.Value.Int32 = 0;

		uint8_t Checksum = HOST_ADDRESS + MODULE_ADDRESS + TMCL_handle.ActualReply.Status + TMCL_handle.ActualReply.Opcode + TMCL_handle.ActualReply.Value.Byte[3] + TMCL_handle.ActualReply.Value.Byte[2] + TMCL_handle.ActualReply.Value.Byte[1] + TMCL_handle.ActualReply.Value.Byte[0];

		USBReply[0] = HOST_ADDRESS;
		USBReply[1] = MODULE_ADDRESS;
		USBReply[2] = TMCL_handle.ActualReply.Status;
		USBReply[3] = TMCL_handle.ActualReply.Opcode;
		USBReply[4] = TMCL_handle.ActualReply.Value.Byte[3];
		USBReply[5] = TMCL_handle.ActualReply.Value.Byte[2];
		USBReply[6] = TMCL_handle.ActualReply.Value.Byte[1];
		USBReply[7] = TMCL_handle.ActualReply.Value.Byte[0];
		USBReply[8] = Checksum;

		tmcl_transmitt(USBReply, sizeof(USBReply));
	}

	// reset command state (reply has been send)
	TMCLCommandState = TCS_IDLE;
	TMCL_handle.TMCLReplyFormat = RF_STANDARD;

	// last command was a reset?

	/* read next request */
	CMDLen = tmcl_get_data(USBCmd, sizeof(USBCmd));
	if(CMDLen == 9) {
		TMCL_handle.commLoopCounter++;

		if(USBCmd[0] == MODULE_ADDRESS) {	 // check address

			uint8_t checksum = 0;
			for(i = 0; i < 8; i++) {
				checksum += USBCmd[i];
			}

			if(checksum == USBCmd[8]) {  // check checksum

				TMCL_handle.ActualCommand.Opcode = USBCmd[1];
				TMCL_handle.ActualCommand.Type = USBCmd[2];
				TMCL_handle.ActualCommand.Motor = USBCmd[3];
				TMCL_handle.ActualCommand.Value.Byte[3] = USBCmd[4];
				TMCL_handle.ActualCommand.Value.Byte[2] = USBCmd[5];
				TMCL_handle.ActualCommand.Value.Byte[1] = USBCmd[6];
				TMCL_handle.ActualCommand.Value.Byte[0] = USBCmd[7];
				TMCLCommandState = TCS_USB;
			} else {
				TMCLCommandState = TCS_USB_ERROR;  // checksum was wrong
			}
		}
	}

	// handle request after successful reading
	if(TMCLCommandState != TCS_IDLE && TMCLCommandState != TCS_UART_ERROR && TMCLCommandState != TCS_RS485_ERROR && TMCLCommandState != TCS_USB_ERROR) {
		tmcl_executeActualCommand();
	}
}

