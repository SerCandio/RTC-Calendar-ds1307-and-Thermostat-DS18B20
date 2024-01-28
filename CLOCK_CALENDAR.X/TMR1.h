/* 
 * File:   TMR1.h
 * Author: CURSOS
 *
 * Created on 23 de mayo de 2023, 11:56 AM
 */

#ifndef TMR1_H
#define	TMR1_H

#include <pic18f45k50.h>
#include <xc.h>
#include <stdbool.h>

/* 
 * This defines the mode wether simulation or Real board.
 * Note: A XTAL of 32.768Khz cann't be simulated, rather implemented!
 *       Instead, we use a T1CKI on pin RC0 for simulation 
 * 
 * Note: You can also use 4/FOSC as internal clock for Timer 1, if FOSC change you must re-calculate!
 */

/* 
 * Macros para simulacion del Timer 1 o implementacion en placa fisica
 * OBS: Descomente solo 1 macro
 */
#define TMR1_SIMULATION_MODE
//#define TMR1_REAL_BOARD_MODE

/* 
 * Macros para la fuente de reloj del Timer 1
 * OBS: Descomente solo 1 macro
 */
//#define TMR1_4_FOSC
#define TMR1_RTC


//////////////////////////////////////////////////////////////////////////////////////

#if defined(TMR1_4_FOSC)

/* 
     * Periodo de 0.001 sec                     (T1CK = FOSC/4)
     * RELOAD_TMR1 = 65536 - ( (1ms * FOSC) / (4*PRE) )  ; si PRE =1:2 y FOSC =48MHz
     * RELOAD_TMR1 = 59536 (0xE890)
*/

#define TMR1_SOSCEN     0               //Oscilador secundario deshabilitado 
#define TMR1_CS         0b00            // Internal Clock FOSC/4
#define TMR1_CKPS       0b01            // Prescaler =1:2
#define RELOAD_TMR1H    0xE8       
#define RELOAD_TMR1L    0x90

#elif defined(TMR1_RTC)

/* 
     * Periodo de 0.001 sec                     (T1CK / SOSCO, SOSCI  = 32.768KHz)
     * RELOAD_TMR1 = 65536 - ( (1ms * 32.768Khz) / PRE )  ; si PRE =1:1
     * RELOAD_TMR1 = 65502 (0xFFDE)
*/

#if defined(TMR1_SIMULATION_MODE)
    #define TMR1_SOSCEN     0               //Oscilador secundario deshabilitado 
#elif defined(TMR1_REAL_BOARD_MODE)
    #define TMR1_SOSCEN     1               //Oscilador secundario Habilitado
    
#endif

#define TMR1_CS         0b10            // External clock on T1CKI pin(if SOSCEN=0) /SOSCO , SOSCI (if SOSCEN=1)
#define TMR1_CKPS       0b00            // Prescaler =1:1
#define RELOAD_TMR1H    0xFF       
#define RELOAD_TMR1L    0xDE

#endif

/* 
 * Global variables
 * Access : Private
 */
static bool _highPriority;


/*
 * Function Prototypes
 * Access: Public
 */
extern void TMR1_Initialize(bool highPriority);
extern void TMR1_Clear_Flag_Ovf(void);
extern void TMR1_Turn_On(void);
extern void TMR1_Turn_Off(void);

extern void TMR1_Enable_Interrupt(void);
extern void TMR1_Disable_Interrupt(void);
extern void TMR1_ISR(void);

#endif	/* TMR1_H */

