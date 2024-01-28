/* 
 * File:   Alarms.h
 * Author: CURSOS
 *
 * Created on 9 de enero de 2024, 03:21 PM
 */

#ifndef ALARMS_H
#define	ALARMS_H

#include <stdint.h>

/*
 * This header file is for configuring alarms of magnitude of any type, 
 * You can define your own data types for alarms here in order to be included
 * as global variables
 
 */

#define ALARM_EEPROM_DATAGRAM_SIZE      7  // According to # of member of 'timeUserAlarm_t'

/* 
 * Data strcutures 
 * Access: Public
 * Thus data structure sets your alarm in order to do something passed certain time
 */
typedef struct{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    int8_t highTemperatureLimit;
    int8_t lowTemperatureLimit;
    uint8_t timeAlarmDuration;
    uint8_t temperatureAlarmOffset;
}userAlarm_t;

#endif	/* ALARMS_H */

