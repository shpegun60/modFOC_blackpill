/*
 * tmcl_debug_timer.c
 *
 *  Created on: Sep 30, 2023
 *      Author: admin
 */

#include "tmcl_debug_timer.h"

#include "tim.h"

#define TIMER_INST (&htim10)
#define TIMER_BASE_CLK 60000000

static uint16_t dbg_period_min = 0;
static float dbg_freq_min = 0.0f;
static timer_overflow_t dbg_overflow_foo = NULL;

void setOverflowCallback(timer_overflow_t foo)
{
	dbg_overflow_foo = foo;
}

int debugTimerIsInitialized(void)
{
	int state = (HAL_TIM_Base_GetState(TIMER_INST) == HAL_TIM_STATE_READY) ? 1 : 0;
	return state;
}

void setDebugPeriodMin(uint16_t period_min)
{
	dbg_period_min = period_min;
}

void setFrequencyMin(float freq_min)
{
	dbg_freq_min = freq_min;
}

uint16_t getDebugPeriod()
{
	return __HAL_TIM_GET_AUTORELOAD(TIMER_INST);
}


void setDebugFrequency(float freq)
{
	const float freq_min = dbg_freq_min;
	const uint16_t period_min = dbg_period_min;

	uint16_t prescaler = 0;
	uint16_t period = 0xFFFF;

	if(freq < 0.01)
		return;

	if(freq > (float)TIMER_BASE_CLK)
		return;

	for(; prescaler < 0xFFFF; ++prescaler)
	{
		if(freq > ((float)TIMER_BASE_CLK / ((prescaler + 1) * period)))
		{
			period = (float)TIMER_BASE_CLK / ((prescaler + 1) * freq);
			if(period < period_min)
			    period = (float)TIMER_BASE_CLK / (prescaler * freq);
			break;
		}
	}

	__HAL_TIM_SET_PRESCALER(TIMER_INST, prescaler);
	//timer_prescaler_config(timer, prescaler, TIMER_PSC_RELOAD_NOW);

	__HAL_TIM_SET_AUTORELOAD(TIMER_INST, period);
	//timer_autoreload_value_config(timer, period);

	(void)freq_min;
}

void debugTimerStart(void)
{
	HAL_TIM_Base_Start_IT(TIMER_INST);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIMER_INST->Instance)
  {
      if(dbg_overflow_foo != NULL) {
    	  dbg_overflow_foo();
      }
  }
}

