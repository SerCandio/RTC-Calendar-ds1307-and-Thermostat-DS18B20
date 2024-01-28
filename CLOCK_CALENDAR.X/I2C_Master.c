#include "I2C_Master.h"

extern void I2C_BeginMaster(uint32_t bitRateHz){
    SSP1CON1bits.SSPEN=0; //Disables serial port on SDA , SCL lines
    
    //Deshabilitar analogico pines RB0 y RB1
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    
    //Configurar los pines SDA y SCL como entradas 
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB1=1;
    
    //Configurar la velocidad y habilitar puerto I2C
    SSP1STATbits.SMP=1; // Standard speed mode (100 Khz - 1MHz)
    SSP1STATbits.CKE=0; // Disable SMBus specific Inputs
    SSP1CON1bits.SSPM=0b1000 ; // I2C Master Clock , FCLK= FOSC / 4*(SSPADD+1))
   
    //Configurar el bit -rate en el registro del clock divider SSP1ADD(MSSP ADDRESS AND BAUDRATE REG)
    /*  
     * Si XTAL=16Mhz y el bit rate es de 100KHz, el valor de n=39
     */
    SSP1ADD=(uint8_t) ( (_XTAL_FREQ / bitRateHz ) / 4UL ) -1UL ; 
    
    //Todas las banderas a cero  ...ACKSTAT, ACKDT(Master Receive mode only) , RCEN, PEN, RESEN, SEN
    SSP1CON2=0x00;
    
    SSP1CON1bits.SSPEN=1; //Enables serial port on SDA , SCL lines
}

extern void I2C_StartMaster(void){
    SSP1CON2bits.SEN=1; //Initiate START condition on SDA, SCL pins
    while(PIR1bits.SSPIF==0); // Completion of START bit?
    PIR1bits.SSPIF=0; // Clear SSPIF flag
}

extern void I2C_StopMaster(void){
    SSP1CON2bits.PEN=1; //Generate Stop condition on SDA,SCL lines
    while(PIR1bits.SSPIF==0); // Completion of STOP bit?
    PIR1bits.SSPIF=0; // Clear SSPIF flag
}

extern void I2C_ReStartMaster(void){
    SSP1CON2bits.RSEN=1; //Initiate RESTART condition
    while(PIR1bits.SSPIF==0); // Completion of START bit?
    PIR1bits.SSPIF=0; // Clear SSPIF flag
}

extern bool I2C_WriteMaster(uint8_t data){
    bool ackStatus;
   
    SSP1BUF=data;
    
    while(PIR1bits.SSPIF==0) ; // Transmision complete?
    PIR1bits.SSPIF=0; // Clear SSPIF flag
    
    ackStatus=(bool) (SSP1CON2bits.ACKSTAT);
    SSP1CON2bits.ACKSTAT=0;         //Clears ACK flag (From Slave)
    
    return ackStatus; // 0  :ACK OK 
                      // 1  : ACK Failed
}

extern bool I2C_WriteWordMaster(uint16_t data){
    /*Transmit the first MSB byte og the 16-bit word*/
    if(I2C_WriteMaster( (uint8_t) ( (data>>8) & 0x000F ) )==true){
        return true; //O : Write OK
                      //1 : Wrtie Fail
    }
    
     /*Transmit the next LSB byte og the 16-bit word*/
    if(I2C_WriteMaster((uint8_t) (data& 0x00FF))==true){
        return true; //O : Write OK
                      //1 : Wrtie Fail
    }
    
    return false; //No Erros!!
 
}

extern uint8_t I2C_ReadMaster(bool sendACK){
    SSP1CON2bits.RCEN=1; //Enables reception
    while(PIR1bits.SSPIF==0) ; // Reception complete?
    PIR1bits.SSPIF=0; //Clears SSPIF Flag
    SSP1STATbits.BF=0; //Clears BF Flag
    
    if(sendACK==true){
        I2C_SendACK_FromMaster();
    }else{
        I2C_SendNotACK_FromMaster();
    }
   
    return (uint8_t) SSP1BUF ;
}

static void I2C_SendACK_FromMaster(void){
    SSP1CON2bits.ACKDT=0; // ACK data bit
    SSP1CON2bits.ACKEN=1; //Enable ACK sequence from Master
    while(PIR1bits.SSPIF==0) ; 
    PIR1bits.SSPIF=0; //Clears SSPIF Flag
}

static void I2C_SendNotACK_FromMaster(void){
    SSP1CON2bits.ACKDT=1; //  NOT ACK data bit
    SSP1CON2bits.ACKEN=1; //Enable ACK sequence from Master
    while(PIR1bits.SSPIF==0) ; 
    PIR1bits.SSPIF=0; //Clears SSPIF Flag
}
