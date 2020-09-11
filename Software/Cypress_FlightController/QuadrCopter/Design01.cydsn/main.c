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
#include "ITG3200.h"
#include "stdio.h"

void DmaTxConfiguration(void);
void DmaRxConfiguration(void);

/* DMA Configuration for DMA_TX */
#define DMA_TX_BYTES_PER_BURST      (1u)
#define DMA_TX_REQUEST_PER_BURST    (1u)
#define DMA_TX_SRC_BASE             (CYDEV_SRAM_BASE)
#define DMA_TX_DST_BASE             (CYDEV_PERIPH_BASE)

#define BUFFER_SIZE                 (8u)
#define STORE_TD_CFG_ONCMPLT        (1u)

#define USBFS_DEVICE    (0u)

/* The buffer size is equal to the maximum packet size of the IN and OUT bulk
* endpoints.
*/
#define USBUART_BUFFER_SIZE (64u)
#define LINE_STR_LENGTH     (20u)

/* Variable declarations for DMA_TX*/
uint8 txChannel;
uint8 txTD;

/* Data buffers */
uint8 txBuffer [BUFFER_SIZE] = {0x0u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u};
char USB_CDC_Buffer[USBUART_BUFFER_SIZE];

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
    uint16 count;
    uint8 buffer[USBUART_BUFFER_SIZE];
    uint8_t dummy;
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Cycle_ISR_1_Start();
    DMA_Timer_Start();
    Timer_1_Start();
    DieTemp_1_Start();
    //set to fullscale and 42HzFilrer
    setup_itg3200(0, 0, (3<<3)|(3));

    CyGlobalIntEnable;
    
     /* Start USBFS operation with 5-V operation. */
    USB_CDC_1_Start(USBFS_DEVICE, USB_CDC_1_5V_OPERATION);
    
    for(;;)
    {
        /* Place your application code here. */
        /******  get Sensor Data *****************/
        DieTemp_1_GetTemp(&DIEtemperature);
        
        /* Host can send double SET_INTERFACE request. */
        if (0u != USB_CDC_1_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USB_CDC_1_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data 
                 * from host. */
                USB_CDC_1_Init();
            }
        }
        
        /* Service USB CDC when device is configured. */
        dummy = USB_CDC_1_GetConfiguration();
        if (0u != dummy){
            
            /* Check for input data from host. */
            if (0u != USB_CDC_1_DataIsReady())
            {
                /* Read received data and re-enable OUT endpoint. */
                count = USB_CDC_1_GetAll(buffer);

                if (0u != count)
                {
                    /* Wait until component is ready to send data to host. */
                    while (0u == USB_CDC_1_CDCIsReady())
                    {
                    }

                    /* Send data back to host. */
                    USB_CDC_1_PutData(buffer, count);

                    /* If the last sent packet is exactly the maximum packet 
                    *  size, it is followed by a zero-length packet to assure
                    *  that the end of the segment is properly identified by 
                    *  the terminal.
                    */
                    if (USBUART_BUFFER_SIZE == count)
                    {
                        /* Wait until component is ready to send data to PC. */
                        while (0u == USB_CDC_1_CDCIsReady())
                        {
                        }

                        /* Send zero-length packet to PC. */
                        USB_CDC_1_PutData(NULL, 0u);
                    }
                }
            }
            
        }
        
        /*****IF USB Connect send telemetrie***/
        //send die
        
        
        
 
    
    }
}

/* [] END OF FILE */
