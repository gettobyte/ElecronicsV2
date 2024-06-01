################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeMASTER/src_platforms/S32xx/freemaster_S32xx.c 

OBJS += \
./FreeMASTER/src_platforms/S32xx/freemaster_S32xx.o 

C_DEPS += \
./FreeMASTER/src_platforms/S32xx/freemaster_S32xx.d 


# Each subdirectory must supply rules for building sources it contributes
FreeMASTER/src_platforms/S32xx/%.o: ../FreeMASTER/src_platforms/S32xx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@FreeMASTER/src_platforms/S32xx/freemaster_S32xx.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


