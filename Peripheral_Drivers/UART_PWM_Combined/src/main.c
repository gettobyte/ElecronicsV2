/*
 * main.c
 *
 *  Created on: 04-Mar-2024
 *      Author: Rohan
 */

/*!
** Copyright 2020 NXP
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "s32k144.h"
#include "sdk_project_config.h"
#include "string.h"
#include "osif.h"
#include <stdio.h>
//Includes important global variable
volatile int exit_code = 0;
uint8_t rxBuff[2];
//Welcome message for start of driver
#define welcomeMsg "Welcome to Driver Development Program\n"
#define waiTing "\tInput the data to configure your desired transfer type:\n"
#define STATUS_UART_FRAMING_Error "Framing Error have occurred please check Baudrate or BitsPerChar or other parameter"
#define STATUS_UART_NOISE_Error "Noise error occurs due to electrical interference, disrupting signal integrity and causing erroneous data transmission or reception."
#define STATUS_UART_PARITY_Error "Parity error occurs when the received data's parity bit doesn't match the expected parity, indicating a transmission error."
#define STATUS_UART_RX_Overrun "Overrun error occurs when the receive buffer is full, and new data is received before the previous data is read, causing data loss"
#define STATUS_Busy "Check for all flags to be cleared because status is busy"
status_t error;
int dutyCycle = 0U;
/* User includes */
void increaseGreen(){
   for (dutyCycle = 0; dutyCycle < 32768; dutyCycle += 50)
	{
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_0,
								 flexTimer_pwm_0_IndependentChannelsConfig[1].hwChannelId,
								 1, (uint16_t)dutyCycle,
								 0U,
								 true);
		OSIF_TimeDelay(1);
	}
}

void increaseRed(){
	/* Increase the PWM of RED */
	for (dutyCycle = 0; dutyCycle < 32768; dutyCycle += 50)
	{
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_0,
								 flexTimer_pwm_0_IndependentChannelsConfig[0].hwChannelId,
								 1, (uint16_t)dutyCycle,
								 0U,
								 true);
		OSIF_TimeDelay(1);
	}
}

void increaseBlue(){
	/* Increase the PWM of Blue */
	for (dutyCycle = 0; dutyCycle < 32768; dutyCycle += 50)
	{
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_0,
								 flexTimer_pwm_0_IndependentChannelsConfig[2].hwChannelId,
								 1, (uint16_t)dutyCycle,
								 0U,
								 true);
		OSIF_TimeDelay(1);
	}
}

void decreaseGreen(){
	/* Decrease the PWM of Green */
	for (dutyCycle = 32768; dutyCycle > 0; dutyCycle -= 50)
	{
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_0,
								 flexTimer_pwm_0_IndependentChannelsConfig[1].hwChannelId,
								 1, (uint16_t)dutyCycle,
								 0U,
								 true);
		OSIF_TimeDelay(1);
	}
}

void decreaseRed(){
//  Decrease the PWM of Red
	for (dutyCycle = 32768; dutyCycle > 0; dutyCycle -= 50)
	{
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_0,
								 flexTimer_pwm_0_IndependentChannelsConfig[0].hwChannelId,
								 1, (uint16_t)dutyCycle,
								 0U,
								 true);
		OSIF_TimeDelay(1);
	}
}
void decreaseBlue(){
//  Decrease the PWM of Red
	for (dutyCycle = 32768; dutyCycle > 0; dutyCycle -= 50)
	{
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_0,
								 flexTimer_pwm_0_IndependentChannelsConfig[2].hwChannelId,
								 1, (uint16_t)dutyCycle,
								 0U,
								 true);
		OSIF_TimeDelay(1);
	}
}

