/* 
 * File:   Random.h
 * Author: CURSOS
 *
 * Created on 14 de octubre de 2023, 04:43 PM
 */

#ifndef RANDOM_H
#define	RANDOM_H

#include <stdint.h>
#include <time.h>

extern void Init_Random_Number(void);
extern uint16_t Get_Random_Number(uint16_t lowerLimit, uint16_t upperLimit); // See https://www.geeksforgeeks.org/generating-random-number-range-c/


#endif	/* RANDOM_H */

