

#include "ST7789_low_level.h"
#include "peripherals_ST7789_Interface.h"

#define gb_ST7789_CS_pin_low()  PINS_DRV_WritePin(CS_PORT, CS_PIN, 0) //Slave select pin LOW
#define gb_ST7789_CS_pin_high() PINS_DRV_WritePin(CS_PORT, CS_PIN, 1 )

#define gb_ST7789_DC_pin_low()  PINS_DRV_WritePin(DC_PORT, DC_PIN, 0);
#define gb_ST7789_DC_pin_high() PINS_DRV_WritePin(DC_PORT, DC_PIN, 1);

#define gb_ST7789_reset_pin_low()  PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 0)
#define gb_ST7789_reset_pin_high() PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 1)



// For ST7789 Driver IC and we are using 240*240 pixel and 1.3 inch display
#define ST7789_WIDTH 240
#define ST7789_HEIGHT 240

/*
 * @brief write data to ST7789 controller
 * @param commonByte -> address of ST7789 that needs to be accessed
 * @param dataBytes  -> pointer to the data buffer that needs to send/write at that address
 * @param numDataBytes -> number of data bytes that needs to send
 */

void GB_STT7789_SPI_CS_low()
{
	gb_ST7789_CS_pin_low();
}

void GB_STT7789_SPI_CS_high()
{
	gb_ST7789_CS_pin_high();
}

void GB_ST7789_SendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes, uint32_t timeout)
{

	gb_ST7789_DC_pin_low();
	GB_MA_SPI_send_byte_conti(&commandByte,1,timeout);
	gb_ST7789_DC_pin_high();

	GB_MA_SPI_send_byte_conti(dataBytes, numDataBytes, timeout );

}

void GB_ST7789_SendData(uint8_t *data, uint32_t data_size)
{
	gb_ST7789_DC_pin_high();

	GB_MA_SPI_send_byte_conti(data, data_size, 5000);
}

void GB_ST7789_SendDataIm( const uint8_t *data, uint32_t data_size)
{
	gb_ST7789_DC_pin_high();

	GB_MA_SPI_send_byte_conti(data, data_size, 5000);
}

void GB_ST7789_Init()
{

	uint8_t ColorMod = ST7789_16_Bit_5_6_5_Input_64K_Color;
	uint8_t MADCTL_SetRotation1 = 0x80; //MX = 0, MY=1, MX = 0
	uint8_t MADCTL_SetRotation0 = 0xC0; // MY = 0, MX= 0, RGB is set

	uint8_t ColAddr[4]={ 0x00,0x00,0x00,240};
	uint8_t RowAddr[4] = {0x00,0x00,320>>8,320&0xFF};
	uint8_t data;

	gb_ST7789_reset_pin_low();
	OSIF_TimeDelay(25);
	gb_ST7789_reset_pin_high();
	OSIF_TimeDelay(50);

	gb_ST7789_CS_pin_low();

	GB_ST7789_SendCommand(ST77XX_SWRESET, &data, 0, 1000 );

	GB_ST7789_SendCommand(ST77XX_SLPOUT, &data, 0, 1000 );

	GB_ST7789_SendCommand(ST77XX_COLMOD, &ColorMod, 1, 1000);   // Set color Mode

	//GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL_SetRotation1, 1, 10);  // Set display rotation

	GB_ST7789_SendCommand(ST77XX_INVON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_NORON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_DISPON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL_SetRotation0, 1, 10);
	gb_ST7789_CS_pin_high();
}

/**
 * @brief Set address of DisplayWindow, this API sets the cursor for ST7789
 * Total canvas size is of 240*320 for ST7789 Driver IC, that is 240 columns and 320 rows.
 * But the module of ST7789, which we are using has canvas size of 240*240.
 * That is 240 columns from 0->239 amd 240 rows from 80->319.
 * Now columns are represented by x-axis, their value would be ranging from 0 to 239
 * and rows are represented by y-axis, their value would be ranging from 80 to 319.
 *
 *       (0,0)   -----------------------(239,0)      --> x-axis
 *               |*********************|
 *               |*********************|
 *               |*********************|
 *               |*********************|
 *               |*********************|
 *               (coloumns from 0 to 240)
 *       (0,80)  |---------------------|(239,80)
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *           R   |-|-|-|-|-|-|-|-|-|-|-|
 *           O   |-|-|-|-|-|-|-|-|-|-|-|
 *           W   |-|-|-|-|-|-|-|-|-|-|-|
 *           S   |-|-|-|-|-|-|-|-|-|-|-|
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *           F   |-|-|-|-|-|-|-|-|-|-|-|
 *           R   |-|-|-|-|-|-|-|-|-|-|-|
 *           O   |-|-|-|-|-|-|-|-|-|-|-|
 *           M   |-|-|-|-|-|-|-|-|-|-|-|
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *          80   |-|-|-|-|-|-|-|-|-|-|-|
 *          to   |-|-|-|-|-|-|-|-|-|-|-|
 *          319  |-|-|-|-|-|-|-|-|-|-|-|
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *               |-|-|-|-|-|-|-|-|-|-|-|
 *      (0,319)  -----------------------(239,319)
 *
 *               |
 *               |^
 *               y axis
 *
 * So for using full canvas of ST7789, xStart = 0, xEnd=239
 * For using full canvas of ST7789, yStart = 0, yEnd = 319
 *
 * @param x0&y0 -> x(Xstart) and y(Ystart) axis start
 * @param x1&y1 -> x(Xend) and y(Yend) axis end
 * @return none
 */

