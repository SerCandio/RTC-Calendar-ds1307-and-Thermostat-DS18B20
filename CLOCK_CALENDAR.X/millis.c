#include "millis.h"


extern void Millis_Init(bool highPriority){
    //Configure Timer
    Timer_Init(highPriority);
}

/* 
 * Get actual microseconds
 */
extern millis_t Millis_Get(void){
    millis_t m;
    
    INTCONbits.GIEH=0;          
    INTCONbits.GIEL=0;
    
    m = milliseconds; 
    
    INTCONbits.GIEH=1;          
    INTCONbits.GIEL=1;
  
    return m;
}


/*Trun ON Timer and continue counting....*/
static void Millis_Resume(void){
    Power_TimerEnable();
}

/*Turn Off Timer*/
static void Millis_Pause(void){
    Power_TimerDisable();
}

/*Clear ms counter var*/
static void Millis_Reset(void){
   milliseconds=0;
}

/*
 *  ISR
 */
extern void MillisISR(void){
    milliseconds++;
}
