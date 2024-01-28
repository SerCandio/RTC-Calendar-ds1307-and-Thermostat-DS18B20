#include "ds18b20.h"

extern void ds18b20_Init(void){
    /*Initialize Bus on IDLE State: HIGH
     * Note:If analog pin, should be disabled first
     */
    DS18B20_PIN_DIR = 1; //configure DS18B20 DQ pin as input
    
    TMR2=0x00;                        //Reset Timer 2
    Timer2_Turn_On();                // enable Timer2 module
    
    while(TMR2<0x02);               //Wait at Least 2uS 
    
    Timer2_Turn_Off();          //disble Timer 2 module
}

extern bool ds18b20_Configure(ds18b20Config_t *pds18b20Config){
    /*Set : Temperature USER High Limit
     *      Temperature USER LOW Limit
     *      Resolution (9, 10,11 or 12)
     */
    
    __ds18b20_Start_Reset_Signal();
    
    if(!__ds18b20_Check_Response())     //Check if device is present....
    {
        __ds18b20_Write_byte(0xCC); //Send Skip ROM command
        __ds18b20_Write_byte(0x4E); //Send Write Scratchpad command
        
        /*The following 3 bytes writeen will be put on RAM locations from 2 t 4*/
        __ds18b20_Write_byte(pds18b20Config->TH);
        __ds18b20_Write_byte(pds18b20Config->TL);
        
        /*Consideration format: 0XY11111, where XY is the configuration value mask, upon Bit resolution*/
        __ds18b20_Write_byte( (uint8_t) (((pds18b20Config->CONFIG)<<5))  );
        
        __ds18b20_Start_Reset_Signal(); //Issue Reset
        
        /*Make Copy of Config Register, to use as index on conversion formula*/
        _fractConversionFactorIndex = pds18b20Config->CONFIG ;

        return false; // OK --> No errors
    
    }else
    {
        return true;    //Device has a problem with response
    }
    
}

extern bool ds18b20_Read_ROM(uint64_t *promCode){ 
    uint64_t romCode;
   __ds18b20_Start_Reset_Signal();
    
    if(!__ds18b20_Check_Response())     //Check if device is present....(TX presence pulse)
    {
        __ds18b20_Write_byte(0x33); //Send Read ROM command
        
        romCode =(uint64_t)  __ds18b20_Read_Byte(); //ds18b20 Tx family code (first byte LSB)
        
        for(uint8_t i=0; i<6; i++)      //ds18b20 Tx Serial Number 6 bytes
        {
            romCode|= ((uint64_t) __ds18b20_Read_Byte())<<(8 + 8*i);
        }
        
        romCode|=( (uint64_t) __ds18b20_Read_Byte())<<56;  //Read Last byte of ROM code: CRC
        
        *promCode = romCode;
  
        return false;  //OK -> No errors
        
    }else
    {
        return true;    //Device has a problem with response
    }
    
    
    
}

/*Send START or RESET signal to the sensor*/
static void __ds18b20_Start_Reset_Signal(void){
    DS18B20_PIN_DIR = 0; //configure DS18B20 DQ pin as output
    DS18B20_PIN_OUT = 0; // Clear Bus Master Active LOW
    
    __delay_us(480); //Master TX Reset Pulse 480 us 960 us max
 
    DS18B20_PIN_DIR = 1; //configure DS18B20 DQ pin as input
}

/*Check sensor "presence pulse" */
static bool __ds18b20_Check_Response(void){
  TMR2=0x00;                        //Reset Timer 2

  Timer2_Turn_On();                // enable Timer2 module
  
  /*Wait until DQ becomes LOW : Init of Response
   * ds18b20 waits 60 uS Max
   */
  while(DS18B20_PIN_INPUT && TMR2<0x2E);
  
  /* Wait until DQ pin becomes high : End of Response
   * Checking of 60 - 240 us µs LOW time response, we choose 240uS max timeout*/
  while(!DS18B20_PIN_INPUT && TMR2 < 0xB9); 
  
  if(TMR2>0xBA)                     // if response time > 240µS  ==> Response error
  {
     return true;                  // return 1 (Device has a problem with response) 
     
  }else
  {
      TMR2=0x00;                        //Reset Timer 2
      
      /*Master RX receive pulse should last at least 480 uS till last time Master TX released the bus
       * So we add the difference Time.... 180 uS, which is the time Bus line must remain HIGH
       * Note: See datasheet p. 14-18
       */
      while(TMR2<0x8B);
         
      return false;             // return 0 (response OK)
      
  }
   
}

static void __ds18b20_Write_bit(uint8_t value){
  DS18B20_PIN_DIR = 0;  // configure DS18B20_PIN pin as output
  DS18B20_PIN_OUT = 0;  //Pull DQ to LOW
  
  switch(value&0x01)
  {
      case 0:
          /*Master Write 0 Slot: Pull to LOW and remain 60uS*/
          TMR2=0x00;                        //Reset Timer 2
          while(TMR2<0x2E);
          
          /*Release Bus Line*/
          DS18B20_PIN_DIR = 1;  // configure DS18B20_PIN pin as input
          TMR2=0x00;                        //Reset Timer 2
          while(TMR2<0x02);               //Wait at Least 2uS 
          break;
          
      case 1:
          /*Master Write 1 Slot: Pull to LOW at least 1uS */
          TMR2=0x00;                        //Reset Timer 2
          while(TMR2<0x02);             //Wait at Least 2uS 
          
          /*Release Bus Line
           */
          DS18B20_PIN_DIR = 1;  // configure DS18B20_PIN pin as input
          
          /*Write Slot should last at least 60 uS, so we add 58 uS*/
          TMR2=0x00;                        //Reset Timer 2
          while(TMR2<0x2D);
          break;
  }
  

}

