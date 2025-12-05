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
#include "string.h"
#include "stdlib.h"

#define TX_MAILBOX 1UL
#define TX_MSG_ID 0x800

status_t error;
uint8_t data[] = "Kunal";
#define size 5UL

flexcan_data_info_t tx_info =
{
        .data_length = 6,
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .enable_brs  = false,
        .fd_enable   = false,
        .fd_padding  = 0U
};


flexcan_msgbuff_t recvBuff;

status_t data_Send;



void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

volatile int exit_code = 0;
/* User includes */


/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

    lpi2c_master_state_t lpi2c1MasterState;
    lpuart_state_t lpuart1state;

uint8_t tx_bno055[1] = { 0x00};

uint8_t tx_oled[1] = { 0x70};

bno055_status_t st;
bno055_euler_t euler;
flexcan_data_info_t tx_info;

#define BUFF_SIZE 100

// Function to send Euler data over CAN
status_t sendEulerOverCan(bno055_euler_t *euler)
{
    // Convert Euler values to integers (2 decimal places)
    int32_t heading = (int32_t)(euler->heading_deg * 100);
    int32_t roll = (int32_t)(euler->roll_deg * 100);
    int32_t pitch = (int32_t)(euler->pitch_deg * 100);

    // Pack data into a message buffer (6 bytes for 3 values)
    uint8_t mb_data[6];
    mb_data[0] = (uint8_t)(heading >> 8);  // MSB of Heading
    mb_data[1] = (uint8_t)(heading & 0xFF); // LSB of Heading
    mb_data[2] = (uint8_t)(roll >> 8);     // MSB of Roll
    mb_data[3] = (uint8_t)(roll & 0xFF);   // LSB of Roll
    mb_data[4] = (uint8_t)(pitch >> 8);    // MSB of Pitch
    mb_data[5] = (uint8_t)(pitch & 0xFF);  // LSB of Pitch

    // Send data over CAN using the global tx_info
    status_t status = FLEXCAN_DRV_ConfigTxMb(INST_FLEXCAN_CONFIG_1, 0, &tx_info, TX_MSG_ID);
    // Send data over CAN
    status_t send_status = FLEXCAN_DRV_SendBlocking(
    		INST_FLEXCAN_CONFIG_1,
        0,               // Use message buffer 0
        &tx_info,        // Configuration data
		TX_MSG_ID,          // CAN message ID
        mb_data,         // CAN message data
        1000             // Timeout in ms
    );

    return send_status;  // Return the status of the CAN transmission


}


// Function to send the euler data over UART
void printEulerOverUart(bno055_euler_t *euler) {
    // Create separate buffers for each Euler angle
    char heading_str[BUFF_SIZE];
    char roll_str[BUFF_SIZE];
    char pitch_str[BUFF_SIZE];

    // Convert the Euler angles into strings using gcvt() (6 decimal places)
    gcvt(euler->heading_deg, 6, heading_str);  // Convert heading to string
    gcvt(euler->roll_deg, 6, roll_str);       // Convert roll to string
    gcvt(euler->pitch_deg, 6, pitch_str);     // Convert pitch to string

    // Now construct the formatted string to be sent over UART
    char buffer[BUFF_SIZE];
    snprintf(buffer, sizeof(buffer), "Heading: %s, Roll: %s, Pitch: %s\n", heading_str, roll_str, pitch_str);

    // Debug: Check if the buffer is being correctly populated
    printf("Formatted Buffer: %s\n", buffer);

    // Send the string over UART (assuming LPUART instance 1, adjust as necessary)
    status_t status = LPUART_DRV_SendDataBlocking(INST_LPUART_1, (const uint8_t*)buffer, strlen(buffer), 1000);
    if (status != STATUS_SUCCESS) {
        printf("Error in UART transmission\n");
    }

}

//bno055_euler_t euler;

int main(void)
{
	     CLOCK_DRV_Init(&clockMan1_InitConfig0);

	    /* Initialize pins
	     *  - Configure LPI2C pins
	     *  -   See PinSettings component for more info
	     */
	    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	    error = FLEXCAN_DRV_Init(INST_FLEXCAN_CONFIG_1, &flexcanState0, &flexcanInitConfig0);
	    error = LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0, &lpi2c1MasterState);
	    // 2. Initialize the BNO055 sensor
	    error = LPUART_DRV_Init(INST_LPUART_1, &lpuart1state, &lpuart_0_InitConfig0 );
	    error = OLED_I2C_MASTER_SEND(INST_LPI2C0, tx_bno055,1, false );
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
	            	 sendEulerOverCan(&euler);

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
