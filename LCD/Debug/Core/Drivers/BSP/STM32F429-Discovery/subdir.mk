################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.c \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.c \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.c \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.c \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.c \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.c \
../Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.c 

OBJS += \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.o \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.o \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.o \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.o \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.o \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.o \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.o 

C_DEPS += \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.d \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.d \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.d \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.d \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.d \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.d \
./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Drivers/BSP/STM32F429-Discovery/%.o Core/Drivers/BSP/STM32F429-Discovery/%.su Core/Drivers/BSP/STM32F429-Discovery/%.cyclo: ../Core/Drivers/BSP/STM32F429-Discovery/%.c Core/Drivers/BSP/STM32F429-Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/STM32F429I-Discovery" -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/Components" -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/Components/ili9341" -I"C:/Users/Danie/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/BSP/Components/stmpe811" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Drivers-2f-BSP-2f-STM32F429-2d-Discovery

clean-Core-2f-Drivers-2f-BSP-2f-STM32F429-2d-Discovery:
	-$(RM) ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery.su ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_eeprom.su ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_gyroscope.su ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_io.su ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_lcd.su ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_sdram.su ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.cyclo ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.d ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.o ./Core/Drivers/BSP/STM32F429-Discovery/stm32f429i_discovery_ts.su

.PHONY: clean-Core-2f-Drivers-2f-BSP-2f-STM32F429-2d-Discovery

