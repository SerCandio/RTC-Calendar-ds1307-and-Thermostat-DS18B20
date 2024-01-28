#include "EEPROM_24C32A.h"


extern bool EERPOM_Write_byte(uint8_t eepromSlaveADDR, uint16_t regMapADDR, uint8_t dataByte){
    
    if(regMapADDR>=EEPROM_24C32_REG_MAP_SIZE){
        return true ;       //Fcn error
    }
    
    /*Iniciar bus I2C: START*/
    I2C_StartMaster();
    
    /* Enviar el slave ADDR , modo WR / RD + ACK
     */
    if(I2C_WriteMaster(eepromSlaveADDR)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Enviar el data pointer o el data word de 16 bits  + ACK*/
    if(I2C_WriteWordMaster(EEPROM_24C32_REG_MAP_BASE_ADDR + regMapADDR)==true){
       return true ;   //I2C Bus ERROR !!!
    }
    
     /*Enviar el data pointer o el data byte + ACK*/
    if(I2C_WriteMaster(dataByte)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Iniciar bus I2C: STOP*/
    I2C_StopMaster();
    
    __delay_ms(5);  //Max Write Cycle delay
    
    return false;  //No errros !!!
}

extern bool EEPROM_Write_Page(uint8_t eepromSlaveADDR, uint16_t regMapADDR, char *pString){
    
    /*
     * Accordng to datasheet, 24C32 only supports 8 byte wirte page
     */
    
    uint8_t numPages=EEPROM_24C32_MAX_PAGE_SIZE;
    
    char *pstring=NULL;
    
    pstring = pString;
     
    /*Iniciar bus I2C: START*/
    I2C_StartMaster();
    
    /* Enviar el slave ADDR , modo WR / RD + ACK
     */
    if(I2C_WriteMaster(eepromSlaveADDR)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Enviar el data pointer o el data word de 16 bits  + ACK*/
    if(I2C_WriteWordMaster(EEPROM_24C32_REG_MAP_BASE_ADDR + regMapADDR)==true){
       return true ;   //I2C Bus ERROR !!!
    }
    
    /*Enviar cada caracter de la cadena hasta hallar el caracter nulo*/
    while(numPages){
        if(I2C_WriteMaster(*pstring++)==true){
            return true ;   //I2C Bus ERROR !!!
        }  
        
        numPages--;
    }
    
     /*Iniciar bus I2C: STOP*/
    I2C_StopMaster();
    
    __delay_ms(5);  //Max Write Cycle delay , see datasheet p.1 
    
    return false;  //No errros !!!
    
}

extern bool EEPROM_Flush(uint8_t eepromSlaveADDR){
    
    /*Iniciar bus I2C: START*/
    I2C_StartMaster();
    
    /* Enviar el slave ADDR , modo WR / RD + ACK
     */
    if(I2C_WriteMaster(eepromSlaveADDR)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Enviar el data pointer o el data word de 16 bits  + ACK*/
    if(I2C_WriteWordMaster(EEPROM_24C32_REG_MAP_BASE_ADDR)==true){
       return true ;   //I2C Bus ERROR !!!
    }
    
    for(unsigned int i=0 ; i<EEPROM_24C32_REG_MAP_SIZE;i++){  //From Addr 0x0000 to 4Kb.....see datasheet
         /*Enviar el data pointer o el data byte + ACK*/
        if(I2C_WriteMaster(0xFF)==true){
            return true ;   //I2C Bus ERROR !!!
        }
    }
    
     /*Iniciar bus I2C: STOP*/
    I2C_StopMaster();
    
    __delay_ms(5);  //Max Write Cycle delay , see datasheet p.1 
    
    return false;  //No errros !!!
    
}

extern bool EEPROM_Random_Read(uint8_t eepromRDSlaveADDR, uint16_t regMapADDR, uint8_t *pdataByteRead){
    
    if(regMapADDR>=EEPROM_24C32_REG_MAP_SIZE){
        return true ;       //Fcn error
    }
    
    /*Iniciar bus I2C: START*/
    I2C_StartMaster();
    
    /* Enviar el slave ADDR , modo WR(R/W bit =0) / RD + ACK
     */
    //Enviar el slave ADDR en modo WR 
    if(I2C_WriteMaster(eepromRDSlaveADDR - 1U)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Enviar el data pointer o el data word ADDR de 16 bits desde donde leer + ACK*/
    if(I2C_WriteWordMaster(EEPROM_24C32_REG_MAP_BASE_ADDR + regMapADDR)==true){
       return true ;   //I2C Bus ERROR !!!
    }
    
    /*Inicio Repetido bus I2C: RE-START*/
    I2C_ReStartMaster();
    
    //Enviar el slave ADDR en modo RD (R/W bit =1)
    if(I2C_WriteMaster(eepromRDSlaveADDR)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Leer el byte de dato + send NACK*/
    *pdataByteRead = I2C_ReadMaster(SEND_NACK);
    
     /*Iniciar bus I2C: STOP*/
    I2C_StopMaster();
    
    return false;  //No errros !!!
    
}

extern bool EEPROM_Sequential_Read(uint8_t eepromRDSlaveADDR, uint16_t regMapADDR, unsigned int numDataBytes, uint8_t *pdataByteList){
   
    /*Entire contents of memoery can be read*/
    if( (regMapADDR>=EEPROM_24C32_REG_MAP_SIZE) ){
        return true ;       //Fcn error
   }
    
    /*Iniciar bus I2C: START*/
    I2C_StartMaster();
    
    /* Enviar el slave ADDR , modo WR / RD + ACK
     */
    //Enviar el slave ADDR en modo WR (R/W bit=0)
    if(I2C_WriteMaster(eepromRDSlaveADDR -1U)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Enviar el data pointer o el data word de 16 bits  + ACK*/
    if(I2C_WriteWordMaster(EEPROM_24C32_REG_MAP_BASE_ADDR + regMapADDR)==true){
       return true ;   //I2C Bus ERROR !!!
    }
    
    /*Inicio Repetido bus I2C: RE-START*/
    I2C_ReStartMaster();
    
    //Enviar el slave ADDR en modo RD (R/W bit =1)
    if(I2C_WriteMaster(eepromRDSlaveADDR)==true){
        return true ;   //I2C Bus ERROR !!!
    }
    
    /*Leer el byte de dato + send ACK
     *                       send NACK en el ultimo byte
     */
    for(unsigned int i=0 ;i<numDataBytes; i++){ 
        /*Se leyo el ultimo byte ? */
        if(i==(numDataBytes - 1)){
            *pdataByteList++= I2C_ReadMaster(SEND_NACK);
            break;
        }else{
            *pdataByteList++= I2C_ReadMaster(SEND_ACK);
        }
    }
    
    /*Iniciar bus I2C: STOP*/
    I2C_StopMaster();
    
    return false;  //No errros !!!
}
