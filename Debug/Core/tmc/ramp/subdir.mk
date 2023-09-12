################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/tmc/ramp/LinearRamp.c \
../Core/tmc/ramp/LinearRamp1.c \
../Core/tmc/ramp/Ramp.c 

C_DEPS += \
./Core/tmc/ramp/LinearRamp.d \
./Core/tmc/ramp/LinearRamp1.d \
./Core/tmc/ramp/Ramp.d 

OBJS += \
./Core/tmc/ramp/LinearRamp.o \
./Core/tmc/ramp/LinearRamp1.o \
./Core/tmc/ramp/Ramp.o 


# Each subdirectory must supply rules for building sources it contributes
Core/tmc/ramp/%.o Core/tmc/ramp/%.su Core/tmc/ramp/%.cyclo: ../Core/tmc/ramp/%.c Core/tmc/ramp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/TMCL" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC6100" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/helpers" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC4671" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ramp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-tmc-2f-ramp

clean-Core-2f-tmc-2f-ramp:
	-$(RM) ./Core/tmc/ramp/LinearRamp.cyclo ./Core/tmc/ramp/LinearRamp.d ./Core/tmc/ramp/LinearRamp.o ./Core/tmc/ramp/LinearRamp.su ./Core/tmc/ramp/LinearRamp1.cyclo ./Core/tmc/ramp/LinearRamp1.d ./Core/tmc/ramp/LinearRamp1.o ./Core/tmc/ramp/LinearRamp1.su ./Core/tmc/ramp/Ramp.cyclo ./Core/tmc/ramp/Ramp.d ./Core/tmc/ramp/Ramp.o ./Core/tmc/ramp/Ramp.su

.PHONY: clean-Core-2f-tmc-2f-ramp

