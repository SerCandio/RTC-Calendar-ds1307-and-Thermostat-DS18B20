#include "DS1307.h"

extern bool DS1307_Conf_Init(rtcDs1307Control_t  *prtcDs1307Control){
     memcpy(&rtcDs1307CtrlBuffer, prtcDs1307Control, sizeof(*prtcDs1307Control) ); //Pre-Build Control Register -datagram before send to I2C
    
     /*Iniciar Bus I2C : START*/
    I2C_StartMaster();

    /*Enviar la direccion del esclavo I2C en modo WRITE*/
    if(I2C_WriteMaster(RTC_DS1307_SLAVE_ADDR_WR_MODE) ==true){
         return true;    //Fallo bus I2C
    }

    /*Enviar el valor del Register Pointer al cual se escribira*/
    if(I2C_WriteMaster(RTC_D1307_REG_MAP_CONTROL) ==true){
         return true;    //Fallo bus I2C
    }
    
    /*Enviar el byte de control al registro de configuracion, leer su ACK del esclavo
     * Register Pointer se auto- incrementa..
     */
     if(I2C_WriteMaster(rtcDs1307CtrlBuffer) ==true){
         return true;    //Fallo bus I2C
     }

    /*Detener bus I2C : STOP*/
    I2C_StopMaster();
}

extern bool DS1307_Set_TimeDate(rtcDs1307UserData_t *prtcDs1307UserData){
    
   __DS1307_Build_To_TimeDate(&rtcDs1307TimeData, prtcDs1307UserData); //Pre-Builts Time-data frame structure from user Data
   
   memcpy(&rtcDs1307Buffer, &rtcDs1307TimeData, sizeof(rtcDs1307TimeData) ); //Pre-Build TimeKeeper Registers (7) -datagram before send to I2C
   
    /*Iniciar Bus I2C : START*/
    I2C_StartMaster();

    /*Enviar la direccion del esclavo I2C en modo WRITE*/
    if(I2C_WriteMaster(RTC_DS1307_SLAVE_ADDR_WR_MODE) ==true){
         return true;    //Fallo bus I2C
    }

    /*Enviar el valor del Register Pointer al cual se escribira*/
    if(I2C_WriteMaster(RTC_DS1307_REG_MAP_BASE_ADDR) ==true){
         return true;    //Fallo bus I2C
    }

    /*Enviar los bytes de data, cada byte enviado con su recpectivo ACK leido desde el esclavo*/
    /*El 'Register Pointer' incrementa auto*/
    for(int i=0 ; i<sizeof(rtcDs1307Buffer) ;i++){
        if(I2C_WriteMaster(rtcDs1307Buffer[i]) ==true){
             return true;    //Fallo bus I2C
        }
    }

    /*Detener bus I2C : STOP*/
    I2C_StopMaster();
    
    return false;  //No I2C errors !!
   
}

extern bool DS1307_Read_TimeDate(rtcDs1307UserData_t *prtcDs1307UserData){
   /*Log DS1307 Data*/
    /*Iniciar Bus I2C : START*/
    I2C_StartMaster();

    /*Enviar la direccion del esclavo I2C en modo WRITE*/
    if(I2C_WriteMaster(RTC_DS1307_SLAVE_ADDR_WR_MODE) ==true){
        return true;    //Fallo bus I2C
    }

    /*Enviar el valor del Register Pointer desde donde se va a leer*/
    if(I2C_WriteMaster(RTC_DS1307_REG_MAP_BASE_ADDR) ==true){
        return true;    //Fallo bus I2C
    }

    /*Inicio Repetido: RE-START*/
    I2C_ReStartMaster();

    /*Enviar la direccion del esclavo I2C en modo READ*/
    if(I2C_WriteMaster(RTC_DS1307_SLAVE_ADDR_RD_MODE) ==true){
        return true;    //Fallo bus I2C
    }

    /*Leer los bytes de data, cada uno con su ACK enviado desde el Master*/
    /*El Master envia  NACK tras el ultimo byte leido*/
    for(int i=0 ; i<sizeof(rtcDs1307Buffer);i++){
        if(i==(sizeof(rtcDs1307Buffer) - 1)){
            rtcDs1307Buffer[i]=I2C_ReadMaster(SEND_NACK);
            break;
        }
        rtcDs1307Buffer[i]=I2C_ReadMaster(SEND_ACK);

    }

    /*Detener bus I2C : STOP*/
    I2C_StopMaster();

    /*Obtener la data del RTC DS1307*/
    memcpy(&rtcDs1307TimeData,&rtcDs1307Buffer,sizeof(rtcDs1307TimeData)); 
    
     /*Format DS1307 data into user-readle format: 'rtcDs1307UserData_t'*/
     __DS1307_Retreive_From_TimeDate(prtcDs1307UserData, &rtcDs1307TimeData );
    
    return false ; // No I2C Bus Errors!!
}

