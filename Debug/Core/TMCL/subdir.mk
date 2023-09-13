################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TMCL/TMCL-Variables.c \
../Core/TMCL/TMCL.c \
../Core/TMCL/TMCL_cmds.c \
../Core/TMCL/TMCL_receiver.c 

C_DEPS += \
./Core/TMCL/TMCL-Variables.d \
./Core/TMCL/TMCL.d \
./Core/TMCL/TMCL_cmds.d \
./Core/TMCL/TMCL_receiver.d 

OBJS += \
./Core/TMCL/TMCL-Variables.o \
./Core/TMCL/TMCL.o \
./Core/TMCL/TMCL_cmds.o \
./Core/TMCL/TMCL_receiver.o 


# Each subdirectory must supply rules for building sources it contributes
Core/TMCL/%.o Core/TMCL/%.su Core/TMCL/%.cyclo: ../Core/TMCL/%.c Core/TMCL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/TMCL" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC6100" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/helpers" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ic/TMC4671" -I"C:/Users/admin/Documents/STM32/blackpill_test/Core/tmc/ramp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TMCL

clean-Core-2f-TMCL:
	-$(RM) ./Core/TMCL/TMCL-Variables.cyclo ./Core/TMCL/TMCL-Variables.d ./Core/TMCL/TMCL-Variables.o ./Core/TMCL/TMCL-Variables.su ./Core/TMCL/TMCL.cyclo ./Core/TMCL/TMCL.d ./Core/TMCL/TMCL.o ./Core/TMCL/TMCL.su ./Core/TMCL/TMCL_cmds.cyclo ./Core/TMCL/TMCL_cmds.d ./Core/TMCL/TMCL_cmds.o ./Core/TMCL/TMCL_cmds.su ./Core/TMCL/TMCL_receiver.cyclo ./Core/TMCL/TMCL_receiver.d ./Core/TMCL/TMCL_receiver.o ./Core/TMCL/TMCL_receiver.su

.PHONY: clean-Core-2f-TMCL

