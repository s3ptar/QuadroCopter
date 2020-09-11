/*******************************************************************************
* File Name: KHz.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_KHz_H)
#define CY_CLOCK_KHz_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void KHz_Start(void) ;
void KHz_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void KHz_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void KHz_StandbyPower(uint8 state) ;
void KHz_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 KHz_GetDividerRegister(void) ;
void KHz_SetModeRegister(uint8 modeBitMask) ;
void KHz_ClearModeRegister(uint8 modeBitMask) ;
uint8 KHz_GetModeRegister(void) ;
void KHz_SetSourceRegister(uint8 clkSource) ;
uint8 KHz_GetSourceRegister(void) ;
#if defined(KHz__CFG3)
void KHz_SetPhaseRegister(uint8 clkPhase) ;
uint8 KHz_GetPhaseRegister(void) ;
#endif /* defined(KHz__CFG3) */

#define KHz_Enable()                       KHz_Start()
#define KHz_Disable()                      KHz_Stop()
#define KHz_SetDivider(clkDivider)         KHz_SetDividerRegister(clkDivider, 1u)
#define KHz_SetDividerValue(clkDivider)    KHz_SetDividerRegister((clkDivider) - 1u, 1u)
#define KHz_SetMode(clkMode)               KHz_SetModeRegister(clkMode)
#define KHz_SetSource(clkSource)           KHz_SetSourceRegister(clkSource)
#if defined(KHz__CFG3)
#define KHz_SetPhase(clkPhase)             KHz_SetPhaseRegister(clkPhase)
#define KHz_SetPhaseValue(clkPhase)        KHz_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(KHz__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define KHz_CLKEN              (* (reg8 *) KHz__PM_ACT_CFG)
#define KHz_CLKEN_PTR          ((reg8 *) KHz__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define KHz_CLKSTBY            (* (reg8 *) KHz__PM_STBY_CFG)
#define KHz_CLKSTBY_PTR        ((reg8 *) KHz__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define KHz_DIV_LSB            (* (reg8 *) KHz__CFG0)
#define KHz_DIV_LSB_PTR        ((reg8 *) KHz__CFG0)
#define KHz_DIV_PTR            ((reg16 *) KHz__CFG0)

/* Clock MSB divider configuration register. */
#define KHz_DIV_MSB            (* (reg8 *) KHz__CFG1)
#define KHz_DIV_MSB_PTR        ((reg8 *) KHz__CFG1)

/* Mode and source configuration register */
#define KHz_MOD_SRC            (* (reg8 *) KHz__CFG2)
#define KHz_MOD_SRC_PTR        ((reg8 *) KHz__CFG2)

#if defined(KHz__CFG3)
/* Analog clock phase configuration register */
#define KHz_PHASE              (* (reg8 *) KHz__CFG3)
#define KHz_PHASE_PTR          ((reg8 *) KHz__CFG3)
#endif /* defined(KHz__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define KHz_CLKEN_MASK         KHz__PM_ACT_MSK
#define KHz_CLKSTBY_MASK       KHz__PM_STBY_MSK

/* CFG2 field masks */
#define KHz_SRC_SEL_MSK        KHz__CFG2_SRC_SEL_MASK
#define KHz_MODE_MASK          (~(KHz_SRC_SEL_MSK))

#if defined(KHz__CFG3)
/* CFG3 phase mask */
#define KHz_PHASE_MASK         KHz__CFG3_PHASE_DLY_MASK
#endif /* defined(KHz__CFG3) */

#endif /* CY_CLOCK_KHz_H */


/* [] END OF FILE */
