/*******************************************************************************
* File Name: cyfitter.h
* 
* PSoC Creator  4.3
*
* Description:
* 
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include "cydevice.h"
#include "cydevice_trm.h"

/* ss */
#define ss__0__INTTYPE CYREG_PICU0_INTTYPE3
#define ss__0__MASK 0x08u
#define ss__0__PC CYREG_PRT0_PC3
#define ss__0__PORT 0u
#define ss__0__SHIFT 3u
#define ss__AG CYREG_PRT0_AG
#define ss__AMUX CYREG_PRT0_AMUX
#define ss__BIE CYREG_PRT0_BIE
#define ss__BIT_MASK CYREG_PRT0_BIT_MASK
#define ss__BYP CYREG_PRT0_BYP
#define ss__CTL CYREG_PRT0_CTL
#define ss__DM0 CYREG_PRT0_DM0
#define ss__DM1 CYREG_PRT0_DM1
#define ss__DM2 CYREG_PRT0_DM2
#define ss__DR CYREG_PRT0_DR
#define ss__INP_DIS CYREG_PRT0_INP_DIS
#define ss__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU0_BASE
#define ss__LCD_COM_SEG CYREG_PRT0_LCD_COM_SEG
#define ss__LCD_EN CYREG_PRT0_LCD_EN
#define ss__MASK 0x08u
#define ss__PORT 0u
#define ss__PRT CYREG_PRT0_PRT
#define ss__PRTDSI__CAPS_SEL CYREG_PRT0_CAPS_SEL
#define ss__PRTDSI__DBL_SYNC_IN CYREG_PRT0_DBL_SYNC_IN
#define ss__PRTDSI__OE_SEL0 CYREG_PRT0_OE_SEL0
#define ss__PRTDSI__OE_SEL1 CYREG_PRT0_OE_SEL1
#define ss__PRTDSI__OUT_SEL0 CYREG_PRT0_OUT_SEL0
#define ss__PRTDSI__OUT_SEL1 CYREG_PRT0_OUT_SEL1
#define ss__PRTDSI__SYNC_OUT CYREG_PRT0_SYNC_OUT
#define ss__PS CYREG_PRT0_PS
#define ss__SHIFT 3u
#define ss__SLW CYREG_PRT0_SLW

