/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * =======================================

*/
#include "project.h"

const char dma_test_seq[] = "DMA Output \r\n";

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /*############## INIT #############################*/
    UART_Start();
    UART_PutString("QuadroCopterController E00 Start\r\n");
    UART_PutString("Init Start \r\n");
    UART_DMA_TIMER_TC_ISR_Start();
    UART_DMA_Clock_Start();
    UART_PutString(".");
    UART_DMA_TIMER_Start();
    UART_PutString(".");
    

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
