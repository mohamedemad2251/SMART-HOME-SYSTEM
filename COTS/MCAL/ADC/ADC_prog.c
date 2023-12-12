#include "../DIO/DIO.h"

#include "ADC_address.h"
#include "ADC_priv.h"
#include "ADC_config.h"
#include "ADC_int.h"

#if( ADC_VOLTAGE_SOURCE == AREF)
#define REFS_MODE	((0<<REFS1)|(0<<REFS0))
#elif( ADC_VOLTAGE_SOURCE == AVCC)
#define REFS_MODE	((0<<REFS1)|(1<<REFS0))
#elif( ADC_VOLTAGE_SOURCE == INTERNAL)
#define REFS_MODE	((1<<REFS1)|(1<<REFS0))
#endif

#if( ADC_ADJUSTMENT == RIGHT)
#define	ADLAR_MODE	(0<<ADLAR)
#elif( ADC_ADJUSTMENT == LEFT)
#define ADLAR_MODE	(1<<ADLAR)
#endif

#if( ADC_AUTO_TRIGGER == OFF)
#define ADATE_MODE	(0<<ADATE)
#elif( ADC_AUTO_TRIGGER == ON)
#define ADATE_MODE	(1<<ADATE)
#endif

#if( ADC_INTERRUPT == ADC_INTERRUPT_OFF)
#define ADIE_MODE	(0<<ADIE)
#elif( ADC_INTERRUPT == ADC_INTERRUPT_ON)
#define	ADIE_MODE	(1<<ADIE)
#endif

#if( ADC_PRESCALER == 2)
#define	ADPS_MODE	((0<<ADPS2)|(0<<ADPS1)|(0<<ADPS0))
#elif( ADC_PRESCALER == 4)
#define ADPS_MODE	((0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0))
#elif( ADC_PRESCALER == 8)
#define ADPS_MODE	((0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0))
#elif( ADC_PRESCALER == 16)
#define ADPS_MODE	((1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0))
#elif( ADC_PRESCALER == 32)
#define ADPS_MODE	((1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0))
#elif( ADC_PRESCALER == 64)
#define ADPS_MODE	((1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0))
#elif( ADC_PRESCALER == 128)
#define ADPS_MODE	((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0))
#endif

#define ADMUX_COMMAND	(REFS_MODE|ADLAR_MODE)
#define ADCSRA_COMMAND	(ADATE_MODE|ADIE_MODE|ADPS_MODE)

#if( ADC_INTERRUPT == ADC_INTERRUPT_ON)
static u16 GLOB_u16ADCData;
static u8 GLOB_u8ADCDataChannel = ADC0;
static u8 GLOB_u8ADCDataRecieved = FALSE;
static void (*GLOB_PvoidADCGptr)(void) = NULL;
#endif

STD_Type MCAL_ADC_u8Init(u8 LOC_u8ADCNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8ADCNo <= ADC7)
	{
		MCAL_DIO_u8SetPinDirection(ADC_PORT,LOC_u8ADCNo,DIO_INPUT);
		ADMUX = ADMUX_COMMAND | LOC_u8ADCNo;
		ADCSRA = ADCSRA_COMMAND;
		SET_BIT(ADCSRA,ADIF);
#if( ADC_INTERRUPT == ADC_INTERRUPT_ON)
		SET_BIT(SREG,I_BIT);	//Enable GIE, PIE is already handled in the macro "ADIE_MODE"
#endif
		SET_BIT(ADCSRA,ADEN);
		CLEAR_BIT(ADCSRA,ADSC);
		GLOB_u8ADCDataChannel = LOC_u8ADCNo;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_ADC_u8ChangeChannel(u8 LOC_u8ADCNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8ADCNo <= ADC7)
	{
		MCAL_DIO_u8SetPinDirection(ADC_PORT,LOC_u8ADCNo,DIO_INPUT);
		ADMUX &= CLEAR_ADC_SELECT;	//Clears MUX4..0 while masking the rest
		ADMUX |= LOC_u8ADCNo;		//Sets MUX4..0 as desired
#if( ADC_INTERRUPT == ADC_INTERRUPT_ON)
		SET_BIT(SREG,I_BIT);		//Enable GIE
		if(GLOB_u8ADCDataRecieved == TRUE)
		{
			GLOB_u8ADCDataChannel = LOC_u8ADCNo;
		}
#endif
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_ADC_u8StartConversion(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	SET_BIT(ADCSRA,ADSC);	//Start a conversion
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

#if( ADC_INTERRUPT == ADC_INTERRUPT_OFF)
STD_Type MCAL_ADC_u8GetConversion(u16 * LOC_Pu16ConversionValue)
#elif( ADC_INTERRUPT == ADC_INTERRUPT_ON)
STD_Type MCAL_ADC_u8GetConversion(u16 * LOC_Pu16ConversionValue,u8 * LOC_Pu8ADCDataChannel)
#endif
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu16ConversionValue != NULL)
	{
#if( ADC_INTERRUPT == ADC_INTERRUPT_OFF)

		while( (GET_BIT(ADCSRA,ADIF)) == FALSE);
		*LOC_Pu16ConversionValue = ((u16)(ADCL) | (u16) (ADCH<<ADC_REGISTER_SHIFT));
		SET_BIT(ADCSRA,ADIF);	//Clear Flag
#elif( ADC_INTERRUPT == ADC_INTERRUPT_ON)
		if(!GLOB_u8ADCDataRecieved)
		{
			*LOC_Pu16ConversionValue = GLOB_u16ADCData;
			*LOC_Pu8ADCDataChannel = GLOB_u8ADCDataChannel;
			GLOB_u8ADCDataRecieved = TRUE;
		}
		else
		{
			//Do Nothing.
		}
#endif
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

#if( ADC_INTERRUPT == ADC_INTERRUPT_ON)
STD_Type MCAL_ADC_u8CallbackFun(void (*LOC_PvoidPtr)(void))
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_PvoidPtr != NULL)
	{
		GLOB_PvoidADCGptr = LOC_PvoidPtr;
		LOC_u8ReturnValue = E_OK;
	}	
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

ISR(ADC_vect)
{
	GLOB_u8ADCDataRecieved = FALSE;
	GLOB_u16ADCData = ((u16)(ADCL) | (u16) (ADCH<<ADC_REGISTER_SHIFT));
	if(GLOB_PvoidADCGptr != NULL)
	{
		GLOB_PvoidADCGptr();
	}
}

#endif