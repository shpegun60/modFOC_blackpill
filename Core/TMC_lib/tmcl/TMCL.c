/*******************************************************************************
* Copyright © 2019 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2023 Analog Devices Inc. All Rights Reserved. This software is
* proprietary & confidential to Analog Devices, Inc. and its licensors.
*******************************************************************************/


#include "TMCL.h"
#include "TMCL_receiver.h"

#include "BoardAssignment.h"
#include "IdDetection.h"
#include "RAMDebug.h"
#include "main.h"

#define BUILD_VERSION 0

// these addresses are fixed
#define SERIAL_MODULE_ADDRESS  1
#define SERIAL_HOST_ADDRESS    2

// todo CHECK 2: these are unused - delete? (LH) #11
// tmcl interpreter states
#define TM_IDLE      0
#define TM_RUN       1
#define TM_STEP      2
#define TM_RESET     3 // unused
#define TM_DOWNLOAD  4
#define TM_DEBUG     5 // wie TM_IDLE, es wird jedoch der Akku nicht modifiziert bei GAP etc.

// todo CHECK 2: these are unused - delete? (LH) #12
#define TCS_IDLE           0
#define TCS_CAN7           1
#define TCS_CAN8           2
#define TCS_UART           3
#define TCS_UART_ERROR     4
#define TCS_UART_II        5
#define TCS_UART_II_ERROR  6
#define TCS_USB            7
#define TCS_USB_ERROR      8
#define TCS_MEM            9

// TMCL commands
#define TMCL_ROR                     1
#define TMCL_ROL                     2
#define TMCL_MST                     3
#define TMCL_MVP                     4
#define TMCL_SAP                     5
#define TMCL_GAP                     6
#define TMCL_STAP                    7
#define TMCL_RSAP                    8
#define TMCL_SGP                     9
#define TMCL_GGP                     10
#define TMCL_STGP                    11
#define TMCL_RSGP                    12
#define TMCL_RFS                     13
#define TMCL_SIO                     14
#define TMCL_GIO                     15
#define TMCL_CALC                    19
#define TMCL_COMP                    20
#define TMCL_JC                      21
#define TMCL_JA                      22
#define TMCL_CSUB                    23
#define TMCL_RSUB                    24
#define TMCL_EI                      25
#define TMCL_DI                      26
#define TMCL_WAIT                    27
#define TMCL_STOP                    28
#define TMCL_SAC                     29
#define TMCL_SCO                     30
#define TMCL_GCO                     31
#define TMCL_CCO                     32
#define TMCL_CALCX                   33
#define TMCL_AAP                     34
#define TMCL_AGP                     35
#define TMCL_CLE                     36
#define TMCL_VECT                    37
#define TMCL_RETI                    38
#define TMCL_ACO                     39

#define TMCL_UF0                     64
#define TMCL_UF1                     65
#define TMCL_UF2                     66
#define TMCL_UF3                     67
#define TMCL_UF4                     68
#define TMCL_UF5                     69
#define TMCL_UF6                     70
#define TMCL_UF7                     71
#define TMCL_UF8                     72

#define TMCL_ApplStop                128
#define TMCL_ApplRun                 129
#define TMCL_ApplStep                130
#define TMCL_ApplReset               131
#define TMCL_DownloadStart           132
#define TMCL_DownloadEnd             133
#define TMCL_ReadMem                 134
#define TMCL_GetStatus               135
#define TMCL_GetVersion              136
#define TMCL_FactoryDefault          137
#define TMCL_SetEvent                138
#define TMCL_SetASCII                139
#define TMCL_SecurityCode            140
#define TMCL_Breakpoint              141
#define TMCL_RamDebug                142
#define TMCL_GetIds                  143
#define TMCL_UF_CH1                  144
#define TMCL_UF_CH2                  145
#define TMCL_writeRegisterChannel_1  146
#define TMCL_writeRegisterChannel_2  147
#define TMCL_readRegisterChannel_1   148
#define TMCL_readRegisterChannel_2   149

#define TMCL_BoardMeasuredSpeed      150
#define TMCL_BoardError              151
#define TMCL_BoardReset              152

#define TMCL_WLAN                    160
#define TMCL_WLAN_CMD                160
#define TMCL_WLAN_IS_RTS             161
#define TMCL_WLAN_CMDMODE_EN         162
#define TMCL_WLAN_IS_CMDMODE         163

#define TMCL_MIN                     170
#define TMCL_MAX                     171
#define TMCL_OTP                     172

#define TMCL_Boot                    242
#define TMCL_SoftwareReset           255

// Command type variants
#define MVP_ABS  0
#define MVP_REL  1
#define MVP_PRF  2

// GetVersion() Format types
#define VERSION_FORMAT_ASCII      0
#define VERSION_FORMAT_BINARY     1
#define VERSION_BOOTLOADER        2 // todo CHECK 2: implemented this way in IDE - probably means getting the bootloader version. Not implemented in firmware (LH)
#define VERSION_SIGNATURE         3 // todo CHECK 2: implemented under "Signature" in IDE. Not sure what to return for that. Not implemented in firmware (LH)
#define VERSION_BOARD_DETECT_SRC  4 // todo CHECK 2: This doesn't really fit under GetVersion, but its implemented there in the IDE - change or leave this way? (LH)
#define VERSION_BUILD             5

//Statuscodes
#define REPLY_OK                     100
#define REPLY_CMD_LOADED             101
#define REPLY_CHKERR                 1
#define REPLY_INVALID_CMD            2
#define REPLY_INVALID_TYPE           3
#define REPLY_INVALID_VALUE          4
#define REPLY_EEPROM_LOCKED          5
#define REPLY_CMD_NOT_AVAILABLE      6
#define REPLY_CMD_LOAD_ERROR         7
#define REPLY_WRITE_PROTECTED        8
#define REPLY_MAX_EXCEEDED           9
#define REPLY_DOWNLOAD_NOT_POSSIBLE  10
#define REPLY_CHIP_READ_FAILED       11
#define REPLY_DELAYED                128
#define REPLY_ACTIVE_COMM            129

// TMCL communication status
#define TMCL_RX_ERROR_NONE      0
#define TMCL_RX_ERROR_NODATA    1
#define TMCL_RX_ERROR_CHECKSUM  2

extern const char *VersionString;

// TMCL request
typedef struct
{
	uint8_t   Opcode;
	uint8_t   Type;
	uint8_t   Motor;
	uint32_t  Error;
	union
	{
		uint8_t Byte[4];
		uint32_t UInt32;
		int32_t Int32;
		float32_t Float32;
	} Value;
} TMCLCommandTypeDef;

// TMCL reply
typedef struct
{
	uint8_t Status;
	uint8_t Opcode;
	union
	{
		uint8_t Byte[4];
		uint32_t UInt32;
		int32_t Int32;
		float32_t Float32;
	} Value;

	uint8_t Special[9];
	uint8_t IsSpecial;  // next transfer will not use the serial address and the checksum bytes - instead the whole datagram is filled with data (used to transmit ASCII version string)
} TMCLReplyTypeDef;

void ExecuteActualCommand();
uint8_t setTMCLStatus(uint8_t evalError);
void rx(void);
void tx(void);

// Helper functions - used to prevent ExecuteActualCommand() from getting too big.
// No parameters or return value are used.
static void SetGlobalParameter(void);
static void GetGlobalParameter(void);
static void boardAssignment(void);
static void boardsErrors(void);
static void boardsReset(void);
static void boardsMeasuredSpeed(void);
static void setDriversEnable(void);
static void checkIDs(void);
static void SoftwareReset(void);
static void GetVersion(void);
static void GetInput(void);
static void HandleWlanCommand(void);
static void handleRamDebug(void);
static void handleOTP(void);

TMCLCommandTypeDef ActualCommand;
TMCLReplyTypeDef ActualReply;
uint32_t resetRequest = 0;

// Sets TMCL status from Evalboard error. Returns the parameter given to allow for compact error handling
uint8_t setTMCLStatus(uint8_t evalError)
{
	if(evalError == TMC_ERROR_NONE)          ActualReply.Status = REPLY_OK;
	else if(evalError & TMC_ERROR_FUNCTION)  ActualReply.Status = REPLY_INVALID_CMD;
	else if(evalError & TMC_ERROR_TYPE)      ActualReply.Status = REPLY_INVALID_TYPE;
	else if(evalError & TMC_ERROR_MOTOR)     ActualReply.Status = REPLY_INVALID_TYPE; // todo CHECK ADD 2: Different errors for Evalboard type/motor errors? (LH) #1
	else if(evalError & TMC_ERROR_VALUE)     ActualReply.Status = REPLY_INVALID_VALUE;
	else if(evalError & TMC_ERROR_NOT_DONE)  ActualReply.Status = REPLY_DELAYED;
	else if(evalError & TMC_ERROR_CHIP)      ActualReply.Status = REPLY_EEPROM_LOCKED;
	return evalError;
}

