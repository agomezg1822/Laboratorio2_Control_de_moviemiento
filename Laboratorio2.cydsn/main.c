/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "stdlib.h"

uint8 Estado_motor=0;
uint8 Boton_actual=0;
uint8 Boton_anterior=0;
uint8 pwm_value = 0;
char buffer_uart[5];


void Lectura_Boton(){
    Boton_actual = Button_Read();
    
    if (Boton_actual == 0){
        CyDelay(100);
        Boton_actual = Button_Read();
        
        if (Boton_anterior==1 && Boton_actual==0){
            Estado_motor++;
            if (Estado_motor >2){
                Estado_motor=0;
            }
        } 
        Boton_anterior= Boton_actual;
    }
}

void Lectura_Serial(char direccion){
    
    if (direccion == 'S'){//stop
        IN3_Write(0);
        IN4_Write(0);
    }
    if (direccion == 'I'){//Izquierda
        IN3_Write(1);
        IN4_Write(0);
    }
    if (direccion == 'D'){//Derecha
        IN3_Write(0);
        IN4_Write(1);  
    }
    direccion = '\0';
 
}

void Direccion(){
    
    if (Estado_motor == 0){//stop
        IN3_Write(0);
        IN4_Write(0);
    }
    if (Estado_motor == 1){//Izquierda
        IN3_Write(1);
        IN4_Write(0);
    }
    if (Estado_motor == 2){//Derecha
        IN3_Write(0);
        IN4_Write(1);
       
    }
}

void comunicacion_serial(){
    uint8 i=0;
    char caracteres;
    char sentido=0;
    if (UART_GetRxBufferSize()>0){
        while (1){
           caracteres = UART_GetChar();
           if (caracteres == '\n'){
                break;
            }
            if (caracteres != 0){
            
                buffer_uart[i++] = caracteres;
                
                if (i > 4){
                  break;
                }}
        buffer_uart[i] = '\0';  
        sentido = buffer_uart[0];
        pwm_value = atoi(&buffer_uart[1]);
        PWM_WriteCompare(pwm_value);
        //Lectura_Serial(sentido);
        }
       Lectura_Serial(sentido);
    }    
}

void velocidad (){


}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_Start();
    UART_Start();
    UART_PutString("hola esta es la prueba");
    //I2C_Start();
    //QuadDec_Start();
    Encoder_Start();
    Encoder_SetCounter(0);
    
 
    

    for(;;)
    {
        //Lectura_Boton();
        //Direccion();
        comunicacion_serial();
        
        
       
    }
    return 0;
    
}

/* [] END OF FILE */
