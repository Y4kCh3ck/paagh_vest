################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/AHT20.c \
../Core/Src/App/MAX30102.c \
../Core/Src/App/NEO6.c \
../Core/Src/App/nRF24.c \
../Core/Src/App/pv_app.c \
../Core/Src/App/pv_pm.c \
../Core/Src/App/pv_pulse.c \
../Core/Src/App/pv_sensors.c 

OBJS += \
./Core/Src/App/AHT20.o \
./Core/Src/App/MAX30102.o \
./Core/Src/App/NEO6.o \
./Core/Src/App/nRF24.o \
./Core/Src/App/pv_app.o \
./Core/Src/App/pv_pm.o \
./Core/Src/App/pv_pulse.o \
./Core/Src/App/pv_sensors.o 

C_DEPS += \
./Core/Src/App/AHT20.d \
./Core/Src/App/MAX30102.d \
./Core/Src/App/NEO6.d \
./Core/Src/App/nRF24.d \
./Core/Src/App/pv_app.d \
./Core/Src/App/pv_pm.d \
./Core/Src/App/pv_pulse.d \
./Core/Src/App/pv_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su Core/Src/App/%.cyclo: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L071xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -u _scanf_float -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/AHT20.cyclo ./Core/Src/App/AHT20.d ./Core/Src/App/AHT20.o ./Core/Src/App/AHT20.su ./Core/Src/App/MAX30102.cyclo ./Core/Src/App/MAX30102.d ./Core/Src/App/MAX30102.o ./Core/Src/App/MAX30102.su ./Core/Src/App/NEO6.cyclo ./Core/Src/App/NEO6.d ./Core/Src/App/NEO6.o ./Core/Src/App/NEO6.su ./Core/Src/App/nRF24.cyclo ./Core/Src/App/nRF24.d ./Core/Src/App/nRF24.o ./Core/Src/App/nRF24.su ./Core/Src/App/pv_app.cyclo ./Core/Src/App/pv_app.d ./Core/Src/App/pv_app.o ./Core/Src/App/pv_app.su ./Core/Src/App/pv_pm.cyclo ./Core/Src/App/pv_pm.d ./Core/Src/App/pv_pm.o ./Core/Src/App/pv_pm.su ./Core/Src/App/pv_pulse.cyclo ./Core/Src/App/pv_pulse.d ./Core/Src/App/pv_pulse.o ./Core/Src/App/pv_pulse.su ./Core/Src/App/pv_sensors.cyclo ./Core/Src/App/pv_sensors.d ./Core/Src/App/pv_sensors.o ./Core/Src/App/pv_sensors.su

.PHONY: clean-Core-2f-Src-2f-App

