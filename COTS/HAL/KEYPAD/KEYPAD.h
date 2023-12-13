#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../../MCAL/DIO/DIO.h"

#define KEYPAD_PORT DIO_PortB

#define KEYPAD_ROWS 4
#define KEYPAD_COLUMNS 4

#define KEYPAD_ROW0 Pin0
#define KEYPAD_ROW1 Pin1
#define KEYPAD_ROW2 Pin2
#define KEYPAD_ROW3 Pin3

#define KEYPAD_COLUMN0 Pin4
#define KEYPAD_COLUMN1 Pin5
#define KEYPAD_COLUMN2 Pin6
#define KEYPAD_COLUMN3 Pin7

#define FALSE 0
#define TRUE 1

STD_Type HAL_KEYPAD_u8Init(void);
STD_Type HAL_KEYPAD_u8GetButtonPressed(u8 * LOC_Pu8Char);

#endif /* KEYPAD_H_ */
