################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TestApp_USB.c \
../src/USB_IO.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/TestApp_USB.o \
./src/USB_IO.o 

C_DEPS += \
./src/TestApp_USB.d \
./src/USB_IO.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: PowerPC gcc compiler
	powerpc-eabi-gcc -Wall -O3 -c -fmessage-length=0 -I../../RobotRacers_bsp_0/ppc405_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


