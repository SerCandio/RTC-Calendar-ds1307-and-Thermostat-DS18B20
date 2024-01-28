#include "RTOS.h"

//Funcion para agregar tareas a la lista
extern uint8_t xTaskCreate(const char *pcName, uint16_t xPriority, const uint32_t pvTaskRateMs, void (*pvTaskCode)(void)){
    uint8_t i=1;
    uint8_t ret =0;
//    TCB_t temp;
    
//    int length = sizeof(taskList)/sizeof(taskList[0]) ; 
    
    printf("Creando Tarea\r\n");
    
    while(i <=MAX_NUM_TASKS)
    {
        if(strlen(pcName) > MAX_NAME_LEN)
        {
            printf("Error -> Nombre de la Tarea muy grande\r\n");
            return ret;
        }
        
        if (taskList[i].taskInList != 1)
        {
            printf("Tarea creada #: %d\r\n\r\n", i);
            taskList[i].tID =i;
            strcpy(taskList[i].nameTask,pcName);
            taskList[i].priority = xPriority;
            taskList[i].tPollRateMs = pvTaskRateMs;
            taskList[i].tElapsedMs = millis(); 
            taskList[i].pvFuncTask=pvTaskCode;
            taskList[i].taskInList =1;
            ret =1;
            break;
        }
        i++; //Si el espacio ya esta reservado, incrementa a otro
        
    }
        
    return ret ; 
    
}

//Implementacion basica de un scheduler
/*No se toma en cuenta la prioridad*/
extern void vTaskStartScheduler(void){
    uint16_t i = 1;
    void (*pvTask) (void);
    int length = sizeof(taskList)/sizeof(taskList[0]) ; 
    TCB_t temp;
    
    /*  
     * Reordenar la lista de tareas segun si prioridad. 
     * Nota: Numero 'priority' mas bajo tiene prioridad de ejecucion mas alta
     */
    for (int j = 0; j <=length; j++) {     
        for (int k = j+1; k <=length; k++) {     
           if(taskList[j].priority > taskList[k].priority) {    
               temp = taskList[j];    
               taskList[j] = taskList[k];    
               taskList[k] = temp;    
           }     
        }     
    }  
    
    /* 
     * Mostrar el orden de ejecucion de las tareas, solo si el modo DEBUG habilitado
     */
#if defined(TASK_DEBUG_MODE)
    for(i = 1; i<=MAX_NUM_TASKS; i++)
    {
         printf("\r\n%s ,order: %u", taskList[i].nameTask, taskList[i].priority);
    }
    puts("\r\n\r\n");
#endif
    
    while(1)
    {
        for(i = 0; i<=MAX_NUM_TASKS; i++)
        {
            if(taskList[i].taskInList ==1)
            {
                if(millis()>=(taskList[i].tElapsedMs + taskList[i].tPollRateMs))
                {
                    taskList[i].tElapsedMs = millis();
#if defined(TASK_DEBUG_MODE)
                    printf("\r\n%s ,order: %u", taskList[i].nameTask, taskList[i].priority);
#endif
                    pvTask = (void *) taskList[i].pvFuncTask;
                    (*pvTask) (); //Llalmado indirecto a funcion de la tarea
                }
                
            }
            
        }  
        
#if defined(SLEEP_MODE)
        asm("SLEEP");
        asm("NOP");
#endif
        
    }
}

