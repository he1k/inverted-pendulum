#include "stm32l4xx_hal_motor_driver.h"
#include <stdint.h>
#include "main.h"

static inline uint32_t us_to_tim1_ticks(uint32_t us) { return us * 80u; } // TIM1=80MHz

uint8_t HAL_MOTOR_fault(){
	return HAL_GPIO_ReadPin(nFAULT_GPIO_Port, nFAULT_Pin);
}
// IN1 | IN2 | OUT1 | OUT2
// -----------------------
//  0  |  0  |  L   |  L
//  0  |  1  |  L   |  H
//  1  |  0  |  H   |  L
//  1  |  1  |  H   |  H

// Dir 0: IN1 = 0, IN2 = PWM
// Dir 1: IN2 = 0, IN1 = PWM
void HAL_MOTOR_write_in1(motordriver_t *md, uint32_t val)
{
	if(val > PWM_MAX)
	{
		md->htim->Instance->CCR1 = PWM_MAX;
	}else
	{
		md->htim->Instance->CCR1 = val;
	}
}
void HAL_MOTOR_write_in2(motordriver_t *md, uint32_t val)
{
	if(val > PWM_MAX)
	{
		md->htim->Instance->CCR2 = PWM_MAX;
	}else
	{
		md->htim->Instance->CCR2 = val;
	}
}
void HAL_MOTOR_init(motordriver_t *md, TIM_HandleTypeDef *htim, uint8_t mode, uint16_t *adc_buf)
{
	//---------------- Disable driver and setup fault
	md->DISABLE = 1;
	HAL_GPIO_WritePin(DISABLE_GPIO_Port, DISABLE_Pin, 1); // Disable motor driver on startup
	md->nFAULT = 1;
	md->mode = mode;
	//---------------- ADC setup
	md->adc_buf = adc_buf;
	md->en_adc = 0;
	//---------------- Timer setup
	md->htim = htim;
	if(HAL_TIM_PWM_Start(md->htim, TIM_CHANNEL_1) != HAL_OK)
	{ // TODO Find better solution for errors
		while(1){}
	}
	if(mode == MODE_ANTI_PHASE)
	{ // Star channel 2 as complementary
		if(HAL_TIMEx_PWMN_Start(md->htim, TIM_CHANNEL_1) != HAL_OK)
		{ // TODO Find better solution for errors
			while(1){}
		}
	}else
	{
		if(HAL_TIM_PWM_Start(md->htim, TIM_CHANNEL_2) != HAL_OK)
		{ // TODO Find better solution for errors
			while(1){}
		}
	}
	if(HAL_TIM_PWM_Start(md->htim, TIM_CHANNEL_3) != HAL_OK)
	{ // TODO Find better solution for errors
		while(1){}
	}
	md->htim->Instance->ARR = PWM_MAX;  // Reload value for all channels
	md->htim->Instance->CCR3 = 512; // DELETE
}
void HAL_MOTOR_write(motordriver_t *md, uint32_t val, uint8_t dir)
{
	if(md->mode == MODE_REGULAR) // Normal mode: pwm signal for motor voltage on pwm pin
															 // non pwm high and low signal for direction on dir pin
	{
		HAL_MOTOR_write_in2(md, dir ? 0   : val);
		HAL_MOTOR_write_in1(md, dir ? val : 0);
		if (md->en_adc) // Update ADC timer if enabled
		{
			uint32_t ccr1 = md->htim->Instance->CCR1;
			uint32_t ccr2 = md->htim->Instance->CCR2;
			uint32_t arr = md->htim->Instance->ARR;
			uint32_t ccr3;
			uint32_t ccr_active = dir ? ccr1 : ccr2;
			uint32_t margin = 160;
			if (ccr_active > 2*margin) {
					ccr3 = ccr_active/2;
					if (ccr3 < margin) ccr3 = margin;
					if (ccr3 > ccr_active - margin) ccr3 = ccr_active - margin;
			} else {
					// duty too small to sample safely "inside" on-time
					ccr3 = arr/2; // or skip sampling when duty is tiny
			}
			md->htim->Instance->CCR3 = ccr3;
		}
	}else if(md->mode == MODE_ANTI_PHASE) // Locked anti-phase mode: pwm signal on dir pin
																				// for motor voltage (50% duty cycle -> 0V)
																				// 100% duty cycle on pwm pin
	{
		// NOT IMPLEMENTED!!
		md->htim->Instance->CCR2 = PWM_MAX; // 100% duty cycle on pwm pin
		int32_t val_adj;
		if(dir)
		{
			val_adj = PWM_MAX/2 + val;
			if(val_adj > PWM_MAX)
			{
				val_adj = PWM_MAX;
			}
		}else
		{
			val_adj = PWM_MAX/2-1;
			if(val_adj < PWM_MIN)
			{
				val_adj = PWM_MIN;
			}
		}
		HAL_MOTOR_write_in2(md, val_adj);
		HAL_MOTOR_write_in1(md, val_adj);
	}
}
void HAL_MOTOR_enable(motordriver_t *md)
{
	md->DISABLE = 0;
	HAL_GPIO_WritePin(DISABLE_GPIO_Port, DISABLE_Pin, 0);
	//HAL_ADC_Start();
}
void HAL_MOTOR_disable(motordriver_t *md)
{
	md->DISABLE = 1;
	HAL_GPIO_WritePin(DISABLE_GPIO_Port, DISABLE_Pin, 1);
}


