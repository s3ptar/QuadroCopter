/*******************************************************************************
* File Name: DBG_OUT.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_DBG_OUT_H)
#define CY_UART_DBG_OUT_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define DBG_OUT_RX_ENABLED                     (1u)
#define DBG_OUT_TX_ENABLED                     (1u)
#define DBG_OUT_HD_ENABLED                     (0u)
#define DBG_OUT_RX_INTERRUPT_ENABLED           (0u)
#define DBG_OUT_TX_INTERRUPT_ENABLED           (0u)
#define DBG_OUT_INTERNAL_CLOCK_USED            (1u)
#define DBG_OUT_RXHW_ADDRESS_ENABLED           (0u)
#define DBG_OUT_OVER_SAMPLE_COUNT              (8u)
#define DBG_OUT_PARITY_TYPE                    (0u)
#define DBG_OUT_PARITY_TYPE_SW                 (0u)
#define DBG_OUT_BREAK_DETECT                   (0u)
#define DBG_OUT_BREAK_BITS_TX                  (13u)
#define DBG_OUT_BREAK_BITS_RX                  (13u)
#define DBG_OUT_TXCLKGEN_DP                    (1u)
#define DBG_OUT_USE23POLLING                   (1u)
#define DBG_OUT_FLOW_CONTROL                   (0u)
#define DBG_OUT_CLK_FREQ                       (0u)
#define DBG_OUT_TX_BUFFER_SIZE                 (4u)
#define DBG_OUT_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define DBG_OUT_CONTROL_REG_REMOVED            (0u)
#else
    #define DBG_OUT_CONTROL_REG_REMOVED            (1u)
#endif /* End DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct DBG_OUT_backupStruct_
{
    uint8 enableState;

    #if(DBG_OUT_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End DBG_OUT_CONTROL_REG_REMOVED */

} DBG_OUT_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void DBG_OUT_Start(void) ;
void DBG_OUT_Stop(void) ;
uint8 DBG_OUT_ReadControlRegister(void) ;
void DBG_OUT_WriteControlRegister(uint8 control) ;

void DBG_OUT_Init(void) ;
void DBG_OUT_Enable(void) ;
void DBG_OUT_SaveConfig(void) ;
void DBG_OUT_RestoreConfig(void) ;
void DBG_OUT_Sleep(void) ;
void DBG_OUT_Wakeup(void) ;

/* Only if RX is enabled */
#if( (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) )

    #if (DBG_OUT_RX_INTERRUPT_ENABLED)
        #define DBG_OUT_EnableRxInt()  CyIntEnable (DBG_OUT_RX_VECT_NUM)
        #define DBG_OUT_DisableRxInt() CyIntDisable(DBG_OUT_RX_VECT_NUM)
        CY_ISR_PROTO(DBG_OUT_RXISR);
    #endif /* DBG_OUT_RX_INTERRUPT_ENABLED */

    void DBG_OUT_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void DBG_OUT_SetRxAddress1(uint8 address) ;
    void DBG_OUT_SetRxAddress2(uint8 address) ;

    void  DBG_OUT_SetRxInterruptMode(uint8 intSrc) ;
    uint8 DBG_OUT_ReadRxData(void) ;
    uint8 DBG_OUT_ReadRxStatus(void) ;
    uint8 DBG_OUT_GetChar(void) ;
    uint16 DBG_OUT_GetByte(void) ;
    uint8 DBG_OUT_GetRxBufferSize(void)
                                                            ;
    void DBG_OUT_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define DBG_OUT_GetRxInterruptSource   DBG_OUT_ReadRxStatus

#endif /* End (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) */

