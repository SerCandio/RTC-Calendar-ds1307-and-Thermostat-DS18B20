/* 
 * File:   main.c
 * Author: CURSOS
 *
 * Created on 21 de diciembre de 2023, 02:01 PM
 */

#include "RTOS.h"

#define BUZZER_Time_Alarm_Toogle()          LATDbits.LATD6=~LATDbits.LATD6
#define RELAY_Set_Temperature_TH_Alarm()    LATDbits.LATD7=1
#define RELAY_Clear_Temperature_TH_Alarm()  LATDbits.LATD7=0
#define RELAY_Set_Temperature_TL_Alarm()    LATCbits.LATC2=1
#define RELAY_Clear_Temperature_TL_Alarm()  LATCbits.LATC2=0

#define SHIFT_TIME_Button_In()      PORTDbits.RD2
#define SHIFT_ALARM_Button_In()     PORTDbits.RD3
#define INC_Button_In()             PORTDbits.RD4
#define DEC_Button_In()             PORTDbits.RD5

#define PRESSED                     0U
#define RELEASED                    1U

/*Task polling rate in miliseconds 'ms'*/
#define TASK1_POLLRATE_MS             250UL     //Read Temperature sensor
#define TASK2_POLLRATE_MS             200UL       //Read RTC actual Time & Date ISR
#define TASK3_POLLRATE_MS             10UL        //Process && Decisions for Outputs
#define TASK4_POLLRATE_MS             250UL       //Update outputs
#define TASK5_POLLRATE_MS             150UL       //Read User Pushbuttons (Asyncronous)
#define TASK6_POLLRATE_MS             100UL       //Update RTC with desired user time & date
#define TASK7_POLLRATE_MS             100UL       //Update EEPROM with user desired values
#define TASK8_POLLRATE_MS             5000UL    //Display Alarms on Console   

/*Task Priority Order*/
#define TASK1_PRIORITY              1U
#define TASK2_PRIORITY              2U
#define TASK3_PRIORITY              3U
#define TASK4_PRIORITY              4U
#define TASK5_PRIORITY              5U
#define TASK6_PRIORITY              6U
#define TASK7_PRIORITY              7U
#define TASK8_PRIORITY              8U

/* 
 * Global Variables
 */
static char printLCDBuffer[LCD_16_2_MAX_COLUMN_SIZE];
static uint8_t buttonUserMode =0, buttonAlarmUserMode=0 ;
static uint16_t rawTemperature;
static char     Str_Real_temperature[DS18B20_PRINT_BUFFER_SIZE];
static int8_t   temperatureInteger;
static bool timeAlarmEnable=false, temperatureHighAlarmEnable=false, temperatureLowAlarmEnable=false;
static bool actualTimeReadyToSaveRTC = false;
static bool alarmsReadyToSaveEEPROM = false;
static bool alarmsReadyToReadEEPROM = false;

/*
 * Function Prototypes
 */
static void LCD_Update(void);
static void LCD_Time_Alarm_Update(void);
static void LCD_All_Alarms_Update_OK(void);
static void LCD_Temperature_Alarm_Update(void);
static void LCD_Time_Cadence_Alarm_Update(void);
static void LCD_Temperature_Offset_Alarm_Update(void);


/* 
 * RTOS User Tasks...
 */
void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);
void Task5(void);
void Task6(void);
void Task7(void);
void Task8(void);


