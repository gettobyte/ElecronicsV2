/*
 * main4.c
 *
 *  Created on: 04-Mar-2024
 *      Author: Rohan
 */

/* Including necessary configuration files. */
#include "sdk_project_config.h"
#include "string.h"
volatile int exit_code = 0;
#define max_size_float 20
unsigned int hex_value = 0x41424344;
char txBuff[max_size_float];
#define welcomeMsg "Welcome to Trial 2\n"
#define waiTing "Change the pre-defined variable in line 13\n"

void hex_to_ascii_array(unsigned int hex_value, char* txBuff) {
    // Convert float to string
    char hex_str[20]; // Assuming a maximum of 20 characters for the string
    sprintf(hex_str, "%f", hex_value);

    // Copy characters to the ASCII array
    strcpy(txBuff, hex_str);
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

//       Below is for Hexadecimal Convertor
	   hex_to_ascii_array(hex_value, txBuff);

       LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, &txBuff, sizeof(txBuff));

       OSIF_TimeDelay(1000);

    }
    return exit_code;
}

/* END main */
/*!
** @}
*/