void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	gb_ST7789_CS_pin_low();

	uint16_t x_start = x0;
	uint16_t x_end = x1;
	uint16_t y_start = y0;
	uint16_t y_end = y1;

	/*
	 *
	 */
	uint8_t ColAddr[4]={ x_start >> 8,x_start & 0xFF,x_end >> 8, x_end & 0xff};
	//uint8_t ColAddr[4]={ 0x00,0x00,0x00, 0xEF};

	GB_ST7789_SendCommand(ST77XX_CASET, ColAddr, 4, 10);
	uint8_t RowAddr[4] = { y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
	//uint8_t RowAddr[4] = { 0x00, 0x50,0x01, 0x3F};

	GB_ST7789_SendCommand(ST77XX_RASET, RowAddr, 4, 10);

	gb_ST7789_CS_pin_high();
}
uint16_t total =0;
void ST7789_Fill_Color(uint16_t color)
{

	uint16_t i,j;
	uint8_t data;
	gb_ST7789_CS_pin_low();


	GB_ST7789_SendCommand(ST77XX_RAMWR, &data, 0, 10);

	for (i =0; i < ST7789_WIDTH; i++)
		for (j=0; j<ST7789_HEIGHT; j++)
		{
			uint8_t data[] = { color >>8, color & 0xFF};
			GB_ST7789_SendData(data, sizeof(data));
		}

	gb_ST7789_CS_pin_high();
}
uint32_t length = 0;
uint16_t i,j;

/*
 * @brief This API, will print images on the canvas
 * @param x -> value of cell(x cordinate) from where image should start(range 0->240)
 * @param y -> value of cell(y cordinate) from where image should start(range 0->240)
 * @param w -> width of image
 * @param h -> height of image
 * @param data -> pointer to array of bytes of image
 */
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *data)
{
	uint8_t d_data;

	ST7789_SetAddressWindow(x,y, (x+w)-1, (y+h)-1);
	gb_ST7789_CS_pin_low();

	GB_ST7789_SendCommand(ST77XX_RAMWR, &d_data, 0, 10);

	uint16_t BytesToSend,NoOfRounds =0;
	uint32_t TotalCellsToBeFilled = w*h;
	uint32_t TotalBytesToBeFilled = w*h*2;
	if(TotalBytesToBeFilled <= 14400)
	{
		NoOfRounds = 1;
		BytesToSend = TotalBytesToBeFilled;
	}else if(TotalBytesToBeFilled > 14400)
	{
		NoOfRounds = TotalBytesToBeFilled/7200;
		BytesToSend = TotalBytesToBeFilled/NoOfRounds;
	}

	for (j =0; j<NoOfRounds; j++)
	 {
		{
			GB_ST7789_SendDataIm(&data[(BytesToSend *j)], BytesToSend);

		}
	}

   gb_ST7789_CS_pin_high();

}

void ST7789DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	ST7789_SetAddressWindow(x,y,x,y);
	uint8_t data[] = {color>>8, color & 0xFF};
	gb_ST7789_CS_pin_low();
	GB_ST7789_SendData(data, sizeof(data));
	gb_ST7789_CS_pin_high();

}

//void ST7789_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
//{
//
//}
/**
 * @brief Write a char
 * @param  x&y -> cursor of the start point.
 * @param ch -> char to write
 * @param font -> fontstyle of the string
 * @param color -> color of the char
 * @param bgcolor -> background color of the char
 * @return  none
 */
void ST7789_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
	uint32_t i, b, j;
	uint8_t d_ata;

	ST7789_SetAddressWindow(x,y, (x+font.width)-1, (y+font.height)-1);

	gb_ST7789_CS_pin_low();

	GB_ST7789_SendCommand(ST77XX_RAMWR, &d_ata, 0, 10);

	for (i = 0; i < font.height; i++)
	{
		b = font.data[(ch - 32) * font.height + i];
		for (j = 0; j < font.width; j++)
		{
			if ((b << j) & 0x8000)
			{
				uint8_t data[] = {color >> 8, color & 0xFF};

				GB_ST7789_SendData(data, sizeof(data));
			}
			else
			{
				uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};

				GB_ST7789_SendData(data, sizeof(data));
			}
		}
	}
	gb_ST7789_CS_pin_high();
}
/**
 * @brief Write a string
 * @param  x&y -> cursor of the start point.
 * @param str -> string to write
 * @param font -> fontstyle of the string
 * @param color -> color of the string
 * @param bgcolor -> background color of the string
 * @return  none
 * */
void ST7789_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor)
{
	{
		while (*str)
		{
			if (x + font.width >= ST7789_WIDTH)
			{
				x = 0;
				y += font.height;
				if (y + font.height >= ST7789_HEIGHT)
				{
					break;
				}

				if (*str == ' ')
				{
					// skip spaces in the beginning of the new line
					str++;
					continue;
				}
			}
			ST7789_WriteChar(x, y, *str, font, color, bgcolor);
			x += font.width;
			str++;
		}
	}
}
//void ST7789_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
//{
//	//SPI start: chip select low
//   for(uint16_t i = x; i<x+w; i++)
//   {
//	   ST7789_DrawLineV
//	   //draw vertical line function
//   }
//   //SPI end: chip select high
//}
//
//void ST7789_DrawLineV()
//{
//	//SPI start: chip select low
//
//	ST7789_DrawLine();
//	//SPI end: chip select high
//}