/* LCD */
#define LCD_LCDPort__0__INTTYPE CYREG_PICU2_INTTYPE0
#define LCD_LCDPort__0__MASK 0x01u
#define LCD_LCDPort__0__PC CYREG_PRT2_PC0
#define LCD_LCDPort__0__PORT 2u
#define LCD_LCDPort__0__SHIFT 0u
#define LCD_LCDPort__1__INTTYPE CYREG_PICU2_INTTYPE1
#define LCD_LCDPort__1__MASK 0x02u
#define LCD_LCDPort__1__PC CYREG_PRT2_PC1
#define LCD_LCDPort__1__PORT 2u
#define LCD_LCDPort__1__SHIFT 1u
#define LCD_LCDPort__2__INTTYPE CYREG_PICU2_INTTYPE2
#define LCD_LCDPort__2__MASK 0x04u
#define LCD_LCDPort__2__PC CYREG_PRT2_PC2
#define LCD_LCDPort__2__PORT 2u
#define LCD_LCDPort__2__SHIFT 2u
#define LCD_LCDPort__3__INTTYPE CYREG_PICU2_INTTYPE3
#define LCD_LCDPort__3__MASK 0x08u
#define LCD_LCDPort__3__PC CYREG_PRT2_PC3
#define LCD_LCDPort__3__PORT 2u
#define LCD_LCDPort__3__SHIFT 3u
#define LCD_LCDPort__4__INTTYPE CYREG_PICU2_INTTYPE4
#define LCD_LCDPort__4__MASK 0x10u
#define LCD_LCDPort__4__PC CYREG_PRT2_PC4
#define LCD_LCDPort__4__PORT 2u
#define LCD_LCDPort__4__SHIFT 4u
#define LCD_LCDPort__5__INTTYPE CYREG_PICU2_INTTYPE5
#define LCD_LCDPort__5__MASK 0x20u
#define LCD_LCDPort__5__PC CYREG_PRT2_PC5
#define LCD_LCDPort__5__PORT 2u
#define LCD_LCDPort__5__SHIFT 5u
#define LCD_LCDPort__6__INTTYPE CYREG_PICU2_INTTYPE6
#define LCD_LCDPort__6__MASK 0x40u
#define LCD_LCDPort__6__PC CYREG_PRT2_PC6
#define LCD_LCDPort__6__PORT 2u
#define LCD_LCDPort__6__SHIFT 6u
#define LCD_LCDPort__AG CYREG_PRT2_AG
#define LCD_LCDPort__AMUX CYREG_PRT2_AMUX
#define LCD_LCDPort__BIE CYREG_PRT2_BIE
#define LCD_LCDPort__BIT_MASK CYREG_PRT2_BIT_MASK
#define LCD_LCDPort__BYP CYREG_PRT2_BYP
#define LCD_LCDPort__CTL CYREG_PRT2_CTL
#define LCD_LCDPort__DM0 CYREG_PRT2_DM0
#define LCD_LCDPort__DM1 CYREG_PRT2_DM1
#define LCD_LCDPort__DM2 CYREG_PRT2_DM2
#define LCD_LCDPort__DR CYREG_PRT2_DR
#define LCD_LCDPort__INP_DIS CYREG_PRT2_INP_DIS
#define LCD_LCDPort__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU2_BASE
#define LCD_LCDPort__LCD_COM_SEG CYREG_PRT2_LCD_COM_SEG
#define LCD_LCDPort__LCD_EN CYREG_PRT2_LCD_EN
#define LCD_LCDPort__MASK 0x7Fu
#define LCD_LCDPort__PORT 2u
#define LCD_LCDPort__PRT CYREG_PRT2_PRT
#define LCD_LCDPort__PRTDSI__CAPS_SEL CYREG_PRT2_CAPS_SEL
#define LCD_LCDPort__PRTDSI__DBL_SYNC_IN CYREG_PRT2_DBL_SYNC_IN
#define LCD_LCDPort__PRTDSI__OE_SEL0 CYREG_PRT2_OE_SEL0
#define LCD_LCDPort__PRTDSI__OE_SEL1 CYREG_PRT2_OE_SEL1
#define LCD_LCDPort__PRTDSI__OUT_SEL0 CYREG_PRT2_OUT_SEL0
#define LCD_LCDPort__PRTDSI__OUT_SEL1 CYREG_PRT2_OUT_SEL1
#define LCD_LCDPort__PRTDSI__SYNC_OUT CYREG_PRT2_SYNC_OUT
#define LCD_LCDPort__PS CYREG_PRT2_PS
#define LCD_LCDPort__SHIFT 0u
#define LCD_LCDPort__SLW CYREG_PRT2_SLW

