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
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include "osif.h"
#include <stdio.h>

volatile int exit_code = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your local variable definition here */
    ftm_state_t ftmStateStruct;
    int dutyCycle_R = 32768U;
    int dutyCycle_G = 32768U;
    int dutyCycle_B = 32768U;
    uint32_t Frequency;

    int channel_R = flexTimer_pwm_1_IndependentChannelsConfig[0].hwChannelId;
    int channel_G = flexTimer_pwm_1_IndependentChannelsConfig[1].hwChannelId;
    int channel_B = flexTimer_pwm_1_IndependentChannelsConfig[2].hwChannelId;

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize FTM instance */
    FTM_DRV_Init(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_InitConfig, &ftmStateStruct);

    /* Initialize FTM PWM */
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_PwmConfig);

    /* Infinite loop */
    for ( ;; )
    {
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_B,false);
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_G,false);
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_R,true);
    	Frequency = FTM_DRV_GetFrequency(INST_FLEXTIMER_PWM_1);

        /* Increase the brightness */
        for (dutyCycle_R = 32768; dutyCycle_R > 0; dutyCycle_R -= 10000)
        {

        	FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_1,channel_R,FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle_R,0U,true);

            OSIF_TimeDelay(1000);
        }
        OSIF_TimeDelay(1000);

    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_R,false);
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_B,false);
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_G,true);

        /* Decrease the brightness */
        for (dutyCycle_G = 32768; dutyCycle_G > 0; dutyCycle_G -= 10000)
        {
            FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_1,channel_G,FTM_PWM_UPDATE_IN_TICKS,(uint16_t)dutyCycle_G,0U,true);

            OSIF_TimeDelay(1000);
        }
        OSIF_TimeDelay(1000);

    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_R,false);
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_G,false);
    	FTM_DRV_ControlChannelOutput(INST_FLEXTIMER_PWM_1,channel_B,true);

        /* Decrease the brightness */
        for (dutyCycle_B = 32768; dutyCycle_B > 0; dutyCycle_B -= 10000)
        {
            FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_1,channel_B,FTM_PWM_UPDATE_IN_TICKS,(uint16_t)dutyCycle_B,0U,true);

            OSIF_TimeDelay(1000);
        }


    }

    return exit_code;
}
/* END main */
