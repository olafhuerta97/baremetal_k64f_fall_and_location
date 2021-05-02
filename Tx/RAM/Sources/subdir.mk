################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/i2c.c" \
"../Sources/lptmr.c" \
"../Sources/main.c" \
"../Sources/nrf24.c" \
"../Sources/uart.c" \

C_SRCS += \
../Sources/i2c.c \
../Sources/lptmr.c \
../Sources/main.c \
../Sources/nrf24.c \
../Sources/uart.c \

OBJS += \
./Sources/i2c.o \
./Sources/lptmr.o \
./Sources/main.o \
./Sources/nrf24.o \
./Sources/uart.o \

C_DEPS += \
./Sources/i2c.d \
./Sources/lptmr.d \
./Sources/main.d \
./Sources/nrf24.d \
./Sources/uart.d \

OBJS_QUOTED += \
"./Sources/i2c.o" \
"./Sources/lptmr.o" \
"./Sources/main.o" \
"./Sources/nrf24.o" \
"./Sources/uart.o" \

C_DEPS_QUOTED += \
"./Sources/i2c.d" \
"./Sources/lptmr.d" \
"./Sources/main.d" \
"./Sources/nrf24.d" \
"./Sources/uart.d" \

OBJS_OS_FORMAT += \
./Sources/i2c.o \
./Sources/lptmr.o \
./Sources/main.o \
./Sources/nrf24.o \
./Sources/uart.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/i2c.o: ../Sources/i2c.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/i2c.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/i2c.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/lptmr.o: ../Sources/lptmr.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/lptmr.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/lptmr.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/nrf24.o: ../Sources/nrf24.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/nrf24.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/nrf24.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart.o: ../Sources/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/uart.o"
	@echo 'Finished building: $<'
	@echo ' '


