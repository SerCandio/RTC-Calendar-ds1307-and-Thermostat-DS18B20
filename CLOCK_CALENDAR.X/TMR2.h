/* 
 * File:   TMR2.h
 * Author: CURSOS
 *
 * Created on 10 de noviembre de 2023, 08:04 PM
 */

#ifndef TMR2_H
#define	TMR2_H

#include <xc.h>
#include <pic18f45k50.h>

/*
 * Function Prototypes
 * Access: Public
 */
extern void Timer2_Initialize(void);
extern void Timer2_Turn_On(void);
extern void Timer2_Turn_Off(void);

#endif	/* TMR2_H */