static void __ds18b20_Write_byte(uint8_t value){
    for(uint8_t i = 0; i < 8; i++)
    {
        __ds18b20_Write_bit(value >> i); 
    }
}

static bool __ds18b20_Read_bit(void){
    bool bitValue;
    
    DS18B20_PIN_DIR = 0; //configure DS18B20 DQ pin as output
    DS18B20_PIN_OUT = 0; // Clear Bus Master Active LOW
    TMR2=0x00;                        //Reset Timer 2
    while(TMR2<0x02);             //Wait at Least 2uS 
    
    /*Release Bus Line and sample line after 15 uS*/
    DS18B20_PIN_DIR = 1;  // configure DS18B20_PIN pin as input
    TMR2=0x00;                        //Reset Timer 2
    while(TMR2<0x0A);   
    
    bitValue = (bool) DS18B20_PIN_INPUT;
    
     /*Write Slot should last at least 60 uS, so we add 45 uS*/
    TMR2=0x00;                        //Reset Timer 2
    while(TMR2<0x23);
    
    /*Also add minimal pulse recovery of 2uS*/
    TMR2=0x00;                      
    while(TMR2<0x02);       

    return bitValue;

}

static  uint8_t __ds18b20_Read_Byte(void){
   uint8_t value = 0x00;
 
  for(uint8_t i = 0; i < 8; i++)
  {  
      if(__ds18b20_Read_bit()==true)
          value|=(1<<i);
      else
          value&=~(1<<i);
  }
   
  return value;
}

extern bool ds18b20_Read_Raw_Temperature(uint16_t *rawTempValue){
    *rawTempValue = 0;

    __ds18b20_Start_Reset_Signal();
    
    if(!__ds18b20_Check_Response())     //Check if device is present....
    {
        __ds18b20_Write_byte(0xCC); //Send Skip ROM command
        __ds18b20_Write_byte(0x44); //Send Temperature conversion command
        
        while(__ds18b20_Read_Byte()==0) ; //Wait for conversion to complete
        
        __ds18b20_Start_Reset_Signal();
        
        if(!__ds18b20_Check_Response()) //Check if device is present....
        {
            __ds18b20_Write_byte(0xCC); //Send Skip ROM command
            __ds18b20_Write_byte(0xBE); //Send Read Command
            
            // read temperature LSB byte and store it on raw_temp_value LSB byte
          *rawTempValue  = __ds18b20_Read_Byte();
          // read temperature MSB byte and store it on raw_temp_value MSB byte
          *rawTempValue |= (uint16_t)(__ds18b20_Read_Byte() << 8);
          
          __ds18b20_Start_Reset_Signal(); //Terminate Reading
          
          Timer2_Turn_Off();        // disable Timer2 module
          return false; // OK --> No errors
 
        }else
        {
          Timer2_Turn_Off();        // disable Timer2 module
          return true;    //Device has a problem with response  
        }
   
    }else
    {
        Timer2_Turn_Off();        // disable Timer2 module
        return true;    //Device has a problem with response
    }

}

extern bool ds18b20_Read_Real_Temperature(char *realTempStr, int8_t *integerTemp){ 
    bool signBit=false;
    char signTempStr[DS18B20_PRINT_BUFFER_SIZE+1];
    
    //Read _temperatureRaw
    if(!ds18b20_Read_Raw_Temperature(&_unsignedTemperatureRaw))
    {
        /*Evaluate if result is signed format*/
        if(_unsignedTemperatureRaw&0x8000)
        {
           signBit = true;
           _unsignedTemperatureRaw = (~_unsignedTemperatureRaw) +1;    
        }
        
        /*Get Integer part only*/
        *integerTemp = (int8_t) (_unsignedTemperatureRaw>>4);
 
        /*Print on variable*/
        sprintf(realTempStr,"%u.%u", _unsignedTemperatureRaw>>4,
                /*Get fraction digits: Mask fractional Part first
                 * Why 625?  because we're working with 12-bit resolution (default resolution)
                 * Same calculation formula can be considered for other resolution
                 * 
                 * Upon resolution, we will have many decimal places in fractional sttring as follows:
                 * 9 bits : 0.5°C   -> %u.%u
                 * 10 bits: 0.25 °C  -> %u.%u%u
                 * 11 bits: 0.125 °c   -> %u.%u%u%u
                 * 12 bits: 0.0625 °C  -> "%u.%u%u%u%u"
                 * 
                 * So you should change this accordindly
                 */
                ((_unsignedTemperatureRaw & 0x000F) * 625U ) /1000U
//                (((_unsignedTemperatureRaw & 0x000F) * 625U ) /100U)%10
//                (((_unsignedTemperatureRaw & 0x000F) * 625U ) /10U) %10 ,
//                (((_unsignedTemperatureRaw & 0x000F) * 625U ) /1U) %10
                );
        
         /*Add sign bit if it is the case*/
        if(signBit==true)
        {
          sprintf(signTempStr,"-%s",realTempStr);
          strcpy(realTempStr,signTempStr );
          
          *integerTemp = - (*integerTemp);
        }
         
        return false; // OK --> No response  errors
        
    }else
    {
        return true;    //Device has a problem with response
    }
}
