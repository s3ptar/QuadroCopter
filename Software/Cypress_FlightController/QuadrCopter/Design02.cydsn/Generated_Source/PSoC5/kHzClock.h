/*******************************************************************************
* File Name: kHzClock.h
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

#if !defined(CY_CLOCK_kHzClock_H)
#define CY_CLOCK_kHzClock_H

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

void kHzClock_Start(void) ;
void kHzClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void kHzClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void kHzClock_StandbyPower(uint8 state) ;
void kHzClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 kHzClock_GetDividerRegister(void) ;
void kHzClock_SetModeRegister(uint8 modeBitMask) ;
void kHzClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 kHzClock_GetModeRegister(void) ;
void kHzClock_SetSourceRegister(uint8 clkSource) ;
uint8 kHzClock_GetSourceRegister(void) ;
#if defined(kHzClock__CFG3)
void kHzClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 kHzClock_GetPhaseRegister(void) ;
#endif /* defined(kHzClock__CFG3) */

#define kHzClock_Enable()                       kHzClock_Start()
#define kHzClock_Disable()                      kHzClock_Stop()
#define kHzClock_SetDivider(clkDivider)         kHzClock_SetDividerRegister(clkDivider, 1u)
#define kHzClock_SetDividerValue(clkDivider)    kHzClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define kHzClock_SetMode(clkMode)               kHzClock_SetModeRegister(clkMode)
#define kHzClock_SetSource(clkSource)           kHzClock_SetSourceRegister(clkSource)
#if defined(kHzClock__CFG3)
#define kHzClock_SetPhase(clkPhase)             kHzClock_SetPhaseRegister(clkPhase)
#define kHzClock_SetPhaseValue(clkPhase)        kHzClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(kHzClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define kHzClock_CLKEN              (* (reg8 *) kHzClock__PM_ACT_CFG)
#define kHzClock_CLKEN_PTR          ((reg8 *) kHzClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define kHzClock_CLKSTBY            (* (reg8 *) kHzClock__PM_STBY_CFG)
#define kHzClock_CLKSTBY_PTR        ((reg8 *) kHzClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define kHzClock_DIV_LSB            (* (reg8 *) kHzClock__CFG0)
#define kHzClock_DIV_LSB_PTR        ((reg8 *) kHzClock__CFG0)
#define kHzClock_DIV_PTR            ((reg16 *) kHzClock__CFG0)

/* Clock MSB divider configuration register. */
#define kHzClock_DIV_MSB            (* (reg8 *) kHzClock__CFG1)
#define kHzClock_DIV_MSB_PTR        ((reg8 *) kHzClock__CFG1)

/* Mode and source configuration register */
#define kHzClock_MOD_SRC            (* (reg8 *) kHzClock__CFG2)
#define kHzClock_MOD_SRC_PTR        ((reg8 *) kHzClock__CFG2)

#if defined(kHzClock__CFG3)
/* Analog clock phase configuration register */
#define kHzClock_PHASE              (* (reg8 *) kHzClock__CFG3)
#define kHzClock_PHASE_PTR          ((reg8 *) kHzClock__CFG3)
#endif /* defined(kHzClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define kHzClock_CLKEN_MASK         kHzClock__PM_ACT_MSK
#define kHzClock_CLKSTBY_MASK       kHzClock__PM_STBY_MSK

/* CFG2 field masks */
#define kHzClock_SRC_SEL_MSK        kHzClock__CFG2_SRC_SEL_MASK
#define kHzClock_MODE_MASK          (~(kHzClock_SRC_SEL_MSK))

#if defined(kHzClock__CFG3)
/* CFG3 phase mask */
#define kHzClock_PHASE_MASK         kHzClock__CFG3_PHASE_DLY_MASK
#endif /* defined(kHzClock__CFG3) */

#endif /* CY_CLOCK_kHzClock_H */


/* [] END OF FILE */
