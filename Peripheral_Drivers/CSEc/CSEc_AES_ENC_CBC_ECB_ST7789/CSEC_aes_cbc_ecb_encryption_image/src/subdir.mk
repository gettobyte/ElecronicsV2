################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CSEC_aes_cbc_ecb_encryption_image.c \
../src/csec_utils.c 

OBJS += \
./src/CSEC_aes_cbc_ecb_encryption_image.o \
./src/csec_utils.o 

C_DEPS += \
./src/CSEC_aes_cbc_ecb_encryption_image.d \
./src/csec_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/CSEC_aes_cbc_ecb_encryption_image.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


