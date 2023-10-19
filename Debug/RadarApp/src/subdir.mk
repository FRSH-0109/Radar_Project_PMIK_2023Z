################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RadarApp/src/servo_driver.c 

OBJS += \
./RadarApp/src/servo_driver.o 

C_DEPS += \
./RadarApp/src/servo_driver.d 


# Each subdirectory must supply rules for building sources it contributes
RadarApp/src/%.o RadarApp/src/%.su RadarApp/src/%.cyclo: ../RadarApp/src/%.c RadarApp/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/studia/pmik/Radar_PMIK_STM32G030/RadarApp/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-RadarApp-2f-src

clean-RadarApp-2f-src:
	-$(RM) ./RadarApp/src/servo_driver.cyclo ./RadarApp/src/servo_driver.d ./RadarApp/src/servo_driver.o ./RadarApp/src/servo_driver.su

.PHONY: clean-RadarApp-2f-src

