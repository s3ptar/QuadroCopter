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
#include <stdlib.h>
#include "ssd1306.h"
#include "SSD1306_Reset.h"
#include "I2C_1.h"
#include "font.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.nxp.com/files-static/sensors/doc/data_sheet/MMA7260QT.pdf
//https://www.mikrocontroller.net/topic/415651
/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* Constant and Definitions
***********************************************************************/

#define SSD1306_CMD_REG         		                    0x00
#define SSD1306_DATA_REG        		                    0x40

#define DISPLAYWIDTH                                        128
#define DISPLAYHEIGHT                                       64
#define DISPLAYPAGES                                        8                      
    
#define SSD1306_SETCONTRAST                                 0x81
#define SSD1306_DISPLAYALLON_RESUME                         0xA4
#define SSD1306_DISPLAYALLON                                0xA5
#define SSD1306_NORMALDISPLAY                               0xA6
#define SSD1306_INVERTDISPLAY                               0xA7
#define SSD1306_DISPLAYOFF                                  0xAE
#define SSD1306_DISPLAYON                                   0xAF
#define SSD1306_SETDISPLAYOFFSET                            0xD3
#define SSD1306_SETCOMPINS                                  0xDA
#define SSD1306_SETVCOMDETECT                               0xDB
#define SSD1306_SETDISPLAYCLOCKDIV                          0xD5
#define SSD1306_SETPRECHARGE                                0xD9
#define SSD1306_SETMULTIPLEX                                0xA8
#define SSD1306_SETLOWCOLUMN                                0x00
#define SSD1306_SETHIGHCOLUMN                               0x10
#define SSD1306_SETSTARTLINE                                0x40
#define SSD1306_MEMORYMODE                                  0x20
#define SSD1306_COLUMNADDR                                  0x21
#define SSD1306_PAGEADDR                                    0x22
#define SSD1306_COMSCANINC                                  0xC0
#define SSD1306_COMSCANDEC                                  0xC8
#define SSD1306_SEGREMAP                                    0xA0
#define SSD1306_CHARGEPUMP                                  0x8D
#define SSD1306_EXTERNALVCC                                 0x1
#define SSD1306_SWITCHCAPVCC                                0x2
#define SSD1306_ACTIVATE_SCROLL                             0x2F
#define SSD1306_DEACTIVATE_SCROLL                           0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 				    0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 				    0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 					    0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 	    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 	    0x2A

// I2C result status
#define TRANSFER_CMPLT    (0x00u)
#define TRANSFER_ERROR    (0xFFu)

#define SSD1306_PIXEL_BLACK  		  	                    0 /* 0 is a black pixel */
#define SSD1306_PIXEL_WHITE  		  	                    1 /* 1 is a color/white pixel */
#define SSD1306_COLOR_PIXEL_SET      	                    SSD1306_PIXEL_WHITE /* color for a pixel set */
#define SSD1306_COLOR_PIXEL_CLR      	                    SSD1306_PIXEL_BLACK /* color for a pixel cleared */

#define SSD1306_COLOR_BLACK          	                    SSD1306_PIXEL_BLACK
#define SSD1306_COLOR_WHITE          	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_RED            	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_BRIGHT_RED     	                    SSD1306_COLOR_WHITE
#define SSD1306_COLOR_DARK_RED       	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_GREEN          	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_BRIGHT_GREEN   	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_DARK_GREEN     	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_BLUE           	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_BRIGHT_BLUE    	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_DARK_BLUE      	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_YELLOW         	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_BRIGHT_YELLOW  	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_ORANGE         	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_GREY           	                    SSD1306_PIXEL_WHITE
#define SSD1306_COLOR_BRIGHT_GREY    	                    SSD1306_PIXEL_WHITE

#define SSD1306_PIXEL_ON  			  	                    SSD1306_COLOR_WHITE /* value of a pixel if it is 'on' */
#define SSD1306_PIXEL_OFF 			 	                    SSD1306_COLOR_BLACK /* value of a pixel if it is 'off' */


