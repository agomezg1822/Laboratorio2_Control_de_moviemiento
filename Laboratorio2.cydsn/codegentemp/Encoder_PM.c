/*******************************************************************************
* File Name: Encoder_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Encoder.h"

static Encoder_BACKUP_STRUCT Encoder_backup = {0u};


/*******************************************************************************
* Function Name: Encoder_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Encoder_SaveConfig(void) 
{
    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
        Encoder_Cnt8_SaveConfig();
    #else 
        /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) || 
         * (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
         */
        Encoder_Cnt16_SaveConfig();
    #endif /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Encoder_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Encoder_RestoreConfig(void) 
{
    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
        Encoder_Cnt8_RestoreConfig();
    #else 
        /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) || 
         * (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT) 
         */
        Encoder_Cnt16_RestoreConfig();
    #endif /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Encoder_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Encoder_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Encoder_Sleep(void) 
{
    if (0u != (Encoder_SR_AUX_CONTROL & Encoder_INTERRUPTS_ENABLE))
    {
        Encoder_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        Encoder_backup.enableState = 0u;
    }

    Encoder_Stop();
    Encoder_SaveConfig();
}


/*******************************************************************************
* Function Name: Encoder_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Encoder_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void Encoder_Wakeup(void) 
{
    Encoder_RestoreConfig();

    if (Encoder_backup.enableState != 0u)
    {
        #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
            Encoder_Cnt8_Enable();
        #else 
            /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) || 
            *  (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT) 
            */
            Encoder_Cnt16_Enable();
        #endif /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        Encoder_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

