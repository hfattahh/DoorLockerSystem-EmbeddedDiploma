################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../buzzer.c \
../gpio.c \
../hmi_ecu.c \
../keypad.c \
../lcd.c \
../timer0_driver_test.c \
../timers.c \
../uart.c 

OBJS += \
./buzzer.o \
./gpio.o \
./hmi_ecu.o \
./keypad.o \
./lcd.o \
./timer0_driver_test.o \
./timers.o \
./uart.o 

C_DEPS += \
./buzzer.d \
./gpio.d \
./hmi_ecu.d \
./keypad.d \
./lcd.d \
./timer0_driver_test.d \
./timers.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


