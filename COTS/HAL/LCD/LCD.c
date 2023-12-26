#include "LCD.h"

static u8 GLOB_u8LCDCurrentXCoordinate;	//Used to remember cursor location
static u8 GLOB_u8LCDCurrentYCoordinate; //(Important for custom char)

#if (CUSTOM_CHAR_MODE == ON)
static u8 GLOB_u8CurrentCustomChar = 0x0;

u8 GLOB_u8CustomChar1[VERTICAL_PIXELS] = {
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b01001,
	0b01001,
	0b01111,
	0b00000
};

u8 GLOB_u8CustomChar2[VERTICAL_PIXELS] = {
	0b01000,
	0b10100,
	0b00010,
	0b11111,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

u8 GLOB_u8CustomChar3[VERTICAL_PIXELS] = {
		0b00000,
		0b00001,
		0b00001,
		0b01111,
		0b00000,
		0b00000,
		0b00000,
		0b00000
};

u8 GLOB_u8CustomChar4[VERTICAL_PIXELS] = {
	0b00000,
	0b00000,
	0b00000,
	0b11011,
	0b11111,
	0b11111,
	0b01110,
	0b00100
};
#endif

STD_Type HAL_LCD_u8Execute(u8 LOC_u8Data, u8 LOC_u8Mode)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8BitValue;
	u8 i = 0;
	if(LOC_u8Mode <= DATA)
	{
		switch(LOC_u8Mode)
		{
		case INSTRUCTION:
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LCD_CONTROL_PORT,LCD_RS_PIN,INSTRUCTION);
			break;
		case DATA:
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LCD_CONTROL_PORT,LCD_RS_PIN,DATA);
			break;
		}

		MCAL_DIO_u8SetPinValue(LCD_CONTROL_PORT,LCD_E_PIN,DIO_HIGH);
		if(LOC_u8Mode == INSTRUCTION)
		{
			_delay_ms(INSTRUCTION_TIME);
		}
		else
		{
			_delay_ms(DATA_TIME);
		}

		for(i;i<DB_SIZE;i++)
		{
			LOC_u8BitValue = GET_BIT(LOC_u8Data,i);
			switch(i)
			{
				case BIT_0:
					MCAL_DIO_u8SetPinValue(LCD_DATA_PORT,LCD_DB4_PIN,LOC_u8BitValue);
					break;
				case BIT_1:
					MCAL_DIO_u8SetPinValue(LCD_DATA_PORT,LCD_DB5_PIN,LOC_u8BitValue);
					break;
				case BIT_2:
					MCAL_DIO_u8SetPinValue(LCD_DATA_PORT,LCD_DB6_PIN,LOC_u8BitValue);
					break;
				case BIT_3:
					MCAL_DIO_u8SetPinValue(LCD_DATA_PORT,LCD_DB7_PIN,LOC_u8BitValue);
					break;
			}
		}

		MCAL_DIO_u8SetPinValue(LCD_CONTROL_PORT,LCD_E_PIN,DIO_LOW);
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type HAL_LCD_u8Init(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_DATA_PORT,LCD_DB4_PIN,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_DATA_PORT,LCD_DB5_PIN,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_DATA_PORT,LCD_DB6_PIN,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_DATA_PORT,LCD_DB7_PIN,DIO_OUTPUT);

	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_CONTROL_PORT,LCD_RS_PIN,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_CONTROL_PORT,LCD_RW_PIN,DIO_OUTPUT);
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LCD_CONTROL_PORT,LCD_E_PIN,DIO_OUTPUT);

	LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LCD_CONTROL_PORT,LCD_RW_PIN,WRITE);
	HAL_LCD_u8Execute(FOUR_BIT_INIT>>DB_SIZE,INSTRUCTION);
	HAL_LCD_u8Execute(FOUR_BIT_INIT,INSTRUCTION);
	//Addition in 123 and 124 lines
	HAL_LCD_u8Execute(FUNCTION_SET>>DB_SIZE,INSTRUCTION);
	HAL_LCD_u8Execute(FUNCTION_SET,INSTRUCTION);

	HAL_LCD_u8Execute(DISPLAY_CONTROL>>DB_SIZE,INSTRUCTION);
	HAL_LCD_u8Execute(DISPLAY_CONTROL,INSTRUCTION);

	HAL_LCD_u8Execute(DISPLAY_CLEAR>>DB_SIZE,INSTRUCTION);
	HAL_LCD_u8Execute(DISPLAY_CLEAR,INSTRUCTION);

	HAL_LCD_u8Execute(ENTRY_MODE>>DB_SIZE,INSTRUCTION);
	HAL_LCD_u8Execute(ENTRY_MODE,INSTRUCTION);

	_delay_ms(INIT_TIME);

	#if(CUSTOM_CHAR_MODE == ON)
	HAL_LCD_u8AddCustomCharacter(GLOB_u8CustomChar1);

	HAL_LCD_u8AddCustomCharacter(GLOB_u8CustomChar2);

	HAL_LCD_u8AddCustomCharacter(GLOB_u8CustomChar3);

	HAL_LCD_u8AddCustomCharacter(GLOB_u8CustomChar4);|
	#endif

	HAL_LCD_u8ClearScreen();

	return LOC_u8ReturnValue;
}