/* Only if TX is enabled */
#if(DBG_OUT_TX_ENABLED || DBG_OUT_HD_ENABLED)

    #if(DBG_OUT_TX_INTERRUPT_ENABLED)
        #define DBG_OUT_EnableTxInt()  CyIntEnable (DBG_OUT_TX_VECT_NUM)
        #define DBG_OUT_DisableTxInt() CyIntDisable(DBG_OUT_TX_VECT_NUM)
        #define DBG_OUT_SetPendingTxInt() CyIntSetPending(DBG_OUT_TX_VECT_NUM)
        #define DBG_OUT_ClearPendingTxInt() CyIntClearPending(DBG_OUT_TX_VECT_NUM)
        CY_ISR_PROTO(DBG_OUT_TXISR);
    #endif /* DBG_OUT_TX_INTERRUPT_ENABLED */

    void DBG_OUT_SetTxInterruptMode(uint8 intSrc) ;
    void DBG_OUT_WriteTxData(uint8 txDataByte) ;
    uint8 DBG_OUT_ReadTxStatus(void) ;
    void DBG_OUT_PutChar(uint8 txDataByte) ;
    void DBG_OUT_PutString(const char8 string[]) ;
    void DBG_OUT_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void DBG_OUT_PutCRLF(uint8 txDataByte) ;
    void DBG_OUT_ClearTxBuffer(void) ;
    void DBG_OUT_SetTxAddressMode(uint8 addressMode) ;
    void DBG_OUT_SendBreak(uint8 retMode) ;
    uint8 DBG_OUT_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define DBG_OUT_PutStringConst         DBG_OUT_PutString
    #define DBG_OUT_PutArrayConst          DBG_OUT_PutArray
    #define DBG_OUT_GetTxInterruptSource   DBG_OUT_ReadTxStatus

#endif /* End DBG_OUT_TX_ENABLED || DBG_OUT_HD_ENABLED */

#if(DBG_OUT_HD_ENABLED)
    void DBG_OUT_LoadRxConfig(void) ;
    void DBG_OUT_LoadTxConfig(void) ;
#endif /* End DBG_OUT_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_DBG_OUT) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    DBG_OUT_CyBtldrCommStart(void) CYSMALL ;
    void    DBG_OUT_CyBtldrCommStop(void) CYSMALL ;
    void    DBG_OUT_CyBtldrCommReset(void) CYSMALL ;
    cystatus DBG_OUT_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus DBG_OUT_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_DBG_OUT)
        #define CyBtldrCommStart    DBG_OUT_CyBtldrCommStart
        #define CyBtldrCommStop     DBG_OUT_CyBtldrCommStop
        #define CyBtldrCommReset    DBG_OUT_CyBtldrCommReset
        #define CyBtldrCommWrite    DBG_OUT_CyBtldrCommWrite
        #define CyBtldrCommRead     DBG_OUT_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_DBG_OUT) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define DBG_OUT_BYTE2BYTE_TIME_OUT (25u)
    #define DBG_OUT_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define DBG_OUT_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define DBG_OUT_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define DBG_OUT_SET_SPACE      (0x00u)
#define DBG_OUT_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (DBG_OUT_TX_ENABLED) || (DBG_OUT_HD_ENABLED) )
    #if(DBG_OUT_TX_INTERRUPT_ENABLED)
        #define DBG_OUT_TX_VECT_NUM            (uint8)DBG_OUT_TXInternalInterrupt__INTC_NUMBER
        #define DBG_OUT_TX_PRIOR_NUM           (uint8)DBG_OUT_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* DBG_OUT_TX_INTERRUPT_ENABLED */

    #define DBG_OUT_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define DBG_OUT_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define DBG_OUT_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(DBG_OUT_TX_ENABLED)
        #define DBG_OUT_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (DBG_OUT_HD_ENABLED) */
        #define DBG_OUT_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (DBG_OUT_TX_ENABLED) */

    #define DBG_OUT_TX_STS_COMPLETE            (uint8)(0x01u << DBG_OUT_TX_STS_COMPLETE_SHIFT)
    #define DBG_OUT_TX_STS_FIFO_EMPTY          (uint8)(0x01u << DBG_OUT_TX_STS_FIFO_EMPTY_SHIFT)
    #define DBG_OUT_TX_STS_FIFO_FULL           (uint8)(0x01u << DBG_OUT_TX_STS_FIFO_FULL_SHIFT)
    #define DBG_OUT_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << DBG_OUT_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (DBG_OUT_TX_ENABLED) || (DBG_OUT_HD_ENABLED)*/

#if( (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) )
    #if(DBG_OUT_RX_INTERRUPT_ENABLED)
        #define DBG_OUT_RX_VECT_NUM            (uint8)DBG_OUT_RXInternalInterrupt__INTC_NUMBER
        #define DBG_OUT_RX_PRIOR_NUM           (uint8)DBG_OUT_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* DBG_OUT_RX_INTERRUPT_ENABLED */
    #define DBG_OUT_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define DBG_OUT_RX_STS_BREAK_SHIFT             (0x01u)
    #define DBG_OUT_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define DBG_OUT_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define DBG_OUT_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define DBG_OUT_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define DBG_OUT_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define DBG_OUT_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define DBG_OUT_RX_STS_MRKSPC           (uint8)(0x01u << DBG_OUT_RX_STS_MRKSPC_SHIFT)
    #define DBG_OUT_RX_STS_BREAK            (uint8)(0x01u << DBG_OUT_RX_STS_BREAK_SHIFT)
    #define DBG_OUT_RX_STS_PAR_ERROR        (uint8)(0x01u << DBG_OUT_RX_STS_PAR_ERROR_SHIFT)
    #define DBG_OUT_RX_STS_STOP_ERROR       (uint8)(0x01u << DBG_OUT_RX_STS_STOP_ERROR_SHIFT)
    #define DBG_OUT_RX_STS_OVERRUN          (uint8)(0x01u << DBG_OUT_RX_STS_OVERRUN_SHIFT)
    #define DBG_OUT_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << DBG_OUT_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define DBG_OUT_RX_STS_ADDR_MATCH       (uint8)(0x01u << DBG_OUT_RX_STS_ADDR_MATCH_SHIFT)
    #define DBG_OUT_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << DBG_OUT_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define DBG_OUT_RX_HW_MASK                     (0x7Fu)
#endif /* End (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) */

/* Control Register definitions */
#define DBG_OUT_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define DBG_OUT_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define DBG_OUT_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define DBG_OUT_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define DBG_OUT_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define DBG_OUT_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define DBG_OUT_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define DBG_OUT_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define DBG_OUT_CTRL_HD_SEND               (uint8)(0x01u << DBG_OUT_CTRL_HD_SEND_SHIFT)
#define DBG_OUT_CTRL_HD_SEND_BREAK         (uint8)(0x01u << DBG_OUT_CTRL_HD_SEND_BREAK_SHIFT)
#define DBG_OUT_CTRL_MARK                  (uint8)(0x01u << DBG_OUT_CTRL_MARK_SHIFT)
#define DBG_OUT_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << DBG_OUT_CTRL_PARITY_TYPE0_SHIFT)
#define DBG_OUT_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << DBG_OUT_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define DBG_OUT_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define DBG_OUT_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define DBG_OUT_SEND_BREAK                         (0x00u)
#define DBG_OUT_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define DBG_OUT_REINIT                             (0x02u)
#define DBG_OUT_SEND_WAIT_REINIT                   (0x03u)

#define DBG_OUT_OVER_SAMPLE_8                      (8u)
#define DBG_OUT_OVER_SAMPLE_16                     (16u)

#define DBG_OUT_BIT_CENTER                         (DBG_OUT_OVER_SAMPLE_COUNT - 2u)

#define DBG_OUT_FIFO_LENGTH                        (4u)
#define DBG_OUT_NUMBER_OF_START_BIT                (1u)
#define DBG_OUT_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define DBG_OUT_TXBITCTR_BREAKBITS8X   ((DBG_OUT_BREAK_BITS_TX * DBG_OUT_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define DBG_OUT_TXBITCTR_BREAKBITS ((DBG_OUT_BREAK_BITS_TX * DBG_OUT_OVER_SAMPLE_COUNT) - 1u)

#define DBG_OUT_HALF_BIT_COUNT   \
                            (((DBG_OUT_OVER_SAMPLE_COUNT / 2u) + (DBG_OUT_USE23POLLING * 1u)) - 2u)
