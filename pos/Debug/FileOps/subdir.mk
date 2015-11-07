################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FileOps/FileRecordHandler.c \
../FileOps/StoreCpuData.c \
../FileOps/StoreNetData.c \
../FileOps/StoreStorageData.c \
../FileOps/StoreVmData.c 

OBJS += \
./FileOps/FileRecordHandler.o \
./FileOps/StoreCpuData.o \
./FileOps/StoreNetData.o \
./FileOps/StoreStorageData.o \
./FileOps/StoreVmData.o 

C_DEPS += \
./FileOps/FileRecordHandler.d \
./FileOps/StoreCpuData.d \
./FileOps/StoreNetData.d \
./FileOps/StoreStorageData.d \
./FileOps/StoreVmData.d 


# Each subdirectory must supply rules for building sources it contributes
FileOps/%.o: ../FileOps/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


