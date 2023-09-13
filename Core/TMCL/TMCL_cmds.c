/*
 * TMCL_cmds.c
 *
 *  Created on: Sep 13, 2023
 *      Author: admin
 */
#include "TMCL_cmds.h"
#include "TMC4671.h"
#include "TMC4671_Constants.h"

#include "TMCL-Defines.h"
#include "TMCL-Variables.h"

static const char *VersionString = "0011V307";


int32_t getVM(uint8_t motor)
{
	int32_t rawVMADC = tmc4671_readFieldWithDependency(motor, TMC4671_ADC_RAW_DATA, TMC4671_ADC_RAW_ADDR, 1, TMC4671_ADC_VM_RAW_MASK, TMC4671_ADC_VM_RAW_SHIFT) - VM_ADC_OFFSET;
	return ((rawVMADC * (VM_ADC_REF / VM_ADC_RES)) / (VM_DIV_FACTOR)) * 10;
}


void tmcl_getVersion()
{
	uint32_t i;

	if (TMCL_handle.ActualCommand.Type == 0) {
		TMCL_handle.TMCLReplyFormat = RF_SPECIAL;
		TMCL_handle.SpecialReply[0] = HOST_ADDRESS;
		for (i = 0; i < 8; i++) {
			TMCL_handle.SpecialReply[i + 1] = VersionString[i];
		}
	} else if (TMCL_handle.ActualCommand.Type == 1) {
		TMCL_handle.ActualReply.Value.Byte[3] = ((VersionString[0] - '0') * 10) + (VersionString[1] - '0');
		TMCL_handle.ActualReply.Value.Byte[2] = ((VersionString[2] - '0') * 10) + (VersionString[3] - '0');
		TMCL_handle.ActualReply.Value.Byte[1] = (VersionString[5] - '0');
		TMCL_handle.ActualReply.Value.Byte[0] = ((VersionString[6] - '0') * 10) + (VersionString[7] - '0');
	}
}

void tmcl_getInput()
{
	switch (TMCL_handle.ActualCommand.Type)
	{
	case 5: // analog_inputs
		TMCL_handle.ActualReply.Value.Int32 = getVM(DEFAULT_MC);
		break;
	default:
		TMCL_handle.ActualReply.Status = REPLY_INVALID_VALUE;
		break;
	}
}



