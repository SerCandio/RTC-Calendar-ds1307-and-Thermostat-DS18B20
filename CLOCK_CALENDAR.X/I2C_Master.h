/* 
 * File:   I2C_Master.h
 * Author: CURSOS
 *
 * Created on 16 de enero de 2023, 03:07 PM
 */

#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

#include <pic18f45k50.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include  "ConfBits_RealBoardPLL.h"

#define I2C_BIT_RATE_100KHZ         100000UL
#define I2C_BIT_RATE_400KHZ         400000UL

#define I2C_ACKSTAT_OK              false
#define I2C_ACKSTAT_FAIL            true

#define SEND_ACK                    true
#define SEND_NACK                   false


/*  
 * Function Prototypes
 * Access : Public
 */
extern void I2C_BeginMaster(uint32_t bitRateHz);
extern void I2C_StartMaster(void);
extern void I2C_StopMaster(void);
extern void I2C_ReStartMaster(void);
extern bool I2C_WriteMaster(uint8_t data);
extern bool I2C_WriteWordMaster(uint16_t data);
extern uint8_t I2C_ReadMaster(bool sendACK);

/*  
 * Function Prototypes
 * Access : Private
 */
static void I2C_SendACK_FromMaster(void);
static void I2C_SendNotACK_FromMaster(void);

#endif	/* I2C_MASTER_H */

