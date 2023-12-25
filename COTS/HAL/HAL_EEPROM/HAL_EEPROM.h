/*
 * HAL_EEPROM.h
 *
 *  Created on: Nov 17, 2023
 *      Author: 20155
 */
#include "../../MCAL/MCAL_TWI/MCAL_TWI_Private.h"
#include "../../../DEFS/STD_types.h"

#ifndef HAL_HAL_EEPROM_HAL_EEPROM_H_
#define HAL_HAL_EEPROM_HAL_EEPROM_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define EEPROM_DELAY 2
#define CONTROL_CODE 0x50
#define PAGE_CONTROL 0x0700
#define EEPROM_LAST_ADDRESS 1023
#define EEPROM_SIZE 1024
#define EIGHT_BITS_SHIFTING 8

#ifndef ZERO
#define ZERO 0
#endif


u8 HAL_EEPROM_u8Init(void);
u8 HAL_EEPROM_u8WriteByte(u16 LOC_u16EEPROMDataAddress, u8 LOC_u8data);   /*Write char*/
u8 HAL_EEPROM_u8ReadByte(u16 LOC_u16EEPROMDataAddress, u8 * LOC_u8data);  /*Read char*/
u8 HAL_EEPROM_u8strlength(u8 LOC_u8str[]);
u8 HAL_EEPROM_u8WriteWord(u16 LOC_u16StartingLocation, u8 LOC_u8Word[]);  /*Write String only*/
u8 HAL_EEPROM_u8ReadWord (u16 LOC_u16StartingLocation, u16 LOC_u16ByteNum, u8 (*LOC_Pu8WordArray)[]);  /*Read String only*/
u8 HAL_EEPROM_u8ClearAllBytes(void);
u8 HAL_EEPROM_u8ClearBytes(u16 LOC_u16StartingLocation, u16 LOC_u16ByteNum);

#endif /* HAL_HAL_EEPROM_HAL_EEPROM_H_ */
