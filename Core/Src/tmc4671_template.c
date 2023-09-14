/*
 * tmc4671_template.c
 *
 *  Created on: Sep 11, 2023
 *      Author: admin
 */

#include "tmc4671_template.h"
//#include "tmc/ic/TMC4671/TMC4671.h"
#include "main.h"
#include "spi.h"
//#include "TMC6100_Register.h"
//#include "TMC6100_Fields.h"
//#include "TMC6100.h"

//tmc6100_t tmc6100;
void StartupConfig()
{
//	// Initialize the driver chip
//	tmc6100_init(&tmc6100, &hspi1, CS_DRV_GPIO_Port, CS_DRV_Pin);
//	tmc6100_writeInt(&tmc6100, TMC6100_GCONF,
//			(0 << TMC6100_DISABLE_SHIFT)      | // Enable
//			(0 << TMC6100_SINGLELINE_SHIFT)   | // Use individual L+H signals
//			(1 << TMC6100_FAULTDIRECT_SHIFT)  | // Fault output shows each protective action
//			(1 << TMC6100_CURRENT_ZERO_SHIFT) | // Disable current amplifier;
//			(0 << TMC6100_TEST_MODE_SHIFT));
//	tmc6100_writeInt(&tmc6100, TMC6100_GSTAT, 		0x00);
//	tmc6100_writeInt(&tmc6100, TMC6100_DRV_CONF, 	0x04);
//
//	//================================================================================
//	//Prototype motor init and movement START
//	//================================================================================
//
//	// Motor type &  PWM configuration
//	tmc4671_writeInt(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS,
//			(0x03 << TMC4671_N_POLE_PAIRS_SHIFT) | 	//3 pole pairs
//			(0x03 << TMC4671_MOTOR_TYPE_SHIFT)); 	//Three phase BLDC
//	tmc4671_writeInt(0, TMC4671_PWM_POLARITIES, 	0x00000000);	//low and low
//	tmc4671_writeInt(0, TMC4671_PWM_MAXCNT, 		0x00000F9F);	//3999 for 25kHz ( fPWM[Hz] = 100MHz/(PWM_MAXCNT+1) )
//	tmc4671_writeInt(0, TMC4671_PWM_BBM_H_BBM_L, 	0x00005050);	//80 * 10ns for 800ns; first two digits for Low, second two for High deadtime
//	tmc4671_writeInt(0, TMC4671_PWM_SV_CHOP, 		0x00000007);	//FOC PWM Enabled with SVM disabled; 0x00000107 for SVM
//
//	HAL_GPIO_WritePin(CTRL_EN_GPIO_Port, CTRL_EN_Pin, GPIO_PIN_SET);			//Enable TMC4671
//
////	// ADC configuration
////	tmc4671_writeInt(0, TMC4671_ADC_I_SELECT, 0x18000100);
////	tmc4671_writeInt(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
////	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_A, 0x0051EB85);
////	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_B, 0x0051EB85);
////	tmc4671_writeInt(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x00540054);
////	tmc4671_writeInt(0, TMC4671_ADC_I0_SCALE_OFFSET, 0x010081CB);
////	tmc4671_writeInt(0, TMC4671_ADC_I1_SCALE_OFFSET, 0x010081F9);
//
//	// Digital hall settings
////	tmc4671_writeInt(0,  TMC4671_HALL_MODE, 0x00001001);
////	tmc4671_writeInt(0,  TMC4671_HALL_PHI_E_PHI_M_OFFSET, 0x1F400000);
////	// ABN encoder settings
////	tmc4671_writeInt(0, TMC4671_ABN_DECODER_MODE, 0x00000000);
////	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PPR, 0x00000800);					//8192 ppr
////	tmc4671_writeInt(0, TMC4671_ABN_DECODER_COUNT, 0x00000539);
////	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
////	// Limits
////	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 8000);
////	//set PI constants
////	tmc4671_setTorqueFluxPI(0, 700, 20);
////	tmc4671_setVelocityPI(0, 15000, 20000);
////	tmc4671_setPositionPI(0, 80, 4);
//
//	//reset encoder position
//	//tmc4671_setActualPosition(0, 0);
//	//HAL_GPIO_WritePin(GPIOA, TMC_OK_LED_Pin, GPIO_PIN_SET);
//	//HAL_GPIO_WritePin(GPIOC, FAULT_LED_Pin, GPIO_PIN_SET);
//	//HAL_GPIO_WritePin(GPIOF, DEBUG_LED_Pin, GPIO_PIN_SET);
//	//start new testing procedure

}

