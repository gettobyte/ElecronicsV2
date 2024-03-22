/*
 * main3.c
 *
 *  Created on: 04-Mar-2024
 *      Author: Rohan
 */

/* Including necessary configuration files. */
#include "sdk_project_config.h"
#include "string.h"
volatile int exit_code = 0;
#define max_size_float 20
float float_value = 3.1428;
char txBuff[max_size_float];
#define welcomeMsg "Welcome to Trial 2\n"
#define waiTing "Change the pre-defined variable in line 13\n"

void float_to_ascii_array(float float_value, char* txBuff) {
    // Convert float to string
    char float_str[20]; // Assuming a maximum of 20 characters for the string
    sprintf(float_str, "%f", float_value);

    // Copy characters to the ASCII array
    strcpy(txBuff, float_str);
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
	  status_t error;
	  /* Configure clocks for PORT */
	  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = LPUART_DRV_Init(INST_TERMINAL_SCREEN, &terminal_screen_state, &terminal_screen_InitConfig);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) welcomeMsg , strlen(welcomeMsg));

	  OSIF_TimeDelay(1000);



    for(;;)
    {
       LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) waiTing,strlen(waiTing));

//       Below is for Float Convertor
	   float_to_ascii_array(float_value, txBuff);

       LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, &txBuff, sizeof(txBuff));

       OSIF_TimeDelay(1000);

    }
    return exit_code;
}

/* END main */
/*!
** @}
*/

