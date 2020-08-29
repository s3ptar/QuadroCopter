/*******************************************************************************
* File Name: DMA_Timer.h
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

#if !defined(CY_CLOCK_DMA_Timer_H)
#define CY_CLOCK_DMA_Timer_H

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

void DMA_Timer_Start(void) ;
void DMA_Timer_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void DMA_Timer_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void DMA_Timer_StandbyPower(uint8 state) ;
void DMA_Timer_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 DMA_Timer_GetDividerRegister(void) ;
void DMA_Timer_SetModeRegister(uint8 modeBitMask) ;
void DMA_Timer_ClearModeRegister(uint8 modeBitMask) ;
uint8 DMA_Timer_GetModeRegister(void) ;
void DMA_Timer_SetSourceRegister(uint8 clkSource) ;
uint8 DMA_Timer_GetSourceRegister(void) ;
#if defined(DMA_Timer__CFG3)
void DMA_Timer_SetPhaseRegister(uint8 clkPhase) ;
uint8 DMA_Timer_GetPhaseRegister(void) ;
#endif /* defined(DMA_Timer__CFG3) */

#define DMA_Timer_Enable()                       DMA_Timer_Start()
#define DMA_Timer_Disable()                      DMA_Timer_Stop()
#define DMA_Timer_SetDivider(clkDivider)         DMA_Timer_SetDividerRegister(clkDivider, 1u)
#define DMA_Timer_SetDividerValue(clkDivider)    DMA_Timer_SetDividerRegister((clkDivider) - 1u, 1u)
#define DMA_Timer_SetMode(clkMode)               DMA_Timer_SetModeRegister(clkMode)
#define DMA_Timer_SetSource(clkSource)           DMA_Timer_SetSourceRegister(clkSource)
#if defined(DMA_Timer__CFG3)
#define DMA_Timer_SetPhase(clkPhase)             DMA_Timer_SetPhaseRegister(clkPhase)
#define DMA_Timer_SetPhaseValue(clkPhase)        DMA_Timer_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(DMA_Timer__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define DMA_Timer_CLKEN              (* (reg8 *) DMA_Timer__PM_ACT_CFG)
#define DMA_Timer_CLKEN_PTR          ((reg8 *) DMA_Timer__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define DMA_Timer_CLKSTBY            (* (reg8 *) DMA_Timer__PM_STBY_CFG)
#define DMA_Timer_CLKSTBY_PTR        ((reg8 *) DMA_Timer__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define DMA_Timer_DIV_LSB            (* (reg8 *) DMA_Timer__CFG0)
#define DMA_Timer_DIV_LSB_PTR        ((reg8 *) DMA_Timer__CFG0)
#define DMA_Timer_DIV_PTR            ((reg16 *) DMA_Timer__CFG0)

/* Clock MSB divider configuration register. */
#define DMA_Timer_DIV_MSB            (* (reg8 *) DMA_Timer__CFG1)
#define DMA_Timer_DIV_MSB_PTR        ((reg8 *) DMA_Timer__CFG1)

/* Mode and source configuration register */
#define DMA_Timer_MOD_SRC            (* (reg8 *) DMA_Timer__CFG2)
#define DMA_Timer_MOD_SRC_PTR        ((reg8 *) DMA_Timer__CFG2)

#if defined(DMA_Timer__CFG3)
/* Analog clock phase configuration register */
#define DMA_Timer_PHASE              (* (reg8 *) DMA_Timer__CFG3)
#define DMA_Timer_PHASE_PTR          ((reg8 *) DMA_Timer__CFG3)
#endif /* defined(DMA_Timer__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define DMA_Timer_CLKEN_MASK         DMA_Timer__PM_ACT_MSK
#define DMA_Timer_CLKSTBY_MASK       DMA_Timer__PM_STBY_MSK

/* CFG2 field masks */
#define DMA_Timer_SRC_SEL_MSK        DMA_Timer__CFG2_SRC_SEL_MASK
#define DMA_Timer_MODE_MASK          (~(DMA_Timer_SRC_SEL_MSK))

#if defined(DMA_Timer__CFG3)
/* CFG3 phase mask */
#define DMA_Timer_PHASE_MASK         DMA_Timer__CFG3_PHASE_DLY_MASK
#endif /* defined(DMA_Timer__CFG3) */

#endif /* CY_CLOCK_DMA_Timer_H */


/* [] END OF FILE */
