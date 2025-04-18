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
uint8 Estado_motor=0;
uint8 Boton_actual=0;
uint8 Boton_anterior=0;


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
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_Start();
    UART_Start();
   
    

    for(;;)
    {
        Lectura_Boton();
        Direccion();
        
    }
    return 0;
    
}

/* [] END OF FILE */
