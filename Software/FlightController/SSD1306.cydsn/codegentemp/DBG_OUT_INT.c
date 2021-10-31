/*******************************************************************************
* File Name: DBG_OUTINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DBG_OUT.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (DBG_OUT_RX_INTERRUPT_ENABLED && (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED))
    /*******************************************************************************
    * Function Name: DBG_OUT_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_rxBuffer - RAM buffer pointer for save received data.
    *  DBG_OUT_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  DBG_OUT_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  DBG_OUT_rxBufferOverflow - software overflow flag. Set to one
    *     when DBG_OUT_rxBufferWrite index overtakes
    *     DBG_OUT_rxBufferRead index.
    *  DBG_OUT_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when DBG_OUT_rxBufferWrite is equal to
    *    DBG_OUT_rxBufferRead
    *  DBG_OUT_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  DBG_OUT_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(DBG_OUT_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef DBG_OUT_RXISR_ENTRY_CALLBACK
        DBG_OUT_RXISR_EntryCallback();
    #endif /* DBG_OUT_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START DBG_OUT_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = DBG_OUT_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in DBG_OUT_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (DBG_OUT_RX_STS_BREAK | 
                            DBG_OUT_RX_STS_PAR_ERROR |
                            DBG_OUT_RX_STS_STOP_ERROR | 
                            DBG_OUT_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                DBG_OUT_errorStatus |= readStatus & ( DBG_OUT_RX_STS_BREAK | 
                                                            DBG_OUT_RX_STS_PAR_ERROR | 
                                                            DBG_OUT_RX_STS_STOP_ERROR | 
                                                            DBG_OUT_RX_STS_OVERRUN);
                /* `#START DBG_OUT_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef DBG_OUT_RXISR_ERROR_CALLBACK
                DBG_OUT_RXISR_ERROR_Callback();
            #endif /* DBG_OUT_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & DBG_OUT_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = DBG_OUT_RXDATA_REG;
            #if (DBG_OUT_RXHW_ADDRESS_ENABLED)
                if(DBG_OUT_rxAddressMode == (uint8)DBG_OUT__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & DBG_OUT_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & DBG_OUT_RX_STS_ADDR_MATCH) != 0u)
                        {
                            DBG_OUT_rxAddressDetected = 1u;
                        }
                        else
                        {
                            DBG_OUT_rxAddressDetected = 0u;
                        }
                    }
                    if(DBG_OUT_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        DBG_OUT_rxBuffer[DBG_OUT_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    DBG_OUT_rxBuffer[DBG_OUT_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                DBG_OUT_rxBuffer[DBG_OUT_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (DBG_OUT_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(DBG_OUT_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        DBG_OUT_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    DBG_OUT_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(DBG_OUT_rxBufferWrite >= DBG_OUT_RX_BUFFER_SIZE)
                    {
                        DBG_OUT_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(DBG_OUT_rxBufferWrite == DBG_OUT_rxBufferRead)
                    {
                        DBG_OUT_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (DBG_OUT_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            DBG_OUT_RXSTATUS_MASK_REG  &= (uint8)~DBG_OUT_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(DBG_OUT_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (DBG_OUT_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & DBG_OUT_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START DBG_OUT_RXISR_END` */

        /* `#END` */

    #ifdef DBG_OUT_RXISR_EXIT_CALLBACK
        DBG_OUT_RXISR_ExitCallback();
    #endif /* DBG_OUT_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (DBG_OUT_RX_INTERRUPT_ENABLED && (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)) */


#if (DBG_OUT_TX_INTERRUPT_ENABLED && DBG_OUT_TX_ENABLED)
    /*******************************************************************************
    * Function Name: DBG_OUT_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_txBuffer - RAM buffer pointer for transmit data from.
    *  DBG_OUT_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  DBG_OUT_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(DBG_OUT_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef DBG_OUT_TXISR_ENTRY_CALLBACK
        DBG_OUT_TXISR_EntryCallback();
    #endif /* DBG_OUT_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START DBG_OUT_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((DBG_OUT_txBufferRead != DBG_OUT_txBufferWrite) &&
             ((DBG_OUT_TXSTATUS_REG & DBG_OUT_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(DBG_OUT_txBufferRead >= DBG_OUT_TX_BUFFER_SIZE)
            {
                DBG_OUT_txBufferRead = 0u;
            }

            DBG_OUT_TXDATA_REG = DBG_OUT_txBuffer[DBG_OUT_txBufferRead];

            /* Set next pointer */
            DBG_OUT_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START DBG_OUT_TXISR_END` */

        /* `#END` */

    #ifdef DBG_OUT_TXISR_EXIT_CALLBACK
        DBG_OUT_TXISR_ExitCallback();
    #endif /* DBG_OUT_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (DBG_OUT_TX_INTERRUPT_ENABLED && DBG_OUT_TX_ENABLED) */


/* [] END OF FILE */
