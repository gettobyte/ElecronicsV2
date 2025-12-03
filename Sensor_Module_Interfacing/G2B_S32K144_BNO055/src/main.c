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
#include "BNO055.h"


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

    lpi2c_master_state_t lpi2c1MasterState;
    lpuart_state_t lpuart1state;

uint8_t tx_bno055[1] = { 0x00};

uint8_t tx_oled[1] = { 0x70};

// Function to send the euler data over UART
void printEulerOverUart(bno055_euler_t *euler) {
    // Create a buffer to store the formatted string
    char buffer[100];

    // Format the Euler angles into a string
    snprintf(buffer, sizeof(buffer),
             "Heading: %.2f, Roll: %.2f, Pitch: %.2f\n",
             euler->heading_deg, euler->roll_deg, euler->pitch_deg);

    // Send the string over UART (assuming LPUART instance 0, adjust as necessary)
    status_t status = LPUART_DRV_SendDataBlocking(0, (const uint8_t*)buffer, strlen(buffer), 1000);
    if (status != STATUS_SUCCESS) {
        // Handle error (could print error or take other action)
    }
}

//bno055_euler_t euler;

int main(void)
{
	 bno055_status_t st;
	    bno055_euler_t euler;

	     CLOCK_DRV_Init(&clockMan1_InitConfig0);

	    /* Initialize pins
	     *  - Configure LPI2C pins
	     *  -   See PinSettings component for more info
	     */
	    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

	    x = LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0, &lpi2c1MasterState);
	    // 2. Initialize the BNO055 sensor

	    x = LPUART_DRV_Init(INST_LPUART_1, &lpuart1state, &lpuart_0_InitConfig0 );

	     x = OLED_I2C_MASTER_SEND(INST_LPI2C0, tx_bno055,1, false );

	     st = bno055_init();
	      if (st != BNO055_STATUS_OK) {
	          printf("BNO055 init failed: %d\r\n", st);
	          while(1);  // Loop forever on failure
	      }

	      //Main loop
	         while (1) {
	             st = bno055_read_euler_deg(&euler);
	             if (st == BNO055_STATUS_OK) {

	            	 printEulerOverUart(&euler);
	            	 printf("Heading: %.2f, Roll: %.2f, Pitch: %.2f\r\n",
	                        euler.heading_deg, euler.roll_deg, euler.pitch_deg);
	             } else {
	                 printf("Error reading Euler angles: %d\r\n", st);
	             }

	             // Delay to get ~10Hz sampling rate
	             OSIF_TimeDelay(100u);  // Delay 100 ms
	         }

}

/* END main */
/*!
** @}
*/
