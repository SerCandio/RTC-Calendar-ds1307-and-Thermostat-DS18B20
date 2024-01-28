#include "TMR2.h"

extern void Timer2_Initialize(void){
    /* 
     * La fuente de reloj del Timer 2 es de FOSC/4 ; si FOSC=48Mhz, FOSC/4 =12Mhz
     *                                              Si PRE1:16, Timer 2 12Mhz/16 = 0.75 Mhz  -->> cada 1.3uS de incremento
     */
    T2CONbits.TMR2ON=0; //Apagar Timer 2
    T2CONbits.T2CKPS=0b10; //PRE 1:16
    T2CONbits.T2OUTPS=0b0000; //PORT 1:1 (Default)
    
    //Reset Timer 2
    TMR2=0x00;
    
    //T2CONbits.TMR2ON=1; //Timer 2 on
}

extern void Timer2_Turn_On(void){
    T2CONbits.TMR2ON=1;        // enable Timer2 module
}

extern void Timer2_Turn_Off(void){
    T2CONbits.TMR2ON=0;        // disable Timer2 module
}
