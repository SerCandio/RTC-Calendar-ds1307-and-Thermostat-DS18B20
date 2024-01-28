/* 
 * File:   EEPROM_24C32A.h
 * Author: CURSOS
 *
 * Created on 8 de enero de 2024, 05:03 PM
 */

#ifndef EEPROM_24C32A_H
#define	EEPROM_24C32A_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "I2C_Master.h"

/*
 * Supposing A0 , A1 and A2 are pulled to GND !
 * Remember on Write Mode 
 */
#define EEPROM_24C32_ADDR_WR_MODE               0xA0
#define EEPROM_24C32_ADDR_RD_MODE               0xA1

#define EEPROM_24C32_REG_MAP_SIZE               4096U

//For Page Write, 64 bytes maximum is allowed (8 pages of 8 bytes each), then STOP on I2C bus is mandatory
#define EEPROM_24C32_MAX_PAGE_SIZE              8U    

#define EEPROM_24C32_REG_MAP_BASE_ADDR          0x0000

/*
 * Function Prototypes
 * Access: Public
 * 
 * Note: All these functions will 
 *      
 * -> return FALSE  if no errors
 * -> return TRUE   if error   (No Write or Read was Posible)
 */

/*For writting a single byte at specific ADDR*/
extern bool EERPOM_Write_byte(uint8_t eepromSlaveADDR, uint16_t regMapADDR, uint8_t dataByte);

/*For writting a string of characters starting at specific ADDR*/
extern bool EEPROM_Write_Page(uint8_t eepromSlaveADDR, uint16_t regMapADDR, char *pString);   

/*For restroing to initial contents of EEPROM (0xFF)*/
extern bool EEPROM_Flush(uint8_t eepromSlaveADDR);

/*Read single byte at specific ADDR*/
extern bool EEPROM_Random_Read(uint8_t eepromRDSlaveADDR, uint16_t regMapADDR, uint8_t *pdataByteRead);

/*Read a group of bytes starting at sprcific ADDR*/
extern bool EEPROM_Sequential_Read(uint8_t eepromRDSlaveADDR, uint16_t regMapADDR, unsigned int numDataBytes, uint8_t *pdataByteList);

#endif	/* EEPROM_24C32A_H */