int main(int argc, char** argv) {
    
    /* 
     * Incicializaciob de perifericos
     */
    vHardwareInit();
    
    /* 
     * Creacion de Tareas RTOS + prioridades
     */
    xTaskCreate("Task 1",                       //Read Temperature sensor  
            TASK1_PRIORITY, 
            TASK1_POLLRATE_MS, 
            Task1);
    
    xTaskCreate("Task 2",                       //Read RTC actual Time & Date ISR
            TASK2_PRIORITY, 
            TASK2_POLLRATE_MS, 
            Task2);
    
    xTaskCreate("Task 3",                   //Process && Decisions for Outputs
            TASK3_PRIORITY, 
            TASK3_POLLRATE_MS, 
            Task3);
    
    xTaskCreate("Task 4",                   //Update outputs
            TASK4_PRIORITY, 
            TASK4_POLLRATE_MS, 
            Task4);
    
    xTaskCreate("Task 5",               //Read User Pushbuttons (Asyncronous)
            TASK5_PRIORITY, 
            TASK5_POLLRATE_MS, 
            Task5);
    
    xTaskCreate("Task 6",               //Update RTC with desired user time & date
            TASK6_PRIORITY, 
            TASK6_POLLRATE_MS, 
            Task6);
    
    xTaskCreate("Task 7",           //Update EEPROM with user desired values
            TASK7_PRIORITY, 
            TASK7_POLLRATE_MS, 
            Task7);
    
    xTaskCreate("Task 8",       //Display Alarms on Console   
            TASK8_PRIORITY, 
            TASK8_POLLRATE_MS, 
            Task8);
    

    
    //Iniciar el Scheduler de ejecuacion de  tareas del RTOS........bucle while()
    vTaskStartScheduler();
    

    return (EXIT_SUCCESS);
}


/* 
 * Sub-Functions code....
 */
static void LCD_Update(void){
    
     /*Actualizar la LCD ->%02u/%02u/20%02u %02u:%02u:%02u*/
    /*Display Time and Temperature*/
    sprintf(printLCDBuffer, "%02u:%02u:%02u %s",
               rtcDs1307UserData.hours,
               rtcDs1307UserData.minutes,
               rtcDs1307UserData.seconds,
               Str_Real_temperature
               );

    LCD_I2C_SetCursor(0 ,1);
    LCD_I2C_Print_String(printLCDBuffer);
    
    /*Print Symbol Temperature units*/
    LCD_I2C_SetCursor(14 ,1);
    LCD_I2C_Write_New_Char(TEMP_DEG_SYMBOL);
    LCD_I2C_SetCursor(15 ,1);
    LCD_I2C_Print_Char('C');

    /*Display date only*/
    sprintf(printLCDBuffer, "Date: %02u/%02u/20%02u",
            rtcDs1307UserData.date,
            rtcDs1307UserData.month,
            rtcDs1307UserData.year
            );
    
    LCD_I2C_SetCursor(0 ,2);
    LCD_I2C_Print_String(printLCDBuffer);
}

static void LCD_Time_Alarm_Update(void){

    /*Actualizar la LCD en modo alarmas de tiempo*/
    sprintf(printLCDBuffer,"H:%02u M:%02u S:%02u",
            userAlarm.hours,
            userAlarm.minutes,
            userAlarm.seconds
            );
    
    
    /*Titulo de la alarma*/
    LCD_I2C_SetCursor(0 ,1);
    LCD_I2C_Print_String("  TIME ALARM");
    
    /*Mostra en pantalla el valor de la alarma*/
    LCD_I2C_SetCursor(1 ,2);
    LCD_I2C_Print_String(printLCDBuffer);
    
}

static void LCD_All_Alarms_Update_OK(void){
    
    LCD_I2C_SetCursor(0 ,1);
    LCD_I2C_Print_String("   ALL ALARMS");
    
    LCD_I2C_SetCursor(0 ,2);
    LCD_I2C_Print_String("  Update OK!!");
}

static void LCD_Temperature_Alarm_Update(void){
    
    /*Actualizar la LCD en modo alarmas de temperatura*/
     sprintf(printLCDBuffer," TH:%03i TL:%03i",
             userAlarm.highTemperatureLimit,
             userAlarm.lowTemperatureLimit  
            );
     
      /*Titulo de la alarma*/
    LCD_I2C_SetCursor(0 ,1);
    LCD_I2C_Print_String(" TEMP ALARM [ C]");
    
    /*Print Symbol Temperature units °C*/
    LCD_I2C_SetCursor(13 ,1);
    LCD_I2C_Write_New_Char(TEMP_DEG_SYMBOL);
    
    /*Mostra en pantalla el valor de la alarma*/
    LCD_I2C_SetCursor(1 ,2);
    LCD_I2C_Print_String(printLCDBuffer);
    
}

