################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Drivers/BSP/Components/ili9341.c \
../Core/Drivers/BSP/Components/stmpe811.c 

OBJS += \
./Core/Drivers/BSP/Components/ili9341.o \
./Core/Drivers/BSP/Components/stmpe811.o 

C_DEPS += \
./Core/Drivers/BSP/Components/ili9341.d \
./Core/Drivers/BSP/Components/stmpe811.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Drivers/BSP/Components/%.o Core/Drivers/BSP/Components/%.su Core/Drivers/BSP/Components/%.cyclo: ../Core/Drivers/BSP/Components/%.c Core/Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/STM32F429I-Discovery" -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/Components" -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/Components/ili9341" -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/Components/stmpe811" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Drivers-2f-BSP-2f-Components

clean-Core-2f-Drivers-2f-BSP-2f-Components:
	-$(RM) ./Core/Drivers/BSP/Components/ili9341.cyclo ./Core/Drivers/BSP/Components/ili9341.d ./Core/Drivers/BSP/Components/ili9341.o ./Core/Drivers/BSP/Components/ili9341.su ./Core/Drivers/BSP/Components/stmpe811.cyclo ./Core/Drivers/BSP/Components/stmpe811.d ./Core/Drivers/BSP/Components/stmpe811.o ./Core/Drivers/BSP/Components/stmpe811.su

.PHONY: clean-Core-2f-Drivers-2f-BSP-2f-Components

