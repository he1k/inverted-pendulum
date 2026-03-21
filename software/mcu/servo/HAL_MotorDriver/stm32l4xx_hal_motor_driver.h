/*
 * stm32l4xx_hal_motor_driver.h
 *
 *  Created on: Aug 26, 2023
 *      Author: farfar
 */

#ifndef STM32L4XX_HAL_MOTOR_DRIVER_H_
#define STM32L4XX_HAL_MOTOR_DRIVER_H_
#include <stdint.h>
#include <math.h>
#include "usb_device.h"
#define PWM_RES 10
#define PWM_MIN 0
#define PWM_MAX (pow(2, PWM_RES)-1.0)
#define MODE_REGULAR 0
#define MODE_ANTI_PHASE 1
#define CH_PWM TIM_CHANNEL_2
#define CH_DIR TIM_CHANNEL_1
// CCR1 -> IN1
// CCR2 -> IN2
typedef struct{
	uint8_t nFAULT;
	uint8_t DISABLE;
	uint8_t mode;
	TIM_HandleTypeDef *htim;
	uint16_t *adc_buf;
	uint8_t en_adc;
} motordriver_t;
void HAL_MOTOR_init(motordriver_t *md, TIM_HandleTypeDef *htim, uint8_t mode, uint16_t *adc_buf);
void HAL_MOTOR_write(motordriver_t *md, uint32_t val, uint8_t dir);
void HAL_MOTOR_enable(motordriver_t *md);
void HAL_MOTOR_disable(motordriver_t *md);
#endif /* STM32L4XX_HAL_MOTOR_DRIVER_H_ */


