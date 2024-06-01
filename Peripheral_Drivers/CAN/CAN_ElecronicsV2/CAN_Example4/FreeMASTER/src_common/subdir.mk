################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeMASTER/src_common/freemaster_appcmd.c \
../FreeMASTER/src_common/freemaster_bdm.c \
../FreeMASTER/src_common/freemaster_can.c \
../FreeMASTER/src_common/freemaster_lin.c \
../FreeMASTER/src_common/freemaster_pipes.c \
../FreeMASTER/src_common/freemaster_protocol.c \
../FreeMASTER/src_common/freemaster_rec.c \
../FreeMASTER/src_common/freemaster_scope.c \
../FreeMASTER/src_common/freemaster_serial.c \
../FreeMASTER/src_common/freemaster_sfio.c \
../FreeMASTER/src_common/freemaster_tsa.c 

OBJS += \
./FreeMASTER/src_common/freemaster_appcmd.o \
./FreeMASTER/src_common/freemaster_bdm.o \
./FreeMASTER/src_common/freemaster_can.o \
./FreeMASTER/src_common/freemaster_lin.o \
./FreeMASTER/src_common/freemaster_pipes.o \
./FreeMASTER/src_common/freemaster_protocol.o \
./FreeMASTER/src_common/freemaster_rec.o \
./FreeMASTER/src_common/freemaster_scope.o \
./FreeMASTER/src_common/freemaster_serial.o \
./FreeMASTER/src_common/freemaster_sfio.o \
./FreeMASTER/src_common/freemaster_tsa.o 

C_DEPS += \
./FreeMASTER/src_common/freemaster_appcmd.d \
./FreeMASTER/src_common/freemaster_bdm.d \
./FreeMASTER/src_common/freemaster_can.d \
./FreeMASTER/src_common/freemaster_lin.d \
./FreeMASTER/src_common/freemaster_pipes.d \
./FreeMASTER/src_common/freemaster_protocol.d \
./FreeMASTER/src_common/freemaster_rec.d \
./FreeMASTER/src_common/freemaster_scope.d \
./FreeMASTER/src_common/freemaster_serial.d \
./FreeMASTER/src_common/freemaster_sfio.d \
./FreeMASTER/src_common/freemaster_tsa.d 


# Each subdirectory must supply rules for building sources it contributes
FreeMASTER/src_common/%.o: ../FreeMASTER/src_common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@FreeMASTER/src_common/freemaster_appcmd.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


