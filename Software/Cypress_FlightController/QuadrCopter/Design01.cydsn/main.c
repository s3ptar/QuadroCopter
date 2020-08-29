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

void DmaTxConfiguration(void);
void DmaRxConfiguration(void);

/* DMA Configuration for DMA_TX */
#define DMA_TX_BYTES_PER_BURST      (1u)
#define DMA_TX_REQUEST_PER_BURST    (1u)
#define DMA_TX_SRC_BASE             (CYDEV_SRAM_BASE)
#define DMA_TX_DST_BASE             (CYDEV_PERIPH_BASE)

#define BUFFER_SIZE                 (8u)
#define STORE_TD_CFG_ONCMPLT        (1u)

/* Variable declarations for DMA_TX*/
uint8 txChannel;
uint8 txTD;

/* Data buffers */
uint8 txBuffer [BUFFER_SIZE] = {0x0u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u};

/* DIE TEMP */
int16 DIEtemperature;


/***********************************************************************
*! \fn          void DmaTxConfiguration()
*  \brief       Configures the DMA transfer for TX direction
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void DmaTxConfiguration()
{
    /* Init DMA, 1 byte bursts, each burst requires a request */ 
    txChannel = DMA_UART_Tx(DMA_TX_BYTES_PER_BURST, DMA_TX_REQUEST_PER_BURST, 
                                        HI16(DMA_TX_SRC_BASE), HI16(DMA_TX_DST_BASE));

    txTD = CyDmaTdAllocate();

    /* Configure this Td as follows:
    *  - Increment the source address, but not the destination address   
    */
    CyDmaTdSetConfiguration(txTD, BUFFER_SIZE, CY_DMA_DISABLE_TD, TD_INC_SRC_ADR);

    /* From the memory to the SPIM */
    //CyDmaTdSetAddress(txTD, LO16((uint32)txBuffer), LO16((uint32) SPIM_TXDATA_PTR));
    
    /* Associate the TD with the channel */
    CyDmaChSetInitialTd(txChannel, txTD); 
} 

/***********************************************************************
*! \fn          int main(void)
*  \brief       Start Up Function 
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USB_CDC_1_CDC_Init();
    Cycle_ISR_1_Start();
    DMA_Timer_Start();
    Timer_1_Start();
    DieTemp_1_Start();

    CyGlobalIntEnable;
    
    for(;;)
    {
        /* Place your application code here. */
        
        /*****IF USB Connect send telemetrie***/
        //send die
        DieTemp_1_GetTemp(&DIEtemperature);
        USB_CDC_1_PutString("DIE - Temp");
        
 
    
    }
}

/* [] END OF FILE */
