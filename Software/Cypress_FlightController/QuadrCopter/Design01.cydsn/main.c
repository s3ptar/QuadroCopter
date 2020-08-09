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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USB_CDC_1_CDC_Init();
    Cycle_ISR_1_Start();
    timer_clock_Start();
    Timer_1_Start();

    CyGlobalIntEnable;
    
    for(;;)
    {
        /* Place your application code here. */
        
 
    
    }
}

/* [] END OF FILE */
