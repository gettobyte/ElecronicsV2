################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/rtos/FreeRTOS/Source/croutine.c \
../SDK/rtos/FreeRTOS/Source/event_groups.c \
../SDK/rtos/FreeRTOS/Source/list.c \
../SDK/rtos/FreeRTOS/Source/queue.c \
../SDK/rtos/FreeRTOS/Source/stream_buffer.c \
../SDK/rtos/FreeRTOS/Source/tasks.c \
../SDK/rtos/FreeRTOS/Source/timers.c 

OBJS += \
./SDK/rtos/FreeRTOS/Source/croutine.o \
./SDK/rtos/FreeRTOS/Source/event_groups.o \
./SDK/rtos/FreeRTOS/Source/list.o \
./SDK/rtos/FreeRTOS/Source/queue.o \
./SDK/rtos/FreeRTOS/Source/stream_buffer.o \
./SDK/rtos/FreeRTOS/Source/tasks.o \
./SDK/rtos/FreeRTOS/Source/timers.o 

C_DEPS += \
./SDK/rtos/FreeRTOS/Source/croutine.d \
./SDK/rtos/FreeRTOS/Source/event_groups.d \
./SDK/rtos/FreeRTOS/Source/list.d \
./SDK/rtos/FreeRTOS/Source/queue.d \
./SDK/rtos/FreeRTOS/Source/stream_buffer.d \
./SDK/rtos/FreeRTOS/Source/tasks.d \
./SDK/rtos/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/FreeRTOS/Source/%.o: ../SDK/rtos/FreeRTOS/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/rtos/FreeRTOS/Source/croutine.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


