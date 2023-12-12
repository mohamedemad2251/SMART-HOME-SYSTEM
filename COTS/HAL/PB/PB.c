#include "PB.h"

static u8 LOC_u8PB0Init = FALSE;
static u8 LOC_u8PB1Init = FALSE;
static u8 LOC_u8PB2Init = FALSE;

STD_Type HAL_PB_u8Init(u8 LOC_u8ButtonNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8ButtonNo <= PB2)
	{
		switch(LOC_u8ButtonNo)
		{
		case PB0: MCAL_DIO_u8SetPinDirection(PB0_PORT,PB0_PIN,DIO_INPUT);
			LOC_u8PB0Init = TRUE;
			break;
		case PB1: MCAL_DIO_u8SetPinDirection(PB1_PORT,PB1_PIN,DIO_INPUT);
			LOC_u8PB1Init = TRUE;
			break;
		case PB2: MCAL_DIO_u8SetPinDirection(PB2_PORT,PB2_PIN,DIO_INPUT);
			LOC_u8PB2Init = TRUE;
			break;
		}
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}


STD_Type HAL_PB_u8GetButtonPressed(u8 LOC_u8ButtonNo,u8 * LOC_Pu8ButtonPressed)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	static u8 LOC_u8PB0SameStateCount;
	static u8 LOC_u8PB1SameStateCount;
	static u8 LOC_u8PB2SameStateCount;

	static u8 LOC_u8PB0PrevState = FALSE;
	static u8 LOC_u8PB1PrevState = FALSE;
	static u8 LOC_u8PB2PrevState = FALSE;

	static u8 LOC_u8PB0HoldFlag = FALSE;
	static u8 LOC_u8PB1HoldFlag = FALSE;
	static u8 LOC_u8PB2HoldFlag = FALSE;

	u8 LOC_u8PB0CurrentState = FALSE;
	u8 LOC_u8PB1CurrentState = FALSE;
	u8 LOC_u8PB2CurrentState = FALSE;

	if(LOC_u8ButtonNo <= PB2)
	{
		switch(LOC_u8ButtonNo)
		{
			case PB0:
				if(LOC_u8PB0Init == TRUE)
				{
					MCAL_DIO_u8GetPinValue(PB0_PORT,PB0_PIN, &LOC_u8PB0CurrentState);
					if(LOC_u8PB0CurrentState == LOC_u8PB0PrevState && LOC_u8PB0CurrentState == TRUE)
					{
						LOC_u8PB0SameStateCount++;
						if(LOC_u8PB0SameStateCount == PB_MAX_COUNT && LOC_u8PB0HoldFlag == FALSE)
						{
							*LOC_Pu8ButtonPressed = TRUE;
							LOC_u8PB0HoldFlag = TRUE;
						}
						else
						{
							*LOC_Pu8ButtonPressed = FALSE;
						}
					}
					else
					{
						LOC_u8PB0SameStateCount = 0;
						*LOC_Pu8ButtonPressed = FALSE;
						LOC_u8PB0HoldFlag = FALSE;
					}
					LOC_u8PB0PrevState = LOC_u8PB0CurrentState;
				}
				break;
			case PB1:
				if(LOC_u8PB1Init == TRUE)
				{
					MCAL_DIO_u8GetPinValue(PB1_PORT,PB1_PIN, &LOC_u8PB1CurrentState);
					if(LOC_u8PB1CurrentState == LOC_u8PB1PrevState && LOC_u8PB1CurrentState == TRUE)
					{
						LOC_u8PB1SameStateCount++;
						if(LOC_u8PB1SameStateCount == PB_MAX_COUNT && LOC_u8PB1HoldFlag == FALSE)
						{
							*LOC_Pu8ButtonPressed = TRUE;
							LOC_u8PB1HoldFlag = TRUE;
						}
						else
						{
							*LOC_Pu8ButtonPressed = FALSE;
						}
					}
					else
					{
						LOC_u8PB1SameStateCount = 0;
						*LOC_Pu8ButtonPressed = FALSE;
						LOC_u8PB1HoldFlag = FALSE;
					}
					LOC_u8PB1PrevState = LOC_u8PB1CurrentState;
				}
				break;
			case PB2:
				if(LOC_u8PB2Init == TRUE)
				{
					MCAL_DIO_u8GetPinValue(PB2_PORT,PB2_PIN, &LOC_u8PB2CurrentState);
					if(LOC_u8PB2CurrentState == LOC_u8PB2PrevState && LOC_u8PB2CurrentState == TRUE)
					{
						LOC_u8PB2SameStateCount++;
						if(LOC_u8PB2SameStateCount == PB_MAX_COUNT && LOC_u8PB2HoldFlag == FALSE)
						{
							*LOC_Pu8ButtonPressed = TRUE;
							LOC_u8PB2HoldFlag = TRUE;
						}
						else
						{
							*LOC_Pu8ButtonPressed = FALSE;
						}
					}
					else
					{
						LOC_u8PB2SameStateCount = 0;
						*LOC_Pu8ButtonPressed = FALSE;
						LOC_u8PB2HoldFlag = FALSE;
					}
					LOC_u8PB2PrevState = LOC_u8PB2CurrentState;
				}
				break;
		}


	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
