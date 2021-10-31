/***********************************************************************
*! \file:                   SSD1306.c
*  \projekt:                JmBadge Bicycle
*  \created on:             Apr 19, 2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief					±1.5g - 6g Three Axis Low-g Micromachined 
*  AccelerometerThe MMA7260QT low cost capacitive micromachined 
*  accelerometer features signal conditioning, a 1-pole low pass filter, 
*  temperature compensation and g-Select which allows for the selection 
*  among 4 sensitivities. Zero-g offset full scale span and filter 
*  cut-off are factory set and require no external devices. Includes 
*  a Sleep Mode that makes it ideal for handheld battery powered 
*  electronics
***********************************************************************/

/***********************************************************************
* Includes
***********************************************************************/
#include "SSD1306.h"
#include "WAIT1.h" /* Waiting routines */
#include "CI2C1.h"
#include "SSD1306_Reset.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.nxp.com/files-static/sensors/doc/data_sheet/MMA7260QT.pdf
//https://www.mikrocontroller.net/topic/415651
/***********************************************************************
* Global Variable
***********************************************************************/


/***********************************************************************
* Local Funtions
***********************************************************************/
void SSD1306_Gotoxy(uint8_t x, uint8_t y);
void SSD1306_Invert(uint8_t invert);
/***********************************************************************
* Local Variable
***********************************************************************/
//uint8_t SSD1306_DisplayBuf[((SSD1306_DISPLAY_HW_NOF_ROWS-1)/8)+1][SSD1306_DISPLAY_HW_NOF_COLUMNS]; /* buffer for the display + 1 Byte for Command*/
//SSD1306_DisplayBuf_t SSD1306_DisplayBuf;
static uint8_t actCol = 0;
static uint8_t actPage = 0;
static word bytes_send;
uint8_t FrameBuffer[((SSD1306_DISPLAY_HW_NOF_ROWS)/8)+1][SSD1306_DISPLAY_HW_NOF_COLUMNS]; /* buffer for the display + 1 Byte for Command*/
const uint16_t BufferSize = 1024;
/***********************************************************************
* Constant
***********************************************************************/



