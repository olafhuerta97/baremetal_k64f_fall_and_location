################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ftm.c" \
"../Sources/i2c.c" \
"../Sources/lcd.c" \
"../Sources/main.c" \
"../Sources/nrf24.c" \
"../Sources/pit.c" \
"../Sources/uart.c" \

C_SRCS += \
../Sources/ftm.c \
../Sources/i2c.c \
../Sources/lcd.c \
../Sources/main.c \
../Sources/nrf24.c \
../Sources/pit.c \
../Sources/uart.c \

OBJS += \
./Sources/ftm.o \
./Sources/i2c.o \
./Sources/lcd.o \
./Sources/main.o \
./Sources/nrf24.o \
./Sources/pit.o \
./Sources/uart.o \

C_DEPS += \
./Sources/ftm.d \
./Sources/i2c.d \
./Sources/lcd.d \
./Sources/main.d \
./Sources/nrf24.d \
./Sources/pit.d \
./Sources/uart.d \

OBJS_QUOTED += \
"./Sources/ftm.o" \
"./Sources/i2c.o" \
"./Sources/lcd.o" \
"./Sources/main.o" \
"./Sources/nrf24.o" \
"./Sources/pit.o" \
"./Sources/uart.o" \

C_DEPS_QUOTED += \
"./Sources/ftm.d" \
"./Sources/i2c.d" \
"./Sources/lcd.d" \
"./Sources/main.d" \
"./Sources/nrf24.d" \
"./Sources/pit.d" \
"./Sources/uart.d" \

OBJS_OS_FORMAT += \
./Sources/ftm.o \
./Sources/i2c.o \
./Sources/lcd.o \
./Sources/main.o \
./Sources/nrf24.o \
./Sources/pit.o \
./Sources/uart.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/ftm.o: ../Sources/ftm.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ftm.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ftm.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/i2c.o: ../Sources/i2c.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/i2c.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/i2c.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd.o: ../Sources/lcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/lcd.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/lcd.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/nrf24.o: ../Sources/nrf24.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/nrf24.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/nrf24.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/pit.o: ../Sources/pit.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/pit.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/pit.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart.o: ../Sources/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/uart.o"
	@echo 'Finished building: $<'
	@echo ' '