//INCIO DE PERIFERICOS DE HARDWARE
extern void vHardwareInit(void){
    
    //48 MHz System Clock FOSC..........................OK
    OSCILATOR_Init();

    //Iniciar Pines I/O
    PORT_Init();
    
    //Iniciar Timer 2 ...
    /*1.3 uS resolution*/
    Timer2_Initialize();
    
    //Iniciar Sensor DS18B20
    ds18b20_Init();
    
    //Leer la memoeria ROM del ds18b20
    if(ds18b20_Read_ROM(&ds18b20ROMCode)){
        Set_ERROR();
        printf("\r\nError en el bus 1-Wire !!, reason 122");
        while(1);           //1-Wire bus communication error !
    }
    
    //Configurar el ds18b20 : TH, TL, Resolution
    ds18b20Config.CONFIG = DS18B20_RESOLUTION_MASK_9BIT; //9 bit resoltion tkes about 94 mS
    ds18b20Config.TH    =  DS18B20_TH_USER_BYTE1;
    ds18b20Config.TL    =  DS18B20_TL_USER_BYTE2;
    
    if(ds18b20_Configure(&ds18b20Config)){
        Set_ERROR();
        printf("\r\nError en el bus 1-Wire !!, reason 133");
        while(1);           //1-Wire bus communication error !
    }
           
    //Iniciar puerto Serial
    EUSART_SerialBegin(9600UL);
    
    //Iniciar Bus I2C
    I2C_BeginMaster(I2C_BIT_RATE_100KHZ);
    
    /*Display Current alarm value on Power Up:
     * 
     * Note : EEPROM memoery is non-volatile    
     */
   if(EEPROM_Sequential_Read(EEPROM_24C32_ADDR_RD_MODE, 0x0000, 7U, eepromBufferRx)==true){
        Set_ERROR(); //I2C bus communication error
        printf("\r\nError en el bus I2C, reason: 149");
        while(1);
   }

    /*Retreive actual alarm...*/
   memcpy(&userAlarm, &eepromBufferRx, sizeof(userAlarm) );
   
   /*Check for valid data on power && up*/
    if(  !(  (userAlarm.seconds>=0 && userAlarm.seconds<=59 ) &&
           (userAlarm.minutes>=0 && userAlarm.minutes<=59 ) &&
           (userAlarm.hours>=0 && userAlarm.hours<=23 ) &&
           (userAlarm.highTemperatureLimit>-55 && userAlarm.highTemperatureLimit<125 ) &&
           (userAlarm.lowTemperatureLimit>-55 && userAlarm.lowTemperatureLimit<125 ) &&
           (userAlarm.timeAlarmDuration>=0 && userAlarm.timeAlarmDuration<=59 ) &&
            (userAlarm.temperatureAlarmOffset>=0 && userAlarm.temperatureAlarmOffset<=100 ) )
     )
   {
        /*If out of range data (new EEPROM), then zero all elemtns except cadence values*/
        userAlarm.seconds=0;userAlarm.minutes=0;userAlarm.hours=0;
        userAlarm.highTemperatureLimit=20;userAlarm.lowTemperatureLimit=5;
        userAlarm.timeAlarmDuration=1;
        userAlarm.temperatureAlarmOffset=1; 
   }
    
    /* 
     * Configurar DS1307 : CONTROL REGISTER
     * Enable SQW /OUT to 1Hz signal
     */
    rtcDs1307Control.rs=RTC_DS1307_RS_SQW_OUT_1HZ;
    rtcDs1307Control.sqwe=RTC_DS1307_SQW_EN;
    rtcDs1307Control.out =0 ; 
      
    /*Enviar la configuracion al DS1307 via el  I2C protocolo*/
    if(DS1307_Conf_Init(&rtcDs1307Control)==true){
        Set_ERROR();
        printf("\r\nError en el bus I2C !!, reason 184");
        while(1);           //I2C bus communication error !
    }

    //Iniciar LCD
    LCD_I2C_Init();
    
    //Limpiar LCD
    LCD_I2C_Clear();
    
    //Guardar caracteres personalizados en la LCD
    LCD_I2C_New_Char(TEMP_DEG_SYMBOL,tempDegSymbol[0],tempDegSymbol[1],tempDegSymbol[2],tempDegSymbol[3],tempDegSymbol[4],tempDegSymbol[5],tempDegSymbol[6],tempDegSymbol[7]);

    //Setear cursor ( 0 posciones X a la drecha, 1era fila de arriba a abajo)
    LCD_I2C_SetCursor(0 , 1);
    //Mensaje de bienvenida
    LCD_I2C_Print_String("  WELCOME TO   ");
    
    //Setear cursor ( 0 posciones X a la drecha, 2da fila de arriba a abajo)
    LCD_I2C_SetCursor(0 , 2);
     //Mensaje......
    LCD_I2C_Print_String("Clock && Calendr");
    
     __delay_ms(1000);
   
    LCD_I2C_Clear();
    
    //Setear cursor ( 0 posciones X a la drecha, 1era fila de arriba a abajo)
    LCD_I2C_SetCursor(0 , 1);
    //Mensaje de Bienvenida
    LCD_I2C_Print_String("DS18B20 ROM Code");
    
    //Setear cursor ( 0 posciones X a la drecha, 2da fila de arriba a abajo)
    LCD_I2C_SetCursor(0 , 2);
    sprintf(printLCDBuffer,"%" PRIx64 "\n", ds18b20ROMCode);
    LCD_I2C_Print_String(printLCDBuffer);
    
    __delay_ms(1000);
    
    LCD_I2C_Clear();
    
    // Iniciar millis() , con su interrupcion por Timer 1 (cada 1ms) a Alta  prioridad
    Millis_Init(true);
    
    //Inciiar interrucpiones de perifericos
    INTERRUUPT_Init();
   
}

//Implementacion de delay de Task, Power Saving
extern void vTaskDelay(uint32_t taskDelayMs){
    uint32_t start=millis();
    
    while((millis() - start)<=taskDelayMs){
        /* Sleep mode only works on a Real Board, otherwise crashes simulation
         * Once "1ms" interrupt occurs(Timer 1), wakes up processor. This saves power
         */
#if defined(SLEEP_MODE)
        asm("SLEEP");
        asm("NOP");
#endif
        
    }
}

/* 
 * I/O Ports.....
 */
static void PORT_Init(void){
    /* 
     * RC0 y RC1 ya no estan disponibles !!  : Timer 1 / Secondary Oscilator
    */
    
    /*
     * RB0 y RC1 ya no estan disponibles !!  : I2C Bus de comunicacion
     */
    
    /*
     * RC2 como salida digital e inicialmente OFF
     */
    TRISCbits.TRISC2=0;
    LATCbits.LATC2=0;
    
    /* 
     * RB2 como entrada: Interrupcion Externa INT2 : Periferico Ext
     */
    ANSELB = 0x00 ; //Deshabilitar analgico puerto B
    TRISBbits.TRISB2 = 1; //RB2 input
  
    /* 
     * Disable analog on PORTD : as digital I/O only!
     * Note: This includes ds18b20 sensor
     */
    ANSELD = 0x00;
    
    /* Configure digital I/O pins : User buttons and leds. Ouputs are initally LOW
     * 
     * Note: ds18b20 pin (RD1) sensor is bidirectional so it is configured in execution time
     */
    TRISD|=(1<<5) | (1<<4)| (1<<3)| (1<<2);  //Input pins
    TRISD&=~(1<<7) & ~(1<<6) & ~(1<<0);       //Output pins
    LATD&=~(1<<7) & ~(1<<6) & ~(1<<0);    //Output pins initallly LOW
   
}