/***********************************************************************
*! \fn			void SSD1306_WriteCommand(uint8_t cmd)
*  \brief		write an Command to SSD1306 Controller
*  \param		uint8_t cmd
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_WriteCommand(uint8_t cmd){
	//define cmd write write routine here

	uint8_t buffer[2];
	buffer[0] = SSD1306_CMD_REG;
	buffer[1] = cmd;
	//wait for free bu
	
	CI2C1_SelectSlave(SSD1306_CONFIG_SSD1306_I2C_ADDR);
	CI2C1_SendBlock(buffer, sizeof(buffer), &bytes_send);
	//GI2C1_WriteByteAddress8(SSD1306_CONFIG_SSD1306_I2C_ADDR, SSD1306_CMD_REG, cmd);
#if SSD1306_CONFIG_SSD1306_I2C_DELAY_US>0
	WAIT1_Waitus(SSD1306_CONFIG_SSD1306_I2C_DELAY_US);
#endif
}

/***********************************************************************
*! \fn			void SSD1306_WriteData(uint8_t data)
*  \brief		write Data to SSD1306 Controller
*  \param		uint8_t cmd
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_WriteData(uint8_t data){

	//define cmd write write routine here
	uint8_t buffer[2];

	buffer[0] = SSD1306_DATA_REG;
	buffer[1] = data;

	CI2C1_SelectSlave(SSD1306_CONFIG_SSD1306_I2C_ADDR);
	CI2C1_SendBlock(buffer, sizeof(buffer), &bytes_send);
	//(SSD1306_CONFIG_SSD1306_I2C_ADDR, SSD1306_DATA_REG, data);
#if SSD1306_CONFIG_SSD1306_I2C_DELAY_US>0
	WAIT1_Waitus(SSD1306_CONFIG_SSD1306_I2C_DELAY_US);
#endif
}

/***********************************************************************
*! \fn			void SSD1306_WriteDataBlock(uint8_t *data, size_t size)
*  \brief		write datablock to ssd1306
*  \param		uint8_t cmd
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_WriteDataBlock(uint8_t *data, uint16_t size){

	CI2C1_SelectSlave(SSD1306_CONFIG_SSD1306_I2C_ADDR);
	CI2C1_SendBlock(data, size, &bytes_send);
	
#if SSD1306_CONFIG_SSD1306_I2C_DELAY_US>0
	WAIT1_Waitus(SSD1306_CONFIG_SSD1306_I2C_DELAY_US);
#endif

}

/***********************************************************************
*! \fn			void SSD1306_Reset(bool reset)
*  \brief		reset the SSD1306
*  \param		bool reset -  1 = Reset, 0 = active
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_Reset(bool reset){
	
	if(reset)
		SSD1306_Reset_ClrVal();
	else
		SSD1306_Reset_SetVal();
}
/***********************************************************************
*! \fn			static void SSD1306_SetPageStartAddr(uint8_t page)
*  \brief		set the Adress from the page
*  \param		uint8_t page -> Set Page
*  \exception	none
*  \return		none
***********************************************************************/
static void SSD1306_SetPageStartAddr(uint8_t page) {
	
	actPage = page;
	if(actPage>=SSD1306_DISPLAY_HW_NOF_PAGES) {
		return;
	}
	SSD1306_WriteCommand(0xB0 | actPage);
}
/***********************************************************************
*! \fn			static void SSD1306_SetColStartAddr(uint8_t col)
*  \brief		set the Adress from the Colum
*  \param		uint8_t col
*  \exception	none
*  \return		none
***********************************************************************/
static void SSD1306_SetColStartAddr(uint8_t col){
	
	actCol = col;
	if(actCol>=SSD1306_DISPLAY_HW_NOF_COLUMNS) {
		return;
	}
	SSD1306_WriteCommand(0x10 | (actCol>>4));
	SSD1306_WriteCommand(actCol & 0x0F);
}

/***********************************************************************
*! \fn			void SSD1306_gotoxy(uint8_t x, uint8_t y)
*  \brief		set display to x,y position
*  \param		uint8_t x
*  \param		uint8_t y
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_Gotoxy(uint8_t x, uint8_t y){
	x = x * 6;
    SSD1306_SetPageStartAddr(y);
	SSD1306_SetColStartAddr(x);
}

/***********************************************************************
*! \fn			void SSD1306_Invert(uint8_t invert)
*  \brief		invert selected pixel
*  \param		uint8_t invert - 1 = invert
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_Invert(uint8_t invert){

    if (invert == 1) {
    	SSD1306_WriteCommand(SSD1306_INVERT_DISPLAY);
    } else {
    	SSD1306_WriteCommand(SSD1306_NORMAL_DISPLAY);
    }
}



/***********************************************************************
*! \fn			void SSD1306_Clear(void)
*  \brief		Clears the whole display memory.
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_Clear(void){
	uint16_t index;
	uint8_t *p = &FrameBuffer[0][127];
	//(*SSD1306_DisplayBuf)[1][0];
	
	for(index=0; index<(BufferSize+1); index++) {
		*p = 0x00;
		p++;
	}
	SSD1306_UpdateFull();
}

/***********************************************************************
*! \fn			void SSD1306_UpdateFull_UpdateFull(void)
*  \brief		Updates the whole display from the microcontroller RAM
*		        display buffer
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_UpdateFull(void){
	
#if SSD1306_CONFIG_SSD1306_DRIVER_TYPE==1306 /* SSD1306 */
	SSD1306_SetPageStartAddr(0);
	SSD1306_SetColStartAddr(0);
	FrameBuffer[0][127] = 0x40;
	SSD1306_WriteDataBlock(&FrameBuffer[0][127], (BufferSize+1));
	
	