static void LCD_Time_Cadence_Alarm_Update(void){ 
    
    /*Actualizar la LCD en modo : Cadencia de tiempo*/
     sprintf(printLCDBuffer,"Min:%02u",
             userAlarm.timeAlarmDuration
            );
     
    /*Titulo de la alarma*/
    LCD_I2C_SetCursor(0 ,1);
    LCD_I2C_Print_String(" TIME Cadence");
    
    /*Mostra en pantalla el valor de la alarma*/
    LCD_I2C_SetCursor(1 ,2);
    LCD_I2C_Print_String(printLCDBuffer);
  
}

static void LCD_Temperature_Offset_Alarm_Update(void){
    
    /*Actualizar la LCD en modo : Offset de Temperatura*/
     sprintf(printLCDBuffer,"THRS:%03u",
             userAlarm.temperatureAlarmOffset
            );
     
    /*Titulo de la alarma*/
    LCD_I2C_SetCursor(0 ,1);
    LCD_I2C_Print_String("TEMP Offset [ C]");
    
    /*Print Symbol Temperature units °C*/
    LCD_I2C_SetCursor(13 ,1);
    LCD_I2C_Write_New_Char(TEMP_DEG_SYMBOL);
    
    /*Mostra en pantalla el valor de la alarma*/
    LCD_I2C_SetCursor(1 ,2);
    LCD_I2C_Print_String(printLCDBuffer);
      
}

/* 
 * RTOS User Tasks...
 */
void Task1(void){    
    /* 
     * This Task will retrive temperature data from ds18b20 One Wire Sensor every: TASK1_POLLRATE_MS
     */
    if(!ds18b20_Read_Real_Temperature(Str_Real_temperature, &temperatureInteger) )
    {
        /*Note: On console we will show string and integer data for dbugging pirposes*/
        printf("\r\nTemp (str): %s°C , (int)#: %i°C",Str_Real_temperature, temperatureInteger);
        
    }else
    {   Set_ERROR(); //One Wire communication error
        printf("\r\nError en el bus 1-Wire, reason 278");
        while(1);
    }
   
}

void Task2(void){
     /* 
     * This Task will poll to read RTC DS1307 every : TASK2_POLLRATE_MS
      * and log Time and date on global variable
     */
    
    //Transcurrio 1.0 sec ???
    if(DS1307_GetISR_FlagStatus() == true)
    {
        DS1307_ClearsISR_Flag();
        
        /*Leer la data del RTC*/
       if(DS1307_Read_TimeDate(&rtcDs1307UserData)==true){  //24 h mode
            Set_ERROR();
            printf("\r\nError en el Bus I2C !!, reason 298");
            while(1);           //I2C bus communication error !
       }
        
       /*Imprimir la data por UART*/
        printf("\r\n\r\nTime#: %02u/%02u/20%02u %02u %02u:%02u:%02u\r\n",
               rtcDs1307UserData.date,
               rtcDs1307UserData.month,
               rtcDs1307UserData.year,

               rtcDs1307UserData.day,

               rtcDs1307UserData.hours,
               rtcDs1307UserData.minutes,
               rtcDs1307UserData.seconds
               );
        
        /*Imprimir la data por LCD 16X2*/
        LCD_Update();
    }
    
}

