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

int main(void)
{
  status_t error;
  /* Configure clocks for PORT */
  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
  DEV_ASSERT(error == STATUS_SUCCESS);
  /* Set pins as GPIO */
  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
  DEV_ASSERT(error == STATUS_SUCCESS);

  for (;;)
  {
      if(ReadSwitch1 & 0x01) // Switch1 HIGH
      {
    	  RED_LED_HIGH;  // RED LED HIGH
      }else if(!(ReadSwitch1 & 0x01)) // Switch1 LOW
      {
    	  RED_LED_LOW; // RED LED LOW
      }

      if(ReadSwitch2 & 0x01) // Switch2 High
	  {
    	  GREEN_LED_HIGH; //Green LED High
	  }else if(!(ReadSwitch2 & 0x01)) // Switch2 Low
	  {
		  GREEN_LED_LOW;  //Green LED Low
	  }
  }
}
