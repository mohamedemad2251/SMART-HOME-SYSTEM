#include "DIO.h"

STD_Type MCAL_DIO_u8SetPinDirection(u8 LOC_u8PortNo, u8 LOC_u8PinNo, u8 LOC_u8PinDirection)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_DDRD && LOC_u8PinNo <= Pin7 && LOC_u8PinDirection <= DIO_OUTPUT)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_DDRA:
				switch(LOC_u8PinDirection)
				{
					case DIO_INPUT: CLEAR_BIT(DDRA,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_OUTPUT: SET_BIT(DDRA,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
			case DIO_DDRB:
				switch(LOC_u8PinDirection)
				{
					case DIO_INPUT: CLEAR_BIT(DDRB,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_OUTPUT: SET_BIT(DDRB,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
			case DIO_DDRC:
				switch(LOC_u8PinDirection)
				{
					case DIO_INPUT: CLEAR_BIT(DDRC,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_OUTPUT: SET_BIT(DDRC,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
			case DIO_DDRD:
				switch(LOC_u8PinDirection)
				{
					case DIO_INPUT: CLEAR_BIT(DDRD,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_OUTPUT: SET_BIT(DDRD,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8SetPortDirection(u8 LOC_u8PortNo, u8 LOC_u8PortDirection)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <=DIO_DDRD)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_DDRA: DDRA = LOC_u8PortDirection;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_DDRB: DDRB = LOC_u8PortDirection;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_DDRC: DDRC = LOC_u8PortDirection;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_DDRD: DDRD = LOC_u8PortDirection;
				LOC_u8ReturnValue = E_OK;
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8SetPinValue(u8 LOC_u8PortNo, u8 LOC_u8PinNo, u8 LOC_u8PinValue)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PortD && LOC_u8PinNo <= Pin7 && LOC_u8PinValue <= DIO_HIGH)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_PortA:
				switch(LOC_u8PinValue)
				{
					case DIO_LOW: CLEAR_BIT(PORTA,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_HIGH: SET_BIT(PORTA,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
			case DIO_PortB:
				switch(LOC_u8PinValue)
				{
					case DIO_LOW: CLEAR_BIT(PORTB,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_HIGH: SET_BIT(PORTB,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
			case DIO_PortC:
				switch(LOC_u8PinValue)
				{
					case DIO_LOW: CLEAR_BIT(PORTC,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_HIGH: SET_BIT(PORTC,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
			case DIO_PortD:
				switch(LOC_u8PinValue)
				{
					case DIO_LOW: CLEAR_BIT(PORTD,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
					case DIO_HIGH: SET_BIT(PORTD,LOC_u8PinNo);
						LOC_u8ReturnValue = E_OK;
						break;
				}
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8SetPortValue(u8 LOC_u8PortNo, u8 LOC_u8PortValue)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PortD)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_PortA: PORTA = LOC_u8PortValue;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortB: PORTB = LOC_u8PortValue;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortC: PORTC = LOC_u8PortValue;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortD: PORTD = LOC_u8PortValue;
				LOC_u8ReturnValue = E_OK;
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8GetPinValue(u8 LOC_u8PortNo, u8 LOC_u8PinNo, u8 * LOC_Pu8PinValue)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PinD && LOC_u8PinNo <= Pin7)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_PinA: *LOC_Pu8PinValue = GET_BIT(PINA,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PinB: *LOC_Pu8PinValue = GET_BIT(PINB,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PinC: *LOC_Pu8PinValue = GET_BIT(PINC,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PinD: *LOC_Pu8PinValue = GET_BIT(PIND,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8GetPortValue(u8 LOC_u8PortNo, u8 * LOC_Pu8PortValue)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PinD)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_PinA: *LOC_Pu8PortValue = PINA;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PinB: *LOC_Pu8PortValue = PINB;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PinC: *LOC_Pu8PortValue = PINC;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PinD: *LOC_Pu8PortValue = PIND;
				LOC_u8ReturnValue = E_OK;
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8TogglePinValue(u8 LOC_u8PortNo, u8 LOC_u8PinNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PortD && LOC_u8PinNo <= Pin7)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_PortA: TOGGLE_BIT(PORTA,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortB: TOGGLE_BIT(PORTB,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortC: TOGGLE_BIT(PORTC,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortD: TOGGLE_BIT(PORTD,LOC_u8PinNo);
				LOC_u8ReturnValue = E_OK;
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_DIO_u8TogglePortValue(u8 LOC_u8PortNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PortD)
	{
		switch(LOC_u8PortNo)
		{
			case DIO_PortA: PORTA ^= 0xFF;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortB: PORTB ^= 0xFF;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortC: PORTC ^= 0xFF;
				LOC_u8ReturnValue = E_OK;
				break;
			case DIO_PortD: PORTD ^= 0xFF;
				LOC_u8ReturnValue = E_OK;
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}