/* 
 * This method pre-builts Time-Date data structure before sending to I2C Bus from User custom data....
 * 
 */
static void __DS1307_Build_To_TimeDate(rtcDs1307TimeData_t *prtcDs1307TimeData,rtcDs1307UserData_t *prtcDs1307UserData){
 
  prtcDs1307TimeData->secL=(prtcDs1307UserData->seconds ) %10;
  prtcDs1307TimeData->secH= (prtcDs1307UserData->seconds) /10 ;
  
  prtcDs1307TimeData->minL=(prtcDs1307UserData->minutes ) %10;
  prtcDs1307TimeData->minH=(prtcDs1307UserData->minutes) /10 ;
  
  prtcDs1307TimeData->hoursL=(prtcDs1307UserData->hours ) %10;
  prtcDs1307TimeData->hoursH=(prtcDs1307UserData->hours) /10 ; //Include AM/PM bit if 12hour mode is selected !
  
  prtcDs1307TimeData->hourMode=prtcDs1307UserData->hour12Mode;  //Selects 12/24 hour mode !!
  
  prtcDs1307TimeData->day=7;
  
  prtcDs1307TimeData->dateL=(prtcDs1307UserData->date ) %10;
  prtcDs1307TimeData->dateH=(prtcDs1307UserData->date) /10 ;
  
  prtcDs1307TimeData->monthL=(prtcDs1307UserData->month ) %10;
  prtcDs1307TimeData->monthH=(prtcDs1307UserData->month ) /10;
  
  prtcDs1307TimeData->yearL=(prtcDs1307UserData->year ) %10;
  prtcDs1307TimeData->yearH=(prtcDs1307UserData->year) /10 ;
  
}

/* 
 * This method retreives Time-date info in a user readable format : rtcDs1307UserData_t
 * 
 */
static void __DS1307_Retreive_From_TimeDate(rtcDs1307UserData_t *prtcDs1307UserData, rtcDs1307TimeData_t *prtcDs1307TimeData ){
    prtcDs1307UserData->seconds = (prtcDs1307TimeData->secH)*10 + prtcDs1307TimeData->secL ;
    prtcDs1307UserData->minutes = (prtcDs1307TimeData->minH)*10 + prtcDs1307TimeData->minL ;
   
    if(prtcDs1307TimeData->hourMode ==1){   //12 hour mode AM/PM
        prtcDs1307UserData->hours   = ((prtcDs1307TimeData->hoursH) &0x01 )*10 + prtcDs1307TimeData->hoursL ;
        prtcDs1307UserData->pmAM    =  ((prtcDs1307TimeData->hoursH) &0x02)>>1 ;
    
    }else{                              //24 hour mode
        prtcDs1307UserData->hours   = (prtcDs1307TimeData->hoursH)*10 + prtcDs1307TimeData->hoursL ;
    }
    
    prtcDs1307UserData->day = prtcDs1307TimeData->day;
    
    prtcDs1307UserData->date   = (prtcDs1307TimeData->dateH)*10 + prtcDs1307TimeData->dateL;
    
    prtcDs1307UserData->month   = (prtcDs1307TimeData->monthH)*10 + prtcDs1307TimeData->monthL;
    prtcDs1307UserData->year   = (prtcDs1307TimeData->yearH)*10 + prtcDs1307TimeData->yearL;
}

extern void DS1307_ISR(void){
    rtcTimeReady=true; //1.00 sec has elapsed, we now read registers !!
}

extern bool DS1307_GetISR_FlagStatus(void){
    return rtcTimeReady; //Return current status of RTC ISR flag
}

extern bool DS1307_ClearsISR_Flag(void){
    rtcTimeReady=false; //Clear 1.00 sec ISR software flag
}