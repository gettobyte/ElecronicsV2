/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include "osif.h"
#include <stdio.h>

volatile int exit_code = 0;

int main(void)
{
    /* Write your local variable definition here */
    ftm_state_t ftmStateStruct;
    int dutyCycle = 0U;

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize FTM instance */
    FTM_DRV_Init(INST_FLEXTIMER_PWM_2, &flexTimer_pwm_2_InitConfig, &ftmStateStruct);

    /* Initialize FTM PWM */
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM_2, &flexTimer_pwm_2_PwmConfig);

    /* Infinite loop */
    for ( ;; )
    {

		/* Increase the PWM of PTE16 */
		for (dutyCycle = 0; dutyCycle < 32768; dutyCycle += 50)
		{
			FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_2,
									 flexTimer_pwm_2_IndependentChannelsConfig[2].hwChannelId,
									 FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle,
									 0U,
									 true);
			OSIF_TimeDelay(1);
		}
		OSIF_TimeDelay(100);

		/* Decrease the PWM of PTE16 */
		for (dutyCycle = 32768; dutyCycle > 0; dutyCycle -= 50)
		{
			FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_2,
									 flexTimer_pwm_2_IndependentChannelsConfig[2].hwChannelId,
									 FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle,
									 0U,
									 true);
			OSIF_TimeDelay(1);
		}
		OSIF_TimeDelay(100);

		/* Increase the PWM of PTD1 */
		for (dutyCycle = 0; dutyCycle < 32768; dutyCycle += 50)
		{
			FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_2,
									 flexTimer_pwm_2_IndependentChannelsConfig[0].hwChannelId,
									 FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle,
									 0U,
									 true);
			OSIF_TimeDelay(1);
		}
		OSIF_TimeDelay(100);

		/* Decrease the PWM of PTD1 */
		for (dutyCycle = 32768; dutyCycle > 0; dutyCycle -= 50)
		{
			FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_2,
									 flexTimer_pwm_2_IndependentChannelsConfig[0].hwChannelId,
									 FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle,
									 0U,
									 true);
			OSIF_TimeDelay(1);
		}
		OSIF_TimeDelay(100);

		/* Increase the PWM of PTE15 */
		for (dutyCycle = 0; dutyCycle < 32768; dutyCycle += 50)
		{
			FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_2,
									 flexTimer_pwm_2_IndependentChannelsConfig[1].hwChannelId,
									 FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle,
									 0U,
									 true);
			OSIF_TimeDelay(1);
		}
		OSIF_TimeDelay(100);

		/* Decrease the PWM of PTE15 */
		for (dutyCycle = 32768; dutyCycle > 0; dutyCycle -= 50)
		{
			FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_2,
									 flexTimer_pwm_2_IndependentChannelsConfig[1].hwChannelId,
									 FTM_PWM_UPDATE_IN_TICKS, (uint16_t)dutyCycle,
									 0U,
									 true);
			OSIF_TimeDelay(1);
		}
		OSIF_TimeDelay(100);

    }

    return exit_code;
}
/* END main */