void Task3(void){    
    /* 
     * This Task will poll to activate alarms(time or temperature) every : TASK3_POLLRATE_MS
     * 
      * */
    
    /*Evaluate Alarms: Time Alarm
     * Note: This evaluates if actual time data is equal to alarm time or has passed alarm time by 
     *       certain value in minutes
     */
   if( (userAlarm.hours==rtcDs1307UserData.hours) &&   //If hours Match and..
       (userAlarm.minutes==rtcDs1307UserData.minutes)  &&  //If Minutes Match and....
        (rtcDs1307UserData.minutes-userAlarm.minutes)<=userAlarm.timeAlarmDuration // If actual minutes not passed alarm certain threshold  
     )
   {
       //Enable Time Alarm
       timeAlarmEnable=true;  

   }else
   {
       //Disable Time Alarm  temperatureAlarmEnable
       timeAlarmEnable=false;  
   }
   
   
   /*Evaluate alarms : High Limit Temperature Alarm 
    */
   //As long as actual temperature is upper than TH - THRS(threshold)
   //Fan keeps turning...
   if(temperatureInteger>=(userAlarm.highTemperatureLimit - userAlarm.temperatureAlarmOffset))
   {
       //Enable High Limit Temperature Alarm: TH
       temperatureHighAlarmEnable = true;
       
   }else
   {
       //Disable High Limit Temperature Alarm: TH
       temperatureHighAlarmEnable = false;     
   }
   
   
   
    /*Evaluate alarms : LOW Limit Temperature Alarm: TL
    */
   //As long as actual temperature is less than TL + THRS(threshold)
   if(temperatureInteger<=(userAlarm.lowTemperatureLimit + userAlarm.temperatureAlarmOffset))
   {
       //Enable Low Limit Temperature Alarm: TL
       temperatureLowAlarmEnable = true;
       
   }else
   {
       //Disable Low Limit Temperature Alarm: TL
       temperatureLowAlarmEnable = false;     
   }
    
}

void Task4(void){
    /* 
     * This Task will toogle time alarm : TASK4_POLLRATE_MS
     */
    if(timeAlarmEnable==true)
    {
        BUZZER_Time_Alarm_Toogle();
        
    }
    
    if(temperatureHighAlarmEnable==true)
    {
        RELAY_Set_Temperature_TH_Alarm();  //Set temperature alarm: TH
        
    }else
    {
        RELAY_Clear_Temperature_TH_Alarm();  //Turn Off temperature alarm:TH 
        
    }
    
    if(temperatureLowAlarmEnable==true)
    {
        RELAY_Set_Temperature_TL_Alarm();       //Set temperature alarm: TL
        
    }else
    {
        RELAY_Clear_Temperature_TL_Alarm(); //Clear temperature alarm: TL
        
    }
     
}