#elif SSD1306_CONFIG_SSD1306_DRIVER_TYPE==1106 /* SH1306 */
	/* the SSH1306 has a 132x64 memory organization (compared to the 128x64 of the SSD1306) */
	int page;

	for(page=0; page<SSD1306_DISPLAY_HW_NOF_PAGES; page++) {
		SSD1306_SetPageStartAddr(page);
		SSD1306_SetColStartAddr(0);
		SSD1306_WriteDataBlock(&SSD1306_DisplayBuf[0][0]+(page*SSD1306_DISPLAY_HW_NOF_COLUMNS), SSD1306_DISPLAY_HW_NOF_COLUMNS);
	}
#else
  	  #error "unknown display type?"
#endif
}

/***********************************************************************
*! \fn			uint8_t SSD1306_SetContrast(uint8_t contrast)
*  \brief		Sets the display contrast level (default:0x7F)
*  \param		uint8_t contrast
*  \exception	none
*  \return		none
***********************************************************************/
uint8_t SSD1306_SetContrast(uint8_t contrast){
	SSD1306_WriteCommand(SSD1306_SET_CONTRAST);
	SSD1306_WriteCommand(contrast);
	return ERR_OK;
}

/***********************************************************************
*! \fn			uint8_t SSD1306_DisplayOn(bool on)
*  \brief		Turns the display on or off (sleep)
*  \param		bool on
*  \exception	none
*  \return		none
***********************************************************************/
uint8_t SSD1306_DisplayOn(bool on){
	if(on) {
		SSD1306_Reset(0);
		SSD1306_WriteCommand(SSD1306_DISPLAY_ON);                        /* turn display on */
	} else {
		SSD1306_WriteCommand(SSD1306_DISPLAY_OFF);                       /* turn display off */
	}
	return ERR_OK;
}


/***********************************************************************
*! \fn			uint8_t SSD1306_DisplayInvert(bool invert)
*  \brief		sed to inverse the display. In normal mode, 1 one in the
*		        display RAM is a pixel set and a 0 means pixel clear.
*  \param		bool invert - if TRUE, a 1 is pixel clear and a 0
*               is a pixel set.
*  \exception	none
*  \return		none
***********************************************************************/
uint8_t SSD1306_DisplayInvert(bool invert){
	if(invert) {
		SSD1306_WriteCommand(SSD1306_INVERT_DISPLAY);
	} else {
		SSD1306_WriteCommand(SSD1306_NORMAL_DISPLAY);
	}
	return ERR_OK;
}



/***********************************************************************
*! \fn			void SSD1306_DrawDot(uint8_t x, uint8_t y, bool bit)
*  \brief		draw an dot to the display
*  \param		uint8_t x	- x position
*  \param		uint8_t y	- y position
*  \param		bool bit	- 1 draw dot, 0 clear dot
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_DrawDot(uint8_t x, uint8_t y, bool bit){
	
	uint8_t mask = (y%8);
	if(bit)
		FrameBuffer[((y/8)+1)][x] |= 1<<mask ;		//set bit
	else
		FrameBuffer[((y/8)+1)][x] &= ~(1<<mask) ;	//clear bit
}

/***********************************************************************
*! \fn			void SSD1306_Init(void)
*  \brief		Display driver initialization
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_Init(void){

	//Reset Display
	SSD1306_Reset(1);
	WAIT1_Waitms(SSD1306_CONFIG_INIT_DELAY_MS);
	WAIT1_Waitms(10);
	WAIT1_Waitms(SSD1306_CONFIG_INIT_DELAY_MS);
	SSD1306_Reset(0);
	WAIT1_Waitms(500);
	
#if SSD1306_CONFIG_INIT_DELAY_MS>0
	WAIT1_Waitms(SSD1306_CONFIG_INIT_DELAY_MS);                           /* give hardware time to power up*/
