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
#include "freemaster.h"
#include "sdk_project_config.h"
#include "ST7789_low_level.h"
#include "fonts.h"

volatile int exit_code = 0;
/* User includes */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

#define RX_MAILBOX 0UL
#define RX_MSG_ID 0x800
#define size 5UL

status_t error;
uint8_t data[] = "Kunal";

flexcan_msgbuff_t recvBuff;

status_t data_Send, recv_data, recv_data_status;
flexcan_data_info_t dataInfo =
{
        .data_length = size,
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .enable_brs  = false,
        .fd_enable   = false,
        .fd_padding  = 0U
};

int main(void)
{
    /* Write your code here */

	/* Configure clocks for PORT */
	CLOCK_DRV_Init(&clockMan1_InitConfig0);
    error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize LPUART instance */
    LPUART_DRV_Init(INST_LPUART_1, &lpUartState1, &lpuart_1_InitConfig0);
    INT_SYS_InstallHandler(LPUART1_RxTx_IRQn, FMSTR_Isr, NULL);

	LPSPI_DRV_MasterInit(INST_ST7789_INTERFACE, &ST7789_InterfaceState, &ST7789_SPI_Config);
    FLEXCAN_DRV_Init(INST_FLEXCAN_CONFIG_1, &flexcanState0, &flexcanInitConfig0);

   GB_ST7789_Init();
   ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);
   ST7789_Fill_Color(ST77XX_BLACK);
   ST7789_WriteString(0, 80, "CAN Reception with ElecronicsV2", Font_16x26, ST77XX_NEON_GREEN, ST77XX_BLACK);
   ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);
   ST7789_Fill_Color(ST77XX_BLACK);

   OSIF_TimeDelay(2000);

   FLEXCAN_DRV_ConfigRxMb(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &dataInfo, RX_MSG_ID);
   ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);


   /* Initialize FreeMASTER driver */
   FMSTR_Init();

   for(;;)
    {

    	ST7789_WriteString(0, 200, "CAN Receiving", Font_11x18, ST77XX_NEON_GREEN, ST77XX_BLACK);

                 /* Start receiving data in RX_MAILBOX again */
		//FLEXCAN_DRV_ReceiveBlocking(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &recvBuff, 10000);
		while(STATUS_TIMEOUT == (FLEXCAN_DRV_ReceiveBlocking(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &recvBuff, 1000)))
		{
		  	ST7789_WriteString(20, 230, "Waiting to Receive CAN Data", Font_11x18, ST77XX_NEON_GREEN, ST77XX_BLACK);

		}
		recv_data_status = FLEXCAN_DRV_GetTransferStatus(INST_FLEXCAN_CONFIG_1, RX_MAILBOX);

    	ST7789_WriteString(32, 280, recvBuff.data, Font_11x18, ST77XX_NEON_GREEN, ST77XX_BLACK);



   }
    return exit_code;
    }


/* END main */
/*!
** @}
*/
