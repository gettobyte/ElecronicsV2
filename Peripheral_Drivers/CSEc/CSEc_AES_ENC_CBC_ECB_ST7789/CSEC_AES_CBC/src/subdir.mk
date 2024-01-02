################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CSEC_AES_CBC.c \
../src/csec_utils.c 

OBJS += \
./src/CSEC_AES_CBC.o \
./src/csec_utils.o 

C_DEPS += \
./src/CSEC_AES_CBC.d \
./src/csec_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/CSEC_AES_CBC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