#if (DBG_OUT_OVER_SAMPLE_COUNT == DBG_OUT_OVER_SAMPLE_8)
    #define DBG_OUT_HD_TXBITCTR_INIT   (((DBG_OUT_BREAK_BITS_TX + \
                            DBG_OUT_NUMBER_OF_START_BIT) * DBG_OUT_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define DBG_OUT_RXBITCTR_INIT  ((((DBG_OUT_BREAK_BITS_RX + DBG_OUT_NUMBER_OF_START_BIT) \
                            * DBG_OUT_OVER_SAMPLE_COUNT) + DBG_OUT_HALF_BIT_COUNT) - 1u)

#else /* DBG_OUT_OVER_SAMPLE_COUNT == DBG_OUT_OVER_SAMPLE_16 */
    #define DBG_OUT_HD_TXBITCTR_INIT   ((8u * DBG_OUT_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define DBG_OUT_RXBITCTR_INIT      (((7u * DBG_OUT_OVER_SAMPLE_COUNT) - 1u) + \
                                                      DBG_OUT_HALF_BIT_COUNT)
#endif /* End DBG_OUT_OVER_SAMPLE_COUNT */

#define DBG_OUT_HD_RXBITCTR_INIT                   DBG_OUT_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 DBG_OUT_initVar;
#if (DBG_OUT_TX_INTERRUPT_ENABLED && DBG_OUT_TX_ENABLED)
    extern volatile uint8 DBG_OUT_txBuffer[DBG_OUT_TX_BUFFER_SIZE];
    extern volatile uint8 DBG_OUT_txBufferRead;
    extern uint8 DBG_OUT_txBufferWrite;
#endif /* (DBG_OUT_TX_INTERRUPT_ENABLED && DBG_OUT_TX_ENABLED) */
#if (DBG_OUT_RX_INTERRUPT_ENABLED && (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED))
    extern uint8 DBG_OUT_errorStatus;
    extern volatile uint8 DBG_OUT_rxBuffer[DBG_OUT_RX_BUFFER_SIZE];
    extern volatile uint8 DBG_OUT_rxBufferRead;
    extern volatile uint8 DBG_OUT_rxBufferWrite;
    extern volatile uint8 DBG_OUT_rxBufferLoopDetect;
    extern volatile uint8 DBG_OUT_rxBufferOverflow;
    #if (DBG_OUT_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 DBG_OUT_rxAddressMode;
        extern volatile uint8 DBG_OUT_rxAddressDetected;
    #endif /* (DBG_OUT_RXHW_ADDRESS_ENABLED) */
#endif /* (DBG_OUT_RX_INTERRUPT_ENABLED && (DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define DBG_OUT__B_UART__AM_SW_BYTE_BYTE 1
#define DBG_OUT__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define DBG_OUT__B_UART__AM_HW_BYTE_BY_BYTE 3
#define DBG_OUT__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define DBG_OUT__B_UART__AM_NONE 0

#define DBG_OUT__B_UART__NONE_REVB 0
#define DBG_OUT__B_UART__EVEN_REVB 1
#define DBG_OUT__B_UART__ODD_REVB 2
#define DBG_OUT__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define DBG_OUT_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define DBG_OUT_NUMBER_OF_STOP_BITS    (1u)

#if (DBG_OUT_RXHW_ADDRESS_ENABLED)
    #define DBG_OUT_RX_ADDRESS_MODE    (0u)
    #define DBG_OUT_RX_HW_ADDRESS1     (0u)
    #define DBG_OUT_RX_HW_ADDRESS2     (0u)
#endif /* (DBG_OUT_RXHW_ADDRESS_ENABLED) */

#define DBG_OUT_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << DBG_OUT_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << DBG_OUT_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << DBG_OUT_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << DBG_OUT_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << DBG_OUT_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << DBG_OUT_RX_STS_BREAK_SHIFT) \
                                        | (0 << DBG_OUT_RX_STS_OVERRUN_SHIFT))