/* SPIM */
#define SPIM_BSPIM_BitCounter__16BIT_CONTROL_AUX_CTL_REG CYREG_B0_UDB01_02_ACTL
#define SPIM_BSPIM_BitCounter__16BIT_CONTROL_CONTROL_REG CYREG_B0_UDB01_02_CTL
#define SPIM_BSPIM_BitCounter__16BIT_CONTROL_COUNT_REG CYREG_B0_UDB01_02_CTL
#define SPIM_BSPIM_BitCounter__16BIT_COUNT_CONTROL_REG CYREG_B0_UDB01_02_CTL
#define SPIM_BSPIM_BitCounter__16BIT_COUNT_COUNT_REG CYREG_B0_UDB01_02_CTL
#define SPIM_BSPIM_BitCounter__16BIT_MASK_MASK_REG CYREG_B0_UDB01_02_MSK
#define SPIM_BSPIM_BitCounter__16BIT_MASK_PERIOD_REG CYREG_B0_UDB01_02_MSK
#define SPIM_BSPIM_BitCounter__16BIT_PERIOD_MASK_REG CYREG_B0_UDB01_02_MSK
#define SPIM_BSPIM_BitCounter__16BIT_PERIOD_PERIOD_REG CYREG_B0_UDB01_02_MSK
#define SPIM_BSPIM_BitCounter__CONTROL_AUX_CTL_REG CYREG_B0_UDB01_ACTL
#define SPIM_BSPIM_BitCounter__CONTROL_REG CYREG_B0_UDB01_CTL
#define SPIM_BSPIM_BitCounter__CONTROL_ST_REG CYREG_B0_UDB01_ST_CTL
#define SPIM_BSPIM_BitCounter__COUNT_REG CYREG_B0_UDB01_CTL
#define SPIM_BSPIM_BitCounter__COUNT_ST_REG CYREG_B0_UDB01_ST_CTL
#define SPIM_BSPIM_BitCounter__MASK_CTL_AUX_CTL_REG CYREG_B0_UDB01_MSK_ACTL
#define SPIM_BSPIM_BitCounter__PER_CTL_AUX_CTL_REG CYREG_B0_UDB01_MSK_ACTL
#define SPIM_BSPIM_BitCounter__PERIOD_REG CYREG_B0_UDB01_MSK
#define SPIM_BSPIM_BitCounter_ST__16BIT_STATUS_AUX_CTL_REG CYREG_B0_UDB01_02_ACTL
#define SPIM_BSPIM_BitCounter_ST__16BIT_STATUS_REG CYREG_B0_UDB01_02_ST
#define SPIM_BSPIM_BitCounter_ST__MASK_REG CYREG_B0_UDB01_MSK
#define SPIM_BSPIM_BitCounter_ST__MASK_ST_AUX_CTL_REG CYREG_B0_UDB01_MSK_ACTL
#define SPIM_BSPIM_BitCounter_ST__PER_ST_AUX_CTL_REG CYREG_B0_UDB01_MSK_ACTL
#define SPIM_BSPIM_BitCounter_ST__STATUS_AUX_CTL_REG CYREG_B0_UDB01_ACTL
#define SPIM_BSPIM_BitCounter_ST__STATUS_CNT_REG CYREG_B0_UDB01_ST_CTL
#define SPIM_BSPIM_BitCounter_ST__STATUS_CONTROL_REG CYREG_B0_UDB01_ST_CTL
#define SPIM_BSPIM_BitCounter_ST__STATUS_REG CYREG_B0_UDB01_ST
#define SPIM_BSPIM_RxStsReg__16BIT_STATUS_AUX_CTL_REG CYREG_B0_UDB02_03_ACTL
#define SPIM_BSPIM_RxStsReg__16BIT_STATUS_REG CYREG_B0_UDB02_03_ST
#define SPIM_BSPIM_RxStsReg__4__MASK 0x10u
#define SPIM_BSPIM_RxStsReg__4__POS 4
#define SPIM_BSPIM_RxStsReg__5__MASK 0x20u
#define SPIM_BSPIM_RxStsReg__5__POS 5
#define SPIM_BSPIM_RxStsReg__6__MASK 0x40u
#define SPIM_BSPIM_RxStsReg__6__POS 6
#define SPIM_BSPIM_RxStsReg__MASK 0x70u
#define SPIM_BSPIM_RxStsReg__MASK_REG CYREG_B0_UDB02_MSK
#define SPIM_BSPIM_RxStsReg__STATUS_AUX_CTL_REG CYREG_B0_UDB02_ACTL
#define SPIM_BSPIM_RxStsReg__STATUS_REG CYREG_B0_UDB02_ST
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_A0_REG CYREG_B0_UDB01_02_A0
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_A1_REG CYREG_B0_UDB01_02_A1
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_D0_REG CYREG_B0_UDB01_02_D0
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_D1_REG CYREG_B0_UDB01_02_D1
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_DP_AUX_CTL_REG CYREG_B0_UDB01_02_ACTL
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_F0_REG CYREG_B0_UDB01_02_F0
#define SPIM_BSPIM_sR8_Dp_u0__16BIT_F1_REG CYREG_B0_UDB01_02_F1
#define SPIM_BSPIM_sR8_Dp_u0__A0_A1_REG CYREG_B0_UDB01_A0_A1
#define SPIM_BSPIM_sR8_Dp_u0__A0_REG CYREG_B0_UDB01_A0
#define SPIM_BSPIM_sR8_Dp_u0__A1_REG CYREG_B0_UDB01_A1
#define SPIM_BSPIM_sR8_Dp_u0__D0_D1_REG CYREG_B0_UDB01_D0_D1
#define SPIM_BSPIM_sR8_Dp_u0__D0_REG CYREG_B0_UDB01_D0
#define SPIM_BSPIM_sR8_Dp_u0__D1_REG CYREG_B0_UDB01_D1
#define SPIM_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG CYREG_B0_UDB01_ACTL
#define SPIM_BSPIM_sR8_Dp_u0__F0_F1_REG CYREG_B0_UDB01_F0_F1
#define SPIM_BSPIM_sR8_Dp_u0__F0_REG CYREG_B0_UDB01_F0
#define SPIM_BSPIM_sR8_Dp_u0__F1_REG CYREG_B0_UDB01_F1
#define SPIM_BSPIM_sR8_Dp_u0__MSK_DP_AUX_CTL_REG CYREG_B0_UDB01_MSK_ACTL
#define SPIM_BSPIM_sR8_Dp_u0__PER_DP_AUX_CTL_REG CYREG_B0_UDB01_MSK_ACTL
#define SPIM_BSPIM_TxStsReg__0__MASK 0x01u
#define SPIM_BSPIM_TxStsReg__0__POS 0
#define SPIM_BSPIM_TxStsReg__1__MASK 0x02u
#define SPIM_BSPIM_TxStsReg__1__POS 1
#define SPIM_BSPIM_TxStsReg__16BIT_STATUS_AUX_CTL_REG CYREG_B0_UDB03_04_ACTL
#define SPIM_BSPIM_TxStsReg__16BIT_STATUS_REG CYREG_B0_UDB03_04_ST
#define SPIM_BSPIM_TxStsReg__2__MASK 0x04u
#define SPIM_BSPIM_TxStsReg__2__POS 2
#define SPIM_BSPIM_TxStsReg__3__MASK 0x08u
#define SPIM_BSPIM_TxStsReg__3__POS 3
#define SPIM_BSPIM_TxStsReg__4__MASK 0x10u
#define SPIM_BSPIM_TxStsReg__4__POS 4
#define SPIM_BSPIM_TxStsReg__MASK 0x1Fu
#define SPIM_BSPIM_TxStsReg__MASK_REG CYREG_B0_UDB03_MSK
#define SPIM_BSPIM_TxStsReg__STATUS_AUX_CTL_REG CYREG_B0_UDB03_ACTL
#define SPIM_BSPIM_TxStsReg__STATUS_REG CYREG_B0_UDB03_ST

