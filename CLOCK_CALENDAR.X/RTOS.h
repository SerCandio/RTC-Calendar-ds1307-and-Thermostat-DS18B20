/* 
 * File:   RTOS.h
 * Author: CURSOS
 *
 * Created on 13 de diciembre de 2023, 11:34 AM
 */

#ifndef RTOS_H
#define	RTOS_H

/*Periphericals for RTOS*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <xc.h>
#include <pic18f45k50.h>
#include "ConfBits_RealBoardPLL.h"
#include "INTERRUPT_Controller.h"
#include "I2C_Master.h"
#include "LCD_I2C.h"
#include "custom_charLCD.h"
#include "EUSART.h"
#include "DS1307.h"
#include "Alarms.h"
#include "TMR2.h"
#include "ds18b20.h"
#include "EEPROM_24C32A.h"

#define MAX_NUM_TASKS           8  /*Numero maximo de tareas del sistema*/
#define MAX_NAME_LEN            10

#define Set_ERROR()                 LATDbits.LATD0 = 1  /*Error en la comunicacion*/
#define Clear_ERROR()               LATDbits.LATD0 = 0


//#define TASK_DEBUG_MODE 

//#define SLEEP_MODE          //Enables Low Power Mode for PIC18FXX  Only works in Real Board!!

//Bloque de Control de Tareas (Task Control Block) TCB
//Mantiene toda la informacion reelevante de una tarea
typedef struct{
    uint16_t tID;         /*ID de la Tarea*/
    uint16_t priority;      /*prioridad de la tarea*/
    uint32_t tPollRateMs;  /*Poll rate en "ms" de la tarea(cada cuanto se llama)*/
    uint32_t tElapsedMs;   /*Cantidad de "ms" relativos transcurridos de la tarea*/
    uint16_t taskInList;    /*1 si la tarea esta en la lista, caso contrario 0*/
    void (*pvFuncTask)(void);  /*puntero a la funcion de la tarea*/
    char nameTask[MAX_NAME_LEN];  /*Nombre de la tarea*/ 
}TCB_t;

/* 
 * Global Variables
 * Access : Public
 */
TCB_t taskList[MAX_NUM_TASKS];  /*Creacion de una lista de tareas*/

/* 
 * Function Prototypes
 * Access :Public
 */
//Funcion para agregar tareas a la lista
extern uint8_t xTaskCreate(const char *pcName, uint16_t xPriority, const uint32_t pvTaskRateMs, void (*pvTaskCode)(void));

//Implementacion basica de un scheduler.....reemplaza el main loop
extern void vTaskStartScheduler(void);

//Inicio de perifericos
extern void vHardwareInit(void);

//Implememtacion basica de un delay de Task en milisegundos, reemplaza el delay de instruccion y ahorra energia
extern void vTaskDelay(uint32_t taskDelayMs);

//Inicio de I/O digitales
static void PORT_Init(void);

/* 
 * Global Variables
 * Access : Public
 * 
 * Note : Puede usar estas variables dle RTC DS1307 en su codigo main
 */
 rtcDs1307Control_t  rtcDs1307Control;
 rtcDs1307UserData_t rtcDs1307UserData;
 ds18b20Config_t     ds18b20Config;
 uint64_t            ds18b20ROMCode;
static char          printLCDBuffer[LCD_16_2_MAX_COLUMN_SIZE]; 
 uint8_t eepromBufferRx[ALARM_EEPROM_DATAGRAM_SIZE];
 uint8_t eepromBufferTx[ALARM_EEPROM_DATAGRAM_SIZE];
userAlarm_t          userAlarm;

#endif	/* RTOS_H */

