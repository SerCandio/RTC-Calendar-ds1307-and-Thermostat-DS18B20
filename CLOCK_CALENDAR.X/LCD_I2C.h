/* 
 * File:   LCD_I2C.h
 * Author: CURSOS
 *
 * Created on 21 de diciembre de 2023, 02:42 PM
 */

#ifndef LCD_I2C_H
#define	LCD_I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "pcf8574.h"
#include "ConfBits_RealBoardPLL.h"

#define LCD_16_2_MAX_COLUMN_SIZE    16U

/* 
 * Bit definitions
 * Upon connection on paralel I/O port of PCF8574 controller
 * ex. P0-->>RS
 *     P2-->> EN
 *      .
 *      .
 */
#define RS  0U
#define E   2U

#define D4  4U
#define D5  5U
#define D6  6U
#define D7  7U

/* 
 * Global variables
 * Access: Private
 */
static uint8_t lcdParalellData=0x08;

void LCD_I2C_Cmd(uint8_t a);
void LCD_I2C_DataBus(uint8_t a);
void LCD_I2C_Init(void);
void LCD_I2C_SetCursor(uint8_t x , uint8_t y);
void LCD_I2C_Print_Char(char a);
void LCD_I2C_Print_String(char *a);
void LCD_I2C_New_Char(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t f,uint8_t g,uint8_t h,uint8_t i);
void LCD_I2C_Write_New_Char(uint8_t a);
void LCD_I2C_Clear(void);
void LCD_I2C_Home(void);


#endif	/* LCD_I2C_H */