void Task5(void){
   /* 
     * This Task will poll to read pushbuttons every: TASK5_POLLRATE_MS
      * Note: Pushbuttons will enable setting manual time and date onto RTCDS1307
     */
    
    /*'SHIFT_TIME' button will only modify actual time and Date. This data should remain even if power supply goes off*/
    if(SHIFT_TIME_Button_In() ==PRESSED)
    {
         vTaskDelay(15);
         buttonUserMode++;
         if(buttonUserMode>5) buttonUserMode=0;
         
         /*Pre-set an initial Time*/
         rtcDs1307UserData.seconds=0;

        while(SHIFT_TIME_Button_In() ==PRESSED)
        {     
            if(INC_Button_In()==PRESSED)
            {
                vTaskDelay(15);
                while(INC_Button_In()==PRESSED);

                //Decide which Time or date parameter modify accordingly
                switch(buttonUserMode)
                {
                    case 1:
                        /*Increment minutes field*/
                        rtcDs1307UserData.minutes++;
                        if(rtcDs1307UserData.minutes>=60)
                        {
                            rtcDs1307UserData.minutes=0;
                        }
                        break;

                    case 2:
                         /*Increment hours field*/
                        rtcDs1307UserData.hours++;
                        if(rtcDs1307UserData.hours>=24)
                        {
                            rtcDs1307UserData.hours=0;
                        }
                        break;

                    case 3:
                        /*Increment date field*/
                        rtcDs1307UserData.date++;
                        if(rtcDs1307UserData.date>=31)
                        {
                            rtcDs1307UserData.date=0;
                        }
                        break;

                    case 4:
                        /*Increment month field*/
                        rtcDs1307UserData.month++;
                        if(rtcDs1307UserData.month>=13)
                        {
                            rtcDs1307UserData.month=0;
                        }
                        break;

                    case 5:
                        /*Increment year field*/
                        rtcDs1307UserData.year++;
                        if(rtcDs1307UserData.year>=100)
                        {
                            rtcDs1307UserData.year=0;
                        } 
                        break;
                }

            }
            
            if(DEC_Button_In()==PRESSED)
            {
                vTaskDelay(15);
                while(DEC_Button_In()==PRESSED);

                //Decide which Time or date parameter modify accordingly
                switch(buttonUserMode)
                {
                    case 1:
                        /*Increment minutes field*/
                        rtcDs1307UserData.minutes--;
                        if(rtcDs1307UserData.minutes>=250)
                        {
                            rtcDs1307UserData.minutes=59;
                        }
                        break;

                    case 2:
                         /*Increment hours field*/
                        rtcDs1307UserData.hours--;
                        if(rtcDs1307UserData.hours>=250)
                        {
                            rtcDs1307UserData.hours=23;
                        }
                        break;

                    case 3:
                        /*Increment date field*/
                        rtcDs1307UserData.date--;
                        if(rtcDs1307UserData.date>=250)
                        {
                            rtcDs1307UserData.date=31;
                        }
                        break;

                    case 4:
                        /*Increment month field*/
                        rtcDs1307UserData.month--;
                        if(rtcDs1307UserData.month>=250)
                        {
                            rtcDs1307UserData.month=12;
                        }
                        break;

                    case 5:
                        /*Increment year field*/
                        rtcDs1307UserData.year--;
                        if(rtcDs1307UserData.year>=250)
                        {
                            rtcDs1307UserData.year=99;
                        }
                        break;
                }

            }

             /*Actualizar la LCD*/
            LCD_Update();
            
            /*Save some power energy*/
            vTaskDelay(50);

        }
         
         /*We are now ready to save back on RTC*/
         actualTimeReadyToSaveRTC = true;

    }
    
    /*This task will configure Time and Temperature Alarm */
    if(SHIFT_ALARM_Button_In()==PRESSED)
    {
        vTaskDelay(15);
        buttonAlarmUserMode++;
        
        /*Clear for a new screen*/
        LCD_I2C_Clear();  
  
        //As long we are configuring alarms, this should remain pressed, so it's asyncronous Task
        while(SHIFT_ALARM_Button_In()==PRESSED) 
        {
            
            if(INC_Button_In()==PRESSED)
            {
                vTaskDelay(15);
                while(INC_Button_In()==PRESSED);
                
                //Decide which Alarm parameter(Time or Temperature) increment Upper
                switch(buttonAlarmUserMode)
                {
                    case 1:
                        /*Increment seconds field */
                        userAlarm.seconds++;
                        if(userAlarm.seconds>=60) userAlarm.seconds=0;     
                        break;  
                        
                    case 2:
                        /*Increment minutes field */
                        userAlarm.minutes++;
                        if(userAlarm.minutes>=60) userAlarm.minutes=0;     
                        break;
                        
                    case 3:
                        /*Increment hours field */
                        userAlarm.hours++;
                        if(userAlarm.hours>=24) userAlarm.hours=0;  
                        break;
                        
                    case 4:
                        /*Increment temperature high limit: TH*/
                        userAlarm.highTemperatureLimit++;
                        if(userAlarm.highTemperatureLimit>=50)  userAlarm.highTemperatureLimit=50;
                        break;
                        
                    case 5:
                        /*Increment temperature low limit: TL*/
                        userAlarm.lowTemperatureLimit++;
                        if(userAlarm.lowTemperatureLimit>=10)  userAlarm.lowTemperatureLimit=10;
                        break;
                        
                    case 6:
                        /*Increment time cadence alarm duration*/
                        userAlarm.timeAlarmDuration++;
                        if(userAlarm.timeAlarmDuration>=5) userAlarm.timeAlarmDuration=5;
                        break;
                        
                    case 7:
                        /*Increment temperature alarm Offset: deltaTemperature*/
                        userAlarm.temperatureAlarmOffset++;
                        if(userAlarm.temperatureAlarmOffset>=10) userAlarm.timeAlarmDuration=10;
                        break;

                }
                
            }
            
            if(DEC_Button_In()==PRESSED)
            {
                vTaskDelay(15);
                while(DEC_Button_In()==PRESSED);
                
                 //Decide which Alarm parameter(Time or Temperature) decrement Lower
                switch(buttonAlarmUserMode)
                {
                    case 1:
                        /*Decrement seconds field */
                        userAlarm.seconds--;
                        if(userAlarm.seconds>=250) userAlarm.seconds=59;
                        break;
                        
                    case 2:
                        /*Decrement minutes field */
                        userAlarm.minutes--;
                        if(userAlarm.minutes>=250) userAlarm.minutes=59; 
                        break;
                        
                    case 3:
                         /*Decrement hours field */
                        userAlarm.hours--;
                        if(userAlarm.hours>=250) userAlarm.hours=23; 
                        break;
                        
                    case 4:
                        /*Decrement temperature high limit TH*/
                        userAlarm.highTemperatureLimit--;
                        if(userAlarm.highTemperatureLimit<=10)  userAlarm.highTemperatureLimit=10;
                        break;
                        
                    case 5:
                        /*Decrement tempeature low limit: TL*/
                        userAlarm.lowTemperatureLimit--;
                        if(userAlarm.highTemperatureLimit<=-25)  userAlarm.highTemperatureLimit=-25;
                        break;
                        
                    case 6:
                         /*Decrement time cadence alarm duration*/
                        userAlarm.timeAlarmDuration--;
                        if(userAlarm.timeAlarmDuration>=250) userAlarm.timeAlarmDuration=0;
                        break;
                        
                    case 7:
                         /*Decrement temperature alarm Offset: deltaTemperature*/
                        userAlarm.temperatureAlarmOffset--;
                        if(userAlarm.temperatureAlarmOffset>=250) userAlarm.timeAlarmDuration=0;
                        break;
   
                }
   
            }
             
            //Take decisions according 'buttonAlarmUserMode' value
            if(buttonAlarmUserMode==1 || buttonAlarmUserMode==2 || buttonAlarmUserMode==3)
            {
                /*Actualizar la LCD: modo alarma de tiempo*/
                LCD_Time_Alarm_Update();
                
            } else if(buttonAlarmUserMode==4 || buttonAlarmUserMode==5)
            {
                /*Actualizar la LCD: modo limites de temperatura: TH, TL*/
                LCD_Temperature_Alarm_Update();
                
            }else if(buttonAlarmUserMode==6)
            {
                /*Actualzar la LCD: modo duracion de la alarma de tiempo*/
                LCD_Time_Cadence_Alarm_Update();   
                
            }else if(buttonAlarmUserMode==7)
            {
                /*Actualzar la LCD: modo Offset de la temperatura*/
                LCD_Temperature_Offset_Alarm_Update();
            }
            
            /*Save some power energy*/
            vTaskDelay(50);           
        }
        
        //Shift button released
        /*Note: This suggest 'SHIFT_Alarm" has to be pressed 8 times in order to save on EEPROM*/
        if(buttonAlarmUserMode>7)
        {
            buttonAlarmUserMode=0; //
            alarmsReadyToSaveEEPROM=true;

        }
        
    }
    
}

