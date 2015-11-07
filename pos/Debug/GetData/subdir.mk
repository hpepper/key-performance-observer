################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GetData/GetProcDiskstats.c \
../GetData/GetProcMeminfo.c \
../GetData/GetProcNetDev.c \
../GetData/GetProcStat.c \
../GetData/GetProcVmstat.c 

OBJS += \
./GetData/GetProcDiskstats.o \
./GetData/GetProcMeminfo.o \
./GetData/GetProcNetDev.o \
./GetData/GetProcStat.o \
./GetData/GetProcVmstat.o 

C_DEPS += \
./GetData/GetProcDiskstats.d \
./GetData/GetProcMeminfo.d \
./GetData/GetProcNetDev.d \
./GetData/GetProcStat.d \
./GetData/GetProcVmstat.d 


# Each subdirectory must supply rules for building sources it contributes
GetData/%.o: ../GetData/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


