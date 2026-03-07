/*
 * stm32l4xx_hal_motor_driver.h
 *
 *  Created on: Feb 27, 2026
 *      Author: farfar
 *  Notes:
 *  	Conversion time: T_conv = (T_sample + T_convert)/f_ADC
 *  	f_ADC = 80 MHz
 *  	T_convert = 12.5 cycles (approx)
 *  	T_sample = 24.5 cycles (current)
 *  	N_over = 4x (current)
 *  	-> T_conv = 0.3375 us (current)
 *
 *  	Time per sample: T_adc = T_conv*N_over
 *  	-> T_adc = 1.35 us
 *
 *
 *
 *
 */

#ifndef STM32L4XX_HAL_CURRENT_SENSE_H_
#define STM32L4XX_HAL_CURRENT_SENSE_H_
#include <stdint.h>
#include <math.h>
#include "stm32l4xx_hal.h"
#define CS_BUF_LEN 16
#define CS_VREF 3.3f      // 3.3 V reference, 1.65 V midscale
#define CS_VOLT_PER_AMP 0.3f // 300 mv / A
#define CS_RES 12
#define CS_ADC_MAX (float) (pow(2, CS_RES)-1.0)
#define CS_OFFSET_SAMPLES 200
typedef struct{
	ADC_HandleTypeDef *hadc;
	TIM_HandleTypeDef *htim;
	uint16_t buf[CS_BUF_LEN];

	uint32_t offset_count;
	uint64_t offset_sum;
	int32_t  offset_adc;
	uint8_t  offset_ready;

	float i_avg;
	uint8_t meas_ready;



} currentsense_t;
void HAL_CURRENT_OnHalfTransferISR(currentsense_t *cs, ADC_HandleTypeDef *hadc);
void HAL_CURRENT_OnFullTransferISR(currentsense_t *cs, ADC_HandleTypeDef *hadc);
void HAL_CURRENT_init(currentsense_t *cs, ADC_HandleTypeDef *hadc, TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_CURRENT_start(currentsense_t *cs);
static inline uint8_t HAL_CURRENT_meas_ready(currentsense_t *cs) { return cs->meas_ready; }
static inline float HAL_CURRENT_meas_get(currentsense_t *cs) { cs->meas_ready = 0; return cs->i_avg; }
#endif /* STM32L4XX_HAL_CURRENT_SENSE_H_ */
