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

uint8_t tx_bno055[1] = { 0x00};

uint8_t tx_oled[1] = { 0x70};

//bno055_euler_t euler;

int main(void)
{
	 bno055_status_t st;
	    bno055_euler_t euler;

//    /* Allocate memory for the LPI2C driver state structure */
//    lpi2c_master_state_t lpi2c1MasterState;
//
//    /* Initialize and configure clocks
//     *  - Configure system clocks and dividers
//     *  - Configure LPI2C clock gating
//     *  -   see clock manager component for details
//    */
//
//     CLOCK_DRV_Init(&clockMan1_InitConfig0);
//
//    /* Initialize pins
//     *  - Configure LPI2C pins
//     *  -   See PinSettings component for more info
//     */
//    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
//
//    /* Initialize LPI2C Master configuration
//     *  - Slave address 0x32
//     *  - Fast operating mode, 400 KHz SCL frequency
//     *  -   See LPI2C components for configuration details
//     */
//    //bno055
// x = LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0, &lpi2c1MasterState);
//    x = OLED_I2C_MASTER_SEND(INST_LPI2C0, tx_bno055,1, false );

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

	    x = LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0, &lpi2c1MasterState);
	    // 2. Initialize the BNO055 sensor

	     x = OLED_I2C_MASTER_SEND(INST_LPI2C0, tx_bno055,1, false );

	     st = bno055_init();
	      if (st != BNO055_STATUS_OK) {
	          printf("BNO055 init failed: %d\r\n", st);
	          while(1);  // Loop forever on failure
	      }

//	     st = bno055_init(BNO055_I2C_ADDR_0);   /* 0x28 – change to _1 if your ADDR is high */
//	      if (st != BNO055_STATUS_OK) {
//	          printf("BNO055 init failed: %d\r\n", st);
//	          for (;;) { /* error loop */ }
//	      }
//
//	      st = bno055_read_euler_deg(BNO055_I2C_ADDR_0, &euler);
//	           if (st == BNO055_STATUS_OK) {
//	               printf("Heading: %.2f, Roll: %.2f, Pitch: %.2f\r\n",
//	                      euler.heading_deg, euler.roll_deg, euler.pitch_deg);
//	           } else {
//	               printf("Read error: %d\r\n", st);
//	           }
//
//	           bno055_delay_ms(100);  /* ~10 Hz like Arduino example */

//	      bno055_t imu;
//	      bno055_err_t err = bno055_init(&imu, 0, BNO055_I2C_ADDR);  // Passing LPI2C0 instance (0) for communication
//	      if (err != BNO055_ERR_OK) {
//	          // Handle error (e.g., LED blink, stop execution, etc.)
//	          while (1) {}
//	      }
//

	      //Main loop
	         while (1) {
	             st = bno055_read_euler_deg(&euler);
	             if (st == BNO055_STATUS_OK) {
	                 printf("Heading: %.2f, Roll: %.2f, Pitch: %.2f\r\n",
	                        euler.heading_deg, euler.roll_deg, euler.pitch_deg);
	             } else {
	                 printf("Error reading Euler angles: %d\r\n", st);
	             }

	             // Delay to get ~10Hz sampling rate
	             OSIF_TimeDelay(100u);  // Delay 100 ms
	         }


//	      // 3. Read Euler angles from the BNO055 sensor in an infinite loop
//	      while (1) {
//	          //bno055_euler_t euler;
//	          err = bno055_read_euler(&imu, 0, &euler);  // Passing LPI2C0 instance (0)
//
//	          if (err == BNO055_ERR_OK) {
//	              // Process Euler angles (euler.heading_deg, euler.roll_deg, euler.pitch_deg)
//	              // For example, output the values via UART, display, or process further
//	          }
//
//	          // Wait a little before reading again
//	          bno_delay_ms(50);  // Simple delay in ms
	  //    }
   // return exit_code;
}

/* END main */
/*!
** @}
*/
