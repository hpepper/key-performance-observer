################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../tests/check_Common.o \
../tests/check_FileOps.o \
../tests/check_all.o 

C_SRCS += \
../tests/check_Common.c \
../tests/check_FileOps.c \
../tests/check_all.c 

OBJS += \
./tests/check_Common.o \
./tests/check_FileOps.o \
./tests/check_all.o 

C_DEPS += \
./tests/check_Common.d \
./tests/check_FileOps.d \
./tests/check_all.d 


# Each subdirectory must supply rules for building sources it contributes
tests/%.o: ../tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


