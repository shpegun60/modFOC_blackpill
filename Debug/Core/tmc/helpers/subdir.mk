################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/tmc/helpers/CRC.c \
../Core/tmc/helpers/Functions.c 

C_DEPS += \
./Core/tmc/helpers/CRC.d \
./Core/tmc/helpers/Functions.d 

OBJS += \
./Core/tmc/helpers/CRC.o \
./Core/tmc/helpers/Functions.o 


# Each subdirectory must supply rules for building sources it contributes
Core/tmc/helpers/%.o Core/tmc/helpers/%.su Core/tmc/helpers/%.cyclo: ../Core/tmc/helpers/%.c Core/tmc/helpers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/TMCL" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC6100" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/helpers" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC4671" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ramp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-tmc-2f-helpers

clean-Core-2f-tmc-2f-helpers:
	-$(RM) ./Core/tmc/helpers/CRC.cyclo ./Core/tmc/helpers/CRC.d ./Core/tmc/helpers/CRC.o ./Core/tmc/helpers/CRC.su ./Core/tmc/helpers/Functions.cyclo ./Core/tmc/helpers/Functions.d ./Core/tmc/helpers/Functions.o ./Core/tmc/helpers/Functions.su

.PHONY: clean-Core-2f-tmc-2f-helpers