STD_Type HAL_LCD_u8WriteChar(u8 LOC_u8Char)
{
	STD_Type LOC_u8ReturnType = E_NOT_OK;
	if(LOC_u8Char)
	{
		LOC_u8ReturnType = HAL_LCD_u8Execute(LOC_u8Char>>DB_SIZE, DATA);
		LOC_u8ReturnType = HAL_LCD_u8Execute(LOC_u8Char, DATA);

		GLOB_u8LCDCurrentXCoordinate++;
		if(GLOB_u8LCDCurrentXCoordinate > MAX_X_COOR)
		{
			GLOB_u8LCDCurrentYCoordinate++;
			if(GLOB_u8LCDCurrentYCoordinate > MAX_Y_COOR)
			{
				GLOB_u8LCDCurrentYCoordinate = MIN_Y_COOR;
			}
			GLOB_u8LCDCurrentXCoordinate = MIN_X_COOR;
			LOC_u8ReturnType = HAL_LCD_u8GotoCursor(GLOB_u8LCDCurrentXCoordinate,GLOB_u8LCDCurrentYCoordinate);
		}
	}
	return LOC_u8ReturnType;
}

STD_Type HAL_LCD_u8WriteCommand(u8 LOC_u8Command)
{
	STD_Type LOC_u8ReturnType = E_NOT_OK;
	LOC_u8ReturnType = HAL_LCD_u8Execute(LOC_u8Command>>DB_SIZE, INSTRUCTION);
	LOC_u8ReturnType = HAL_LCD_u8Execute(LOC_u8Command, INSTRUCTION);
	return LOC_u8ReturnType;
}

STD_Type HAL_LCD_u8WriteString(u8 * LOC_u8String)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	while( (*LOC_u8String) != '\0')
	{
		LOC_u8ReturnValue = HAL_LCD_u8WriteChar(*LOC_u8String);
		LOC_u8String++;
	}
	return LOC_u8ReturnValue;
}

STD_Type HAL_LCD_u8GotoCursor(u8 LOC_u8XCoordinate,u8 LOC_u8YCoordinate)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8Command;
	if(LOC_u8XCoordinate <= MAX_X_COOR || LOC_u8YCoordinate <= MAX_Y_COOR)
	{
		switch(LOC_u8YCoordinate)
		{
			case ROW_0:
				LOC_u8Command = LCD_SCREEN_ROW0_START | LOC_u8XCoordinate;
				LOC_u8Command |= (1<<LCD_DB7_PIN);
				break;
			case ROW_1:
				LOC_u8Command = LCD_SCREEN_ROW1_START | LOC_u8XCoordinate;
				LOC_u8Command |= (1<<LCD_DB7_PIN);
				break;
		}
		LOC_u8ReturnValue = HAL_LCD_u8WriteCommand(LOC_u8Command);
		GLOB_u8LCDCurrentXCoordinate = LOC_u8XCoordinate;
		GLOB_u8LCDCurrentYCoordinate = LOC_u8YCoordinate;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;

}

STD_Type HAL_LCD_u8ClearScreen(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	LOC_u8ReturnValue = HAL_LCD_u8WriteCommand(DISPLAY_CLEAR);
	LOC_u8ReturnValue = HAL_LCD_u8GotoCursor(MIN_X_COOR,MIN_Y_COOR);
	return LOC_u8ReturnValue;
}

STD_Type HAL_LCD_u8WriteInteger(s32 LOC_s32Int)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u32 LOC_u32Base = BASE_TEN;
	u8 LOC_u8Digits = SINGLE_DIGIT;
	if(LOC_s32Int < ZERO)
	{
		LOC_u8ReturnValue = HAL_LCD_u8WriteChar('-');
	}

	while( (LOC_s32Int/LOC_u32Base) > ZERO )
	{
		LOC_u8Digits++;
		LOC_u32Base = LOC_u32Base * BASE_TEN;
	}
	if(LOC_u8Digits > SINGLE_DIGIT)
	{
		LOC_u32Base = LOC_u32Base / BASE_TEN;	//Base will be multiplied by one extra 10.
								//So we have to get rid of it
	}
	for(LOC_u8Digits;LOC_u8Digits>ZERO;LOC_u8Digits--)
	{
		if(LOC_u8Digits == SINGLE_DIGIT)
		{
			HAL_LCD_u8WriteChar( (LOC_s32Int%LOC_u32Base)+(ASCII));
		}
		else
		{
			HAL_LCD_u8WriteChar( (LOC_s32Int/LOC_u32Base)+(ASCII));
		}
		LOC_s32Int = (LOC_s32Int % LOC_u32Base);
		if(LOC_u32Base != BASE_TEN)
		{
			LOC_u32Base = LOC_u32Base / BASE_TEN;
		}
	}
	return LOC_u8ReturnValue;
}

#if(CUSTOM_CHAR_MODE == ON)
STD_Type HAL_LCD_u8AddCustomCharacter(u8 LOC_u8CustomChar[])
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8PixelRow = 0;
	LOC_u8ReturnValue = HAL_LCD_u8WriteCommand(GLOB_u8CurrentCustomChar | SET_CG_RAM_BITS);
	_delay_ms(CUSTOM_CHAR_TIME);
	for(LOC_u8PixelRow;LOC_u8PixelRow<VERTICAL_PIXELS;LOC_u8PixelRow++)
	{
		LOC_u8ReturnValue = HAL_LCD_u8WriteChar(LOC_u8CustomChar[LOC_u8PixelRow]);
	}
	LOC_u8ReturnValue = HAL_LCD_u8GotoCursor(GLOB_u8LCDCurrentXCoordinate,GLOB_u8LCDCurrentYCoordinate);
	GLOB_u8CurrentCustomChar+=VERTICAL_PIXELS;

	return LOC_u8ReturnValue;
}

STD_Type HAL_LCD_u8SendCustomChar(u8 LOC_u8CharacterCode)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8CharacterCode <= GLOB_u8CurrentCustomChar)
	{
		LOC_u8ReturnValue = HAL_LCD_u8WriteChar(LOC_u8CharacterCode);
		_delay_ms(CUSTOM_CHAR_TIME);
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif
