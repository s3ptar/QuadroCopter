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

#define DeviceADR_Base                  0b110100


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
*! \fn          uint8_t write_data(uint8 SlaveAddress, uint8_t Register, uint8 wrData)
*  \brief       write to an slave register address
*  \param       uint8 SlaveAddress
*  \param       uint8_t Register
*  \param       uint8 wrData
*  \exception   none
*  \return      0 = success
***********************************************************************/
uint8_t write_data(uint8 SlaveAddress, uint8_t Register, uint8 wrData){
    
    uint8_t write_buffer[2];
    write_buffer[0] =  Register;
    write_buffer[1] =  wrData;
    return I2C_Master_1_MasterWriteBuf(SlaveAddress, write_buffer, 2, I2C_Master_1_MODE_COMPLETE_XFER);
}

/***********************************************************************
*! \fn          uint8_t read_register_data(uint8 SlaveAddress, uint8_t Register)
*  \brief       read single register
*  \param       uint8 SlaveAddress
*  \param       uint8 Register - Address of Register
*  \exception   none
*  \return      register value
***********************************************************************/
uint8_t read_register_data(uint8 SlaveAddress, uint8_t Register){
    
    uint8_t rdData;
    I2C_Master_1_MasterWriteBuf(SlaveAddress, &Register, 1, I2C_Master_1_MODE_NO_STOP );
    I2C_Master_1_MasterReadBuf(SlaveAddress, &rdData, 1, I2C_Master_1_MODE_COMPLETE_XFER);
    return rdData;
}


/***********************************************************************
*! \fn          void read_itg3200_data(uint8 SlaveAddress, 
*                   uint8_t StartRegister, uint8_t DataToRead, 
*                   uint16_t * ReadBuffer)
*  \brief       read Number of 8 Bit register
*  \param       uint8 SlaveAddress
*  \param       uint8 StartRegister - Start Registeradress
*  \param       uint8_t nRead - Number of Read Register
*  \param       uint8 * ReadBuffer - Pointer to ReadBuffer
*  \exception   none
*  \return      void
***********************************************************************/
void read_itg3200_data(uint8 SlaveAddress, uint8_t StartRegister, uint8_t nRead, uint8_t * ReadBuffer){
    
    I2C_Master_1_MasterWriteBuf(SlaveAddress, &StartRegister, 1, I2C_Master_1_MODE_NO_STOP );
    I2C_Master_1_MasterReadBuf(SlaveAddress, ReadBuffer, nRead<<1, I2C_Master_1_MODE_COMPLETE_XFER);
    
    //Build Buffer after Read

}

/***********************************************************************
*! \fn          uint8_t set_sample_rate_divider(uint8_t rate_divider)
*  \brief       This register determines the sample rate of the ITG-3200 gyros
*  \param       uint8_t rate_divider
*  \exception   none
*  \return      0 = success
***********************************************************************/
static uint8_t set_sample_rate_divider(uint8_t rate_divider){
    
    return write_data(0x00, SMPLRT_DIV_REG, rate_divider);
    
}

/***********************************************************************
*! \fn          uint8_t set_digital_low_pass_filter_configuration(uint8_t DLPF_CFG)
*  \brief       The DLPF_CFG parameter sets the digital low pass filter configuration. It also determines the
*               internal sampling rate used by the device as shown in the table below
*  \param       uint8_t DLPF_CFG
*  \exception   none
*  \return      0 = success
***********************************************************************/
uint8_t set_digital_low_pass_filter_configuration(uint8_t DLPF_CFG){
    
    //Set to fullscale and filter configuration
    return write_data(0x00, DLPF_FS_REG, ((0x03<<3) | (DLPF_CFG & 0x07)));
    
}

/***********************************************************************
*! \fn          set_interrupt_configuration(_Bool ACTL, _Bool Open, _Bool LATCH_INT_EN, _Bool INT_ANYRD_2CLEAR, _Bool ITG_RDY_EN, _Bool RAW_RDY_EN)
*  \brief       This register configures the interrupt operation of the device
*  \param       _Bool ACTL Logic level for INT output pin – 1=active low, 0=active high 
*  \param       _Bool OPEN Drive type for INT output pin – 1=open drain, 0=push-pull
*  \param       _Bool Latch mode – 1=latch until interrupt is cleared, 0=50us pulse
*  \param       _Bool Latch clear method – 1=any register read, 0=status register read only
*  \param       _Bool Enable interrupt when device is ready (PLL ready after changing clock source) 
*  \param       _Bool Enable interrupt when data is available
*  \exception   none
*  \return      0 = success
***********************************************************************/
uint8_t set_interrupt_configuration(_Bool ACTL, _Bool Open, _Bool LATCH_INT_EN, _Bool INT_ANYRD_2CLEAR, _Bool ITG_RDY_EN, _Bool RAW_RDY_EN){
    
    //Set to fullscale and filter configuration
    return write_data(0x00, INT_CFG_REG, ((ACTL<<7)|(Open<<6)|(LATCH_INT_EN<<5)|(INT_ANYRD_2CLEAR<<4)|(ITG_RDY_EN<<2)|(RAW_RDY_EN<<0)));
    
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
uint8_t setup_itg3200(_Bool ADRPin, uint8_t rate_divider, uint8_t DLPF_CFG){
    
    //setup ADR
    DeviceADR = ((uint8_t)DeviceADR_Base)<<1;
    uint8 DeviceID;
    if(ADRPin)
       DeviceADR |= 0x01;
    /****** Check Chip ID ******/
    if(! ((DeviceID = read_register_data(DeviceADR, WHO_AM_I_REG)>>1) == DeviceADR_Base))
        return itg3200_device_id_error;
    
    set_sample_rate_divider(rate_divider);
    set_digital_low_pass_filter_configuration(DLPF_CFG);
    set_interrupt_configuration(Active_Interrupt_Level_High, Interrupt_Pin_PushPull, Interupt_Latch_50usPulse, 
        Interrupt_Clear_after_read , Interrupt_deactivation , Interrupt_Data_Ready );
        
    return DeviceID;
}