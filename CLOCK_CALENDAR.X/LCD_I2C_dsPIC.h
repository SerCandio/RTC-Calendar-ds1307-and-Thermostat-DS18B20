/* 
 * File:   LCD_I2C_dsPIC.h
 * Author: LENOVO
 *
 * Created on 15 de agosto de 2021, 12:20 PM
 */

#ifndef LCD_I2C_DSPIC_H
#define	LCD_I2C_DSPIC_H
#include <stdint.h>
#include <xc.h>
#include "pcf8574.h"


/* 
 * Bit definitions
 * Upon connection on paralel I/O port of PCF8574 controller
 * ex. P0-->>RS
 *     P2-->> EN
 *      .
 *      .
 */
#define E   2U
#define RS  0U
#define D4  4U
#define D5  5U
#define D6  6U
#define D7  7U

/* 
 * Global variables
 * Access: Private
 */
static uint8_t lcdParalellData=0x00;

void LCD_I2C_dsPIC_Cmd(uint8_t a);
void LCD_I2C_dsPIC_DataBus(uint8_t a);
void LCD_I2C_dsPIC_Init(void);
void LCD_I2C_dsPIC_SetCursor(uint8_t x , uint8_t y);
void LCD_I2C_dsPIC_Print_Char(char a);
void LCD_I2C_dsPIC_Print_String(char *a);
void LCD_I2C_dsPIC_New_Char(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t f,uint8_t g,uint8_t h,uint8_t i);
void LCD_I2C_dsPIC_Write_New_Char(uint8_t a);
void LCD_I2C_dsPIC_Clear(void);
void LCD_I2C_dsPIC_Home(void);

#endif	/* LCD_I2C_DSPIC_H */

