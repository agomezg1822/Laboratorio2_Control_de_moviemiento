/*******************************************************************************
* File Name: Encoder_Cnt8_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Encoder_Cnt8.h"

static Encoder_Cnt8_backupStruct Encoder_Cnt8_backup;


/*******************************************************************************
* Function Name: Encoder_Cnt8_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Encoder_Cnt8_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Encoder_Cnt8_SaveConfig(void) 
{
    #if (!Encoder_Cnt8_UsingFixedFunction)

        Encoder_Cnt8_backup.CounterUdb = Encoder_Cnt8_ReadCounter();

        #if(!Encoder_Cnt8_ControlRegRemoved)
            Encoder_Cnt8_backup.CounterControlRegister = Encoder_Cnt8_ReadControlRegister();
        #endif /* (!Encoder_Cnt8_ControlRegRemoved) */

    #endif /* (!Encoder_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_Cnt8_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Encoder_Cnt8_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_Cnt8_RestoreConfig(void) 
{      
    #if (!Encoder_Cnt8_UsingFixedFunction)

       Encoder_Cnt8_WriteCounter(Encoder_Cnt8_backup.CounterUdb);

        #if(!Encoder_Cnt8_ControlRegRemoved)
            Encoder_Cnt8_WriteControlRegister(Encoder_Cnt8_backup.CounterControlRegister);
        #endif /* (!Encoder_Cnt8_ControlRegRemoved) */

    #endif /* (!Encoder_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Encoder_Cnt8_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Encoder_Cnt8_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Encoder_Cnt8_Sleep(void) 
{
    #if(!Encoder_Cnt8_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Encoder_Cnt8_CTRL_ENABLE == (Encoder_Cnt8_CONTROL & Encoder_Cnt8_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Encoder_Cnt8_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Encoder_Cnt8_backup.CounterEnableState = 0u;
        }
    #else
        Encoder_Cnt8_backup.CounterEnableState = 1u;
        if(Encoder_Cnt8_backup.CounterEnableState != 0u)
        {
            Encoder_Cnt8_backup.CounterEnableState = 0u;
        }
    #endif /* (!Encoder_Cnt8_ControlRegRemoved) */
    
    Encoder_Cnt8_Stop();
    Encoder_Cnt8_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_Cnt8_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Encoder_Cnt8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Encoder_Cnt8_Wakeup(void) 
{
    Encoder_Cnt8_RestoreConfig();
    #if(!Encoder_Cnt8_ControlRegRemoved)
        if(Encoder_Cnt8_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Encoder_Cnt8_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Encoder_Cnt8_ControlRegRemoved) */
    
}


/* [] END OF FILE */
