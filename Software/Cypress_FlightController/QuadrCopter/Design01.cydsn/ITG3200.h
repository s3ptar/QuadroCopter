/***********************************************************************
*! \file:                   ITG3200.h
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/
 
#ifndef _ITG3200_H_
#define _ITG3200_H_
 
/***********************************************************************
 * Includes
 **********************************************************************/
 #include "stdint.h"
 #include "project.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
 * Declarations
 **********************************************************************/
 enum itg_error_codes{
    
    itg3200_okay = 0,
    itg3200_device_id_error,
    
};

#define Active_Interrupt_Level_High             0
#define Active_Interrupt_Level_Low              1
#define Interrupt_Pin_OpenDrain                 1
#define Interrupt_Pin_PushPull                  0
#define Interupt_Latch_unitl_Clear              1
#define Interupt_Latch_50usPulse                0
#define Interrupt_Clear_after_read              1
#define Interrupt_Clear_after_read_Status_Reg   0 
#define Interrupt_deactivation                  0
#define Interrupt_PLL_Ready                     1
#define Interrupt_Data_Ready                    1

/***********************************************************************
 * Global Variable
 **********************************************************************/
 
/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions
 **********************************************************************/
uint8_t setup_itg3200(_Bool ADRPin, uint8_t rate_divider, uint8_t DLPF_CFG);
void read_itg3200_data(uint8 SlaveAddress, uint8_t StartRegister, uint8_t nRead, uint8_t* ReadBuffer);
uint8_t read_register_data(uint8 SlaveAddress, uint8_t Register);
 
 
#endif /* _ITG3200_H_ */