void Task6(void){
    /* 
     * This Task will poll to update RTC DS1307 with actual time parameters every : TASK6_POLLRATE_MS
     * 
     * Once set , this should be the actual Time and Date
    * */
    
    if(actualTimeReadyToSaveRTC==true)
    {
        actualTimeReadyToSaveRTC= false;
        
        /*Time parameters changed, so update accordindly Time-Keeping registers*/
        if(DS1307_Set_TimeDate(&rtcDs1307UserData)==true){
            Set_ERROR(); //I2C bus communication error
            printf("\r\nError en el bus I2C !!, reason 728");
            while(1);
        } 
    }
    
}

void Task7(void){
    /* 
     * This Task will write on EEPROM every : TASK7_POLLRATE_MS
     * 
     * Note: This Task will be condicitioned to flag: 'alarmsReadyToSaveEEPROM'
     *       Write cycle on EEPROM takes 50 ms aprox so all bytes will be writeen accordng this map :
     * 
     * Alarm parameters changed, so update accordindly in EEPROM map:
     * 0x0000 : Alarm Seconds
     * 0x0001 : Alarm Minutes
     * 0x0002 : Alarm Hours  
     * 0x0003 : Temperature HIGH Limit
     * 0x0004 : Temperature LOWER Limit  
     * 0x0005:  Time Cadence Alarm Duration        
     * 0x0006:  Temperature Cadence Alarm Duration 
     */
    
    //If Task 3 pre-set all alarms, then do write on EEPROM 24C32A
    if(alarmsReadyToSaveEEPROM==true)
    {
        alarmsReadyToSaveEEPROM=false;
        
        //Pre-Build buffer datagram
        memcpy(&eepromBufferTx, &userAlarm, sizeof(userAlarm) );
        
        //Send datagram Thorugh I2C bus as a page starting at 0x0000
        if(EEPROM_Write_Page(EEPROM_24C32_ADDR_WR_MODE, 0x0000, eepromBufferTx)==true){
            Set_ERROR();
            printf("\r\nError en el Bus I2C , reason 763!!");
            while(1);           //I2C bus communication error !  
        }
        
        alarmsReadyToReadEEPROM=true;  //We are now ready to retrive alarms from EEPROM 

         /*Clear and display OK 'Alarm Update' message ln LCD */
        LCD_All_Alarms_Update_OK();
        
        vTaskDelay(500UL); //Wait for a while....
        
        LCD_I2C_Clear();    //
           
    }
        
}

