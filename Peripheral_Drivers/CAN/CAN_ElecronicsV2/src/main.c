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

volatile int exit_code = 0;
/* User includes */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

#define TX_MAILBOX 1UL
#define TX_MSG_ID 1UL
#define RX_MAILBOX 1UL
#define RX_MSG_ID 2UL
#define size 5UL


uint8_t data[] = "Kunal";

int main(void)
{
    /* Write your code here */

	/* Configure clocks for PORT */
	CLOCK_DRV_Init(&clockMan1_InitConfig0);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);


    FLEXCAN_DRV_Init(INST_FLEXCAN_CONFIG_1, &flexcanState0, &flexcanInitConfig0);

    flexcan_data_info_t dataInfo =
    {
            .data_length = size,
            .msg_id_type = FLEXCAN_MSG_ID_STD,
            .enable_brs  = false,
            .fd_enable   = false,
            .fd_padding  = 0U
    };

    flexcan_msgbuff_t recvBuff;

//    FLEXCAN_DRV_GetDefaultConfig()
//
//    FLEXCAN_DRV_ConfigTxMb(INST_FLEXCAN_CONFIG_1, TX_MAILBOX, &dataInfo , TX_MSG_ID);
//
//    FLEXCAN_DRV_SendBlocking(INST_FLEXCAN_CONFIG_1, TX_MAILBOX, &dataInfo, TX_MSG_ID,  data, 1000 );
//
//
//    FLEXCAN_DRV_ConfigRxMb(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &dataInfo, RX_MSG_ID);
//
//
//    FLEXCAN_DRV_Receive(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &recvBuff);

    for(;;)
    {
        FLEXCAN_DRV_ConfigTxMb(INST_FLEXCAN_CONFIG_1, TX_MAILBOX, &dataInfo , TX_MSG_ID);

        FLEXCAN_DRV_SendBlocking(INST_FLEXCAN_CONFIG_1, TX_MAILBOX, &dataInfo, TX_MSG_ID,  data, 1000 );

//        FLEXCAN_DRV_ConfigRxMb(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &dataInfo, RX_MSG_ID);
//
//
//        FLEXCAN_DRV_Receive(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &recvBuff);


    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
