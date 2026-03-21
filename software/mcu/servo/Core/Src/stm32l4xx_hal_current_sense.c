/*
 * stm32l4xx_hal_current_sense.c
 *
 *  Created on: Feb 27, 2026
 *      Author: farfar
 */
#include "stm32l4xx_hal_current_sense.h"

static inline float HAL_CURRENT_adc_to_amps(int32_t adc_minus_offset)
{
	float volts = ((float)adc_minus_offset * CS_VREF) / CS_ADC_MAX;
	return volts / CS_VOLT_PER_AMP;
}

static void HAL_CURRENT_process_block(currentsense_t *cs, const uint16_t *p, uint32_t n)
{
	// 1) Offset capture phase
	if (!cs->offset_ready)
	{
		for (uint32_t i = 0; i < n; i++)
		{
			cs->offset_sum += p[i];
			cs->offset_count++;
			if (cs->offset_count >= CS_OFFSET_SAMPLES)
			{
				cs->offset_adc = (int32_t)(cs->offset_sum / cs->offset_count);
				cs->offset_ready = 1;
				break;
			}
		}
		return;
	}

	// 2) Normal phase: block average
	int32_t sum = 0;
	for (uint32_t i = 0; i < n; i++)
	{
		sum += (int32_t)p[i];
	}
	cs->i_avg = HAL_CURRENT_adc_to_amps(sum/n-cs->offset_adc);
	cs->meas_ready = 1;
}

void HAL_CURRENT_init(currentsense_t *cs, ADC_HandleTypeDef *hadc, TIM_HandleTypeDef *htim)
{
    cs->hadc = hadc;
    cs->htim = htim;

    cs->offset_count = 0;
    cs->offset_sum = 0;
    cs->offset_adc = 0;
    cs->offset_ready = 0;

    cs->i_avg = 0.0f;
    cs->meas_ready = 0;

}

HAL_StatusTypeDef HAL_CURRENT_start(currentsense_t *cs)
{
    // Start PWM channels elsewhere if you want; but you *must* ensure CH3 is running.

    // ADC calibration then start DMA
    if (HAL_ADCEx_Calibration_Start(cs->hadc, ADC_SINGLE_ENDED) != HAL_OK)
        return HAL_ERROR;

    return HAL_ADC_Start_DMA(cs->hadc, (uint32_t*)cs->buf, CS_BUF_LEN);
}

void HAL_CURRENT_OnHalfTransferISR(currentsense_t *cs, ADC_HandleTypeDef *hadc)
{
    if (hadc != cs->hadc) return;
    HAL_CURRENT_process_block(cs, &cs->buf[0], CS_BUF_LEN/2);
}

void HAL_CURRENT_OnFullTransferISR(currentsense_t *cs, ADC_HandleTypeDef *hadc)
{
    if (hadc != cs->hadc) return;
    HAL_CURRENT_process_block(cs, &cs->buf[CS_BUF_LEN/2], CS_BUF_LEN/2);
}

