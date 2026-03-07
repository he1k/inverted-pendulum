################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL_MotorDriver/stm32l4xx_hal_motor_driver.c 

OBJS += \
./HAL_MotorDriver/stm32l4xx_hal_motor_driver.o 

C_DEPS += \
./HAL_MotorDriver/stm32l4xx_hal_motor_driver.d 


# Each subdirectory must supply rules for building sources it contributes
HAL_MotorDriver/%.o HAL_MotorDriver/%.su HAL_MotorDriver/%.cyclo: ../HAL_MotorDriver/%.c HAL_MotorDriver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/farfar/Documents/STM32-Projects/Athena/HAL_CurrentSense" -I"C:/Users/farfar/Documents/STM32-Projects/Athena/HAL_MotorDriver" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-HAL_MotorDriver

clean-HAL_MotorDriver:
	-$(RM) ./HAL_MotorDriver/stm32l4xx_hal_motor_driver.cyclo ./HAL_MotorDriver/stm32l4xx_hal_motor_driver.d ./HAL_MotorDriver/stm32l4xx_hal_motor_driver.o ./HAL_MotorDriver/stm32l4xx_hal_motor_driver.su

.PHONY: clean-HAL_MotorDriver

