#include "INTERRUPT_Controller.h"


extern void INTERRUUPT_Init(void){
    // Deshabilitar/ Habilitar prioridades
    RCONbits.IPEN=1; 
    
    //Fuente de Interrucpcion : INT2, Falling Endge , Low Priority (Baja Prioridad)
    INTCON2bits.INTEDG2 = 0; //Interrupcion en FALLING EDGE 
    INTCON3bits.INT2IF =0 ; //Limpiar Bandera de interrupcion
    INTCON3bits.INT2IP = 0; //INT2 a Baja Priridad
    INTCON3bits.INT2IE =1 ; //Habilita INT2
    
    //Fuente Interrupcion Timer 1 overflow
    TMR1_Enable_Interrupt();
    
    //Fuente de Interrupcion por recepcion RX habilitada
    //EUSART_RxInterruptEnable();
     
    //Fuente Interrupcion USB :LOW Priority
    //Ver "usb_hal_pic18.h", IPR3bits.USBIP = 1, se modifico a IPR3bits.USBIP = 0
        
    //Habilitacion general interrupciones
    INTCONbits.GIEH=1;          // Habilita alta prioridad
    INTCONbits.GIEL=1;          // Habilita Baja prioridad  
}


extern void INTERRUPT_Global_Enable_HighPriority(void){
   INTCONbits.GIEH=1;          // Habilita alta prioridad  (
}

extern void INTERRUPT_Global_Disable_HigPriority(void){
   INTCONbits.GIEH=0;          // Deshabilita alta prioridad  ( 
}

extern void INTERRUPT_Global_Enable_LowPriority(void){
   INTCONbits.GIEL=1;          // Habilita Baja prioridad  
}

extern void INTERRUPT_Global_Disable_LowPriority(void){
   INTCONbits.GIEL=0;          // Deshabilita Baja prioridad  
}

/*
 *  ISRs....
 */
void __interrupt(high_priority) ISR_High_Priority(void){
    
  /* 
   * Ocurrio la interrupcion por Timer 1 ?
   */  
  if ( (PIE1bits.TMR1IE==1) && ( PIR1bits.TMR1IF==1) ){
      
      TMR1_ISR();

      //Other.....
      MillisISR();
   
  }
    
}

void __interrupt(low_priority) ISR_Low_Priority(void){
    
    // interrupt handler for USB CDC..............
    if(PIE3bits.USBIE == 1 && PIR3bits.USBIF == 1)
    {
        /*Descomentar solo si se va a trabajr con USB CDC...*/
        //USB_USBDeviceTasks();
    } 
    else
    {
        //Unhandled Interrupt
    }
    
    if (INTCON3bits.INT2IE==1 && INTCON3bits.INT2IF==1)
    {
        INTCON3bits.INT2IF = 0 ; //Clear INT2 flag
        DS1307_ISR();           //Set Software flag
    }
 
  
}