/* miso */
#define miso__0__INTTYPE CYREG_PICU0_INTTYPE0
#define miso__0__MASK 0x01u
#define miso__0__PC CYREG_PRT0_PC0
#define miso__0__PORT 0u
#define miso__0__SHIFT 0u
#define miso__AG CYREG_PRT0_AG
#define miso__AMUX CYREG_PRT0_AMUX
#define miso__BIE CYREG_PRT0_BIE
#define miso__BIT_MASK CYREG_PRT0_BIT_MASK
#define miso__BYP CYREG_PRT0_BYP
#define miso__CTL CYREG_PRT0_CTL
#define miso__DM0 CYREG_PRT0_DM0
#define miso__DM1 CYREG_PRT0_DM1
#define miso__DM2 CYREG_PRT0_DM2
#define miso__DR CYREG_PRT0_DR
#define miso__INP_DIS CYREG_PRT0_INP_DIS
#define miso__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU0_BASE
#define miso__LCD_COM_SEG CYREG_PRT0_LCD_COM_SEG
#define miso__LCD_EN CYREG_PRT0_LCD_EN
#define miso__MASK 0x01u
#define miso__PORT 0u
#define miso__PRT CYREG_PRT0_PRT
#define miso__PRTDSI__CAPS_SEL CYREG_PRT0_CAPS_SEL
#define miso__PRTDSI__DBL_SYNC_IN CYREG_PRT0_DBL_SYNC_IN
#define miso__PRTDSI__OE_SEL0 CYREG_PRT0_OE_SEL0
#define miso__PRTDSI__OE_SEL1 CYREG_PRT0_OE_SEL1
#define miso__PRTDSI__OUT_SEL0 CYREG_PRT0_OUT_SEL0
#define miso__PRTDSI__OUT_SEL1 CYREG_PRT0_OUT_SEL1
#define miso__PRTDSI__SYNC_OUT CYREG_PRT0_SYNC_OUT
#define miso__PS CYREG_PRT0_PS
#define miso__SHIFT 0u
#define miso__SLW CYREG_PRT0_SLW

