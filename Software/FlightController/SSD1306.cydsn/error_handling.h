/***********************************************************************
*! \file:                   error_handling.h
*  \projekt:                FT800_ESP
*  \created on:             25.07.2020
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   interupts handling
***********************************************************************/
 
#ifndef _error_handling_h_
#define _error_handling_h_
 
/***********************************************************************
* Includes
**********************************************************************/
#include "stdint.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
 * Declarations
 **********************************************************************/
 typedef uint16_t error_code_t;
/***********************************************************************
 * Global Variable
 **********************************************************************/
enum error_codes{
    ec_no_error = 0x0000
};
/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions 
 **********************************************************************/


#endif //_error_handling_h_