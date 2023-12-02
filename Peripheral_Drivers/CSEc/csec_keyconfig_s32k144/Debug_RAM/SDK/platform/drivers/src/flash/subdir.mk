################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/NXP/S32DS.3.4/S32DS/software/S32SDK_S32K1XX_RTM_4.0.3/platform/drivers/src/flash/flash_driver.c 

OBJS += \
./SDK/platform/drivers/src/flash/flash_driver.o 

C_DEPS += \
./SDK/platform/drivers/src/flash/flash_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/flash/flash_driver.o: C:/NXP/S32DS.3.4/S32DS/software/S32SDK_S32K1XX_RTM_4.0.3/platform/drivers/src/flash/flash_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/platform/drivers/src/flash/flash_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


