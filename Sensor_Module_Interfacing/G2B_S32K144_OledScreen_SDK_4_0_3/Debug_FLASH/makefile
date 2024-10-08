################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include board/subdir.mk
-include SDK/rtos/osif/subdir.mk
-include SDK/platform/drivers/src/pins/subdir.mk
-include SDK/platform/drivers/src/lpi2c/subdir.mk
-include SDK/platform/drivers/src/interrupt/subdir.mk
-include SDK/platform/drivers/src/edma/subdir.mk
-include SDK/platform/drivers/src/clock/S32K1xx/subdir.mk
-include SDK/platform/devices/S32K144/startup/subdir.mk
-include SDK/platform/devices/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Drivers/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
G2B_S32K144_OledScreen_SDK_4_0_3.siz \


# All Target
all: G2B_S32K144_OledScreen_SDK_4_0_3.elf secondary-outputs

# Tool invocations
G2B_S32K144_OledScreen_SDK_4_0_3.elf: $(OBJS) C:/Users/gettobyte_kunal/Documents/elecronicsv2_autosarMCAL_Workspace/G2B_S32K144_OledScreen_SDK_4_0_3/Project_Settings/Linker_Files/S32K144_64_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "G2B_S32K144_OledScreen_SDK_4_0_3.elf" "@G2B_S32K144_OledScreen_SDK_4_0_3.args"  $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

G2B_S32K144_OledScreen_SDK_4_0_3.siz: G2B_S32K144_OledScreen_SDK_4_0_3.elf
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley G2B_S32K144_OledScreen_SDK_4_0_3.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
