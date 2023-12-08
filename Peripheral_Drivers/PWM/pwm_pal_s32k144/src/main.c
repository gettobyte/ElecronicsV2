#include "sdk_project_config.h"

volatile int exit_code = 0;

void delay(volatile int cycles) {
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}


void controlRedLED(uint8_t channel, uint32_t *duty) {
    status_t status = PWM_UpdateDuty(&pwm_pal_1_instance, channel, *duty);
    DEV_ASSERT(status == STATUS_SUCCESS);

    *duty += 1000; // Example: Increment duty cycle for Red color
    if (*duty > 4999U)
        *duty = 0; // Reset duty cycle when it reaches maximum
}

void controlGreenLED(uint8_t channel, uint32_t *duty) {
    status_t status = PWM_UpdateDuty(&pwm_pal_1_instance, channel, *duty);
    DEV_ASSERT(status == STATUS_SUCCESS);

    *duty += 500; // Example: Increment duty cycle for Green color
    if (*duty > 4999U)
        *duty = 0; // Reset duty cycle when it reaches maximum
}
void controlBlueLED(uint8_t channel, uint32_t *duty) {
    status_t status = PWM_UpdateDuty(&pwm_pal_1_instance, channel, *duty);
    DEV_ASSERT(status == STATUS_SUCCESS);

    *duty += 750; // Example: Increment duty cycle for Blue color
    if (*duty > 4999U)
        *duty = 0; // Reset duty cycle when it reaches maximum
}

int main(void) {
    uint32_t duty_R = 0;
    uint32_t duty_G = 0;
    uint32_t duty_B = 0;

    uint8_t channel_R = pwm_pal_1_configs.pwmChannels[0].channel; // Channel for Red color
    uint8_t channel_G = pwm_pal_1_configs.pwmChannels[1].channel; // Channel for Green color
    uint8_t channel_B = pwm_pal_1_configs.pwmChannels[2].channel; // Channel for Blue color

    status_t status = STATUS_SUCCESS;

    /* Initialize and configure clocks */
    status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Initialize pins - Ensure correct pin configuration for the RGB LED */
    status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Initialize PWM */
    status = PWM_Init(&pwm_pal_1_instance, &pwm_pal_1_configs);
    DEV_ASSERT(status == STATUS_SUCCESS);

    while(1) {
        controlRedLED(channel_R, &duty_R);
        controlGreenLED(channel_G, &duty_G);
        controlBlueLED(channel_B, &duty_B);

        delay(100000); // Small delay between duty cycle adjustments (adjust as needed)
    }

    for(;;) {
        if(exit_code != 0) {
            break;
        }
    }

    return exit_code;
}
