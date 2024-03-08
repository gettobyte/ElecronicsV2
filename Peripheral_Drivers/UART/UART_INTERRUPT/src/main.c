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
volatile int exit_code = 0;
uint32_t timeout = 5000;
uint8_t rxBuff[20];
uint32_t txBuff[256];
#define welcomeMsg "Welcome to Gettobyte\n"
#define waiTing " Input the data, to print it\n"
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
	  status_t error;
	  /* Configure clocks for PORT */
	  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = LPUART_DRV_Init(INST_TERMINAL_SCREEN, &terminal_screen_state, &terminal_screen_InitConfig);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) welcomeMsg , strlen(welcomeMsg));

	  PINS_DRV_SetPins(PTD, 1 << 16);

	  OSIF_TimeDelay(1000);

    for(;;)
    {
       LPUART_DRV_SendData(INST_TERMINAL_SCREEN, (uint8_t *) waiTing, strlen(waiTing));

       status_t status = LPUART_DRV_ReceiveData(INST_TERMINAL_SCREEN, &rxBuff, sizeof(rxBuff));

       if(status == STATUS_SUCCESS){
    	   LPUART_DRV_IRQHandler(INST_TERMINAL_SCREEN);
    	   PINS_DRV_TogglePins(PTD, 1 << 16);
    	   OSIF_TimeDelay(1000);
    	   PINS_DRV_TogglePins(PTD, 1 << 16);
       }

       OSIF_TimeDelay(1000);

    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
