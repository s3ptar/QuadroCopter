/*******************************************************************************
* File Name: DBG_OUT_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
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


/***************************************
* Local data allocation
***************************************/

static DBG_OUT_BACKUP_STRUCT  DBG_OUT_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: DBG_OUT_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the DBG_OUT_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DBG_OUT_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DBG_OUT_SaveConfig(void)
{
    #if(DBG_OUT_CONTROL_REG_REMOVED == 0u)
        DBG_OUT_backup.cr = DBG_OUT_CONTROL_REG;
    #endif /* End DBG_OUT_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: DBG_OUT_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DBG_OUT_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling DBG_OUT_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void DBG_OUT_RestoreConfig(void)
{
    #if(DBG_OUT_CONTROL_REG_REMOVED == 0u)
        DBG_OUT_CONTROL_REG = DBG_OUT_backup.cr;
    #endif /* End DBG_OUT_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: DBG_OUT_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The DBG_OUT_Sleep() API saves the current component state. Then it
*  calls the DBG_OUT_Stop() function and calls 
*  DBG_OUT_SaveConfig() to save the hardware configuration.
*  Call the DBG_OUT_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DBG_OUT_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DBG_OUT_Sleep(void)
{
    #if(DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED)
        if((DBG_OUT_RXSTATUS_ACTL_REG  & DBG_OUT_INT_ENABLE) != 0u)
        {
            DBG_OUT_backup.enableState = 1u;
        }
        else
        {
            DBG_OUT_backup.enableState = 0u;
        }
    #else
        if((DBG_OUT_TXSTATUS_ACTL_REG  & DBG_OUT_INT_ENABLE) !=0u)
        {
            DBG_OUT_backup.enableState = 1u;
        }
        else
        {
            DBG_OUT_backup.enableState = 0u;
        }
    #endif /* End DBG_OUT_RX_ENABLED || DBG_OUT_HD_ENABLED*/

    DBG_OUT_Stop();
    DBG_OUT_SaveConfig();
}


/*******************************************************************************
* Function Name: DBG_OUT_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  DBG_OUT_Sleep() was called. The DBG_OUT_Wakeup() function
*  calls the DBG_OUT_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  DBG_OUT_Sleep() function was called, the DBG_OUT_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DBG_OUT_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DBG_OUT_Wakeup(void)
{
    DBG_OUT_RestoreConfig();
    #if( (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) )
        DBG_OUT_ClearRxBuffer();
    #endif /* End (DBG_OUT_RX_ENABLED) || (DBG_OUT_HD_ENABLED) */
    #if(DBG_OUT_TX_ENABLED || DBG_OUT_HD_ENABLED)
        DBG_OUT_ClearTxBuffer();
    #endif /* End DBG_OUT_TX_ENABLED || DBG_OUT_HD_ENABLED */

    if(DBG_OUT_backup.enableState != 0u)
    {
        DBG_OUT_Enable();
    }
}


/* [] END OF FILE */
