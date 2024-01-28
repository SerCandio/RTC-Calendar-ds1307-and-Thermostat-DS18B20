/* 
 * File:   custom_charLCD.h
 * Author: CURSOS
 *
 * Created on 25 de diciembre de 2023, 07:46 PM
 */

#ifndef CUSTOM_CHARLCD_H
#define	CUSTOM_CHARLCD_H

/*Memoery CGRAM symbol offset */
#define TEMP_DEG_SYMBOL         1U

/*
 * You can generate customs chars to be displayed on LCD on this web : https://maxpromer.github.io/LCD-Character-Creator/
 
 */

#include <stdint.h>

extern const uint8_t tempDegSymbol[8] = {
0x0E,
  0x0A,
  0x0E,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
 
};                                                           //Temperature ° symbol

//extern const uint8_t christmasTree[8] = {
//  0x00,
//  0x04,
//  0x04,
//  0x0E,
//  0x0E,
//  0x1F,
//  0x04,
//  0x04
//};                                                          //Christmas Tree
//
//extern const uint8_t smile[8]={0x00,0x00, 0x0A,0x00,0x1F,0x1F,0x0E,0x00}; //Smile :D
//
//extern const uint8_t christmasReindeer[8] = {
//  0x11,
//  0x1B,
//  0x0A,
//  0x1F,
//  0x0E,
//  0x0E,
//  0x0E,
//  0x00
//};                                                      //Christmas Reindeer

#endif	/* CUSTOM_CHARLCD_H */

