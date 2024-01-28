/* 
 * File:   DS1307.h
 * Author: CURSOS
 *
 * Created on 14 de octubre de 2023, 05:25 PM
 */

#ifndef DS1307_H
#define	DS1307_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include  "I2C_Master.h"

#define RTC_DS1307_SLAVE_ADDR_WR_MODE   0xD0
#define RTC_DS1307_SLAVE_ADDR_RD_MODE   0xD1

#define RTC_DS1307_REG_MAP_BASE_ADDR    0x00

#define RTC_D1307_REG_MAP_CONTROL       0x07

#define RTC_DS1307_RS_SQW_OUT_1HZ     0b00
#define RTC_DS1307_RS_SQW_OUT_4096HZ  0b01
#define RTC_DS1307_RS_SQW_OUT_8192HZ  0b10
#define RTC_DS1307_RS_SQW_OUT_32768HZ 0b11

#define RTC_DS1307_SQW_EN             1
#define RTC_DS1307_SQW_DIS            0

/* 
 * Data structures
 */
/* 
 * RTC User data structure : User may enter time and date info on direct integer format
 * 
 * You may copy this member: 'rtcDs1307TimeData_t' as a  global/local variable
 */
typedef struct{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    
    uint8_t hour12Mode; //If '1', 12 hour mode seleced (AM/PM)
    
    uint8_t pmAM;  //If '1' PM time, '0' AM Time when hour12Mode=='1', otherwise ignored !
    
    uint8_t day;
    
    uint8_t date;
    uint8_t month;
    uint8_t year;
    
}rtcDs1307UserData_t;

/* 
 * RTC Time data structure : To Write /Read Time keeping registers
 * 
 * You may copy this member: 'rtcDs1307TimeData_t' as a  global/local variable
 */
typedef struct{
    uint8_t secL:4; //Goes from 00 to 59
    uint8_t secH:3;
    uint8_t rfu1:1;  //Reserved bit we not use!
    
    uint8_t minL:4; //Goes from 00 to 59
    uint8_t minH:3;
    uint8_t rfu2:1;  //Reserved bit we not use!
    
    uint8_t hoursL:4;  // 1-12 +AM/PM ; 00-23 hrs
    uint8_t hoursH:2; // if 'hourMode' ==HIGH (12 hour mode) , bit 5 is AM/PM bit !!
    uint8_t hourMode:1; //12 (High) or 24-hour (LOW) . 
    uint8_t rfu3:1;  //Reserved bit we not use!
    
    uint8_t day:3 ; //Day goes beyond from 01 (Sunday) up to....07
    uint8_t rfu4:5;  //Reserved bit we not use!
    
    uint8_t dateL:4;
    uint8_t dateH:2; //Date goes 00 to 31
    uint8_t rfu5:2;  //Reserved bit we not use!
    
    uint8_t monthL:4; 
    uint8_t monthH:1 ; // Goes from 01 to 12
    uint8_t rfu6:3;  //Reserved bit we not use!
    
    uint8_t yearL:4;
    uint8_t yearH:4; //Goes from 00 to 99
    
}rtcDs1307TimeData_t;
/* 
 * RTC Control structure : To control Square Wave Out on SW/Out pin
 * 
 * You may copy this member: 'rtcDs1307Control_t' as a  global/local variable
 */
typedef struct{
    uint8_t rs:2; //Rate Select Register for square wave out
    uint8_t rfu1:2;
    uint8_t sqwe:1; //Square Wave Enable bit, if 1Hz selected , clock registers update on falling edge
    uint8_t rfu2:2;
    uint8_t out:1; //Output level of SQW/out when square wave disabled
    
}rtcDs1307Control_t;

/* 
 * Global Static Variables
 * Access: Private
 */
static rtcDs1307TimeData_t rtcDs1307TimeData;

static uint8_t rtcDs1307Buffer[7];
static uint8_t rtcDs1307CtrlBuffer=0; 

static volatile bool rtcTimeReady=false;  //Bool flag to notify Clock and calendar is ready to be shifted 

/*
 * Function Prototypes
 * Note : These methods access I2C bus communication directly to DS1307
 * Access: Public
 */
extern bool DS1307_Conf_Init(rtcDs1307Control_t  *prtcDs1307Control);   //Init confoguration on control register
extern bool DS1307_Set_TimeDate(rtcDs1307UserData_t *prtcDs1307UserData);
extern bool DS1307_Read_TimeDate(rtcDs1307UserData_t *prtcDs1307UserData);

extern void DS1307_ISR(void);
extern bool DS1307_GetISR_FlagStatus(void);
extern bool DS1307_ClearsISR_Flag(void);

/*
 * Function Prototypes
 * Note : These methods only convert formats into readable ones by RTC or User
 * Access: Public
 */
static void __DS1307_Build_To_TimeDate(rtcDs1307TimeData_t *prtcDs1307TimeData,rtcDs1307UserData_t *prtcDs1307UserData);
static void __DS1307_Retreive_From_TimeDate(rtcDs1307UserData_t *prtcDs1307UserData, rtcDs1307TimeData_t *prtcDs1307TimeData );

#endif	/* DS1307_H */