void ExecuteActualCommand()
{
	ActualReply.Opcode = ActualCommand.Opcode;
	ActualReply.Status = REPLY_OK;
	ActualReply.Value.Int32 = ActualCommand.Value.Int32;

	if(ActualCommand.Error == TMCL_RX_ERROR_CHECKSUM)
	{
		ActualReply.Value.Int32  = 0;
		ActualReply.Status       = REPLY_CHKERR;
		return;
	}

	switch(ActualCommand.Opcode)
	{
	case TMCL_ROR:
		// if function doesn't exist for ch1 try ch2
		if(setTMCLStatus(Evalboards.ch1.right(ActualCommand.Motor, ActualCommand.Value.Int32)) & TMC_ERROR_FUNCTION)
		{
			setTMCLStatus(Evalboards.ch2.right(ActualCommand.Motor, ActualCommand.Value.Int32));
		}
		break;
	case TMCL_ROL:
		// if function doesn't exist for ch1 try ch2
		if(setTMCLStatus(Evalboards.ch1.left(ActualCommand.Motor, ActualCommand.Value.Int32)) & TMC_ERROR_FUNCTION)
		{
			setTMCLStatus(Evalboards.ch2.left(ActualCommand.Motor, ActualCommand.Value.Int32));
		}
		break;
	case TMCL_MST:
		// if function doesn't exist for ch1 try ch2
		if(setTMCLStatus(Evalboards.ch1.stop(ActualCommand.Motor)) & TMC_ERROR_FUNCTION)
		{
			setTMCLStatus(Evalboards.ch2.stop(ActualCommand.Motor));
		}
		break;
	case TMCL_MVP:
		// if function doesn't exist for ch1 try ch2
		switch(ActualCommand.Type)
		{
		case MVP_ABS: // move absolute
			if(setTMCLStatus(Evalboards.ch1.moveTo(ActualCommand.Motor, ActualCommand.Value.Int32)) & TMC_ERROR_FUNCTION)
			{
				setTMCLStatus(Evalboards.ch2.moveTo(ActualCommand.Motor, ActualCommand.Value.Int32));
			}
			break;
		case MVP_REL: // move relative
			if(setTMCLStatus(Evalboards.ch1.moveBy(ActualCommand.Motor, &ActualCommand.Value.Int32)) & TMC_ERROR_FUNCTION)
			{
				setTMCLStatus(Evalboards.ch2.moveBy(ActualCommand.Motor, &ActualCommand.Value.Int32));
			}
			ActualReply.Value.Int32 = ActualCommand.Value.Int32;
			break;
		case MVP_PRF:
			if(setTMCLStatus(Evalboards.ch1.moveProfile(ActualCommand.Motor, ActualCommand.Value.Int32)) & TMC_ERROR_FUNCTION)
			{
				setTMCLStatus(Evalboards.ch2.moveProfile(ActualCommand.Motor, ActualCommand.Value.Int32));
			}
			break;
		default:
			ActualReply.Status = REPLY_INVALID_TYPE;
			break;
		}
		break;
	case TMCL_SAP:
		// if function doesn't exist for ch1 try ch2
		if(setTMCLStatus(Evalboards.ch1.SAP(ActualCommand.Type, ActualCommand.Motor, ActualCommand.Value.Int32)) & (TMC_ERROR_TYPE | TMC_ERROR_FUNCTION))
		{
			setTMCLStatus(Evalboards.ch2.SAP(ActualCommand.Type, ActualCommand.Motor, ActualCommand.Value.Int32));
		}
		break;
	case TMCL_GAP:
		// if function doesn't exist for ch1 try ch2
		if(setTMCLStatus(Evalboards.ch1.GAP(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32)) & (TMC_ERROR_TYPE | TMC_ERROR_FUNCTION))
		{
			setTMCLStatus(Evalboards.ch2.GAP(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32));
		}
		break;
	case TMCL_SGP:
		SetGlobalParameter();
		break;
	case TMCL_GGP:
		GetGlobalParameter();
		break;
	case TMCL_GIO:
		GetInput();
		break;
	case TMCL_UF0:
		setDriversEnable();
		break;
	case TMCL_UF1:
		//readIdEeprom();
		break;
	case TMCL_UF2:
		//writeIdEeprom();
		break;
	case TMCL_UF4:
		// if function doesn't exist for ch1 try ch2
		if(setTMCLStatus(Evalboards.ch1.getMeasuredSpeed(ActualCommand.Motor, &ActualReply.Value.Int32)) & TMC_ERROR_FUNCTION)
		{
			setTMCLStatus(Evalboards.ch2.getMeasuredSpeed(ActualCommand.Motor, &ActualReply.Value.Int32));
		}
		break;
	case TMCL_UF5:
		// if function doesn't exist for ch1 try ch2 // todo CHECK REM 2: We have TMCL_writeRegisterChannel_1, we dont need this. Make sure it isnt used in IDE (LH) #1
		Evalboards.ch1.writeRegister(ActualCommand.Motor, ActualCommand.Type, ActualCommand.Value.Int32);
		break;
	case TMCL_UF6:
		// if function doesn't exist for ch1 try ch2 // todo CHECK REM 2: We have TMCL_readRegisterChannel_1, we dont need this. Make sure it isnt used in IDE (LH) #2
		Evalboards.ch1.readRegister(ActualCommand.Motor, ActualCommand.Type, &ActualReply.Value.Int32);
		break;
	case TMCL_UF8:
		// user function for reading Motor0_XActual and Motor1_XActual
		Evalboards.ch1.userFunction(ActualCommand.Type, 0, &ActualCommand.Value.Int32);
		int32_t m0XActual = ActualCommand.Value.Int32;
		Evalboards.ch1.userFunction(ActualCommand.Type, 1, &ActualCommand.Value.Int32);
		int32_t m1XActual = ActualCommand.Value.Int32;
		ActualReply.Value.Byte[0]= m1XActual & 0xFF;
		ActualReply.Value.Byte[1]= (m1XActual & 0xFF00)>>8;
		ActualReply.Value.Byte[2]= (m1XActual & 0xFF0000)>>16;
		ActualReply.Value.Byte[3]= m0XActual & 0xFF;
		ActualReply.Opcode= (m0XActual & 0xFF00)>>8;
		ActualReply.Status= (m0XActual & 0xFF0000)>>16;
		break;
	case TMCL_GetVersion:
		GetVersion();
		break;
	case TMCL_GetIds:
		boardAssignment();
		break;
	case TMCL_UF_CH1:
		// user function for motionController board
		setTMCLStatus(Evalboards.ch1.userFunction(ActualCommand.Type, ActualCommand.Motor, &ActualCommand.Value.Int32));
		ActualReply.Value.Int32 = ActualCommand.Value.Int32;
		break;
	case TMCL_UF_CH2:
		// user function for driver board
		setTMCLStatus(Evalboards.ch2.userFunction(ActualCommand.Type, ActualCommand.Motor, &ActualCommand.Value.Int32));
		ActualReply.Value.Int32 = ActualCommand.Value.Int32;
		break;
	case TMCL_writeRegisterChannel_1:
		Evalboards.ch1.writeRegister(ActualCommand.Motor, ActualCommand.Type, ActualCommand.Value.Int32);
		break;
	case TMCL_writeRegisterChannel_2:
		Evalboards.ch2.writeRegister(ActualCommand.Motor, ActualCommand.Type, ActualCommand.Value.Int32);
		break;
	case TMCL_readRegisterChannel_1:
		// Do not allow reads during brownout to prevent garbage data being used
		// in read-modify-write operations. Bypass this safety with motor = 255
		Evalboards.ch1.readRegister(ActualCommand.Motor, ActualCommand.Type, &ActualReply.Value.Int32);
		break;
	case TMCL_readRegisterChannel_2:
		// Do not allow reads during brownout to prevent garbage data being used
		// in read-modify-write operations. Bypass this safety with motor = 255
		Evalboards.ch2.readRegister(ActualCommand.Motor, ActualCommand.Type, &ActualReply.Value.Int32);
		break;
	case TMCL_BoardMeasuredSpeed:
		// measured speed from motionController board or driver board depending on type
		boardsMeasuredSpeed();
		break;
	case TMCL_BoardError:
		// errors of motionController board or driver board depending on type
		boardsErrors();
		break;
	case TMCL_BoardReset:
		// reset of motionController board or driver board depending on type
		boardsReset();
		break;
	case TMCL_WLAN:
		HandleWlanCommand();
		break;
	case TMCL_RamDebug:
		handleRamDebug();
		break;
	case TMCL_OTP:
		handleOTP();
		break;
	case TMCL_MIN:
		if(setTMCLStatus(Evalboards.ch1.getMin(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32)) & (TMC_ERROR_TYPE | TMC_ERROR_FUNCTION))
		{
			setTMCLStatus(Evalboards.ch2.getMin(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32));
		}
		break;
	case TMCL_MAX:
		if(setTMCLStatus(Evalboards.ch1.getMax(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32)) & (TMC_ERROR_TYPE | TMC_ERROR_FUNCTION))
		{
			setTMCLStatus(Evalboards.ch2.getMax(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32));
		}
		break;
	case TMCL_Boot:
		if(ActualCommand.Type           != 0x81)  break;
		if(ActualCommand.Motor          != 0x92)  break;
		if(ActualCommand.Value.Byte[3]  != 0xA3)  break;
		if(ActualCommand.Value.Byte[2]  != 0xB4)  break;
		if(ActualCommand.Value.Byte[1]  != 0xC5)  break;
		if(ActualCommand.Value.Byte[0]  != 0xD6)  break;
		tmcl_boot();
		break;
	case TMCL_SoftwareReset:
		SoftwareReset();
		break;
	default:
		ActualReply.Status = REPLY_INVALID_CMD;
		break;
	}
}

