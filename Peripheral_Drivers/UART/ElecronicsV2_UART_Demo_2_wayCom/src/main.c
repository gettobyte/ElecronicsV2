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
#include "sdk_project_config.h"
#include "string.h"
uint8_t rxBuff[20];
#define welcomeMsg "Welcome to Gettobyte\n"
#define waiTing " Input the data, to print it\n"

uint8_t data[5] = "ratgy";
/* User includes */

/* User includes */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
	 /* Write your code here */
		  status_t error;
		  /* Configure clocks for PORT */
		  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
		  DEV_ASSERT(error == STATUS_SUCCESS);

		  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
		  DEV_ASSERT(error == STATUS_SUCCESS);

		  error = LPUART_DRV_Init(INST_TERMINAL_SCREEN, &terminal_screen_state, &terminal_screen_InitConfig);
		  DEV_ASSERT(error == STATUS_SUCCESS);

		  error = LPUART_DRV_Init(INST_TERMINAL_SCREEN_2, &terminal_screen_2_state, &terminal_screen_2_InitConfig);
		  DEV_ASSERT(error == STATUS_SUCCESS);


		  OSIF_TimeDelay(1000);
    for(;;)
    {

    	//error = LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN_2, data, 5);

    	//error = LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, data, 5);

    	error = LPUART_DRV_ReceiveDataPolling(INST_TERMINAL_SCREEN_2, &rxBuff, 5);

    	//error = LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, &rxBuff, 5);

    	    //  OSIF_TimeDelay(1000);

    }

}

/* END main */
/*!
** @}
*/
