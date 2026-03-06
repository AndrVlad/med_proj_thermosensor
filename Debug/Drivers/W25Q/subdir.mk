################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/W25Q/w25q_spi.c 

OBJS += \
./Drivers/W25Q/w25q_spi.o 

C_DEPS += \
./Drivers/W25Q/w25q_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/W25Q/%.o Drivers/W25Q/%.su Drivers/W25Q/%.cyclo: ../Drivers/W25Q/%.c Drivers/W25Q/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/W25Q -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-W25Q

clean-Drivers-2f-W25Q:
	-$(RM) ./Drivers/W25Q/w25q_spi.cyclo ./Drivers/W25Q/w25q_spi.d ./Drivers/W25Q/w25q_spi.o ./Drivers/W25Q/w25q_spi.su

.PHONY: clean-Drivers-2f-W25Q