// see data sheet page 25 for Graphic Display Data RAM organization
// 8 pages, each page a row of DISPLAYWIDTH bytes
// start address of of row: y/8*DISPLAYWIDTH
// x pos in row: == x 
#define GDDRAM_ADDRESS(X,Y) ((_displaybuf)+((Y)/8)*(DISPLAYWIDTH)+(X))

// lower 3 bit of y determine vertical pixel position (pos 0...7) in GDDRAM byte
// (y&0x07) == position of pixel on row (page). LSB is top, MSB bottom
#define GDDRAM_PIXMASK(Y) (1 << ((Y)&0x07))

#define PIXEL_ON(X,Y) (*GDDRAM_ADDRESS(x,y) |= GDDRAM_PIXMASK(y))
#define PIXEL_OFF(X,Y) (*GDDRAM_ADDRESS(x,y) &= ~GDDRAM_PIXMASK(y))
#define PIXEL_TOGGLE(X,Y) (*GDDRAM_ADDRESS(x,y) ^= GDDRAM_PIXMASK(y)) 
/***********************************************************************
* Local Funtions
***********************************************************************/
static uint32 SSD1306_write_buf( uint8_t* buf, uint16_t size );
void gfx_init( int16_t width, int16_t height );
// display memory buffer ( === MUST INCLUDE === the preceding I2C 0x40 control byte for the display)
static uint8_t SSD1306_buffer[DISPLAYHEIGHT * DISPLAYWIDTH / 8 + 1] = { 0x40 };
// pointer to actual display memory buffer
static uint8_t* _displaybuf = SSD1306_buffer+1;
static uint16_t _displaybuf_size = sizeof(SSD1306_buffer) - 1;
void SSD1306_HAL_Reset(bool reset);
void SSD1306_SetPageStartAddr(uint8_t page);
static void SSD1306_SetColStartAddr(uint8_t col);
/***********************************************************************
* Local Variable
***********************************************************************/
static uint8 _i2caddr;
int16_t WIDTH, HEIGHT; // This is the 'raw' display w/h - never changes
static int16_t _width, _height; // Display w/h as modified by current rotation
static int16_t cursor_x, cursor_y;
static uint16_t textcolor, textbgcolor;
static uint8_t textsize;
uint8_t rotation;
uint8_t wrap; // If set, 'wrap' text at right edge of display
static uint8_t actCol = 0;
static uint8_t actPage = 0;
/***********************************************************************
*! \fn			void SSD1306_Init(uint8_t i2caddr)
*  \brief		Display driver initialization
*  \param		uint8_t i2caddr - Adress of SSD106
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_Init(uint8_t i2caddr){
    
    I2C_1_Enable();
    I2C_1_Start();

    _i2caddr = i2caddr;
    gfx_init( DISPLAYWIDTH, DISPLAYHEIGHT );
    
	SSD1306_HAL_Reset(1);
	CyDelay(500);
	SSD1306_HAL_Reset(0);
	CyDelay(800);

    uint8_t cmdbuf[] = {
        0x00,
        
        SSD1306_DISPLAYOFF,                         /* turn off display */       
        SSD1306_SETDISPLAYCLOCKDIV,                 /* set display clock divide ratio/oscillator frequency */
        0x80,                                       /* the suggested ratio 0x80 */
        
        SSD1306_SETMULTIPLEX,                       /* set multiplex ratio(1 to 64) */
        DISPLAYHEIGHT-1,                            /* multiplex depending on number of rows */
        
        SSD1306_SETDISPLAYOFFSET,                   /* set display offset */
        0x00,                                       /* no offset */
        SSD1306_SETSTARTLINE | 0x0,                 /* set start line address */
        
        SSD1306_COLUMNADDR,                         /* set start and end address of the columns */
        0,
        DISPLAYWIDTH-1,
        
        SSD1306_PAGEADDR,                           /* set start and end address of the pages */
        0,
        DISPLAYPAGES-1,
        
        SSD1306_CHARGEPUMP,                         /* set charge pump enable/disable */
        0x14,
        
        SSD1306_SETCOMPINS,                         /* set COM pins hardware configuration */
        0x12,
        
        SSD1306_SETCONTRAST,                        /* set contrast control register */
        0x7f,
        
        SSD1306_SETPRECHARGE,                       /* set pre-charge period */
        0xf1,
        
        SSD1306_SETVCOMDETECT,                      /* set vcomh */
        0x40,
        
        SSD1306_DISPLAYALLON_RESUME,                /* disable entire display on */
        SSD1306_NORMALDISPLAY,                      /* set normal display */
        //SSD1306_DEACTIVATE_SCROLL,                /* Deactivate scrolling */
        SSD1306_DISPLAYON,                          /* turn on oled panel */
        
        SSD1306_MEMORYMODE,                         /* set memory mode */
        0x00,                                       /* 00b: horizontal addressing mode mode */
        SSD1306_SEGREMAP | 0x1,                     /* set segment re-map 128 to 0 */
        SSD1306_COMSCANDEC                         /* set COM output scan direction 64 to 0 */
    };
    
    SSD1306_write_buf( cmdbuf, sizeof(cmdbuf) ); 

}
/***********************************************************************
*! \fn			static void SSD1306_setPixel( int16_t x, int16_t y, uint16_t color )
*  \brief		used by gfx_ functions. Needs to be implemented by SSD1306_
*  \param		int16_t x
*  \param		int16_t y
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
static void SSD1306_setPixel( int16_t x, int16_t y, uint16_t color ){
    
    if( (x < 0) || (x >= DISPLAYWIDTH) || (y < 0) || (y >= DISPLAYHEIGHT) )
        return;

    switch( color ){
        case WHITE: 
            PIXEL_ON(x,y);
            break;
        case BLACK:
            PIXEL_OFF(x,y);
            break;
        case INVERSE: 
            PIXEL_TOGGLE(x,y);
            break;
    }
}
/***********************************************************************
*! \fn			void SSD1306_clear(void)
*  \brief		clear display buffer
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_clear(void){
    memset( _displaybuf, 0, _displaybuf_size );
    SSD1306_buffer[0] = 0x40; // to be sure its there
}
/***********************************************************************
*! \fn			void SSD1306_contrast( uint8_t contrast )
*  \brief		contrast: 0 ...255
*  \param		uint8_t contrast
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_contrast( uint8_t contrast ){
        
    uint8 cmdbuf[] = {
        0x00,  
        SSD1306_SETCONTRAST,
        contrast
    };
    SSD1306_write_buf( cmdbuf, sizeof(cmdbuf) ); 
}
/***********************************************************************
*! \fn			void SSD1306_invert( uint8_t invert )
*  \brief		invert <> 0 for inverse display, invert == 0 for normal display
*  \param		uint8_t invert
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_invert( uint8_t invert ){

    uint8 cmdbuf[] = {
        0x00,  
        0
    };
    cmdbuf[1] = invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY;
    SSD1306_write_buf( cmdbuf, sizeof(cmdbuf) ); 
}
/***********************************************************************
*! \fn			void SSD1306_update(void) 
*  \brief		send display buffer
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_update(void) {
      

    SSD1306_SetPageStartAddr(0);
	SSD1306_SetColStartAddr(0);
    SSD1306_write_buf( SSD1306_buffer, sizeof(SSD1306_buffer) );
}
/***********************************************************************
*! \fn			void SSD1306_SetPageStartAddr(uint8_t page) 
*  \brief		set the Adress from the page
*  \param		uint8_t page -> Set Page
*  \exception	none
*  \return		Page Address
***********************************************************************/
void SSD1306_SetPageStartAddr(uint8_t page) {
	
	actPage = page;
	if(actPage>=DISPLAYPAGES ) {
		return;
	}
    uint8 cmdbuf[] = {
        0x00,
        0xB0 | actPage          //set Page Address
    };
	SSD1306_write_buf( cmdbuf, sizeof(cmdbuf) );
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
	if(actCol>=DISPLAYWIDTH) {
		return;
	}
    uint8 cmdbuf[] = {
        0x00,
        (0x10 | (actCol>>4)),         //set Collum Address
        actCol & 0x0F
    };
	SSD1306_write_buf( cmdbuf, sizeof(cmdbuf));
}
/***********************************************************************
*! \fn			static void SSD1306_line( int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color )
*  \brief		draws horizontal or vertical line
*               Note: no check for valid coords, this needs to be done by caller
*               should only be called from gfx_hvline which is doing all validity checking
*  \param		int16_t x1
*  \param		int16_t y1
*  \param		int16_t x2
*  \param		int16_t y2
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
static void SSD1306_line( int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color ){

    if( x1 == x2 ){
        // vertical
        uint8_t* pstart = GDDRAM_ADDRESS(x1,y1);
        uint8_t* pend = GDDRAM_ADDRESS(x2,y2);       
        uint8_t* ptr = pstart;             
        
        while( ptr <= pend ){
            
            uint8_t mask;
            if( ptr == pstart ){
                // top
                uint8_t lbit = y1 % 8;
                // bottom (line can be very short, all inside this one byte)
                uint8_t ubit = lbit + y2 - y1;
                if( ubit >= 7 )
                    ubit = 7;
                mask = ((1 << (ubit-lbit+1)) - 1) << lbit;    
            }else if( ptr == pend ){
                // top is always bit 0, that makes it easy
                // bottom
                mask = (1 << (y2 % 8)) - 1;    
            }

            if( ptr == pstart || ptr == pend ){
                switch( color ){
                    case WHITE:     *ptr |= mask; break;
                    case BLACK:     *ptr &= ~mask; break;
                    case INVERSE:   *ptr ^= mask; break;
                };  
            }else{
                switch( color ){
                    case WHITE:     *ptr = 0xff; break;
                    case BLACK:     *ptr = 0x00; break;
                    case INVERSE:   *ptr ^= 0xff; break;
                };  
            }
            
            ptr += DISPLAYWIDTH;
        }
    }else{
        // horizontal
        uint8_t* pstart = GDDRAM_ADDRESS(x1,y1);
        uint8_t* pend = pstart + x2 - x1;
        uint8_t pixmask = GDDRAM_PIXMASK(y1);    

        uint8_t* ptr = pstart;
        while( ptr <= pend ){
            switch( color ){
                case WHITE:     *ptr |= pixmask; break;
                case BLACK:     *ptr &= ~pixmask; break;
                case INVERSE:   *ptr ^= pixmask; break;
            };
            ptr++;
        }
    }
}
/***********************************************************************
*! \fn			void SSD1306_stopscroll(void)
*  \brief		stop scrolling 
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_stopscroll(void){

    uint8 cmdbuf[] = {
        0x00,
        SSD1306_DEACTIVATE_SCROLL
    };
    SSD1306_write_buf( cmdbuf, sizeof(cmdbuf) ); 
}
/***********************************************************************
*! \fn			void SSD1306_scroll( SCROLL_AREA start, SCROLL_AREA end, SCROLL_DIR dir, SCROLL_SPEED speed 
*  \brief		scrolling
*  \param		SCROLL_AREA start
*  \param		SCROLL_AREA end
*  \param		SCROLL_DIR dir
*  \param		SCROLL_SPEED speed
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_scroll( SCROLL_AREA start, SCROLL_AREA end, SCROLL_DIR dir, SCROLL_SPEED speed ){
   
    uint8 cmdbuf[] = {
        0x00,
        dir,                    // 0x26 or 0x2a
        0x00,                   // dummy byte
        start,                  // start page
        speed,                  // scroll step interval in terms of frame frequency 
        end,                    // end page
        0x00,                   // dummy byte
        0xFF,                   // dummy byte
        SSD1306_ACTIVATE_SCROLL // 0x2F
    };
    SSD1306_write_buf( cmdbuf, sizeof(cmdbuf) ); 
}
//########################################### Graphic ###############################################################
/***********************************************************************
*! \fn			void gfx_init( int16_t width, int16_t height)
*  \brief		start gfx
*  \param		int16_t width
*  \param		int16_t height
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_init( int16_t width, int16_t height ){
    WIDTH = width;
    HEIGHT = height;
    _width = WIDTH;
    _height = HEIGHT;
    
    rotation = 0;
    cursor_y = cursor_x = 0;
    textsize = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap = 1;
}

/***********************************************************************
*! \fn			int16_t gfx_width(void)
*  \brief		Return the width of the display
*  \param		none
*  \exception	none
*  \return		int16_t display width
***********************************************************************/
int16_t gfx_width(void){
    return _width;
}
/***********************************************************************
*! \fn			int16_t gfx_height(void){
*  \brief		Return the height of the display
*  \param		none
*  \exception	none
*  \return		int16_t display heigth
***********************************************************************/
int16_t gfx_height(void){
    return _height;
}
/***********************************************************************
*! \fn			uint8_t gfx_rotation(void)
*  \brief		Return the size of the display (per current rotation
*  \param		none
*  \exception	none
*  \return		uint8_t gfx_rotation
***********************************************************************/
uint8_t gfx_rotation(void){
    return rotation;
}
/***********************************************************************
*! \fn			void gfx_setCursor( int16_t x, int16_t y )
*  \brief		set the cusor at the display
*  \param		int16_t x position
*  \param		int16_t y position
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_setCursor( int16_t x, int16_t y ){
    cursor_x = x;
    cursor_y = y;
}
/***********************************************************************
*! \fn			void gfx_setTextSize( uint8_t size )
*  \brief		set text size
*  \param		uint8_t size - set the text size
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_setTextSize( uint8_t size ){
    textsize = (size > 0) ? size : 1;
}

/***********************************************************************
*! \fn			void gfx_setTextColor( uint16_t color )
*  \brief		set text color
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_setTextColor( uint16_t color ){
    textcolor = textbgcolor = color;
}

/***********************************************************************
*! \fn			void gfx_setTextBg( uint16_t color )
*  \brief		set text backgroundcolor
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_setTextBg( uint16_t color ){
    textbgcolor = color;
}

/***********************************************************************
*! \fn			void gfx_setTextWrap( uint8_t w )
*  \brief		set text wrap
*  \param		uint8_t w
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_setTextWrap( uint8_t w ){
    wrap = w;
}
/***********************************************************************
*! \fn			void gfx_setRotation( uint8_t x )
*  \brief		set rotation
*  \param		uint8_t x
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_setRotation( uint8_t x ){
    
    rotation = (x & 3);
    switch( rotation ){
        case 0:
        case 2:
            _width = WIDTH;
            _height = HEIGHT;
            break;
        case 1:
        case 3:
            _width = HEIGHT;
            _height = WIDTH;
        break;
    }
}
/***********************************************************************
*! \fn			static void gfx_rotation_adjust( int16_t* px, int16_t* py )
*  \brief		set rotation adjust
*  \param		int16_t* px
*  \param		int16_t* py
*  \exception	none
*  \return		none
***********************************************************************/
static void gfx_rotation_adjust( int16_t* px, int16_t* py ){

    int16_t y0 = *py;
    
    switch( rotation ){
        case 1:
            *py = *px;
            *px = WIDTH - y0 - 1;
            break;
        case 2:
            *px = WIDTH - *px - 1;
            *py = HEIGHT - *py - 1;
            break;
        case 3:
            *py = HEIGHT - *px - 1;
            *px = y0;
            break;
    }
}
/***********************************************************************
*! \fn			void gfx_drawPixel( int16_t x, int16_t y, uint16_t color )
*  \brief		draw pixel
*  \param		int16_t x,
*  \param		int16_t y
*  \param		uint16_t color 
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_drawPixel( int16_t x, int16_t y, uint16_t color ){
    
    if( (x < 0) || (x >= _width) || (y < 0) || (y >= _height) )
        return;
    
    gfx_rotation_adjust( &x, &y );

    SSD1306_setPixel(x,y,color);
}

/***********************************************************************
*! \fn			static void gfx_hvLine( int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color )
*  \brief		helper function for gfx_drawLine, handles special 
*               cases of horizontal and vertical lines
*  \param		int16_t x1
*  \param		int16_t y1
*  \param		int16_t x2,
*  \param		int16_t y2
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
static void gfx_hvLine( int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color ){
    
    if( x1 != x2 && y1 != y2 ){
        // neither vertical nor horizontal
        return;
    }    
    
    // bounds check
    if( rotation == 1 || rotation == 3 ){
        if( x1 < 0 || x1 >= HEIGHT || x2 < 0 || x2 >= HEIGHT )
            return;
        if( y1 < 0 || y1 >= WIDTH || y2 < 0 || y2 >= WIDTH )
            return;
    }else{
        if( y1 < 0 || y1 >= HEIGHT || y2 < 0 || y2 >= HEIGHT )
            return;
        if( x1 < 0 || x1 >= WIDTH || x2 < 0 || x2 >= WIDTH )
            return;
    }
    
    gfx_rotation_adjust( &x1, &y1 );
    gfx_rotation_adjust( &x2, &y2 );
    
    // ensure coords are from left to right and top to bottom
    if( (x1 == x2 && y2 < y1) || (y1 == y2 && x2 < x1) ){
        // swap as needed
        int16_t t = x1; x1 = x2; x2 = t;
        t = y1; y1 = y2; y2 = t;
    }
    
    SSD1306_line( x1, y1, x2, y2, color );
}
/***********************************************************************
*! \fn			void gfx_drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color )
*  \brief		always use this function for line drawing
*  \param		int16_t x0
*  \param		int16_t y0
*  \param		int16_t x1
*  \param		int16_t y1
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color ){
 
    if( x0 == x1 || y0 == y1 ){
        // vertical and horizontal lines can be drawn faster
        gfx_hvLine( x0, y0, x1, y1, color );
        return;
    }
    
    int16_t t;
    
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if( steep ){
        t = x0; x0 = y0; y0 = t;
        t = x1; x1 = y1; y1 = t;
    }
    if( x0 > x1 ){
        t = x0; x0 = x1; x1 = t;
        t = y0; y0 = y1; y1 = t;
    }
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep;
    if( y0 < y1 ){
        ystep = 1;
    }else{
        ystep = -1;
    }
    for( ; x0<=x1; x0++ ){
        if( steep ){
            gfx_drawPixel( y0, x0, color );
        }else{
            gfx_drawPixel( x0, y0, color );
        }
        err -= dy;
        if( err < 0 ){
            y0 += ystep;
            err += dx;
        }
    }
}
/***********************************************************************
*! \fn			void gfx_drawRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color )
*  \brief		draw an rectangle
*  \param		int16_t x
*  \param		int16_t y
*  \param		int16_t w - border
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_drawRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ){
    
    gfx_drawLine( x, y, x+w-1, y, color );
    gfx_drawLine( x, y+h-1, x+w-1, y+h-1, color );
    gfx_drawLine( x, y, x, y+h-1, color );
    gfx_drawLine( x+w-1, y, x+w-1, y+h-1, color );
}
/***********************************************************************
*! \fn			gfx_fillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color )
*  \brief		draw an filled rectangle
*  \param		int16_t x
*  \param		int16_t y
*  \param		int16_t w - border
*  \param		int16_t h - heigth of filling
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_fillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ){
    int16_t i = 0;
    if( h > w ){
        for( i = x ; i < x+w ; i++ ){
            gfx_drawLine( i, y, i, y+h-1, color );
        }
    }else{
        for( i = y ; i < y+h ; i++ ){
            gfx_drawLine( x, i, x+w-1, i, color );
        }
    }
}

/***********************************************************************
*! \fn			void gfx_drawCircle( int16_t x0, int16_t y0, int16_t r,uint16_t color )
*  \brief		draw an circle outline
*  \param		int16_t x0
*  \param	    int16_t y0
*  \param		int16_t r
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_drawCircle( int16_t x0, int16_t y0, int16_t r,uint16_t color ){

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    gfx_drawPixel( x0 , y0+r, color );
    gfx_drawPixel( x0 , y0-r, color );
    gfx_drawPixel( x0+r, y0 , color );
    gfx_drawPixel( x0-r, y0 , color );
    while( x < y ){
        if( f >= 0 ){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        gfx_drawPixel( x0 + x, y0 + y, color );
        gfx_drawPixel( x0 - x, y0 + y, color );
        gfx_drawPixel( x0 + x, y0 - y, color );
        gfx_drawPixel( x0 - x, y0 - y, color );
        gfx_drawPixel( x0 + y, y0 + x, color );
        gfx_drawPixel( x0 - y, y0 + x, color );
        gfx_drawPixel( x0 + y, y0 - x, color );
        gfx_drawPixel( x0 - y, y0 - x, color );
    }
}
/***********************************************************************
*! \fn			void gfx_drawCircle( int16_t x0, int16_t y0, int16_t r,uint16_t color )
*  \brief		draw an circle outline
*  \param		int16_t x0
*  \param	    int16_t y0
*  \param		int16_t r
*  \param		uint16_t color
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_drawTriangle( int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color ){
    
    gfx_drawLine( x0, y0, x1, y1, color );
    gfx_drawLine( x1, y1, x2, y2, color );
    gfx_drawLine( x2, y2, x0, y0, color );
}

/***********************************************************************
*! \fn			void gfx_drawChar( int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bg, uint8_t size)
*  \brief		Draw a character
*  \param		int16_t x
*  \param		int16_t y
*  \param		unsigned char c
*  \param		uint16_t color
*  \param		uint16_t bg
*  \param		uint8_t size
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_drawChar( int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bg, uint8_t size) {
    if( (x >= _width) || // Clip right
        (y >= _height) || // Clip bottom
        ((x + 6 * size - 1) < 0) || // Clip left
        ((y + 8 * size - 1) < 0)) // Clip top
        return;

    int8_t i = 0;
    for( i = 0 ; i < 6 ; i++ ){
        uint8_t line;
        if( i == 5 )
            line = 0x0;
        else
           line = font[(c*5)+i];
        int8_t j = 0;
        for( j = 0; j < 8 ; j++ ){
            if( line & 0x1 ){
                if( size == 1 ) // default size
                    gfx_drawPixel( x+i, y+j, color );
                else { // big size
                    gfx_fillRect( x+(i*size), y+(j*size), size, size, color );
                }
            } else if( bg != color ){
                if( size == 1 ) // default size
                    gfx_drawPixel( x+i, y+j, bg );
                else { // big size
                    gfx_fillRect( x+i*size, y+j*size, size, size, bg );
                }
            }
            line >>= 1;
        }
    }
}
/***********************************************************************
*! \fn			void gfx_write( uint8_t ch )
*  \brief		write character
*  \param		uint8_t ch 
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_write( uint8_t ch ){
    if( ch == '\n' ){
        cursor_y += textsize*8;
        cursor_x = 0;
    }else if( ch == '\r' ){
        // skip em
    }else{
        gfx_drawChar(cursor_x, cursor_y, ch, textcolor, textbgcolor, textsize);
        cursor_x += textsize*6;
        if( wrap && (cursor_x > (_width - textsize*6)) ){
            cursor_y += textsize*8;
            cursor_x = 0;
        }
    }
}
/***********************************************************************
*! \fn			void gfx_print( const char* s )
*  \brief		write text
*  \param		const char* s - pointer to text
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_print( const char* s ){
    
    unsigned int len = strlen( s );
    unsigned int i = 0; 
    for( i = 0 ; i < len ; i++ ){
        gfx_write( s[i] );
    }
}
/***********************************************************************
*! \fn			void gfx_println( const char* s )
*  \brief		write text with new line
*  \param		const char* s - pointer to text
*  \exception	none
*  \return		none
***********************************************************************/
void gfx_println( const char* s ){ 
    gfx_print( s ); 
    gfx_write( '\n' );
}
//########################################### HAL ###############################################################

