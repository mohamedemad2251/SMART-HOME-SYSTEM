#include "EXTI.h"

void (*GLOB_voidEXTI0Gptr)(void) = NULL;
void (*GLOB_voidEXTI1Gptr)(void) = NULL;
void (*GLOB_voidEXTI2Gptr)(void) = NULL;

STD_Type MCAL_EXTI_u8Init(u8 LOC_u8InterruptNo,u8 LOC_u8EdgeMode)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8InterruptNo <= EXTI2)
	{
		switch(LOC_u8InterruptNo)
		{
			case EXTI0:
				switch(LOC_u8EdgeMode)
				{
					case FALLING_EDGE:
						CLEAR_BIT(MCUCR,ISC00);
						SET_BIT(MCUCR,ISC01);	//Rising Edge Detection
						break;
					case RISING_EDGE:
						SET_BIT(MCUCR,ISC00);
						SET_BIT(MCUCR,ISC01);	//Rising Edge Detection
						break;
				}

				SET_BIT(GICR,INT0);		//Enable Interrupt 0 (PIE)
				break;
			case EXTI1:
				switch(LOC_u8EdgeMode)
				{
					case FALLING_EDGE:
					CLEAR_BIT(MCUCR,ISC10);
					SET_BIT(MCUCR,ISC11);	//Falling Edge Detection
					break;
					case RISING_EDGE:
					SET_BIT(MCUCR,ISC10);
					SET_BIT(MCUCR,ISC11);	//Rising Edge Detection
					break;
				}

				SET_BIT(GICR,INT1);		//Enable Interrupt 1 (PIE)
				break;
			case EXTI2:
				switch(LOC_u8EdgeMode)
				{
					case FALLING_EDGE:
						CLEAR_BIT(MCUCSR,ISC2);
						break;
					case RISING_EDGE:
						SET_BIT(MCUCSR,ISC2);	//Rising Edge Detection
						break;
				}

				SET_BIT(GICR,INT2);		//Enable Interrupt 2 (PIE)
				break;
		}
		SET_BIT(SREG,I_BIT);			//Enable General Interrupt (GIE)
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_EXTI_u8GlobalMode(u8 LOC_u8GlobMode)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8GlobMode <= EXTI_GIE_ON)
	{
		switch(LOC_u8GlobMode)
		{
			case EXTI_GIE_OFF:
				CLEAR_BIT(SREG,I_BIT);
				break;
			case EXTI_GIE_ON:
				SET_BIT(SREG,I_BIT);
				break;
		}
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_EXTI_u8InterruptMode(u8 LOC_u8InterruptNo, u8 LOC_u8IntMode)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8InterruptNo <= EXTI2 && LOC_u8IntMode <= EXTI_PIE_ON)
	{
		switch(LOC_u8InterruptNo)
		{
			case EXTI0:
				switch(LOC_u8IntMode)
				{
					case EXTI_PIE_OFF:
						CLEAR_BIT(GICR,INT0);
						break;
					case EXTI_PIE_ON:
						SET_BIT(GICR,INT0);
						break;
				}
				break;
			case EXTI1:
				switch(LOC_u8IntMode)
				{
					case EXTI_PIE_OFF:
						CLEAR_BIT(GICR,INT1);
						break;
					case EXTI_PIE_ON:
						SET_BIT(GICR,INT1);
						break;
				}
				break;
			case EXTI2:
				switch(LOC_u8IntMode)
				{
					case EXTI_PIE_OFF:
						CLEAR_BIT(GICR,INT2);
						break;
					case EXTI_PIE_ON:
						SET_BIT(GICR,INT2);
						break;
				}
				break;

		}
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_EXTI_u8CallbackFun(u8 LOC_u8InterruptNo, void (*LOC_Pu8Ptr)(void))
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu8Ptr != NULL && LOC_u8InterruptNo <= EXTI2)
	{
		switch(LOC_u8InterruptNo)
		{
			case EXTI0:	GLOB_voidEXTI0Gptr = LOC_Pu8Ptr;
				break;
			case EXTI1:	GLOB_voidEXTI1Gptr = LOC_Pu8Ptr;
				break;
			case EXTI2:	GLOB_voidEXTI2Gptr = LOC_Pu8Ptr;
				break;
		}
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

ISR(EXTI0_vect)
{
	if(GLOB_voidEXTI0Gptr != NULL)
	{
		GLOB_voidEXTI0Gptr();
	}
}

ISR(EXTI1_vect)
{
	if(GLOB_voidEXTI1Gptr != NULL)
	{
		GLOB_voidEXTI1Gptr();
	}
}

ISR(EXTI2_vect)
{
	if(GLOB_voidEXTI2Gptr != NULL)
	{
		GLOB_voidEXTI2Gptr();
	}
}
