#include "TMR1.h"


extern void TMR1_Initialize(bool highPriority){
    _highPriority=highPriority;
    
    T1CONbits.TMR1ON=0; // Stops Timer 1
    
    /*En el modo RTC(32.768Khz), los pines RC0 y RC1 no estan disponibles
     */
    T1CONbits.SOSCEN=TMR1_SOSCEN; //See "TMR1.h"
    
#if defined(TMR1_SIMULATION_MODE) && defined(TMR1_RTC)
    //Pin RC0 :: T1CKI como entrada (Fuente 32.768Khz), el crital no es simulable
   TRISCbits.TRISC0=1;
#endif

   T1CONbits.TMR1CS=TMR1_CS; //See "TMR1.h"
   T1CONbits.T1CKPS=TMR1_CKPS; //See "TMR1.h"
   T1CONbits.T1SYNC=0; //Syncronizar reloj externo con el system clock
   
   /* 
    * Valor de Temporizacion, ver "TMR1.h"
    */
   TMR1H =RELOAD_TMR1H;  
   TMR1L = RELOAD_TMR1L;

   T1CONbits.TMR1ON=1; // Enable Timer 1
   T1GCONbits.TMR1GE=0; // Timer 1 cuenta independiente del modulo gate...always on (pagina 151)
}

extern void TMR1_Clear_Flag_Ovf(void){
   PIR1bits.TMR1IF=0; //Clear Timer 1 Overflow Flag
}

extern void TMR1_Turn_On(void){
    T1CONbits.TMR1ON=1; // Enable Timer 1
}

extern void TMR1_Turn_Off(void){
    T1CONbits.TMR1ON=0; // Disable Timer 1
}

extern void TMR1_Enable_Interrupt(void){
    //Fuente de interrupcion Timer 1 overflow
    PIE1bits.TMR1IE=1;  //Timer 1 interrupt enable
    PIR1bits.TMR1IF=0;  // Limpiar interrupt flag
   
    if(_highPriority==true){
        IPR1bits.TMR1IP=1; //Establecer prioridad: HIGH PRIORITY
    }else{
        IPR1bits.TMR1IP=0; //Establecer prioridad: LOW PRIORITY
    }
}

extern void TMR1_Disable_Interrupt(void){
    //Fuente de interrupcion Timer 1 overflow Deshabilitada
    PIE1bits.TMR1IE=0;  //Timer 1 interrupt Disable
    PIR1bits.TMR1IF=0;  // Limpiar interrupt flag
}

extern void TMR1_ISR(void){
     //Reload  Timer
    TMR1H=RELOAD_TMR1H; 
    TMR1L=RELOAD_TMR1L;
    
    //Clear overflow flag
    TMR1_Clear_Flag_Ovf();
    
}

