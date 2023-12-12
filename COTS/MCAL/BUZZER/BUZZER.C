//#define F_CPU 16000000UL

#include "BUZZER.h"

void MCAL_BUZZER_u8BeepOneSecond(STD_Type LOC_u8ReturnType)
{
	if(LOC_u8ReturnType == E_NOT_OK)
	{
		MCAL_DIO_u8SetPinDirection(DIO_PortA,Pin3,DIO_HIGH);
		MCAL_DIO_u8SetPinValue(DIO_PortA,Pin3,DIO_HIGH);
		//_delay_ms(1000);
		MCAL_DIO_u8SetPinValue(DIO_PortA,Pin3,DIO_LOW);
	}
}