void Task8(void){
    /* 
     * This Task will execute every : TASK8_POLLRATE_MS 
     */
    
    //Alarms on Power up time are already retreived on vHardwareInit() fnc
    
    //A new alarm has bee set (execution time) ?
    if(alarmsReadyToReadEEPROM==true)
    {
        alarmsReadyToReadEEPROM=false;
        
         /*Display Current alarm value*/
       if(EEPROM_Sequential_Read(EEPROM_24C32_ADDR_RD_MODE, 0x0000, 7U, eepromBufferRx)==true){
            Set_ERROR(); //I2C bus communication error
            printf("\r\nError en el bus I2C !!, reason 795");
            while(1);
       }

        /*Retreive actual alarm...*/
       memcpy(&userAlarm, &eepromBufferRx, sizeof(userAlarm) );
    }
     
    /*Display actual alarm*/
   printf("\r\n\r\nAlarms#: H:%02u M:%02u S:%02u C:%02u,%s ; TH:%03i,%s TL:%03i,%s THRS:%03u\r\n",
           userAlarm.hours,
           userAlarm.minutes,
           userAlarm.seconds,
           userAlarm.timeAlarmDuration,
           
           timeAlarmEnable?"ON":"OFF",

           userAlarm.highTemperatureLimit,
           temperatureHighAlarmEnable?"ON":"OFF",
           
           userAlarm.lowTemperatureLimit,
           temperatureLowAlarmEnable?"ON":"OFF",
           
           userAlarm.temperatureAlarmOffset
           );   
   
}
