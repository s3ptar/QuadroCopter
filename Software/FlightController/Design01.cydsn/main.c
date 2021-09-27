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


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ISR_USR_BTN_Enable();
    //setup ISR Routines
    ISR_USR_BTN_StartEx(isr_usr_btn_handling);

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
