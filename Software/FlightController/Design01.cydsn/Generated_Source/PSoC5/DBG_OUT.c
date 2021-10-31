/*******************************************************************************
* File Name: DBG_OUT.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DBG_OUT.h"
#if (DBG_OUT_INTERNAL_CLOCK_USED)
    #include "DBG_OUT_IntClock.h"
#endif /* End DBG_OUT_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 DBG_OUT_initVar = 0u;

#if (DBG_OUT_TX_INTERRUPT_ENABLED && DBG_OUT_TX_ENABLED)
    volatile uint8 DBG_OUT_txBuffer[DBG_OUT_TX_BUFFER_SIZE];
    volatile uint8 DBG_OUT_txBufferRead = 0u;
    uint8 DBG_OUT_txBufferWrite = 0u;
#endif /* (DBG_OUT_TX_INTERRUPT_ENABLED && DBG_OUT_TX_ENABLED) */

#if (DBG_OUT_RX_INTERRUPT_ENABLED && (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED))
    uint8 DBG_OUT_errorStatus = 0u;
    volatile uint8 DBG_OUT_rxBuffer[DBG_OUT_RX_BUFFER_SIZE];
    volatile uint8 DBG_OUT_rxBufferRead  = 0u;
    volatile uint8 DBG_OUT_rxBufferWrite = 0u;
    volatile uint8 DBG_OUT_rxBufferLoopDetect = 0u;
    volatile uint8 DBG_OUT_rxBufferOverflow   = 0u;
    #if (DBG_OUT_RXHW_ADDRESS_ENABLED)
        volatile uint8 DBG_OUT_rxAddressMode = DBG_OUT_RX_ADDRESS_MODE;
        volatile uint8 DBG_OUT_rxAddressDetected = 0u;
    #endif /* (DBG_OUT_RXHW_ADDRESS_ENABLED) */
#endif /* (DBG_OUT_RX_INTERRUPT_ENABLED && (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)) */


