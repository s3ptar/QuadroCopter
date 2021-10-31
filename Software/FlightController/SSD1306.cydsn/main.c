/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "config.h"
#include "interrutps.h"
#include "ssd1306.h"

#define DISPLAY_ADDRESS 0x3C // 011110+SA0+RW - 0x3C or 0x3D

int main(void)
{

    CyGlobalIntEnable;

    SSD1306_Init(DISPLAY_ADDRESS);
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // clearing display
    SSD1306_clear();    
    SSD1306_update();    


    // drawing a pixel
    gfx_drawLine(10, 10, 20, 20, WHITE);    
    SSD1306_update(); 
    
    CyDelay(800);


    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ISR_USR_BTN_Enable();
    //setup ISR Routines
    ISR_USR_BTN_StartEx(isr_usr_btn_handling);
    
    //Start SPI
    
    

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
