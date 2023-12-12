#include "KEYPAD.h"
#include <util/delay.h>

static u8 GLOB_u8CharArray[KEYPAD_ROWS][KEYPAD_COLUMNS] = {{'7','8','9','/'},
{'4','5','6','x'},
{'1','2','3','-'},
{'!','0','=','+'}};

STD_Type HAL_KEYPAD_u8TraverseColumns(u8 LOC_u8Value)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	//if(LOC_u8Value >= KEYPAD_COLUMN0 && LOC_u8Value <= KEYPAD_COLUMN3)
	//{
		switch(LOC_u8Value)
		{
			case KEYPAD_COLUMN0:
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN0,DIO_HIGH);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN1,DIO_LOW);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN2,DIO_LOW);
				LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN3,DIO_LOW);
				break;
			case KEYPAD_COLUMN1:
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN0,DIO_LOW);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN1,DIO_HIGH);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN2,DIO_LOW);
				LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN3,DIO_LOW);
				break;
			case KEYPAD_COLUMN2:
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN0,DIO_LOW);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN1,DIO_LOW);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN2,DIO_HIGH);
				LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN3,DIO_LOW);
				break;
			case KEYPAD_COLUMN3:
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN0,DIO_LOW);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN1,DIO_LOW);
				MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN2,DIO_LOW);
				LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN3,DIO_HIGH);
				break;
		}
	//}
	//else
	//{
		//Do Nothing.
	//}
	return LOC_u8ReturnValue;
}

STD_Type HAL_KEYPAD_u8Init(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN0,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN1,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN2,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN3,DIO_OUTPUT);

	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_ROW0,DIO_INPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_ROW1,DIO_INPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_ROW2,DIO_INPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(KEYPAD_PORT,KEYPAD_ROW3,DIO_INPUT);

	//LOC_u8ReturnValue = MCAL_DIO_u8SetPortValue(KEYPAD_PORT,DIO_ALL_HIGH); //Pull-Up Resistors & HIGH Output Values
	return LOC_u8ReturnValue;
}

STD_Type HAL_KEYPAD_u8GetButtonPressed(u8 * LOC_Pu8Char)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8Column = KEYPAD_COLUMN0; //START OF COLUMNS
	u8 LOC_u8Row = KEYPAD_ROW0;	//START OF ROWS
	u8 LOC_u8ButtonPressed = FALSE;
	*LOC_Pu8Char = '\0';

	for(u8 i = 4;i<8;i++)
	{
		LOC_u8ReturnValue = HAL_KEYPAD_u8TraverseColumns(i);
		for(u8 j=0;j<4;j++)
		{
			LOC_u8ReturnValue = MCAL_DIO_u8GetPinValue(KEYPAD_PORT,j,(&LOC_u8ButtonPressed));
			_delay_ms(40);
			if(LOC_u8ButtonPressed)
			{
				*LOC_Pu8Char = GLOB_u8CharArray[j][i-4];	//i.e. start of columns = 4, so we need to shift it to 0
				return LOC_u8ReturnValue;
			}
			else
			{
				*LOC_Pu8Char = '\0';
			}
		}
	}
	return LOC_u8ReturnValue;
}