/* mosi */
#define mosi__0__INTTYPE CYREG_PICU0_INTTYPE1
#define mosi__0__MASK 0x02u
#define mosi__0__PC CYREG_PRT0_PC1
#define mosi__0__PORT 0u
#define mosi__0__SHIFT 1u
#define mosi__AG CYREG_PRT0_AG
#define mosi__AMUX CYREG_PRT0_AMUX
#define mosi__BIE CYREG_PRT0_BIE
#define mosi__BIT_MASK CYREG_PRT0_BIT_MASK
#define mosi__BYP CYREG_PRT0_BYP
#define mosi__CTL CYREG_PRT0_CTL
#define mosi__DM0 CYREG_PRT0_DM0
#define mosi__DM1 CYREG_PRT0_DM1
#define mosi__DM2 CYREG_PRT0_DM2
#define mosi__DR CYREG_PRT0_DR
#define mosi__INP_DIS CYREG_PRT0_INP_DIS
#define mosi__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU0_BASE
#define mosi__LCD_COM_SEG CYREG_PRT0_LCD_COM_SEG
#define mosi__LCD_EN CYREG_PRT0_LCD_EN
#define mosi__MASK 0x02u
#define mosi__PORT 0u
#define mosi__PRT CYREG_PRT0_PRT
#define mosi__PRTDSI__CAPS_SEL CYREG_PRT0_CAPS_SEL
#define mosi__PRTDSI__DBL_SYNC_IN CYREG_PRT0_DBL_SYNC_IN
#define mosi__PRTDSI__OE_SEL0 CYREG_PRT0_OE_SEL0
#define mosi__PRTDSI__OE_SEL1 CYREG_PRT0_OE_SEL1
#define mosi__PRTDSI__OUT_SEL0 CYREG_PRT0_OUT_SEL0
#define mosi__PRTDSI__OUT_SEL1 CYREG_PRT0_OUT_SEL1
#define mosi__PRTDSI__SYNC_OUT CYREG_PRT0_SYNC_OUT
#define mosi__PS CYREG_PRT0_PS
#define mosi__SHIFT 1u
#define mosi__SLW CYREG_PRT0_SLW

/* sclk */
#define sclk__0__INTTYPE CYREG_PICU0_INTTYPE2
#define sclk__0__MASK 0x04u
#define sclk__0__PC CYREG_PRT0_PC2
#define sclk__0__PORT 0u
#define sclk__0__SHIFT 2u
#define sclk__AG CYREG_PRT0_AG
#define sclk__AMUX CYREG_PRT0_AMUX
#define sclk__BIE CYREG_PRT0_BIE
#define sclk__BIT_MASK CYREG_PRT0_BIT_MASK
#define sclk__BYP CYREG_PRT0_BYP
#define sclk__CTL CYREG_PRT0_CTL
#define sclk__DM0 CYREG_PRT0_DM0
#define sclk__DM1 CYREG_PRT0_DM1
#define sclk__DM2 CYREG_PRT0_DM2
#define sclk__DR CYREG_PRT0_DR
#define sclk__INP_DIS CYREG_PRT0_INP_DIS
#define sclk__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU0_BASE
#define sclk__LCD_COM_SEG CYREG_PRT0_LCD_COM_SEG
#define sclk__LCD_EN CYREG_PRT0_LCD_EN
#define sclk__MASK 0x04u
#define sclk__PORT 0u
#define sclk__PRT CYREG_PRT0_PRT
#define sclk__PRTDSI__CAPS_SEL CYREG_PRT0_CAPS_SEL
#define sclk__PRTDSI__DBL_SYNC_IN CYREG_PRT0_DBL_SYNC_IN
#define sclk__PRTDSI__OE_SEL0 CYREG_PRT0_OE_SEL0
#define sclk__PRTDSI__OE_SEL1 CYREG_PRT0_OE_SEL1
#define sclk__PRTDSI__OUT_SEL0 CYREG_PRT0_OUT_SEL0
#define sclk__PRTDSI__OUT_SEL1 CYREG_PRT0_OUT_SEL1
#define sclk__PRTDSI__SYNC_OUT CYREG_PRT0_SYNC_OUT
#define sclk__PS CYREG_PRT0_PS
#define sclk__SHIFT 2u
#define sclk__SLW CYREG_PRT0_SLW

/* Clock */
#define Clock__CFG0 CYREG_CLKDIST_DCFG0_CFG0
#define Clock__CFG1 CYREG_CLKDIST_DCFG0_CFG1
#define Clock__CFG2 CYREG_CLKDIST_DCFG0_CFG2
#define Clock__CFG2_SRC_SEL_MASK 0x07u
#define Clock__INDEX 0x00u
#define Clock__PM_ACT_CFG CYREG_PM_ACT_CFG2
#define Clock__PM_ACT_MSK 0x01u
#define Clock__PM_STBY_CFG CYREG_PM_STBY_CFG2
#define Clock__PM_STBY_MSK 0x01u