uint32_t arrayToInt(uint8_t *array, uint8_t length) {
    uint32_t result = 0;

    // Iterate through the array
    for (uint8_t i = 0; i < length; i++) {
        result = (result * 10) + (array[i] - '0'); // Convert ASCII digit to integer value
    }

    return result;
}
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
    	ftm_state_t ftmStateStruct;
	  /* Configure clocks for PORT */
	  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	   /* Initialize FTM instance */
	   FTM_DRV_Init(INST_FLEXTIMER_PWM_0, &flexTimer_pwm_0_InitConfig, &ftmStateStruct);

	   /* Initialize FTM PWM */
	   FTM_DRV_InitPwm(INST_FLEXTIMER_PWM_0, &flexTimer_pwm_0_PwmConfig);

	  error = LPUART_DRV_Init(INST_INTERRUPT_LPUART, &interrupt_lpuart_state, &interrupt_lpuart_InitConfig);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) welcomeMsg , strlen(welcomeMsg));

	  PINS_DRV_SetPins(PTD, 1 << 16);
	  PINS_DRV_SetPins(PTD, 1 << 15);
	  PINS_DRV_SetPins(PTD, 1 << 0);

	  OSIF_TimeDelay(1000);

	  LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) waiTing, strlen(waiTing));
    for(;;)
    {

       status_t retVal = LPUART_DRV_ReceiveDataPolling(INST_INTERRUPT_LPUART,(uint8_t *) &rxBuff, 2);
       uint8_t length = sizeof(rxBuff) / sizeof(rxBuff[0]);
	   uint32_t result = arrayToInt(rxBuff, length);
       if(retVal == STATUS_BUSY){
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
		   OSIF_TimeDelay(200);
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
    	   LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) STATUS_Busy, strlen(STATUS_Busy));
       }else if(retVal == STATUS_UART_RX_OVERRUN){
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
		   OSIF_TimeDelay(200);
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
    	   LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) STATUS_UART_RX_Overrun, strlen(STATUS_UART_RX_Overrun));
       }else if(retVal == STATUS_UART_PARITY_ERROR){
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
		   OSIF_TimeDelay(200);
		   PINS_DRV_TogglePins(PTD, 1 << 15);
		   LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) STATUS_UART_PARITY_Error, strlen(STATUS_UART_PARITY_Error));
       }else if(retVal == STATUS_UART_NOISE_ERROR){
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
		   OSIF_TimeDelay(200);
		   PINS_DRV_TogglePins(PTD, 1 << 15);
		   LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) STATUS_UART_NOISE_Error, strlen(STATUS_UART_NOISE_Error));
       }else if(retVal == STATUS_UART_FRAMING_ERROR){
    	   PINS_DRV_TogglePins(PTD, 1 << 15);
		   OSIF_TimeDelay(200);
		   PINS_DRV_TogglePins(PTD, 1 << 15);
		   LPUART_DRV_SendDataPolling(INST_INTERRUPT_LPUART, (uint8_t *) STATUS_UART_FRAMING_Error, strlen(STATUS_UART_FRAMING_Error));
       }else{
    	   if (result < 15) {
				  increaseGreen();  // Call Green for values 0-14
				  decreaseGreen();
			  }
    	   else if (result < 29) {
				  increaseRed();  // Call Yellow for values 15-28
				  increaseGreen();
				  decreaseGreen();
				  decreaseRed();
			  }
    	   else if (result < 43) {
				  increaseRed();  // Call Red for values 29-42
				  decreaseRed();
			  }
    	   else if (result < 57) {
				  increaseBlue();  // Call Magenta for values 43-56
				  increaseRed();
				  decreaseRed();
				  decreaseBlue();
			  }
    	   else if (result < 71) {
				  increaseBlue();  // Call Blue for values 57-70
				  decreaseBlue();
			  }
    	   else if (result < 85) {
				  increaseGreen();  // Call Cyan for values 71-84
				  increaseBlue();
				  decreaseBlue();
				  decreaseGreen();
			  }
    	   else {
				  increaseGreen();  // Call White for values 85-100
				  increaseRed();
				  increaseBlue();
				  decreaseGreen();
				  decreaseRed();
				  decreaseBlue();
			  }

       }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