#define DBG_OUT_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << DBG_OUT_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << DBG_OUT_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << DBG_OUT_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << DBG_OUT_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define DBG_OUT_CONTROL_REG \
                            (* (reg8 *) DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define DBG_OUT_CONTROL_PTR \
                            (  (reg8 *) DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(DBG_OUT_TX_ENABLED)
    #define DBG_OUT_TXDATA_REG          (* (reg8 *) DBG_OUT_BUART_sTX_TxShifter_u0__F0_REG)
    #define DBG_OUT_TXDATA_PTR          (  (reg8 *) DBG_OUT_BUART_sTX_TxShifter_u0__F0_REG)
    #define DBG_OUT_TXDATA_AUX_CTL_REG  (* (reg8 *) DBG_OUT_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define DBG_OUT_TXDATA_AUX_CTL_PTR  (  (reg8 *) DBG_OUT_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define DBG_OUT_TXSTATUS_REG        (* (reg8 *) DBG_OUT_BUART_sTX_TxSts__STATUS_REG)
    #define DBG_OUT_TXSTATUS_PTR        (  (reg8 *) DBG_OUT_BUART_sTX_TxSts__STATUS_REG)
    #define DBG_OUT_TXSTATUS_MASK_REG   (* (reg8 *) DBG_OUT_BUART_sTX_TxSts__MASK_REG)
    #define DBG_OUT_TXSTATUS_MASK_PTR   (  (reg8 *) DBG_OUT_BUART_sTX_TxSts__MASK_REG)
    #define DBG_OUT_TXSTATUS_ACTL_REG   (* (reg8 *) DBG_OUT_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define DBG_OUT_TXSTATUS_ACTL_PTR   (  (reg8 *) DBG_OUT_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(DBG_OUT_TXCLKGEN_DP)
        #define DBG_OUT_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define DBG_OUT_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define DBG_OUT_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define DBG_OUT_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define DBG_OUT_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define DBG_OUT_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define DBG_OUT_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define DBG_OUT_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define DBG_OUT_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define DBG_OUT_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) DBG_OUT_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* DBG_OUT_TXCLKGEN_DP */

#endif /* End DBG_OUT_TX_ENABLED */

#if(DBG_OUT_HD_ENABLED)

    #define DBG_OUT_TXDATA_REG             (* (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__F1_REG )
    #define DBG_OUT_TXDATA_PTR             (  (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__F1_REG )
    #define DBG_OUT_TXDATA_AUX_CTL_REG     (* (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define DBG_OUT_TXDATA_AUX_CTL_PTR     (  (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define DBG_OUT_TXSTATUS_REG           (* (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_REG )
    #define DBG_OUT_TXSTATUS_PTR           (  (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_REG )
    #define DBG_OUT_TXSTATUS_MASK_REG      (* (reg8 *) DBG_OUT_BUART_sRX_RxSts__MASK_REG )
    #define DBG_OUT_TXSTATUS_MASK_PTR      (  (reg8 *) DBG_OUT_BUART_sRX_RxSts__MASK_REG )
    #define DBG_OUT_TXSTATUS_ACTL_REG      (* (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define DBG_OUT_TXSTATUS_ACTL_PTR      (  (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End DBG_OUT_HD_ENABLED */

#if( (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) )
    #define DBG_OUT_RXDATA_REG             (* (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__F0_REG )
    #define DBG_OUT_RXDATA_PTR             (  (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__F0_REG )
    #define DBG_OUT_RXADDRESS1_REG         (* (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__D0_REG )
    #define DBG_OUT_RXADDRESS1_PTR         (  (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__D0_REG )
    #define DBG_OUT_RXADDRESS2_REG         (* (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__D1_REG )
    #define DBG_OUT_RXADDRESS2_PTR         (  (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__D1_REG )
    #define DBG_OUT_RXDATA_AUX_CTL_REG     (* (reg8 *) DBG_OUT_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define DBG_OUT_RXBITCTR_PERIOD_REG    (* (reg8 *) DBG_OUT_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define DBG_OUT_RXBITCTR_PERIOD_PTR    (  (reg8 *) DBG_OUT_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define DBG_OUT_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) DBG_OUT_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define DBG_OUT_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) DBG_OUT_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define DBG_OUT_RXBITCTR_COUNTER_REG   (* (reg8 *) DBG_OUT_BUART_sRX_RxBitCounter__COUNT_REG )
    #define DBG_OUT_RXBITCTR_COUNTER_PTR   (  (reg8 *) DBG_OUT_BUART_sRX_RxBitCounter__COUNT_REG )

    #define DBG_OUT_RXSTATUS_REG           (* (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_REG )
    #define DBG_OUT_RXSTATUS_PTR           (  (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_REG )
    #define DBG_OUT_RXSTATUS_MASK_REG      (* (reg8 *) DBG_OUT_BUART_sRX_RxSts__MASK_REG )
    #define DBG_OUT_RXSTATUS_MASK_PTR      (  (reg8 *) DBG_OUT_BUART_sRX_RxSts__MASK_REG )
    #define DBG_OUT_RXSTATUS_ACTL_REG      (* (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define DBG_OUT_RXSTATUS_ACTL_PTR      (  (reg8 *) DBG_OUT_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) */

#if(DBG_OUT_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define DBG_OUT_INTCLOCK_CLKEN_REG     (* (reg8 *) DBG_OUT_IntClock__PM_ACT_CFG)
    #define DBG_OUT_INTCLOCK_CLKEN_PTR     (  (reg8 *) DBG_OUT_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define DBG_OUT_INTCLOCK_CLKEN_MASK    DBG_OUT_IntClock__PM_ACT_MSK
#endif /* End DBG_OUT_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(DBG_OUT_TX_ENABLED)
    #define DBG_OUT_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End DBG_OUT_TX_ENABLED */

#if(DBG_OUT_HD_ENABLED)
    #define DBG_OUT_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End DBG_OUT_HD_ENABLED */

#if( (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) )
    #define DBG_OUT_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define DBG_OUT_WAIT_1_MS      DBG_OUT_BL_CHK_DELAY_MS   

#define DBG_OUT_TXBUFFERSIZE   DBG_OUT_TX_BUFFER_SIZE
#define DBG_OUT_RXBUFFERSIZE   DBG_OUT_RX_BUFFER_SIZE

#if (DBG_OUT_RXHW_ADDRESS_ENABLED)
    #define DBG_OUT_RXADDRESSMODE  DBG_OUT_RX_ADDRESS_MODE
    #define DBG_OUT_RXHWADDRESS1   DBG_OUT_RX_HW_ADDRESS1
    #define DBG_OUT_RXHWADDRESS2   DBG_OUT_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define DBG_OUT_RXAddressMode  DBG_OUT_RXADDRESSMODE
#endif /* (DBG_OUT_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define DBG_OUT_initvar                    DBG_OUT_initVar

#define DBG_OUT_RX_Enabled                 DBG_OUT_RX_ENABLED
#define DBG_OUT_TX_Enabled                 DBG_OUT_TX_ENABLED
#define DBG_OUT_HD_Enabled                 DBG_OUT_HD_ENABLED
#define DBG_OUT_RX_IntInterruptEnabled     DBG_OUT_RX_INTERRUPT_ENABLED
#define DBG_OUT_TX_IntInterruptEnabled     DBG_OUT_TX_INTERRUPT_ENABLED
#define DBG_OUT_InternalClockUsed          DBG_OUT_INTERNAL_CLOCK_USED
#define DBG_OUT_RXHW_Address_Enabled       DBG_OUT_RXHW_ADDRESS_ENABLED
#define DBG_OUT_OverSampleCount            DBG_OUT_OVER_SAMPLE_COUNT
#define DBG_OUT_ParityType                 DBG_OUT_PARITY_TYPE

#if( DBG_OUT_TX_ENABLED && (DBG_OUT_TXBUFFERSIZE > DBG_OUT_FIFO_LENGTH))
    #define DBG_OUT_TXBUFFER               DBG_OUT_txBuffer
    #define DBG_OUT_TXBUFFERREAD           DBG_OUT_txBufferRead
    #define DBG_OUT_TXBUFFERWRITE          DBG_OUT_txBufferWrite
#endif /* End DBG_OUT_TX_ENABLED */
#if( ( DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED ) && \
     (DBG_OUT_RXBUFFERSIZE > DBG_OUT_FIFO_LENGTH) )
    #define DBG_OUT_RXBUFFER               DBG_OUT_rxBuffer
    #define DBG_OUT_RXBUFFERREAD           DBG_OUT_rxBufferRead
    #define DBG_OUT_RXBUFFERWRITE          DBG_OUT_rxBufferWrite
    #define DBG_OUT_RXBUFFERLOOPDETECT     DBG_OUT_rxBufferLoopDetect
    #define DBG_OUT_RXBUFFER_OVERFLOW      DBG_OUT_rxBufferOverflow
#endif /* End DBG_OUT_RX_ENABLED */

#ifdef DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define DBG_OUT_CONTROL                DBG_OUT_CONTROL_REG
#endif /* End DBG_OUT_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(DBG_OUT_TX_ENABLED)
    #define DBG_OUT_TXDATA                 DBG_OUT_TXDATA_REG
    #define DBG_OUT_TXSTATUS               DBG_OUT_TXSTATUS_REG
    #define DBG_OUT_TXSTATUS_MASK          DBG_OUT_TXSTATUS_MASK_REG
    #define DBG_OUT_TXSTATUS_ACTL          DBG_OUT_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(DBG_OUT_TXCLKGEN_DP)
        #define DBG_OUT_TXBITCLKGEN_CTR        DBG_OUT_TXBITCLKGEN_CTR_REG
        #define DBG_OUT_TXBITCLKTX_COMPLETE    DBG_OUT_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define DBG_OUT_TXBITCTR_PERIOD        DBG_OUT_TXBITCTR_PERIOD_REG
        #define DBG_OUT_TXBITCTR_CONTROL       DBG_OUT_TXBITCTR_CONTROL_REG
        #define DBG_OUT_TXBITCTR_COUNTER       DBG_OUT_TXBITCTR_COUNTER_REG
    #endif /* DBG_OUT_TXCLKGEN_DP */
#endif /* End DBG_OUT_TX_ENABLED */

#if(DBG_OUT_HD_ENABLED)
    #define DBG_OUT_TXDATA                 DBG_OUT_TXDATA_REG
    #define DBG_OUT_TXSTATUS               DBG_OUT_TXSTATUS_REG
    #define DBG_OUT_TXSTATUS_MASK          DBG_OUT_TXSTATUS_MASK_REG
    #define DBG_OUT_TXSTATUS_ACTL          DBG_OUT_TXSTATUS_ACTL_REG
#endif /* End DBG_OUT_HD_ENABLED */

#if( (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) )
    #define DBG_OUT_RXDATA                 DBG_OUT_RXDATA_REG
    #define DBG_OUT_RXADDRESS1             DBG_OUT_RXADDRESS1_REG
    #define DBG_OUT_RXADDRESS2             DBG_OUT_RXADDRESS2_REG
    #define DBG_OUT_RXBITCTR_PERIOD        DBG_OUT_RXBITCTR_PERIOD_REG
    #define DBG_OUT_RXBITCTR_CONTROL       DBG_OUT_RXBITCTR_CONTROL_REG
    #define DBG_OUT_RXBITCTR_COUNTER       DBG_OUT_RXBITCTR_COUNTER_REG
    #define DBG_OUT_RXSTATUS               DBG_OUT_RXSTATUS_REG
    #define DBG_OUT_RXSTATUS_MASK          DBG_OUT_RXSTATUS_MASK_REG
    #define DBG_OUT_RXSTATUS_ACTL          DBG_OUT_RXSTATUS_ACTL_REG
#endif /* End  (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) */

#if(DBG_OUT_INTERNAL_CLOCK_USED)
    #define DBG_OUT_INTCLOCK_CLKEN         DBG_OUT_INTCLOCK_CLKEN_REG
#endif /* End DBG_OUT_INTERNAL_CLOCK_USED */

#define DBG_OUT_WAIT_FOR_COMLETE_REINIT    DBG_OUT_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_DBG_OUT_H */


/* [] END OF FILE */
