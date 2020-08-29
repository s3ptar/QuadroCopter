/***********************************************************************
*! \file:                   ITG3200.c
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   The ITG-3200 triple-axis MEMS gyroscope 
*                           includes a wide range of features
***********************************************************************/
 
/***********************************************************************
* Includes
***********************************************************************/
#include "ITG3200.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://invensense.tdk.com/products/motion-tracking/3-axis/itg-3200/
/***********************************************************************
* Declarations
***********************************************************************/

#define WHO_AM_I_REG                    0x00
#define SMPLRT_DIV_REG                  0x15
#define DLPF_FS_REG                     0x16
#define INT_CFG_REG                     0x17
#define INT_STATUS_REG                  0x1A
#define TEMP_OUT_H_REG                  0x1B
#define TEMP_OUT_L_REG                  0x1C
#define GYRO_XOUT_H_REG                 0x1D
#define GYRO_XOUT_L_REG                 0x1E
#define GYRO_YOUT_H_REG                 0x1F
#define GYRO_YOUT_L_REG                 0x20
#define GYRO_ZOUT_H_REG                 0x21
#define GYRO_ZOUT_L_REG                 0x22
#define PWR_MGM_REG                     0x3E

#define DeviceADR_Base                  0b1101000

/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* Local Variable
***********************************************************************/
uint8_t DeviceADR;
/***********************************************************************
* Constant
***********************************************************************/
 
/***********************************************************************
* Local Funtions
***********************************************************************/

/***********************************************************************
*! \fn          uint8 write_data(uint8 SlaveAddress, uint8_t Register, uint8 wrData)
*  \brief       write to an slave register address
*  \param       uint8 SlaveAddress
*  \param       uint8_t Register
*  \param       uint8 wrData
*  \exception   none
*  \return      0 = success
***********************************************************************/
uint8 write_data(uint8 SlaveAddress, uint8_t Register, uint8 wrData){
    
    uint8_t write_buffer[2];
    write_buffer[0] =  Register;
    write_buffer[1] =  wrData;
    return I2C_Master_1_MasterWriteBuf(SlaveAddress, write_buffer, 2, I2C_Master_1_MODE_COMPLETE_XFER);
}

/***********************************************************************
*! \fn          uint8 read_register_data(uint8 SlaveAddress, uint8_t Register)
*  \brief       read single register
*  \param       uint8 SlaveAddress
*  \exception   none
*  \return      register value
***********************************************************************/
uint8 read_register_data(uint8 SlaveAddress, uint8_t Register){
    
    uint8_t rdData;
    I2C_Master_1_MasterWriteBuf(SlaveAddress, &Register, 1, I2C_Master_1_MODE_NO_STOP );
    I2C_Master_1_MasterReadBuf(SlaveAddress, &rdData, 1, I2C_Master_1_MODE_COMPLETE_XFER);
    return rdData;
}

/***********************************************************************
*! \fn          uint8_t set_sample_rate_divider(uint8_t rate_divider)
*  \brief       
*  \param       uint8_t rate_divider
*  \exception   none
*  \return      0 = success
***********************************************************************/
uint8_t set_sample_rate_divider(uint8_t rate_divider){
    
    write_data(0x00, SMPLRT_DIV_REG, rate_divider);
    
    return 0;
    
}

/***********************************************************************
*! \fn          uint8_t setup_itg3200(_Bool ADRPin)
*  \brief       init the itg3200 gyro
*  \param       _Bool ADRPin - state of ADR Pin
*  \param       uint8_t Register
*  \param       uint8 wrData
*  \exception   none
*  \return      0 = success
***********************************************************************/
uint8_t setup_itg3200(_Bool ADRPin){
    
    DeviceADR = ((uint8_t)DeviceADR_Base)<<1;
    if(ADRPin)
       DeviceADR |= 0x01;
        
    return 0;
}