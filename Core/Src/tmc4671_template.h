/*
 * tmc4671_template.h
 *
 *  Created on: Sep 11, 2023
 *      Author: admin
 */

#ifndef _TMC4671_TEMPLATE_H_
#define _TMC4671_TEMPLATE_H_
#include "stdint.h"

void StartupConfig();
void openloop_test_drive(uint32_t UQ_UD_target);
void ABN_encoder_test_drive(uint32_t UQ_UD_target);
void Exp_speed_ramp();


#endif /* _TMC4671_TEMPLATE_H_ */
