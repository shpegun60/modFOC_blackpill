/*
 * TMCL-Variables.h
 *
 *  Created on: 01.04.2020
 *      Author: OK / ED
 */

#ifndef TMCL_VARIABLES_H
#define TMCL_VARIABLES_H

#include "stdint.h"
#include "TMCL-Defines.h"

extern uint8_t TMCLReplyFormat;
extern uint8_t SpecialReply[9];
extern uint8_t TMCLSuppressReply;

extern TTMCLCommand ActualCommand;
extern TTMCLReply ActualReply;

#endif /* TMCL_VARIABLES_H */
