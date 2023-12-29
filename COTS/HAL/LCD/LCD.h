#ifndef LCD_H_
#define LCD_H_

#ifndef	F_CPU
#define F_CPU 16000000UL
#endif
#include "../../MCAL/DIO/DIO.h"
#include <util/delay.h>

#define OFF 0
#define ON 1

#define CUSTOM_CHAR_MODE OFF

#define LCD_DATA_PORT DIO_PortA
#define LCD_CONTROL_PORT DIO_PortA
#define LCD_DB4_PIN Pin4
#define LCD_DB5_PIN Pin5
#define LCD_DB6_PIN Pin6
#define LCD_DB7_PIN Pin7
#define LCD_RS_PIN Pin1
#define LCD_RW_PIN Pin2
#define LCD_E_PIN Pin3

#define DATA DIO_HIGH
#define INSTRUCTION DIO_LOW

#define DATA_TIME 1 //ms
#define INSTRUCTION_TIME 1 //ms
#define INIT_TIME 50	//ms
#define CUSTOM_CHAR_TIME 10 //ms

#define READ DIO_HIGH
#define WRITE DIO_LOW

#define FOUR_BIT_INIT 0b00000010
#define FUNCTION_SET 0b00101000
#define DISPLAY_CONTROL 0b00001100
#define DISPLAY_CLEAR 0b00000001
#define ENTRY_MODE 0b00000110

#define DB_SIZE 4

#define BIT_0 0
#define BIT_1 1
#define BIT_2 2
#define BIT_3 3

#define MIN_X_COOR 0
#define MIN_Y_COOR 0
#define MAX_X_COOR 15
#define MAX_Y_COOR 1

#define LCD_SCREEN_ROW0_START 0x00
#define LCD_SCREEN_ROW1_START 0x40
#define LCD_SCREEN_ROW0_END 0x07
#define LCD_SCREEN_ROW1_END 0x47
#define LCD_SCREEN_X_MIDDLE 7

#define ROW_0 0
#define ROW_1 1

#define COL_0 0
#define COL_1 1
#define COL_2 2
#define COL_3 3
#define COL_4 4
#define COL_5 5
#define COL_6 6
#define COL_7 7
#define COL_8 8
#define COL_9 9
#define COL_10 10
#define COL_11 11
#define COL_12 12
#define COL_13 13
#define COL_14 14
#define COL_15 15

#define VERTICAL_PIXELS 8

#if (CUSTOM_CHAR_MODE == ON)
#define M_ARABIC 0x0
#define H_ARABIC 0x1
#define D_ARABIC 0x2
#define HEART 0x3

#define SET_CG_RAM_BITS 0b01000000
#endif

#define BASE_TEN 10
#define ZERO 0
#define SINGLE_DIGIT 1

#define ASCII 0x30

STD_Type HAL_LCD_u8Init(void);
STD_Type HAL_LCD_u8WriteChar(u8 LOC_u8Char);
STD_Type HAL_LCD_u8WriteCommand(u8 LOC_u8Command);
STD_Type HAL_LCD_u8WriteString(u8 * LOC_u8String);
STD_Type HAL_LCD_u8GotoCursor(u8 LOC_u8XCoordinate,u8 LOC_u8YCoordinate);
STD_Type HAL_LCD_u8ClearScreen(void);
STD_Type HAL_LCD_u8WriteInteger(s32 LOC_s32Int);

#if (CUSTOM_CHAR_MODE == ON)
STD_Type HAL_LCD_u8AddCustomCharacter(u8 LOC_u8CustomChar[]);
STD_Type HAL_LCD_u8SendCustomChar(u8 LOC_u8CharacterCode);
#endif



#endif /* LCD_H_ */