#endif

	SSD1306_WriteCommand(SSD1306_DISPLAY_OFF);                         /* turn off display */
	SSD1306_WriteCommand(SSD1306_SET_DISPLAY_CLOCK_DIV);               /* set display clock divide ratio/oscillator frequency */
	SSD1306_WriteCommand(0x80);                                        /* the suggested ratio 0x80 */

	SSD1306_WriteCommand(SSD1306_SET_MULTIPLEX);                       /* set multiplex ratio(1 to 64) */
	SSD1306_WriteCommand(SSD1306_DISPLAY_HW_NOF_ROWS-1);                  /* multiplex depending on number of rows */

	SSD1306_WriteCommand(SSD1306_SET_DISPLAY_OFFSET);                  /* set display offset */
	SSD1306_WriteCommand(0x00);                                        /* no offset */
	SSD1306_WriteCommand(SSD1306_SET_START_LINE | 0x00);               /* set start line address */

	SSD1306_WriteCommand(SSD1306_COLUMN_ADDR);                         /* set start and end address of the columns */
	SSD1306_WriteCommand(0);
	SSD1306_WriteCommand(SSD1306_DISPLAY_HW_NOF_COLUMNS-1);

	SSD1306_WriteCommand(SSD1306_PAGE_ADDR);                           /* set start and end address of the pages */
	SSD1306_WriteCommand(0);
	SSD1306_WriteCommand(SSD1306_DISPLAY_HW_NOF_PAGES-1);

	SSD1306_WriteCommand(SSD1306_CHARGE_PUMP);                         /* set charge pump enable/disable */
#if SSD1306_CONFIG_SSD1306_EXTERNAL == 1
  SSD1306_WriteCommand(0x10);                                        /* set to disabled */
#else
  	SSD1306_WriteCommand(0x14);                                        /* set to enabled */
#endif
#if SSD1306_CONFIG_SSD1306_128X32
  SSD1306_WriteCommand(SSD1306_SET_COM_PINS);                        /* set COM pins hardware configuration */
  SSD1306_WriteCommand(0x02);
  SSD1306_WriteCommand(SSD1306_SET_CONTRAST);                        /* set contrast control register */
  SSD1306_WriteCommand(0x8F);
#else
	SSD1306_WriteCommand(SSD1306_SET_COM_PINS);                        /* set COM pins hardware configuration */
	SSD1306_WriteCommand(0x12);
	SSD1306_WriteCommand(SSD1306_SET_CONTRAST);                        /* set contrast control register */
  #if SSD1306_CONFIG_SSD1306_EXTERNAL == 1
	SSD1306_WriteCommand(0x9F);
  #else
	SSD1306_WriteCommand(0xCF);
  #endif
#endif

	SSD1306_WriteCommand(SSD1306_SET_PRECHARGE);                       /* set pre-charge period */
#if SSD1306_CONFIG_SSD1306_EXTERNAL == 1
	SSD1306_WriteCommand(0x22);
#else
	SSD1306_WriteCommand(0xF1);
#endif
	SSD1306_WriteCommand(SSD1306_SET_VCOM_DETECT);                     /* set vcomh */
	SSD1306_WriteCommand(0x40);

	SSD1306_WriteCommand(SSD1306_DISPLAY_ALL_ON_RESUME);               /* disable entire display on */
	SSD1306_WriteCommand(SSD1306_NORMAL_DISPLAY);                      /* set normal display */
	SSD1306_WriteCommand(SSD1306_DEACTIVATE_SCROLL);                   /* Deactivate scrolling */
	SSD1306_WriteCommand(SSD1306_DISPLAY_ON);                          /* turn on oled panel */

	SSD1306_WriteCommand(SSD1306_MEMORY_MODE);                         /* set memory mode */
	SSD1306_WriteCommand(0x00);                                        /* 00b: horizontal addressing mode mode */
	SSD1306_WriteCommand(SSD1306_SEG_REMAP | 0x01);                    /* set segment re-map 128 to 0 */
	SSD1306_WriteCommand(SSD1306_COM_SCAN_DEC);                        /* set COM output scan direction 64 to 0 */

	SSD1306_Clear();

}
