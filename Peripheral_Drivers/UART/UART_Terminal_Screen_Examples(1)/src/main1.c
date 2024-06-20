/*
 * Practice_uart.c
 *
 *  Created on: 14-Feb-2024
 *      Author: Rohan
 */
/* Including necessary configuration files. */
#include "stdio.h"
#include "s32k144.h"
#include "sdk_project_config.h"
#include "string.h"
volatile int exit_code = 0;
char rxBuff[20];
char txBuff[20];
#define welcomeMsg "Welcome to Trial\n"
#define waiTing "\nInput the data\n"
#define start "\nInput the string to find:"
#define affirmation "\n String you are finding, exist in data"
#define refutation "\n String you are finding doesn't exist in data"
#define LED0_PORT PTD
#define LED0_PIN  15
#define LED1_PORT PTD
#define LED1_PIN  16

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

       LPUART_DRV_ReceiveDataPolling(INST_TERMINAL_SCREEN, &rxBuff, 20);

//       const char *recieve = rxBuff;

    	LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) start,strlen(start));

       LPUART_DRV_ReceiveDataPolling(INST_TERMINAL_SCREEN, &txBuff, 5);

//       const char *send = txBuff;

       char *result = strpbrk((char *)txBuff, (char *)rxBuff);

       if(result != NULL){

       LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) affirmation,strlen(affirmation));

       PINS_DRV_TogglePins(LED1_PORT, 1 << LED1_PIN);

       OSIF_TimeDelay(500);

       PINS_DRV_TogglePins(LED1_PORT, 1 << LED1_PIN);

       }

       else{LPUART_DRV_SendDataPolling(INST_TERMINAL_SCREEN, (uint8_t *) refutation,strlen(refutation));

       PINS_DRV_TogglePins(LED0_PORT, 1 << LED0_PIN);

       OSIF_TimeDelay(500);

       PINS_DRV_TogglePins(LED0_PORT, 1 << LED0_PIN);}

       OSIF_TimeDelay(1000);

    }
    return exit_code;
}