/*******************************************************************************
* Function Name: DBG_OUT_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  DBG_OUT_Start() sets the initVar variable, calls the
*  DBG_OUT_Init() function, and then calls the
*  DBG_OUT_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The DBG_OUT_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time DBG_OUT_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the DBG_OUT_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DBG_OUT_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(DBG_OUT_initVar == 0u)
    {
        DBG_OUT_Init();
        DBG_OUT_initVar = 1u;
    }

    DBG_OUT_Enable();
}


/*******************************************************************************
* Function Name: DBG_OUT_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call DBG_OUT_Init() because
*  the DBG_OUT_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DBG_OUT_Init(void) 
{
    #if(DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)

        #if (DBG_OUT_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(DBG_OUT_RX_VECT_NUM, &DBG_OUT_RXISR);
            CyIntSetPriority(DBG_OUT_RX_VECT_NUM, DBG_OUT_RX_PRIOR_NUM);
            DBG_OUT_errorStatus = 0u;
        #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

        #if (DBG_OUT_RXHW_ADDRESS_ENABLED)
            DBG_OUT_SetRxAddressMode(DBG_OUT_RX_ADDRESS_MODE);
            DBG_OUT_SetRxAddress1(DBG_OUT_RX_HW_ADDRESS1);
            DBG_OUT_SetRxAddress2(DBG_OUT_RX_HW_ADDRESS2);
        #endif /* End DBG_OUT_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        DBG_OUT_RXBITCTR_PERIOD_REG = DBG_OUT_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        DBG_OUT_RXSTATUS_MASK_REG  = DBG_OUT_INIT_RX_INTERRUPTS_MASK;
    #endif /* End DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED*/

    #if(DBG_OUT_TX_ENABLED)
        #if (DBG_OUT_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(DBG_OUT_TX_VECT_NUM, &DBG_OUT_TXISR);
            CyIntSetPriority(DBG_OUT_TX_VECT_NUM, DBG_OUT_TX_PRIOR_NUM);
        #endif /* (DBG_OUT_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (DBG_OUT_TXCLKGEN_DP)
            DBG_OUT_TXBITCLKGEN_CTR_REG = DBG_OUT_BIT_CENTER;
            DBG_OUT_TXBITCLKTX_COMPLETE_REG = ((DBG_OUT_NUMBER_OF_DATA_BITS +
                        DBG_OUT_NUMBER_OF_START_BIT) * DBG_OUT_OVER_SAMPLE_COUNT) - 1u;
        #else
            DBG_OUT_TXBITCTR_PERIOD_REG = ((DBG_OUT_NUMBER_OF_DATA_BITS +
                        DBG_OUT_NUMBER_OF_START_BIT) * DBG_OUT_OVER_SAMPLE_8) - 1u;
        #endif /* End DBG_OUT_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (DBG_OUT_TX_INTERRUPT_ENABLED)
            DBG_OUT_TXSTATUS_MASK_REG = DBG_OUT_TX_STS_FIFO_EMPTY;
        #else
            DBG_OUT_TXSTATUS_MASK_REG = DBG_OUT_INIT_TX_INTERRUPTS_MASK;
        #endif /*End DBG_OUT_TX_INTERRUPT_ENABLED*/

    #endif /* End DBG_OUT_TX_ENABLED */

    #if(DBG_OUT_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        DBG_OUT_WriteControlRegister( \
            (DBG_OUT_ReadControlRegister() & (uint8)~DBG_OUT_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(DBG_OUT_PARITY_TYPE << DBG_OUT_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End DBG_OUT_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: DBG_OUT_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call DBG_OUT_Enable() because the DBG_OUT_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DBG_OUT_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void DBG_OUT_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        DBG_OUT_RXBITCTR_CONTROL_REG |= DBG_OUT_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        DBG_OUT_RXSTATUS_ACTL_REG  |= DBG_OUT_INT_ENABLE;

        #if (DBG_OUT_RX_INTERRUPT_ENABLED)
            DBG_OUT_EnableRxInt();

            #if (DBG_OUT_RXHW_ADDRESS_ENABLED)
                DBG_OUT_rxAddressDetected = 0u;
            #endif /* (DBG_OUT_RXHW_ADDRESS_ENABLED) */
        #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */
    #endif /* (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED) */

    #if(DBG_OUT_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!DBG_OUT_TXCLKGEN_DP)
            DBG_OUT_TXBITCTR_CONTROL_REG |= DBG_OUT_CNTR_ENABLE;
        #endif /* End DBG_OUT_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        DBG_OUT_TXSTATUS_ACTL_REG |= DBG_OUT_INT_ENABLE;
        #if (DBG_OUT_TX_INTERRUPT_ENABLED)
            DBG_OUT_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            DBG_OUT_EnableTxInt();
        #endif /* (DBG_OUT_TX_INTERRUPT_ENABLED) */
     #endif /* (DBG_OUT_TX_INTERRUPT_ENABLED) */

    #if (DBG_OUT_INTERNAL_CLOCK_USED)
        DBG_OUT_IntClock_Start();  /* Enable the clock */
    #endif /* (DBG_OUT_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DBG_OUT_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DBG_OUT_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)
        DBG_OUT_RXBITCTR_CONTROL_REG &= (uint8) ~DBG_OUT_CNTR_ENABLE;
    #endif /* (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED) */

    #if (DBG_OUT_TX_ENABLED)
        #if(!DBG_OUT_TXCLKGEN_DP)
            DBG_OUT_TXBITCTR_CONTROL_REG &= (uint8) ~DBG_OUT_CNTR_ENABLE;
        #endif /* (!DBG_OUT_TXCLKGEN_DP) */
    #endif /* (DBG_OUT_TX_ENABLED) */

    #if (DBG_OUT_INTERNAL_CLOCK_USED)
        DBG_OUT_IntClock_Stop();   /* Disable the clock */
    #endif /* (DBG_OUT_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)
        DBG_OUT_RXSTATUS_ACTL_REG  &= (uint8) ~DBG_OUT_INT_ENABLE;

        #if (DBG_OUT_RX_INTERRUPT_ENABLED)
            DBG_OUT_DisableRxInt();
        #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */
    #endif /* (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED) */

    #if (DBG_OUT_TX_ENABLED)
        DBG_OUT_TXSTATUS_ACTL_REG &= (uint8) ~DBG_OUT_INT_ENABLE;

        #if (DBG_OUT_TX_INTERRUPT_ENABLED)
            DBG_OUT_DisableTxInt();
        #endif /* (DBG_OUT_TX_INTERRUPT_ENABLED) */
    #endif /* (DBG_OUT_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DBG_OUT_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 DBG_OUT_ReadControlRegister(void) 
{
    #if (DBG_OUT_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(DBG_OUT_CONTROL_REG);
    #endif /* (DBG_OUT_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: DBG_OUT_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  DBG_OUT_WriteControlRegister(uint8 control) 
{
    #if (DBG_OUT_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       DBG_OUT_CONTROL_REG = control;
    #endif /* (DBG_OUT_CONTROL_REG_REMOVED) */
}


#if(DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)
    /*******************************************************************************
    * Function Name: DBG_OUT_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      DBG_OUT_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      DBG_OUT_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      DBG_OUT_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      DBG_OUT_RX_STS_BREAK            Interrupt on break.
    *      DBG_OUT_RX_STS_OVERRUN          Interrupt on overrun error.
    *      DBG_OUT_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      DBG_OUT_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void DBG_OUT_SetRxInterruptMode(uint8 intSrc) 
    {
        DBG_OUT_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  DBG_OUT_rxBuffer - RAM buffer pointer for save received data.
    *  DBG_OUT_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  DBG_OUT_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  DBG_OUT_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 DBG_OUT_ReadRxData(void) 
    {
        uint8 rxData;

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        DBG_OUT_DisableRxInt();

        locRxBufferRead  = DBG_OUT_rxBufferRead;
        locRxBufferWrite = DBG_OUT_rxBufferWrite;

        if( (DBG_OUT_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = DBG_OUT_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= DBG_OUT_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            DBG_OUT_rxBufferRead = locRxBufferRead;

            if(DBG_OUT_rxBufferLoopDetect != 0u)
            {
                DBG_OUT_rxBufferLoopDetect = 0u;
                #if ((DBG_OUT_RX_INTERRUPT_ENABLED) && (DBG_OUT_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( DBG_OUT_HD_ENABLED )
                        if((DBG_OUT_CONTROL_REG & DBG_OUT_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            DBG_OUT_RXSTATUS_MASK_REG  |= DBG_OUT_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        DBG_OUT_RXSTATUS_MASK_REG  |= DBG_OUT_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end DBG_OUT_HD_ENABLED */
                #endif /* ((DBG_OUT_RX_INTERRUPT_ENABLED) && (DBG_OUT_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = DBG_OUT_RXDATA_REG;
        }

        DBG_OUT_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = DBG_OUT_RXDATA_REG;

    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  DBG_OUT_RX_STS_FIFO_NOTEMPTY.
    *  DBG_OUT_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  DBG_OUT_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   DBG_OUT_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   DBG_OUT_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 DBG_OUT_ReadRxStatus(void) 
    {
        uint8 status;

        status = DBG_OUT_RXSTATUS_REG & DBG_OUT_RX_HW_MASK;

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)
        if(DBG_OUT_rxBufferOverflow != 0u)
        {
            status |= DBG_OUT_RX_STS_SOFT_BUFF_OVER;
            DBG_OUT_rxBufferOverflow = 0u;
        }
    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. DBG_OUT_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  DBG_OUT_rxBuffer - RAM buffer pointer for save received data.
    *  DBG_OUT_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  DBG_OUT_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  DBG_OUT_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 DBG_OUT_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        DBG_OUT_DisableRxInt();

        locRxBufferRead  = DBG_OUT_rxBufferRead;
        locRxBufferWrite = DBG_OUT_rxBufferWrite;

        if( (DBG_OUT_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = DBG_OUT_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= DBG_OUT_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            DBG_OUT_rxBufferRead = locRxBufferRead;

            if(DBG_OUT_rxBufferLoopDetect != 0u)
            {
                DBG_OUT_rxBufferLoopDetect = 0u;
                #if( (DBG_OUT_RX_INTERRUPT_ENABLED) && (DBG_OUT_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( DBG_OUT_HD_ENABLED )
                        if((DBG_OUT_CONTROL_REG & DBG_OUT_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            DBG_OUT_RXSTATUS_MASK_REG |= DBG_OUT_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        DBG_OUT_RXSTATUS_MASK_REG |= DBG_OUT_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end DBG_OUT_HD_ENABLED */
                #endif /* DBG_OUT_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = DBG_OUT_RXSTATUS_REG;
            if((rxStatus & DBG_OUT_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = DBG_OUT_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (DBG_OUT_RX_STS_BREAK | DBG_OUT_RX_STS_PAR_ERROR |
                                DBG_OUT_RX_STS_STOP_ERROR | DBG_OUT_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        DBG_OUT_EnableRxInt();

    #else

        rxStatus =DBG_OUT_RXSTATUS_REG;
        if((rxStatus & DBG_OUT_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = DBG_OUT_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (DBG_OUT_RX_STS_BREAK | DBG_OUT_RX_STS_PAR_ERROR |
                            DBG_OUT_RX_STS_STOP_ERROR | DBG_OUT_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 DBG_OUT_GetByte(void) 
    {
        
    #if (DBG_OUT_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        DBG_OUT_DisableRxInt();
        locErrorStatus = (uint16)DBG_OUT_errorStatus;
        DBG_OUT_errorStatus = 0u;
        DBG_OUT_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | DBG_OUT_ReadRxData() );
    #else
        return ( ((uint16)DBG_OUT_ReadRxStatus() << 8u) | DBG_OUT_ReadRxData() );
    #endif /* DBG_OUT_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  DBG_OUT_rxBufferWrite - used to calculate left bytes.
    *  DBG_OUT_rxBufferRead - used to calculate left bytes.
    *  DBG_OUT_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 DBG_OUT_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        DBG_OUT_DisableRxInt();

        if(DBG_OUT_rxBufferRead == DBG_OUT_rxBufferWrite)
        {
            if(DBG_OUT_rxBufferLoopDetect != 0u)
            {
                size = DBG_OUT_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(DBG_OUT_rxBufferRead < DBG_OUT_rxBufferWrite)
        {
            size = (DBG_OUT_rxBufferWrite - DBG_OUT_rxBufferRead);
        }
        else
        {
            size = (DBG_OUT_RX_BUFFER_SIZE - DBG_OUT_rxBufferRead) + DBG_OUT_rxBufferWrite;
        }

        DBG_OUT_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((DBG_OUT_RXSTATUS_REG & DBG_OUT_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_rxBufferWrite - cleared to zero.
    *  DBG_OUT_rxBufferRead - cleared to zero.
    *  DBG_OUT_rxBufferLoopDetect - cleared to zero.
    *  DBG_OUT_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void DBG_OUT_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        DBG_OUT_RXDATA_AUX_CTL_REG |= (uint8)  DBG_OUT_RX_FIFO_CLR;
        DBG_OUT_RXDATA_AUX_CTL_REG &= (uint8) ~DBG_OUT_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        DBG_OUT_DisableRxInt();

        DBG_OUT_rxBufferRead = 0u;
        DBG_OUT_rxBufferWrite = 0u;
        DBG_OUT_rxBufferLoopDetect = 0u;
        DBG_OUT_rxBufferOverflow = 0u;

        DBG_OUT_EnableRxInt();

    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: DBG_OUT_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  DBG_OUT__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  DBG_OUT__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  DBG_OUT__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  DBG_OUT__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  DBG_OUT__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  DBG_OUT_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void DBG_OUT_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(DBG_OUT_RXHW_ADDRESS_ENABLED)
            #if(DBG_OUT_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* DBG_OUT_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = DBG_OUT_CONTROL_REG & (uint8)~DBG_OUT_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << DBG_OUT_CTRL_RXADDR_MODE0_SHIFT);
                DBG_OUT_CONTROL_REG = tmpCtrl;

                #if(DBG_OUT_RX_INTERRUPT_ENABLED && \
                   (DBG_OUT_RXBUFFERSIZE > DBG_OUT_FIFO_LENGTH) )
                    DBG_OUT_rxAddressMode = addressMode;
                    DBG_OUT_rxAddressDetected = 0u;
                #endif /* End DBG_OUT_RXBUFFERSIZE > DBG_OUT_FIFO_LENGTH*/
            #endif /* End DBG_OUT_CONTROL_REG_REMOVED */
        #else /* DBG_OUT_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End DBG_OUT_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void DBG_OUT_SetRxAddress1(uint8 address) 
    {
        DBG_OUT_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void DBG_OUT_SetRxAddress2(uint8 address) 
    {
        DBG_OUT_RXADDRESS2_REG = address;
    }

#endif  /* DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED*/


#if( (DBG_OUT_TX_ENABLED) || (DBG_OUT_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: DBG_OUT_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   DBG_OUT_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   DBG_OUT_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   DBG_OUT_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   DBG_OUT_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void DBG_OUT_SetTxInterruptMode(uint8 intSrc) 
    {
        DBG_OUT_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  DBG_OUT_txBuffer - RAM buffer pointer for save data for transmission
    *  DBG_OUT_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  DBG_OUT_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  DBG_OUT_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void DBG_OUT_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(DBG_OUT_initVar != 0u)
        {
        #if (DBG_OUT_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            DBG_OUT_DisableTxInt();

            if( (DBG_OUT_txBufferRead == DBG_OUT_txBufferWrite) &&
                ((DBG_OUT_TXSTATUS_REG & DBG_OUT_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                DBG_OUT_TXDATA_REG = txDataByte;
            }
            else
            {
                if(DBG_OUT_txBufferWrite >= DBG_OUT_TX_BUFFER_SIZE)
                {
                    DBG_OUT_txBufferWrite = 0u;
                }

                DBG_OUT_txBuffer[DBG_OUT_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                DBG_OUT_txBufferWrite++;
            }

            DBG_OUT_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            DBG_OUT_TXDATA_REG = txDataByte;

        #endif /*(DBG_OUT_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 DBG_OUT_ReadTxStatus(void) 
    {
        return(DBG_OUT_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_txBuffer - RAM buffer pointer for save data for transmission
    *  DBG_OUT_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  DBG_OUT_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  DBG_OUT_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void DBG_OUT_PutChar(uint8 txDataByte) 
    {
    #if (DBG_OUT_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            DBG_OUT_DisableTxInt();
        #endif /* (DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = DBG_OUT_txBufferWrite;
            locTxBufferRead  = DBG_OUT_txBufferRead;

        #if ((DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            DBG_OUT_EnableTxInt();
        #endif /* (DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(DBG_OUT_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((DBG_OUT_TXSTATUS_REG & DBG_OUT_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            DBG_OUT_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= DBG_OUT_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            DBG_OUT_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3))
            DBG_OUT_DisableTxInt();
        #endif /* (DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3) */

            DBG_OUT_txBufferWrite = locTxBufferWrite;

        #if ((DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3))
            DBG_OUT_EnableTxInt();
        #endif /* (DBG_OUT_TX_BUFFER_SIZE > DBG_OUT_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (DBG_OUT_TXSTATUS_REG & DBG_OUT_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                DBG_OUT_SetPendingTxInt();
            }
        }

    #else

        while((DBG_OUT_TXSTATUS_REG & DBG_OUT_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        DBG_OUT_TXDATA_REG = txDataByte;

    #endif /* DBG_OUT_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void DBG_OUT_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(DBG_OUT_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                DBG_OUT_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void DBG_OUT_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(DBG_OUT_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                DBG_OUT_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void DBG_OUT_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(DBG_OUT_initVar != 0u)
        {
            DBG_OUT_PutChar(txDataByte);
            DBG_OUT_PutChar(0x0Du);
            DBG_OUT_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  DBG_OUT_txBufferWrite - used to calculate left space.
    *  DBG_OUT_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 DBG_OUT_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (DBG_OUT_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        DBG_OUT_DisableTxInt();

        if(DBG_OUT_txBufferRead == DBG_OUT_txBufferWrite)
        {
            size = 0u;
        }
        else if(DBG_OUT_txBufferRead < DBG_OUT_txBufferWrite)
        {
            size = (DBG_OUT_txBufferWrite - DBG_OUT_txBufferRead);
        }
        else
        {
            size = (DBG_OUT_TX_BUFFER_SIZE - DBG_OUT_txBufferRead) +
                    DBG_OUT_txBufferWrite;
        }

        DBG_OUT_EnableTxInt();

    #else

        size = DBG_OUT_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & DBG_OUT_TX_STS_FIFO_FULL) != 0u)
        {
            size = DBG_OUT_FIFO_LENGTH;
        }
        else if((size & DBG_OUT_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (DBG_OUT_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_txBufferWrite - cleared to zero.
    *  DBG_OUT_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void DBG_OUT_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        DBG_OUT_TXDATA_AUX_CTL_REG |= (uint8)  DBG_OUT_TX_FIFO_CLR;
        DBG_OUT_TXDATA_AUX_CTL_REG &= (uint8) ~DBG_OUT_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (DBG_OUT_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        DBG_OUT_DisableTxInt();

        DBG_OUT_txBufferRead = 0u;
        DBG_OUT_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        DBG_OUT_EnableTxInt();

    #endif /* (DBG_OUT_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   DBG_OUT_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   DBG_OUT_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   DBG_OUT_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   DBG_OUT_SEND_WAIT_REINIT - Performs both options: 
    *      DBG_OUT_SEND_BREAK and DBG_OUT_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  DBG_OUT_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with DBG_OUT_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The DBG_OUT_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void DBG_OUT_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(DBG_OUT_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(DBG_OUT_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == DBG_OUT_SEND_BREAK) ||
                (retMode == DBG_OUT_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() |
                                                      DBG_OUT_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                DBG_OUT_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = DBG_OUT_TXSTATUS_REG;
                }
                while((tmpStat & DBG_OUT_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == DBG_OUT_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == DBG_OUT_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = DBG_OUT_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & DBG_OUT_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == DBG_OUT_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == DBG_OUT_REINIT) ||
                (retMode == DBG_OUT_SEND_WAIT_REINIT) )
            {
                DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() &
                                              (uint8)~DBG_OUT_CTRL_HD_SEND_BREAK);
            }

        #else /* DBG_OUT_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == DBG_OUT_SEND_BREAK) ||
                (retMode == DBG_OUT_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (DBG_OUT_PARITY_TYPE != DBG_OUT__B_UART__NONE_REVB) || \
                                    (DBG_OUT_PARITY_TYPE_SW != 0u) )
                    DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() |
                                                          DBG_OUT_CTRL_HD_SEND_BREAK);
                #endif /* End DBG_OUT_PARITY_TYPE != DBG_OUT__B_UART__NONE_REVB  */

                #if(DBG_OUT_TXCLKGEN_DP)
                    txPeriod = DBG_OUT_TXBITCLKTX_COMPLETE_REG;
                    DBG_OUT_TXBITCLKTX_COMPLETE_REG = DBG_OUT_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = DBG_OUT_TXBITCTR_PERIOD_REG;
                    DBG_OUT_TXBITCTR_PERIOD_REG = DBG_OUT_TXBITCTR_BREAKBITS8X;
                #endif /* End DBG_OUT_TXCLKGEN_DP */

                /* Send zeros */
                DBG_OUT_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = DBG_OUT_TXSTATUS_REG;
                }
                while((tmpStat & DBG_OUT_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == DBG_OUT_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == DBG_OUT_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = DBG_OUT_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & DBG_OUT_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == DBG_OUT_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == DBG_OUT_REINIT) ||
                (retMode == DBG_OUT_SEND_WAIT_REINIT) )
            {

            #if(DBG_OUT_TXCLKGEN_DP)
                DBG_OUT_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                DBG_OUT_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End DBG_OUT_TXCLKGEN_DP */

            #if( (DBG_OUT_PARITY_TYPE != DBG_OUT__B_UART__NONE_REVB) || \
                 (DBG_OUT_PARITY_TYPE_SW != 0u) )
                DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() &
                                                      (uint8) ~DBG_OUT_CTRL_HD_SEND_BREAK);
            #endif /* End DBG_OUT_PARITY_TYPE != NONE */
            }
        #endif    /* End DBG_OUT_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       DBG_OUT_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       DBG_OUT_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears DBG_OUT_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void DBG_OUT_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( DBG_OUT_CONTROL_REG_REMOVED == 0u )
            DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() |
                                                  DBG_OUT_CTRL_MARK);
        #endif /* End DBG_OUT_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( DBG_OUT_CONTROL_REG_REMOVED == 0u )
            DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() &
                                                  (uint8) ~DBG_OUT_CTRL_MARK);
        #endif /* End DBG_OUT_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndDBG_OUT_TX_ENABLED */

#if(DBG_OUT_HD_ENABLED)


    /*******************************************************************************
    * Function Name: DBG_OUT_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void DBG_OUT_LoadRxConfig(void) 
    {
        DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() &
                                                (uint8)~DBG_OUT_CTRL_HD_SEND);
        DBG_OUT_RXBITCTR_PERIOD_REG = DBG_OUT_HD_RXBITCTR_INIT;

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        DBG_OUT_SetRxInterruptMode(DBG_OUT_INIT_RX_INTERRUPTS_MASK);
    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: DBG_OUT_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void DBG_OUT_LoadTxConfig(void) 
    {
    #if (DBG_OUT_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        DBG_OUT_SetRxInterruptMode(0u);
    #endif /* (DBG_OUT_RX_INTERRUPT_ENABLED) */

        DBG_OUT_WriteControlRegister(DBG_OUT_ReadControlRegister() | DBG_OUT_CTRL_HD_SEND);
        DBG_OUT_RXBITCTR_PERIOD_REG = DBG_OUT_HD_TXBITCTR_INIT;
    }

#endif  /* DBG_OUT_HD_ENABLED */


/* [] END OF FILE */
