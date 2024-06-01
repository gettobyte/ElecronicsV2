/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

	#define PCC_CLOCK	PCC_PORTD_CLOCK

    #define RED_LED_PORT PTD
	#define RED_LED_PIN  15
	#define GREEN_LED_PORT PTD
	#define GREEN_LED_PIN  16
    #define BLUE_LED_PORT PTD
    #define BLUE_LED_PIN 0

    #define SWITCH2_PORT PTC
    #define SWITCH2_PIN 13

    #define SWITCH1_PORT PTC
    #define SWITCH1_PIN 12

    #define ReadSwitch2  PINS_DRV_ReadPins(SWITCH2_PORT) >> SWITCH2_PIN
    #define ReadSwitch1 PINS_DRV_ReadPins(SWITCH1_PORT) >> SWITCH1_PIN

    #define RED_LED_HIGH     PINS_DRV_SetPins(RED_LED_PORT, 1 << RED_LED_PIN);
    #define RED_LED_LOW    PINS_DRV_ClearPins(RED_LED_PORT, 1 << RED_LED_PIN);
    #define RED_LED_TOGGLE  PINS_DRV_TogglePins(RED_LED_PORT, 1 << RED_LED_PIN);

    #define GREEN_LED_HIGH   PINS_DRV_SetPins(GREEN_LED_PORT, 1 << GREEN_LED_PIN);
    #define GREEN_LED_LOW    PINS_DRV_ClearPins(GREEN_LED_PORT, 1 << GREEN_LED_PIN);
    #define GREEN_LED_TOGGLE  PINS_DRV_TogglePins(GREEN_LED_PORT, 1 << GREEN_LED_PIN);

#include "sdk_project_config.h"
/* Included the FreeMAster Header file  */
#include "freemaster.h"

uint8_t Red_Led_State;
uint8_t Green_Led_State;

#define RED_LED_HIGH_VALUE 1
#define RED_LED_LOW_VALUE 0
#define GREEN_LED_HIGH_VALUE 1
#define GREEN_LED_LOW_VALUE 0

int main(void)
{
  status_t error;
  /* Configure clocks for PORT */

  /* Variables used for processing FreeMASATER application commands */
  static FMSTR_APPCMD_CODE cmd;
  static FMSTR_APPCMD_PDATA cmdDataP;
  static FMSTR_SIZE cmdSize;


  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
  DEV_ASSERT(error == STATUS_SUCCESS);
  /* Set pins as GPIO */
  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
  DEV_ASSERT(error == STATUS_SUCCESS);

  /* Initialize LPUART instance */
  LPUART_DRV_Init(INST_LPUART_1, &lpUartState1, &lpuart_1_InitConfig0);
  INT_SYS_InstallHandler(LPUART1_RxTx_IRQn, FMSTR_Isr, NULL);


  /* Initialize FreeMASTER driver */
  FMSTR_Init();


  for (;;)
  {
      if(ReadSwitch1 & 0x01) // Switch1 HIGH
      {
    	  Red_Led_State = RED_LED_HIGH_VALUE;
    	  RED_LED_HIGH;  // RED LED HIGH
      }else if(!(ReadSwitch1 & 0x01)) // Switch1 LOW
      {
    	  Red_Led_State = RED_LED_LOW_VALUE;
    	  RED_LED_LOW; // RED LED LOW
      }

      if(ReadSwitch2 & 0x01) // Switch2 High
	  {
    	  Green_Led_State = GREEN_LED_HIGH_VALUE;
    	  GREEN_LED_HIGH; //Green LED High
	  }else if(!(ReadSwitch2 & 0x01)) // Switch2 Low
	  {
		  Green_Led_State = GREEN_LED_LOW_VALUE;
		  GREEN_LED_LOW;  //Green LED Low
	  }

      /* Process FreeMASTER application commands */
            cmd = FMSTR_GetAppCmd(); // Command Code
            if (cmd != FMSTR_APPCMDRESULT_NOCMD) {
            cmdDataP = FMSTR_GetAppCmdData(&cmdSize); // arguments or data with command
            switch (cmd)
       {

            case 0:
			  PINS_DRV_TogglePins(BLUE_LED_PORT, 1 << BLUE_LED_PIN);
			  OSIF_TimeDelay(1000);
			  PINS_DRV_TogglePins(GREEN_LED_PORT, 1 << GREEN_LED_PIN);
			  OSIF_TimeDelay(1000);
			  PINS_DRV_TogglePins(RED_LED_PORT, 1 << RED_LED_PIN);
			  OSIF_TimeDelay(1000);
		        /* Acknowledge the command */
		                FMSTR_AppCmdAck(0);
            break;

            case 1:
  			  PINS_DRV_TogglePins(RED_LED_PORT, 1 << RED_LED_PIN);
  			  OSIF_TimeDelay(1000);
  			  PINS_DRV_TogglePins(BLUE_LED_PORT, 1 << BLUE_LED_PIN);
  			  OSIF_TimeDelay(1000);
  			  PINS_DRV_TogglePins(GREEN_LED_PORT, 1 << GREEN_LED_PIN);
  			  OSIF_TimeDelay(1000);
  	        /* Acknowledge the command */
  	                FMSTR_AppCmdAck(0);
            break;

            case 2:

              PINS_DRV_TogglePins(GREEN_LED_PORT, 1 << GREEN_LED_PIN);
              OSIF_TimeDelay(1000);

        	  PINS_DRV_TogglePins(RED_LED_PORT, 1 << RED_LED_PIN);
              OSIF_TimeDelay(1000);

              PINS_DRV_TogglePins(BLUE_LED_PORT, 1 << BLUE_LED_PIN);
              OSIF_TimeDelay(1000);
              /* Acknowledge the command */
              FMSTR_AppCmdAck(0);
            break;

            default:
                /* Acknowledge the command with failure */
                         FMSTR_AppCmdAck(1);
            break;

         }
            }
      /* Handle the protocol decoding and execution */
      FMSTR_Poll();
  }
}
