################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/tmc/ic/TMC4671/TMC4671.c 

C_DEPS += \
./Core/tmc/ic/TMC4671/TMC4671.d 

OBJS += \
./Core/tmc/ic/TMC4671/TMC4671.o 


# Each subdirectory must supply rules for building sources it contributes
Core/tmc/ic/TMC4671/%.o Core/tmc/ic/TMC4671/%.su Core/tmc/ic/TMC4671/%.cyclo: ../Core/tmc/ic/TMC4671/%.c Core/tmc/ic/TMC4671/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/TMCL" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC6100" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/helpers" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC4671" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ramp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-tmc-2f-ic-2f-TMC4671

clean-Core-2f-tmc-2f-ic-2f-TMC4671:
	-$(RM) ./Core/tmc/ic/TMC4671/TMC4671.cyclo ./Core/tmc/ic/TMC4671/TMC4671.d ./Core/tmc/ic/TMC4671/TMC4671.o ./Core/tmc/ic/TMC4671/TMC4671.su

.PHONY: clean-Core-2f-tmc-2f-ic-2f-TMC4671

