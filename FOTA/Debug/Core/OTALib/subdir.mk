################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/OTALib/Flash.c \
../Core/OTALib/Global.c \
../Core/OTALib/OTA.c \
../Core/OTALib/UART.c 

OBJS += \
./Core/OTALib/Flash.o \
./Core/OTALib/Global.o \
./Core/OTALib/OTA.o \
./Core/OTALib/UART.o 

C_DEPS += \
./Core/OTALib/Flash.d \
./Core/OTALib/Global.d \
./Core/OTALib/OTA.d \
./Core/OTALib/UART.d 


# Each subdirectory must supply rules for building sources it contributes
Core/OTALib/%.o Core/OTALib/%.su: ../Core/OTALib/%.c Core/OTALib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/NCPC/Desktop/Danh/FOTA/FOTA/Core/OTALib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-OTALib

clean-Core-2f-OTALib:
	-$(RM) ./Core/OTALib/Flash.d ./Core/OTALib/Flash.o ./Core/OTALib/Flash.su ./Core/OTALib/Global.d ./Core/OTALib/Global.o ./Core/OTALib/Global.su ./Core/OTALib/OTA.d ./Core/OTALib/OTA.o ./Core/OTALib/OTA.su ./Core/OTALib/UART.d ./Core/OTALib/UART.o ./Core/OTALib/UART.su

.PHONY: clean-Core-2f-OTALib

