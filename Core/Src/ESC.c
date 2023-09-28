/*
 * tmc4671_template.c
 *
 *  Created on: Sep 11, 2023
 *      Author: admin
 */

#include "ESC.h"
#include "main.h"

#include "TMC6100_Register.h"
#include "TMC6100_Fields.h"
#include "TMC6100.h"
// use TMC4671 API
#include "TMC4671.h"



void StartupConfig()
{
	//================================================================================
	// Initialize the driver chip
	//================================================================================
	tmc6100_writeInt(0, TMC6100_GCONF,
			(0 << TMC6100_DISABLE_SHIFT)      | // Enable
			(0 << TMC6100_SINGLELINE_SHIFT)   | // Use individual L+H signals
			(1 << TMC6100_FAULTDIRECT_SHIFT)  | // Fault output shows each protective action
			(1 << TMC6100_CURRENT_ZERO_SHIFT) | // Disable current amplifier;
			(0 << TMC6100_TEST_MODE_SHIFT));
	tmc6100_writeInt(0, TMC6100_GSTAT, 		0x00);
	tmc6100_writeInt(0, TMC6100_DRV_CONF, 	0x04);


	//================================================================================
	// Initialize the motion controller
	//================================================================================


	// Motor type &  PWM configuration
	tmc4671_writeInt(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00030003);
	tmc4671_writeInt(0, TMC4671_PWM_POLARITIES, 0x00000000);
	tmc4671_writeInt(0, TMC4671_PWM_MAXCNT, 0x00000F9F);
	tmc4671_writeInt(0, TMC4671_PWM_BBM_H_BBM_L, 0x00002828);
	tmc4671_writeInt(0, TMC4671_PWM_SV_CHOP, 0x00000007);

	// ADC configuration
	tmc4671_writeInt(0, TMC4671_ADC_I_SELECT, 0x18000100);
	tmc4671_writeInt(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_A, 0x20000000);
	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_B, 0x00000000);
	tmc4671_writeInt(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);
	tmc4671_writeInt(0, TMC4671_ADC_I0_SCALE_OFFSET, 0x010081D2);
	tmc4671_writeInt(0, TMC4671_ADC_I1_SCALE_OFFSET, 0x0100819D);

	// Digital hall settings
	tmc4671_writeInt(0, TMC4671_HALL_MODE, 0x00000110);
	tmc4671_writeInt(0, TMC4671_HALL_PHI_E_PHI_M_OFFSET, 0x75307530);

	// Feedback selection
	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000005);
	tmc4671_writeInt(0, TMC4671_VELOCITY_SELECTION, 0x0000000C);

	// Limits
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 0x000003E8);

	// PI settings
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_P_TORQUE_I, 0x01000100);
	tmc4671_writeInt(0, TMC4671_PID_FLUX_P_FLUX_I, 0x01000100);

	// ===== Digital hall test drive =====

	// Switch to torque mode
	tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000001);

	// Rotate right
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_TARGET, 0x03E80000);
	HAL_Delay(3000);

	// Rotate left
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_TARGET, 0xFC180000);
	HAL_Delay(3000);

	// Stop
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_TARGET, 0x00000000);

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