/***********************************************************************
*! \fn			static uint32 SSD1306_write_buf( uint8_t* buf, uint16_t size )
*  \brief		for submitting command sequences:
*               buf[0] must be 0x00
*               for submitting bulk data (writing to display RAM):
*               buf[0] must be 0x40
*  \param		uint8_t cmd - Command to send
*  \exception	none
*  \return		none
***********************************************************************/
static uint32 SSD1306_write_buf( uint8_t* buf, uint16_t size ){

    uint32_t status = TRANSFER_ERROR;
    uint8_t transmit_status;
    uint16_t cnt = 0;
    /*I2C_1_MasterWriteBuf(_i2caddr, buf, size, I2C_1_MODE_COMPLETE_XFER);
    while( 0u == (I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT) );

    if( 0u == (I2C_1_MSTAT_ERR_XFER & I2C_1_MasterStatus()) ){
        // Check if all bytes was written 
        if( I2C_1_MasterGetWriteBufSize() == size ){
            status = TRANSFER_CMPLT;
        }
    }
    I2C_1_MasterClearStatus();

    return status;*/
    I2C_1_DisableInt();
    transmit_status = I2C_1_MasterSendStart(_i2caddr, I2C_1_WRITE_XFER_MODE);
    /* Check if start generation completed without errors */
    if (I2C_1_MSTR_NO_ERROR == transmit_status){
        for(cnt = 0; cnt < size; cnt++){
            transmit_status = I2C_1_MasterWriteByte(*buf++);
            if (transmit_status != I2C_1_MSTR_NO_ERROR){
                break;
            }
        }
        I2C_1_MasterSendStop(); /* Send Stop */
    }
    I2C_1_EnableInt(); /* Enable interrupt, if it was enabled before */

    status = TRANSFER_CMPLT;
    return status;
    
}

/***********************************************************************
*! \fn			void SSD1306_HAL_Reset(bool reset)
*  \brief		reset the SSD1306
*  \param		bool reset -  1 = Reset, 0 = active
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_HAL_Reset(bool reset){
	
	if(reset)
		SSD1306_Reset_Write(0);
	else
		SSD1306_Reset_Write(1);
}

/***********************************************************************
*! \fn			void SSD1306_HAL_CS(bool cs)
*  \brief		control the SSD1306 cs pn
*  \param		bool cs -  1 = enable, 0 = disable
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_HAL_CS(bool cs){
#ifdef _Interface_SPI_
	if(cs)
		SSD1306_CS_Write(0);
	else
		SSD1306_CS_Write(1);
#endif
}

/***********************************************************************
*! \fn			void SSD1306_HAL_DC(bool dc)
*  \brief		control the SSD1306 dc pn
*  \param		bool dc -  0 = Command, 1 = Data
*  \exception	none
*  \return		none
***********************************************************************/
void SSD1306_HAL_DC(bool dc){
#ifdef _Interface_SPI_	
	if(dc)
		SSD1306_DC_Write(1);
	else
		SSD1306_DC_Write(0);
#endif
}