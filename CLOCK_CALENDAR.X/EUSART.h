/* 
 * File:   EUSART.h
 * Author: CURSOS
 *
 * Created on 15 de diciembre de 2022, 12:18 PM
 */

#ifndef EUSART_H
#define	EUSART_H

#include <stdint.h>
#include <stdbool.h>
#include <pic18f45k50.h>
#include <xc.h>
#include "ConfBits_RealBoardPLL.h"

/* 
 * Punteros locales a funcion
 */
void (*pEusartFunISR)(void);

/*
 * Prototipos de funciones
 */
extern void EUSART_SerialBegin(uint32_t baudios);
extern void EUSART_WriteChar(char data);
extern void EUSART_WriteString(char *ptrString);

extern bool EUSART_Rx_Ready(void);
extern char EUSART_ReadChar(void);

extern void EUSART_AttachISR(void (*funISR)(void));
extern void EUSART_RxInterruptEnable(void);
extern void EUSART_RxInterruptDisable(void);

extern void EUSART_ISR(void);

void putch(char c);                     //Uso de printf()

#endif	/* EUSART_H */