void tmcl_init()
{
	ActualCommand.Error  = TMCL_RX_ERROR_NODATA;
}

void tmcl_process()
{
	if(ActualCommand.Error != TMCL_RX_ERROR_NODATA)
		tx();

	if(resetRequest)
		HAL_NVIC_SystemReset();

	ActualReply.IsSpecial = 0;

	rx();
	if(ActualCommand.Error != TMCL_RX_ERROR_NODATA)
	{
		ExecuteActualCommand();
		return;
	}
}

void tx(void)
{
	uint8_t checkSum = 0;

	uint8_t reply[9];

	if(ActualReply.IsSpecial)
	{
		for(uint8_t i = 0; i < 9; i++)
			reply[i] = ActualReply.Special[i];
	}
	else
	{
		checkSum += SERIAL_HOST_ADDRESS;
		checkSum += SERIAL_MODULE_ADDRESS;
		checkSum += ActualReply.Status;
		checkSum += ActualReply.Opcode;
		checkSum += ActualReply.Value.Byte[3];
		checkSum += ActualReply.Value.Byte[2];
		checkSum += ActualReply.Value.Byte[1];
		checkSum += ActualReply.Value.Byte[0];

		reply[0] = SERIAL_HOST_ADDRESS;
		reply[1] = SERIAL_MODULE_ADDRESS;
		reply[2] = ActualReply.Status;
		reply[3] = ActualReply.Opcode;
		reply[4] = ActualReply.Value.Byte[3];
		reply[5] = ActualReply.Value.Byte[2];
		reply[6] = ActualReply.Value.Byte[1];
		reply[7] = ActualReply.Value.Byte[0];
		reply[8] = checkSum;
	}

	tmcl_transmitt(reply, sizeof(reply));
}

