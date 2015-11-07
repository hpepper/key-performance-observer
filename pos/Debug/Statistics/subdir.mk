################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Statistics/CpuStatistics.c \
../Statistics/NetStatistics.c \
../Statistics/StorageStatistics.c \
../Statistics/VmStatistics.c 

OBJS += \
./Statistics/CpuStatistics.o \
./Statistics/NetStatistics.o \
./Statistics/StorageStatistics.o \
./Statistics/VmStatistics.o 

C_DEPS += \
./Statistics/CpuStatistics.d \
./Statistics/NetStatistics.d \
./Statistics/StorageStatistics.d \
./Statistics/VmStatistics.d 


# Each subdirectory must supply rules for building sources it contributes
Statistics/%.o: ../Statistics/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


