/*
 * main2.c
 *
 *  Created on: 02-Mar-2024
 *      Author: Rohan
 */

/* Including necessary configuration files. */
#include "sdk_project_config.h"
#include "string.h"
volatile int exit_code = 0;
#define max_size_int 10
#define max_size_float 20
int integer = 123;
float float_value = 123.989;
uint8_t txBuff[20];
#define welcomeMsg "Welcome to Trial 2\n"
#define waiTing "Input the data, to print it\n"
#define LED0_PORT PTD
#define LED0_PIN  15
#define LED1_PORT PTD
#define LED1_PIN  16

void int_convertor(int temp){
	int total_digit = 0;
	while(temp > 0 && total_digit < max_size_int){
		txBuff[max_size_int - 1 - total_digit] = ('0' + temp % 10);
		temp /= 10;
		total_digit++;
	}
}

uint8_t convertor(int temp){
	uint8_t converted[sizeof(int)];
	for(int i = 0; i < sizeof(temp); i++){
		converted[i] = (uint8_t)(temp = (8 * i));
	}
	return converted;
}
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

	  PINS_DRV_SetPins(LED0_PORT, 1 << LED0_PIN);
	  PINS_DRV_SetPins(LED1_PORT, 1 << LED1_PIN);

	  OSIF_TimeDelay(1000);



    for(;;)
    {
       LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) waiTing,strlen(waiTing));

//       LPUART_DRV_ReceiveDataPolling(INST_TERMINAL_SCREEN, &rxBuff, 5);

//       Below is for Integer Conversion
       int_convertor(integer);

       LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, &txBuff, sizeof(txBuff));

       OSIF_TimeDelay(1000);

    }
    return exit_code;
}

/* END main */
/*!
** @}
*/

