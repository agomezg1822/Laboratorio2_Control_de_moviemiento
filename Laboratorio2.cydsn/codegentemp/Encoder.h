/*******************************************************************************
* File Name: Encoder.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
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

#if !defined(CY_QUADRATURE_DECODER_Encoder_H)
#define CY_QUADRATURE_DECODER_Encoder_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define Encoder_COUNTER_SIZE               (8u)
#define Encoder_COUNTER_SIZE_8_BIT         (8u)
#define Encoder_COUNTER_SIZE_16_BIT        (16u)
#define Encoder_COUNTER_SIZE_32_BIT        (32u)

#if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
    #include "Encoder_Cnt8.h"
#else 
    /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) || 
    *  (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT) 
    */
    #include "Encoder_Cnt16.h"
#endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT */

extern uint8 Encoder_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define Encoder_COUNTER_RESOLUTION         (1u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} Encoder_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  Encoder_Init(void) ;
void  Encoder_Start(void) ;
void  Encoder_Stop(void) ;
void  Encoder_Enable(void) ;
uint8 Encoder_GetEvents(void) ;
void  Encoder_SetInterruptMask(uint8 mask) ;
uint8 Encoder_GetInterruptMask(void) ;
int8 Encoder_GetCounter(void) ;
void  Encoder_SetCounter(int8 value)
;
void  Encoder_Sleep(void) ;
void  Encoder_Wakeup(void) ;
void  Encoder_SaveConfig(void) ;
void  Encoder_RestoreConfig(void) ;

#if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(Encoder_ISR);
#endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
    #define Encoder_ISR_NUMBER             ((uint8) Encoder_isr__INTC_NUMBER)
    #define Encoder_ISR_PRIORITY           ((uint8) Encoder_isr__INTC_PRIOR_NUM)
#endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Encoder_GLITCH_FILTERING           (1u)
#define Encoder_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT)
    #define Encoder_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_16_BIT) ||
    *  (Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_32_BIT)
    */
    #define Encoder_COUNTER_INIT_VALUE    (0x8000u)
    #define Encoder_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* Encoder_COUNTER_SIZE == Encoder_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define Encoder_STATUS_REG                 (* (reg8 *) Encoder_bQuadDec_Stsreg__STATUS_REG)
#define Encoder_STATUS_PTR                 (  (reg8 *) Encoder_bQuadDec_Stsreg__STATUS_REG)
#define Encoder_STATUS_MASK                (* (reg8 *) Encoder_bQuadDec_Stsreg__MASK_REG)
#define Encoder_STATUS_MASK_PTR            (  (reg8 *) Encoder_bQuadDec_Stsreg__MASK_REG)
#define Encoder_SR_AUX_CONTROL             (* (reg8 *) Encoder_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define Encoder_SR_AUX_CONTROL_PTR         (  (reg8 *) Encoder_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define Encoder_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define Encoder_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define Encoder_COUNTER_RESET_SHIFT        (0x02u)
#define Encoder_INVALID_IN_SHIFT           (0x03u)
#define Encoder_COUNTER_OVERFLOW           ((uint8) (0x01u << Encoder_COUNTER_OVERFLOW_SHIFT))
#define Encoder_COUNTER_UNDERFLOW          ((uint8) (0x01u << Encoder_COUNTER_UNDERFLOW_SHIFT))
#define Encoder_COUNTER_RESET              ((uint8) (0x01u << Encoder_COUNTER_RESET_SHIFT))
#define Encoder_INVALID_IN                 ((uint8) (0x01u << Encoder_INVALID_IN_SHIFT))

#define Encoder_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define Encoder_INTERRUPTS_ENABLE          ((uint8)(0x01u << Encoder_INTERRUPTS_ENABLE_SHIFT))
#define Encoder_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define Encoder_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_Encoder_H */


/* [] END OF FILE */
