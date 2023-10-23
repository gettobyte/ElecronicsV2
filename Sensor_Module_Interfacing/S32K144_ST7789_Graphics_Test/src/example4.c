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

uint8_t data = 0x55;
int x,y,z = 0;
status_t error;
char a = 1;

int main(void)
{
	/* Write your code here */
	/* Configure clocks for PORT */
	error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
	DEV_ASSERT(error == STATUS_SUCCESS);

	error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	DEV_ASSERT(error == STATUS_SUCCESS);

	/* Initialize LPSPI0 (Send)*/
	LPSPI_DRV_MasterInit(INST_ST7789_INTERFACE, &ST7789_InterfaceState, &ST7789_SPI_Config);

	GB_ST7789_Init();

	    	OSIF_TimeDelay(100);

	    	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);
	    	ST7789_Fill_Color(ST77XX_BLACK);
for(;;)
    {

	ST7789_Fill_Color(ST77XX_WHITE);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_RED);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_GREEN);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_NEON_GREEN);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_NEON_GREEN);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_BLUE);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_CYAN);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_MAGENTA);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_YELLOW);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);

	ST7789_Fill_Color(ST77XX_ORANGE);
	ST7789_SetAddressWindow(ST7789_XStart,ST7789_YStart, ST7789_XEnd, ST7789_YEnd);



        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
