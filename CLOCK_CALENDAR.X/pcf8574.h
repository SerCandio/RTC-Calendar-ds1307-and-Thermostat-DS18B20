/* 
 * File:   pcf8574.h
 * Author: CURSOS
 *
 * Created on 15 de octubre de 2023, 09:22 PM
 */

#ifndef PCF8574_H
#define	PCF8574_H

#include <stdint.h>
#include <stdbool.h>
#include  "I2C_Master.h"


#define PCF8574_LCD_HITACHHI_ADDR_WR_MODE       0x40 //A0, A1, A2 pulled to  GND...see datasheet p.13

/*
 * Function Prototypes
 * Access: Public
 */
extern bool pcf8574_SendData(uint8_t i2cSlaveAddr, uint8_t dataByte);

#endif	/* PCF8574_H */

