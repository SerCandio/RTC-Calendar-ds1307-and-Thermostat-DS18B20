/* 
 * File:   INTERRUPT_Controller.h
 * Author: CURSOS
 *
 * Created on 5 de noviembre de 2023, 07:24 PM
 */

#ifndef INTERRUPT_CONTROLLER_H
#define	INTERRUPT_CONTROLLER_H

#include <stdint.h>
#include <xc.h>
#include <pic18f45k50.h>
#include "TMR1.h"
#include "millis.h"
#include "EUSART.h"
#include "DS1307.h"
/*Descomentar solo si se va a trabajr con USB*/
//#include "mcc_generated_files/mcc.h" 

extern void INTERRUUPT_Init(void);

extern void INTERRUPT_Global_Enable_HighPriority(void);
extern void INTERRUPT_Global_Disable_HigPriority(void);

extern void INTERRUPT_Global_Enable_LowPriority(void);
extern void INTERRUPT_Global_Disable_LowPriority(void);

#endif	/* INTERRUPT_CONTROLLER_H */