/* DMA_RX */
#define DMA_RX__DRQ_CTL CYREG_IDMUX_DRQ_CTL0
#define DMA_RX__DRQ_NUMBER 0u
#define DMA_RX__NUMBEROF_TDS 0u
#define DMA_RX__PRIORITY 2u
#define DMA_RX__TERMIN_EN 0u
#define DMA_RX__TERMIN_SEL 0u
#define DMA_RX__TERMOUT0_EN 0u
#define DMA_RX__TERMOUT0_SEL 0u
#define DMA_RX__TERMOUT1_EN 0u
#define DMA_RX__TERMOUT1_SEL 0u

/* DMA_TX */
#define DMA_TX__DRQ_CTL CYREG_IDMUX_DRQ_CTL0
#define DMA_TX__DRQ_NUMBER 1u
#define DMA_TX__NUMBEROF_TDS 0u
#define DMA_TX__PRIORITY 2u
#define DMA_TX__TERMIN_EN 0u
#define DMA_TX__TERMIN_SEL 0u
#define DMA_TX__TERMOUT0_EN 0u
#define DMA_TX__TERMOUT0_SEL 0u
#define DMA_TX__TERMOUT1_EN 0u
#define DMA_TX__TERMOUT1_SEL 0u

/* Miscellaneous */
#define BCLK__BUS_CLK__HZ 24000000U
#define BCLK__BUS_CLK__KHZ 24000U
#define BCLK__BUS_CLK__MHZ 24U
#define CY_PROJECT_NAME "SPIM_Example01_DMA"
#define CY_VERSION "PSoC Creator  4.3"
#define CYDEV_CHIP_DIE_LEOPARD 1u
#define CYDEV_CHIP_DIE_PSOC4A 24u
#define CYDEV_CHIP_DIE_PSOC5LP 2u
#define CYDEV_CHIP_DIE_PSOC5TM 3u
#define CYDEV_CHIP_DIE_TMA4 4u
#define CYDEV_CHIP_DIE_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_FM0P 5u
#define CYDEV_CHIP_FAMILY_FM3 6u
#define CYDEV_CHIP_FAMILY_FM4 7u
#define CYDEV_CHIP_FAMILY_PSOC3 1u
#define CYDEV_CHIP_FAMILY_PSOC4 2u
#define CYDEV_CHIP_FAMILY_PSOC5 3u
#define CYDEV_CHIP_FAMILY_PSOC6 4u
#define CYDEV_CHIP_FAMILY_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC5
#define CYDEV_CHIP_JTAG_ID 0x2E161069u
#define CYDEV_CHIP_MEMBER_3A 1u
#define CYDEV_CHIP_MEMBER_4A 24u
#define CYDEV_CHIP_MEMBER_4AA 23u
#define CYDEV_CHIP_MEMBER_4AB 28u
#define CYDEV_CHIP_MEMBER_4AC 14u
#define CYDEV_CHIP_MEMBER_4D 18u
#define CYDEV_CHIP_MEMBER_4E 6u
#define CYDEV_CHIP_MEMBER_4F 25u
#define CYDEV_CHIP_MEMBER_4G 4u
#define CYDEV_CHIP_MEMBER_4H 22u
#define CYDEV_CHIP_MEMBER_4I 30u
#define CYDEV_CHIP_MEMBER_4J 19u
#define CYDEV_CHIP_MEMBER_4K 20u
#define CYDEV_CHIP_MEMBER_4L 29u
#define CYDEV_CHIP_MEMBER_4M 27u
#define CYDEV_CHIP_MEMBER_4N 11u
#define CYDEV_CHIP_MEMBER_4O 8u
#define CYDEV_CHIP_MEMBER_4P 26u
#define CYDEV_CHIP_MEMBER_4Q 15u
#define CYDEV_CHIP_MEMBER_4R 9u
#define CYDEV_CHIP_MEMBER_4S 12u
#define CYDEV_CHIP_MEMBER_4T 10u
#define CYDEV_CHIP_MEMBER_4U 5u
#define CYDEV_CHIP_MEMBER_4V 21u
#define CYDEV_CHIP_MEMBER_4W 13u
#define CYDEV_CHIP_MEMBER_4X 7u
#define CYDEV_CHIP_MEMBER_4Y 16u
#define CYDEV_CHIP_MEMBER_4Z 17u
#define CYDEV_CHIP_MEMBER_5A 3u
#define CYDEV_CHIP_MEMBER_5B 2u
#define CYDEV_CHIP_MEMBER_6A 31u
#define CYDEV_CHIP_MEMBER_FM3 35u
#define CYDEV_CHIP_MEMBER_FM4 36u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 32u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 33u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 34u
#define CYDEV_CHIP_MEMBER_UNKNOWN 0u
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_5B
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_MEMBER_USED
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_REV_LEOPARD_ES1 0u
#define CYDEV_CHIP_REV_LEOPARD_ES2 1u
#define CYDEV_CHIP_REV_LEOPARD_ES3 3u
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3u
#define CYDEV_CHIP_REV_PSOC4A_ES0 17u
#define CYDEV_CHIP_REV_PSOC4A_PRODUCTION 17u
#define CYDEV_CHIP_REV_PSOC5LP_ES0 0u
#define CYDEV_CHIP_REV_PSOC5LP_PRODUCTION 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES0 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES1 1u
#define CYDEV_CHIP_REV_PSOC5TM_PRODUCTION 1u
#define CYDEV_CHIP_REV_TMA4_ES 17u
#define CYDEV_CHIP_REV_TMA4_ES2 33u
#define CYDEV_CHIP_REV_TMA4_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_3A_ES1 0u
#define CYDEV_CHIP_REVISION_3A_ES2 1u
#define CYDEV_CHIP_REVISION_3A_ES3 3u
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3u
#define CYDEV_CHIP_REVISION_4A_ES0 17u
#define CYDEV_CHIP_REVISION_4A_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4AA_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4AB_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4AC_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4D_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD 0u
#define CYDEV_CHIP_REVISION_4E_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256K 0u
#define CYDEV_CHIP_REVISION_4G_ES 17u
#define CYDEV_CHIP_REVISION_4G_ES2 33u
#define CYDEV_CHIP_REVISION_4G_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4H_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4I_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4J_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4K_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4L_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4M_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4N_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4O_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4P_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Q_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4R_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4S_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4T_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4U_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4V_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4W_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4X_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Y_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Z_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_5A_ES0 0u
#define CYDEV_CHIP_REVISION_5A_ES1 1u
#define CYDEV_CHIP_REVISION_5A_PRODUCTION 1u
#define CYDEV_CHIP_REVISION_5B_ES0 0u
#define CYDEV_CHIP_REVISION_5B_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_6A_ES 17u
#define CYDEV_CHIP_REVISION_6A_NO_UDB 33u
#define CYDEV_CHIP_REVISION_6A_PRODUCTION 33u
#define CYDEV_CHIP_REVISION_FM3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_FM4_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_5B_PRODUCTION
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REVISION_USED
#define CYDEV_CONFIG_FASTBOOT_ENABLED 0
#define CYDEV_CONFIG_UNUSED_IO_AllowButWarn 0
#define CYDEV_CONFIG_UNUSED_IO CYDEV_CONFIG_UNUSED_IO_AllowButWarn
#define CYDEV_CONFIG_UNUSED_IO_AllowWithInfo 1
#define CYDEV_CONFIG_UNUSED_IO_Disallowed 2
#define CYDEV_CONFIGURATION_COMPRESSED 0
#define CYDEV_CONFIGURATION_DMA 1
#define CYDEV_CONFIGURATION_ECC 1
#define CYDEV_CONFIGURATION_IMOENABLED CYDEV_CONFIG_FASTBOOT_ENABLED
#define CYDEV_CONFIGURATION_MODE_COMPRESSED 0
#define CYDEV_CONFIGURATION_MODE_DMA 2
#define CYDEV_CONFIGURATION_MODE CYDEV_CONFIGURATION_MODE_DMA
#define CYDEV_CONFIGURATION_MODE_UNCOMPRESSED 1
#define CYDEV_DEBUG_ENABLE_MASK 0x20u
#define CYDEV_DEBUG_ENABLE_REGISTER CYREG_MLOGIC_DEBUG
#define CYDEV_DEBUGGING_DPS_Disable 3
#define CYDEV_DEBUGGING_DPS_JTAG_4 1
#define CYDEV_DEBUGGING_DPS_JTAG_5 0
#define CYDEV_DEBUGGING_DPS_SWD 2
#define CYDEV_DEBUGGING_DPS_SWD_SWV 6
#define CYDEV_DEBUGGING_DPS CYDEV_DEBUGGING_DPS_SWD_SWV
#define CYDEV_DEBUGGING_ENABLE 1
#define CYDEV_DEBUGGING_XRES 0
#define CYDEV_DMA_CHANNELS_AVAILABLE 24u
#define CYDEV_ECC_ENABLE 0
#define CYDEV_HEAP_SIZE 0x80
#define CYDEV_INSTRUCT_CACHE_ENABLED 1
#define CYDEV_INTR_RISING 0x00000000u
#define CYDEV_IS_EXPORTING_CODE 0
#define CYDEV_IS_IMPORTING_CODE 0
#define CYDEV_PROJ_TYPE 0
#define CYDEV_PROJ_TYPE_BOOTLOADER 1
#define CYDEV_PROJ_TYPE_LAUNCHER 5
#define CYDEV_PROJ_TYPE_LOADABLE 2
#define CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER 4
#define CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER 3
#define CYDEV_PROJ_TYPE_STANDARD 0
#define CYDEV_PROTECTION_ENABLE 0
#define CYDEV_STACK_SIZE 0x0800
#define CYDEV_USE_BUNDLED_CMSIS 1
#define CYDEV_VARIABLE_VDDA 0
#define CYDEV_VDDA 5.0
#define CYDEV_VDDA_MV 5000
#define CYDEV_VDDD 5.0
#define CYDEV_VDDD_MV 5000
#define CYDEV_VDDIO0 5.0
#define CYDEV_VDDIO0_MV 5000
#define CYDEV_VDDIO1 5.0
#define CYDEV_VDDIO1_MV 5000
#define CYDEV_VDDIO2 5.0
#define CYDEV_VDDIO2_MV 5000
#define CYDEV_VDDIO3 5.0
#define CYDEV_VDDIO3_MV 5000
#define CYDEV_VIO0 5.0
#define CYDEV_VIO0_MV 5000
#define CYDEV_VIO1 5.0
#define CYDEV_VIO1_MV 5000
#define CYDEV_VIO2 5.0
#define CYDEV_VIO2_MV 5000
#define CYDEV_VIO3 5.0
#define CYDEV_VIO3_MV 5000
#define CYIPBLOCK_ARM_CM3_VERSION 0
#define CYIPBLOCK_P3_ANAIF_VERSION 0
#define CYIPBLOCK_P3_CAN_VERSION 0
#define CYIPBLOCK_P3_CAPSENSE_VERSION 0
#define CYIPBLOCK_P3_COMP_VERSION 0
#define CYIPBLOCK_P3_DECIMATOR_VERSION 0
#define CYIPBLOCK_P3_DFB_VERSION 0
#define CYIPBLOCK_P3_DMA_VERSION 0
#define CYIPBLOCK_P3_DRQ_VERSION 0
#define CYIPBLOCK_P3_DSM_VERSION 0
#define CYIPBLOCK_P3_EMIF_VERSION 0
#define CYIPBLOCK_P3_I2C_VERSION 0
#define CYIPBLOCK_P3_LCD_VERSION 0
#define CYIPBLOCK_P3_LPF_VERSION 0
#define CYIPBLOCK_P3_OPAMP_VERSION 0
#define CYIPBLOCK_P3_PM_VERSION 0
#define CYIPBLOCK_P3_SCCT_VERSION 0
#define CYIPBLOCK_P3_TIMER_VERSION 0
#define CYIPBLOCK_P3_USB_VERSION 0
#define CYIPBLOCK_P3_VIDAC_VERSION 0
#define CYIPBLOCK_P3_VREF_VERSION 0
#define CYIPBLOCK_S8_GPIO_VERSION 0
#define CYIPBLOCK_S8_IRQ_VERSION 0
#define CYIPBLOCK_S8_SAR_VERSION 0
#define CYIPBLOCK_S8_SIO_VERSION 0
#define CYIPBLOCK_S8_UDB_VERSION 0
#define DMA_CHANNELS_USED__MASK0 0x00000003u
#define CYDEV_BOOTLOADER_ENABLE 0

#endif /* INCLUDED_CYFITTER_H */