void rx(void)
{
	uint8_t checkSum = 0;
	uint8_t cmd[9];

	const uint32_t CMDLen = tmcl_get_data(cmd, sizeof(cmd));

	if(CMDLen != 9)
	{
		ActualCommand.Error = TMCL_RX_ERROR_NODATA;
		return;
	}

	// todo ADD CHECK 2: check for SERIAL_MODULE_ADDRESS byte ( cmd[0] ) ? (LH)

	for(uint8_t i = 0; i < 8; i++)
		checkSum += cmd[i];

	if(checkSum != cmd[8])
	{
		ActualCommand.Error	= TMCL_RX_ERROR_CHECKSUM;
		return;
	}

	ActualCommand.Opcode         = cmd[1];
	ActualCommand.Type           = cmd[2];
	ActualCommand.Motor          = cmd[3];
	ActualCommand.Value.Byte[3]  = cmd[4];
	ActualCommand.Value.Byte[2]  = cmd[5];
	ActualCommand.Value.Byte[1]  = cmd[6];
	ActualCommand.Value.Byte[0]  = cmd[7];
	ActualCommand.Error          = TMCL_RX_ERROR_NONE;
}

void tmcl_boot()
{
	Evalboards.driverEnable = DRIVER_DISABLE;
	Evalboards.ch1.enableDriver(DRIVER_DISABLE); // todo CHECK 2: the ch1/2 deInit() calls should already disable the drivers - keep this driver disabling to be sure or remove it and leave the disabling to deInit? (LH)
	Evalboards.ch2.enableDriver(DRIVER_DISABLE);

	Evalboards.ch1.deInit();
	Evalboards.ch2.deInit();


	HAL_Delay(500);
}


