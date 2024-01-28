
#include "EUSART.h"


/*
 * Desarrollo de funciones
 */

extern void EUSART_SerialBegin(uint32_t baudios){
    uint16_t n=0;
    
    //Confgiurar los pines RX y TX como digitales
    ANSELCbits.ANSC6=0; // no analogico
    ANSELCbits.ANSC7=0;
    TRISCbits.TRISC6=0; // TX -> Salida Digital
    TRISCbits.TRISC7=1; // RX -> Entrada Digital
    
    //Configurar alta velocidad
    BAUDCON1=0b00001000; // ...solo BRG16 a 1
    
    //Configirar los baudios.....n es de 16 bit porque los registros SPBRGH:SPBRGL  16 bit en total
    n = (uint16_t) (( (_XTAL_FREQ /  baudios ) / 4UL ) - 1UL);
    SPBRG1=(uint8_t) (n & 0xFF ) ;
    SPBRGH1= (uint8_t) (n >> 8) ;
    
    //Configurar la transmision
    TXSTA1=0b00100100;;
    
    //Configirar la recepcion
    RCSTA1=0b10010000;
}

extern void EUSART_WriteChar(char data){
    
    while(TXSTA1bits.TRMT==0); // Esperar que se transmita el dato
    TXREG1=data; // Cargo el dato a transmitir
}

extern void EUSART_WriteString(char *ptrString){
    
    while(*ptrString !='\0')
    {
        EUSART_WriteChar(*ptrString);
        *ptrString++;
    }
}

extern bool EUSART_Rx_Ready(void){
    // Indica si hay byte en el registro RCREG1
    return (bool) PIR1bits.RCIF;
  
}

extern char EUSART_ReadChar(void){
    char dataError;
    
    //while(PIR1bits.RCIF ==0); // Esperar que se reciba un dato (bloqueante !!)
    
    //Comprobar error de trama FERR  (no se recibio el bit STOP)
    if(RCSTA1bits.FERR ==1){
        //Leer el dato erroneo
        dataError=RCREG1;
        (void) dataError;
    }
    
    //Comprobar error de desbordamiento  (no se leyeron datos)
    if(RCSTA1bits.OERR==1){
        //Clear CREN 
        RCSTA1bits.CREN=0;
        NOP();              //Retardo de 1 ciclo maquina o 4 ciclos de reloj
        NOP();
        RCSTA1bits.CREN=1;
    }
   
    //Comprobar si se recibio el dato..........Auto limpia flag RCIF
    return (RCREG1);
 
}

extern void EUSART_AttachISR(void (*funISR)(void)){
    pEusartFunISR = funISR;
}

extern void EUSART_RxInterruptEnable(void){
    PIE1bits.RCIE=1;    //Habilita interrupcion por recepcion
    IPR1bits.RCIP=0  ; // Recepcion de Baja prioridad
}

extern void EUSART_RxInterruptDisable(void){
    PIE1bits.RCIE=0;
}

extern void EUSART_ISR(void){
    //Ejecutar puntero a funcion por recepcion RX
    (*pEusartFunISR)();
}

void putch(char c){
    while(TXSTA1bits.TRMT==0); // Esperar que se transmita el dato
    TXREG1=c; // Cargo el dato a transmitir
}


