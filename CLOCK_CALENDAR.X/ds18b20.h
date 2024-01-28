/* 
 * File:   ds18b20.h
 * Author: CURSOS
 *
 * Created on 2 de enero de 2024, 11:06 AM
 */

#ifndef DS18B20_H
#define	DS18B20_H

#include <stdio.h>
#include <xc.h>
#include <pic18f45k50.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ConfBits_RealBoardPLL.h"
#include "TMR2.h"

// DS18B20 data pin is connected to pin RBD1
#define DS18B20_PIN_OUT      LATDbits.LATD1
#define DS18B20_PIN_INPUT    PORTDbits.RD1
#define DS18B20_PIN_DIR      TRISDbits.TRISD1

#define DS18B20_PRINT_BUFFER_SIZE       12

/*Please change this accordindly sensor needs*/
#define DS18B20_TH_USER_BYTE1           100U
#define DS18B20_TL_USER_BYTE2           7U

/*User resolution Masks*/
#define DS18B20_RESOLUTION_MASK_9BIT     0b00  // temperature resolution 0.5 °C
#define DS18B20_RESOLUTION_MASK_10BIT    0b01 // temperature resolution 0.25 °C
#define DS18B20_RESOLUTION_MASK_11BIT    0b10 // temperature resolution 0.125 °C
#define DS18B20_RESOLUTION_MASK_12BIT    0b11 // temperature resolution 0.0625 °C

/* Remember 12 bit resolution takes 750 mS to convert !!*/

/* 
 * Data structures
 * Access : Public
 * Please copy this member "ds18b20Config_t ds18b20Config "as a global variable
 */
typedef struct{
    uint8_t TH;
    uint8_t TL;
    uint8_t CONFIG;
}ds18b20Config_t;

/* 
 * Global variables
 * Access: Private
 */
static uint16_t _unsignedTemperatureRaw;
static uint8_t _fractConversionFactorIndex=DS18B20_RESOLUTION_MASK_12BIT; //12 bit as default resolution

/* 
 * Function Prototypes
 * Access : Private   ............Do not access!!
 */
static void __ds18b20_Start_Reset_Signal(void);
static bool __ds18b20_Check_Response(void);

static void __ds18b20_Write_bit(uint8_t value);
static void __ds18b20_Write_byte(uint8_t value);
static bool __ds18b20_Read_bit(void);
static  uint8_t __ds18b20_Read_Byte(void);

/* 
 * Function Prototypes
 * Access : Public ............ Call this function only
 */
extern void ds18b20_Init(void);
extern bool ds18b20_Configure(ds18b20Config_t *pds18b20Config);
extern bool ds18b20_Read_ROM(uint64_t *promCode);
extern bool ds18b20_Read_Raw_Temperature(uint16_t *rawTempValue); //Raw 16-bit word value
extern bool ds18b20_Read_Real_Temperature(char *realTempStr, int8_t *integerTemp); //Fractional string and integer value

#endif	/* DS18B20_H */

