/*!
** Copyright 2020 NXP
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "sdk_project_config.h"
#include "GB_SSD1306.h"



#define BUFF_SIZE 64

#define OLED_ADDRESS 0b0011 1100  // 0x3C
#define OLED_Write 0b01111000 //0x78
#define OLED_Read 0b01111001 //0x79

void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

volatile int exit_code = 0;
/* User includes */

#define PCC_CLOCK	PCC_PORTD_CLOCK
#define LED0_PORT PTD
#define LED0_PIN  15
#define LED1_PORT PTD
#define LED1_PIN  16

uint8_t masterTxBuffer = 0x26;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
status_t x;

uint8_t tx_bno055[1] = { 0x00};

uint8_t tx_oled[1] = { 0x70};
int main(void)
{

    /* Allocate memory for the LPI2C driver state structure */
    lpi2c_master_state_t lpi2c1MasterState;

    /* Initialize and configure clocks
     *  - Configure system clocks and dividers
     *  - Configure LPI2C clock gating
     *  -   see clock manager component for details
    */

     CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize pins
     *  - Configure LPI2C pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize LPI2C Master configuration
     *  - Slave address 0x32
     *  - Fast operating mode, 400 KHz SCL frequency
     *  -   See LPI2C components for configuration details
     */
    //bno055
    LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0, &lpi2c1MasterState);

    OLED_I2C_MASTER_SEND(INST_LPI2C0, tx_bno055,1, false );

    for(;;)
    {

    }

   // return exit_code;
}

/* END main */
/*!
** @}
*/
