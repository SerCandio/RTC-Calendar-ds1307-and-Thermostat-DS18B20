/* 
 * File:   millis.h
 * Author: CURSOS
 *
 * Created on 26 de octubre de 2023, 10:38 AM
 */

#ifndef MILLIS_H
#define	MILLIS_H

#include <stdint.h>
#include <stdbool.h>
#include "TMR1.h"

//Macro Timer Initializer
#define Timer_Init(highPriority)        TMR1_Initialize(highPriority)

#define Power_TimerEnable()             TMR1_Turn_On()
#define Power_TimerDisable()            TMR1_Turn_Off()

#define millis()                        Millis_Get()

/* 
 * Global variables and type-definitions
 * Access : Public
 */
typedef uint32_t millis_t;
static volatile millis_t milliseconds=0;

/* 
 * Function Prototypes
 * Acess: Public
 */
extern void Millis_Init(bool highPriority);
extern millis_t Millis_Get(void);
extern void MillisISR(void);

/* 
 * Function Prototypes
 * Acess: Private
 */
static void Millis_Resume(void);
static void Millis_Pause(void);
static void Millis_Reset(void);

#endif	/* MILLIS_H */

