/*******************************************************************************
* File Name: Encoder.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
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

#if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
    #include "Encoder_PVT.h"
#endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */

uint8 Encoder_initVar = 0u;


/*******************************************************************************
* Function Name: Encoder_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Encoder_Init(void) 
{
    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(Encoder_ISR_NUMBER);
        /* Set the ISR to point to the Encoder_isr Interrupt. */
        (void) CyIntSetVector(Encoder_ISR_NUMBER, & Encoder_ISR);
        /* Set the priority. */
        CyIntSetPriority(Encoder_ISR_NUMBER, Encoder_ISR_PRIORITY);
    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Encoder_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Encoder_Enable(void) 
{
    uint8 enableInterrupts;

    Encoder_SetInterruptMask(Encoder_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) Encoder_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    Encoder_SR_AUX_CONTROL |= Encoder_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(Encoder_ISR_NUMBER);
    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Encoder_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Encoder_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Encoder_Start(void) 
{
    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
        Encoder_Cnt8_Start();
        Encoder_Cnt8_WriteCounter(Encoder_COUNTER_INIT_VALUE);
    #else
        /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) || 
        *  (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT) 
        */
        Encoder_Cnt16_Start();
        Encoder_Cnt16_WriteCounter(Encoder_COUNTER_INIT_VALUE);
    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT */
    
    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)        
       Encoder_count32SoftPart = 0;
    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */

    if (Encoder_initVar == 0u)
    {
        Encoder_Init();
        Encoder_initVar = 1u;
    }

    Encoder_Enable();
}


/*******************************************************************************
* Function Name: Encoder_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Encoder_Stop(void) 
{
    uint8 enableInterrupts;

    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
        Encoder_Cnt8_Stop();
    #else 
        /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) ||
        *  (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
        */
        Encoder_Cnt16_Stop();    /* counter disable */
    #endif /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    Encoder_SR_AUX_CONTROL &= (uint8) (~Encoder_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
        CyIntDisable(Encoder_ISR_NUMBER);    /* interrupt disable */
    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Encoder_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  Encoder_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int8 Encoder_GetCounter(void) 
{
    int8 count;
    uint8 tmpCnt;

    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(Encoder_ISR_NUMBER);

        tmpCnt = Encoder_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) Encoder_COUNTER_INIT_VALUE);
        count = Encoder_count32SoftPart + hwCount;

        CyIntEnable(Encoder_ISR_NUMBER);
    #else 
        /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) || 
        *  (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT)
        */
        #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
            tmpCnt = Encoder_Cnt8_ReadCounter();
        #else /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) */
            tmpCnt = Encoder_Cnt16_ReadCounter();
        #endif  /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT */

        count = (int8) ((int16) tmpCnt -
                (int16) Encoder_COUNTER_INIT_VALUE);

    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: Encoder_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  Encoder_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Encoder_SetCounter(int8 value) 
{
    #if ((Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) || \
         (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT))
        uint8 count;
        
        if (value >= 0)
        {
            count = (uint8) value + Encoder_COUNTER_INIT_VALUE;
        }
        else
        {
            count = Encoder_COUNTER_INIT_VALUE - (uint8)(-value);
        }
        #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
            Encoder_Cnt8_WriteCounter(count);
        #else /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) */
            Encoder_Cnt16_WriteCounter(count);
        #endif  /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT */
    #else /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT) */
        CyIntDisable(Encoder_ISR_NUMBER);

        Encoder_Cnt16_WriteCounter(Encoder_COUNTER_INIT_VALUE);
        Encoder_count32SoftPart = value;

        CyIntEnable(Encoder_ISR_NUMBER);
    #endif  /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT) ||
             * (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: Encoder_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 Encoder_GetEvents(void) 
{
    return (Encoder_STATUS_REG & Encoder_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: Encoder_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void Encoder_SetInterruptMask(uint8 mask) 
{
    #if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (Encoder_COUNTER_OVERFLOW | Encoder_COUNTER_UNDERFLOW |
                 Encoder_COUNTER_RESET);
    #endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */

    Encoder_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: Encoder_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 Encoder_GetInterruptMask(void) 
{
    return (Encoder_STATUS_MASK & Encoder_INIT_INT_MASK);
}


/* [] END OF FILE */
