################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CSEC_Cypto_Engine_Init.c \
../src/csec_utils.c 

OBJS += \
./src/CSEC_Cypto_Engine_Init.o \
./src/csec_utils.o 

C_DEPS += \
./src/CSEC_Cypto_Engine_Init.d \
./src/csec_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/CSEC_Cypto_Engine_Init.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