void openloop_test_drive(uint32_t UQ_UD_target)
{
//	// Open loop settings -------------------------------------------------------------------------------------
//	// Feedback selection
//	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 		0x00000002); 	//set to phi_e_openloop
//	// Switch to open loop velocity mode
//	tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 	0x00000008); 	// set to uq_ud_ext mode
//	tmc4671_writeInt(0, TMC4671_UQ_UD_EXT,
//			(0x01 << TMC4671_UD_EXT_SHIFT) | 							// UD_EXT = 1
//			(0x00 << TMC4671_UQ_EXT_SHIFT));							// UQ_EXT = 0
//	tmc4671_writeInt(0, TMC4671_OPENLOOP_ACCELERATION, 	0x0000003C);	// set open loop acceleration to 60 rpm/s
//	//tmc4671_writeInt(0, TMC4671_OPENLOOP_VELOCITY_TARGET, -30);			// set velocity to -30 rpm
//	//tmc4671_writeInt(0, TMC4671_OPENLOOP_MODE, 			0x00000000);	// direction - positive
//	HAL_Delay(100);
//	tmc4671_writeInt(0, TMC4671_OPENLOOP_VELOCITY_TARGET, 	100);
//	tmc4671_writeInt(0, TMC4671_UQ_UD_EXT, 					0x0FA00000);
////	//tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000002);
////
////	tmc4671_writeInt(0, TMC4671_UQ_UD_EXT, 0x106F106F);
////	// ===== Open loop test drive =====
////	// Rotate right
////	//tmc4671_writeInt(0, TMC4671_OPENLOOP_VELOCITY_TARGET, 0x0000015C);
////	//tmc4671_setTargetVelocity(0, 100);												//Rotate right
////	HAL_Delay(5000);
////	// Rotate left
//////	tmc4671_writeInt(0, TMC4671_OPENLOOP_VELOCITY_TARGET, 0xFFFFFFC4);
//////	HAL_Delay(2000);
////	// Stop
////	//tmc4671_writeInt(0, TMC4671_OPENLOOP_VELOCITY_TARGET, 0x00000000);
//////	HAL_Delay(1000);
//////	tmc4671_writeInt(0, TMC4671_UQ_UD_EXT, 0x00000000);
}

void ABN_encoder_test_drive(uint32_t UQ_UD_target)
{
//	// ===== ABN encoder test drive =====
//	// Init encoder (mode 0)
//	tmc4671_writeInt(0, TMC4671_PWM_SV_CHOP, 0x00000007);
//	tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000008);			//open loop velocity mode
//	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);	//reset the offset
//	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000001);					//phi_e_ext - open loop
//	tmc4671_writeInt(0, TMC4671_PHI_E_EXT, 0x00000000);
//	tmc4671_writeInt(0, TMC4671_UQ_UD_EXT, UQ_UD_target);
//	HAL_Delay(2000);
//	//HAL_GPIO_WritePin(GPIOA, TMC_OK_LED_Pin, GPIO_PIN_RESET);
//	tmc4671_writeInt(0, TMC4671_ABN_DECODER_COUNT, 0x00000000);
//	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000003);						//Feedback selection phi_e_abn - use ABN encoder for electrical phase
//	tmc4671_writeInt(0, TMC4671_VELOCITY_SELECTION, 0x00000009);
//	tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);				// Switch to velocity mode
//	/*
//	 tmc4671_setTargetVelocity(0, 200);												//Rotate right
//	 HAL_Delay(2000);
//	 tmc4671_setTargetVelocity(0, 0);
//	 HAL_Delay(1000);
//	 tmc4671_setTargetVelocity(0, -200);											//Rotate left
//	 HAL_Delay(2000);
//	 tmc4671_setTargetVelocity(0, 0);												// Stop
}

void Exp_speed_ramp()
{
//	//velocity mode test drive - crude speed ramp - TO BE TESTED
//	tmc4671_setTorqueFluxLimit_mA(0, 256, 80*141);								//8A * sqrt(2)
//	tmc4671_writeInt(0, TMC4671_VELOCITY_SELECTION, 0x00000009);				//mechanical velocity selection, standard mode
//	tmc4671_setTargetVelocity(0, 100);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 250);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 500);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 750);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 1000);
//	HAL_Delay(5000);
//	tmc4671_setTargetVelocity(0, 750);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 500);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 250);
//	HAL_Delay(200);
//	tmc4671_setTargetVelocity(0, 0);
//	HAL_Delay(200);
}

