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

    #define LED0_PORT PTD
	#define LED0_PIN  15
	#define LED1_PORT PTD
	#define LED1_PIN  16

    #define SWITCH2_PORT PTC
    #define SWITCH2_PIN 13

    #define SWITCH1_PORT PTC
    #define SWITCH1_PIN 12

    #define ReadSwitch2  PINS_DRV_ReadPins(SWITCH2_PORT) >> SWITCH2_PIN
    #define ReadSwitch1 PINS_DRV_ReadPins(SWITCH1_PORT) >> SWITCH1_PIN

    #define LED0_HIGH   PINS_DRV_SetPins(LED0_PORT, 1 << LED0_PIN);
    #define LED0_LOW    PINS_DRV_ClearPins(LED0_PORT, 1 << LED0_PIN);

    #define LED1_HIGH   PINS_DRV_SetPins(LED1_PORT, 1 << LED1_PIN);
    #define LED1_LOW    PINS_DRV_ClearPins(LED1_PORT, 1 << LED1_PIN);
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

  }
}