static void SetGlobalParameter()
{
	switch(ActualCommand.Type)
	{
	case 1:
		//VitalSignsMonitor.errorMask = ActualCommand.Value.Int32;
		break;
	case 2:
		setDriversEnable();
		break;
	case 3:
		switch(ActualCommand.Value.Int32)
		{
		case 0: // normal operation
			//VitalSignsMonitor.debugMode = 0;
			break;
		case 1: // FREE ERROR LED
			//VitalSignsMonitor.debugMode = 1;
			//HAL.LEDs->error.off();
			break;
		default:
			ActualReply.Status = REPLY_INVALID_TYPE;
			break;
		}
		break;
	case 6:
//		if(Evalboards.ch1.onPinChange(HAL.IOs->pins->pins[ActualCommand.Motor], ActualCommand.Value.UInt32)
//				&& Evalboards.ch2.onPinChange(HAL.IOs->pins->pins[ActualCommand.Motor], ActualCommand.Value.UInt32))
//			HAL.IOs->config->setToState(HAL.IOs->pins->pins[ActualCommand.Motor], ActualCommand.Value.UInt32);
		break;
	case 7:
		//ActualReply.Value.UInt32 = spi_setFrequency(&HAL.SPI->ch1, ActualCommand.Value.UInt32);
		break;
	case 8:
		//ActualReply.Value.UInt32 = spi_setFrequency(&HAL.SPI->ch2, ActualCommand.Value.UInt32);
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void GetGlobalParameter()
{
	switch(ActualCommand.Type)
	{
		case 1:
			//ActualReply.Value.Int32 = VitalSignsMonitor.errors;
			ActualReply.Value.Int32 = 0;
			break;
		case 2:
			ActualReply.Value.Int32 = (Evalboards.driverEnable == DRIVER_ENABLE)? 1:0;
			break;
		case 3:
			//ActualReply.Value.Int32 = VitalSignsMonitor.debugMode;
			ActualReply.Value.Int32 = 0;
			break;
		case 4:
			{
				IdAssignmentTypeDef ids;
				ids.ch1.id = Evalboards.ch1.id;
				ids.ch2.id = Evalboards.ch2.id;
				ActualReply.Value.Int32 = Board_supported(&ids);
			}
			break;
		case 5: // Get hardware ID
			//ActualReply.Value.Int32 = hwid;
			ActualReply.Value.Int32 = 0;
			break;
		case 6:
			//ActualReply.Value.UInt32 = HAL.IOs->config->getState(HAL.IOs->pins->pins[ActualCommand.Motor]);
			ActualReply.Value.Int32 = 0;
			break;
		case 7:
			//ActualReply.Value.UInt32 = spi_getFrequency(&HAL.SPI->ch1);
			ActualReply.Value.Int32 = 0;
			break;
		case 8:
			//ActualReply.Value.UInt32 = spi_getFrequency(&HAL.SPI->ch2);
			ActualReply.Value.Int32 = 0;
			break;
		default:
			ActualReply.Status = REPLY_INVALID_TYPE;
			break;
	}
}

static void boardAssignment(void)
{
	uint8_t testOnly = 0;

	IdAssignmentTypeDef ids;
	ids.ch1.id     = (ActualCommand.Value.Int32 >> 0)   & 0xFF;
	ids.ch1.state  = (ActualCommand.Value.Int32 >> 8)   & 0xFF;
	ids.ch2.id     = (ActualCommand.Value.Int32 >> 16)  & 0xFF;
	ids.ch2.state  = (ActualCommand.Value.Int32 >> 24)  & 0xFF;

	switch(ActualCommand.Type)
	{
	case 0:  // auto detect and assign
		checkIDs();
		return;
		break;
	case 1:  // id for channel 2 not changed, reset maybe
		ids.ch2.id     = Evalboards.ch2.id;
		ids.ch2.state  = ID_STATE_WAIT_LOW;
		break;
	case 2:  // id for channel 1 not changed, reset maybe
		ids.ch2.id     = (ActualCommand.Value.Int32 >> 0)  & 0xFF;
		ids.ch2.state  = (ActualCommand.Value.Int32 >> 8)  & 0xFF;
		ids.ch1.id     = Evalboards.ch1.id;
		ids.ch1.state  = ID_STATE_WAIT_LOW;
		break;
	case 3:  // id for both channels
		break;
	case 4:  // test if ids are in firmware
		testOnly = 1;
		if(ActualReply.Value.Int32 == 0)
		{
			ids.ch1.id = Evalboards.ch1.id;
			ids.ch2.id = Evalboards.ch2.id;
		}
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		return;
		break;
	}

	IdAssignmentTypeDef ids_buff;
	ids_buff.ch1.id     = ids.ch1.id;
	ids_buff.ch1.state  = ID_STATE_DONE;
	ids_buff.ch2.id     = ids.ch2.id;
	ids_buff.ch2.state  = ID_STATE_DONE;

	if(!testOnly)
		ActualReply.Value.Int32 = Board_assign(&ids_buff);
	else
		ActualReply.Value.Int32 = Board_supported(&ids_buff);
}

static void boardsErrors(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		ActualReply.Value.Int32 = Evalboards.ch1.errors;
		break;
	case 1:
		ActualReply.Value.Int32 = Evalboards.ch2.errors;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void boardsReset(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		if(!Evalboards.ch1.config->reset())
			ActualReply.Status = REPLY_WRITE_PROTECTED;
		break;
	case 1:
		if(!Evalboards.ch2.config->reset())
			ActualReply.Status = REPLY_WRITE_PROTECTED;
		break;
	case 2:
		if(!Evalboards.ch1.config->reset())
			ActualReply.Status = REPLY_WRITE_PROTECTED;
		if(!Evalboards.ch2.config->reset())
			ActualReply.Status = REPLY_WRITE_PROTECTED;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void boardsMeasuredSpeed(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		ActualReply.Status = Evalboards.ch1.getMeasuredSpeed(ActualCommand.Motor, &ActualReply.Value.Int32);
		break;
	case 1:
		ActualReply.Status = Evalboards.ch2.getMeasuredSpeed(ActualCommand.Motor, &ActualReply.Value.Int32);
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void setDriversEnable()
{
	//vitalsignsmonitor_clearOvervoltageErrors();

	Evalboards.driverEnable = (ActualCommand.Value.Int32) ? DRIVER_ENABLE : DRIVER_DISABLE;
	Evalboards.ch1.enableDriver(DRIVER_USE_GLOBAL_ENABLE);
	Evalboards.ch2.enableDriver(DRIVER_USE_GLOBAL_ENABLE);
}

static void checkIDs(void)
{
	IdAssignmentTypeDef ids = { 0 };

	ids.ch1.state = ID_STATE_DONE;
	ids.ch2.state = ID_STATE_DONE;
	ids.ch1.id = Evalboards.ch1.id;
	ids.ch2.id = Evalboards.ch2.id;
	ActualReply.Value.Int32	= (uint32_t)
	(
		(ids.ch1.id)
		| (ids.ch1.state << 8)
		| (ids.ch2.id    << 16)
		| (ids.ch2.state << 24)
	);
}

static void SoftwareReset(void)
{
	if(ActualCommand.Value.Int32 == 1234)
		resetRequest = true;
}

static void GetVersion(void)
{
	if(ActualCommand.Type == VERSION_FORMAT_ASCII)
	{
		ActualReply.IsSpecial   = 1;
		ActualReply.Special[0]  = SERIAL_HOST_ADDRESS;

		for(uint8_t i = 0; i < 8; i++)
			ActualReply.Special[i+1] = VersionString[i];
	}
	else if(ActualCommand.Type == VERSION_FORMAT_BINARY)
	{
		uint8_t tmpVal;

		// module version high
		tmpVal = (uint8_t) VersionString[0] - '0';	// Ascii digit - '0' = digit value
		tmpVal *= 10;
		tmpVal += (uint8_t) VersionString[1] - '0';
		ActualReply.Value.Byte[3] = tmpVal;

		// module version low
		tmpVal = (uint8_t) VersionString[2] - '0';
		tmpVal *= 10;
		tmpVal += (uint8_t) VersionString[3] - '0';
		ActualReply.Value.Byte[2] = tmpVal;

		// fw version high
		ActualReply.Value.Byte[1] = (uint8_t) VersionString[5] - '0';

		// fw version low
		tmpVal = (uint8_t) VersionString[6] - '0';
		tmpVal *= 10;
		tmpVal += (uint8_t) VersionString[7] - '0';
		ActualReply.Value.Byte[0] = tmpVal;
	}
	//how were the boards detected?	// todo CHECK 2: Doesn't fit into GetVersion. Move somewhere else? Or maybe change GetVersion to GetBoardInfo or something (LH)
	else if(ActualCommand.Type == VERSION_BOARD_DETECT_SRC)
	{
		ActualReply.Value.Byte[0] = IdState.ch1.detectedBy;
		ActualReply.Value.Byte[1] = IdState.ch2.detectedBy;
	}
	else if(ActualCommand.Type == VERSION_BUILD) {
		ActualReply.Value.UInt32 = BUILD_VERSION;
	}
}

static void GetInput(void)
{
	if((Evalboards.ch1.GIO(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32) == TMC_ERROR_NONE)
		|| (Evalboards.ch2.GIO(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32) == TMC_ERROR_NONE))
		return;

	switch(ActualCommand.Type)
	{
	case 0:
		//ActualReply.Value.Int32 = *HAL.ADCs->AIN0;
		ActualReply.Value.Int32 = 0;
		break;
	case 1:
		//ActualReply.Value.Int32 = *HAL.ADCs->AIN1;
		ActualReply.Value.Int32 = 0;
		break;
	case 2:
		//ActualReply.Value.Int32 = *HAL.ADCs->AIN2;
		ActualReply.Value.Int32 = 0;
		break;
	case 3:
		//ActualReply.Value.Int32 = *HAL.ADCs->DIO4;
		ActualReply.Value.Int32 = 0;
		break;
	case 4:
		//ActualReply.Value.Int32 = *HAL.ADCs->DIO5;
		ActualReply.Value.Int32 = 0;
		break;
	case 5:
		//ActualReply.Value.Int32 = VitalSignsMonitor.VM;
		ActualReply.Value.Int32 = 0;
		break;
	case 6:	// Raw VM ADC value, no scaling calculation done // todo QOL 2: Switch this case with case 5? That way we have the raw Values from 0-5, then 6 for scaled VM value. Requires IDE changes (LH)
		//ActualReply.Value.Int32 = *HAL.ADCs->VM;
		ActualReply.Value.Int32 = 0;
		break;
	case 7:
		//ActualReply.Value.Int32 = *HAL.ADCs->AIN_EXT;
		ActualReply.Value.Int32 = 0;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void HandleWlanCommand(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		//ActualReply.Value.Int32 = handleWLANCommand(ActualCommand.Motor, ActualCommand.Value.Int32);
		ActualReply.Value.Int32 = 0;
		break;
	case 1:
		//enableWLANCommandMode();
		break;
	case 2:
		//ActualReply.Value.Int32 = checkReadyToSend();
		ActualReply.Value.Int32 = 0;
		break;
	case 3:
		//ActualReply.Value.Int32 = checkCmdModeEnabled();
		ActualReply.Value.Int32 = 0;
		break;
	case 4:
		//ActualReply.Value.Int32 = getCMDReply();
		ActualReply.Value.Int32 = 0;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void handleRamDebug(void)
{
	switch (ActualCommand.Type)
	{
	case 0:
		debug_init();
		break;
	case 1:
		debug_setSampleCount(ActualCommand.Value.Int32);
		break;
	case 2:
		/* Placeholder: Set sampling time reference*/
		if (ActualCommand.Value.Int32 != 0)
			ActualReply.Status = REPLY_INVALID_VALUE;
		break;
	case 3:
		// RAMDebug expects a divisor value where 1 is original capture frequency,
		// 2 is halved capture frequency etc.
		// The TMCL-IDE sends prescaling values that are one lower than that.
		debug_setPrescaler(ActualCommand.Value.Int32 + 1);
		break;
	case 4:
		if (!debug_setChannel(ActualCommand.Motor, ActualCommand.Value.Int32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 5:
		if (!debug_setTriggerChannel(ActualCommand.Motor, ActualCommand.Value.Int32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 6:
		debug_setTriggerMaskShift(ActualCommand.Value.Int32, ActualCommand.Motor);
		break;
	case 7:
		debug_enableTrigger(ActualCommand.Motor, ActualCommand.Value.Int32);
		break;
	case 8:
		ActualReply.Value.Int32 = debug_getState();
		break;
	case 9:
		if (!debug_getSample(ActualCommand.Value.Int32, (uint32_t *)&ActualReply.Value.Int32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 10:
		ActualReply.Value.Int32 = debug_getInfo(ActualCommand.Value.Int32);
		break;
	case 11:
		if (!debug_getChannelType(ActualCommand.Motor, (uint8_t *) &ActualReply.Value.Int32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 12:
		if (!debug_getChannelAddress(ActualCommand.Motor, (uint32_t *) &ActualReply.Value.Int32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 13:
		debug_setPretriggerSampleCount(ActualCommand.Value.UInt32);
		break;
	case 14:
		ActualReply.Value.UInt32 = debug_getPretriggerSampleCount();
		break;
	case 15:
//		if(Timer.initialized) {
//			Timer.setFrequency(TIMER_CHANNEL_2, ActualCommand.Value.UInt32);
//			ActualReply.Value.UInt32 = Timer.getPeriod(TIMER_CHANNEL_2);
//		}
		ActualReply.Value.UInt32 = 1000;
		break;
	case 16:
		if (!debug_setType(ActualCommand.Value.UInt32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 17:
		if (!debug_setEvalChannel(ActualCommand.Value.UInt32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 18:
		if (!debug_setAddress(ActualCommand.Value.UInt32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 19:
		if (!debug_setTriggerType(ActualCommand.Value.UInt32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 20:
		if (!debug_setTriggerEvalChannel(ActualCommand.Value.UInt32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	case 21:
		if (!debug_setTriggerAddress(ActualCommand.Value.UInt32))
			ActualReply.Status = REPLY_MAX_EXCEEDED;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void handleOTP(void)
{
	switch (ActualCommand.Type)
	{
	case 0: // OTP_INIT
		((ActualCommand.Motor == 1) ? &Evalboards.ch2 : &Evalboards.ch1)->OTP_init();
		break;
	case 1: // OTP_ADDRESS
		((ActualCommand.Motor == 1) ? &Evalboards.ch2 : &Evalboards.ch1)->OTP_address(ActualCommand.Value.UInt32);
		break;
	case 2: // OTP_VALUE
		((ActualCommand.Motor == 1) ? &Evalboards.ch2 : &Evalboards.ch1)->OTP_value(ActualCommand.Value.UInt32);
		break;
	case 3: // OTP_PROGRAM
		((ActualCommand.Motor == 1) ? &Evalboards.ch2 : &Evalboards.ch1)->OTP_program();
		break;
	case 4: // OTP_LOCK
		((ActualCommand.Motor == 1) ? &Evalboards.ch2 : &Evalboards.ch1)->OTP_lock();
		break;
	case 5: // OTP_STATUS
		ActualReply.Value.UInt32 = ((ActualCommand.Motor == 1) ? &Evalboards.ch2 : &Evalboards.ch1)->OTP_status();
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}
