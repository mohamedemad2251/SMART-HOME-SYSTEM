#include "SERVO_int.h"
#include "SERVO_priv.h"
#include "SERVO_config.h"

#if(SERVO_LOGIC == SERVO_DELAY)
#include "../../MCAL/DIO/DIO.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#endif

#if(SERVO_LOGIC == SERVO_PWM)
STD_Type HAL_SERVO_u8Init()
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	MCAL_DIO_u8SetPinDirection(SERVO_PORT,SERVO_PIN,DIO_OUTPUT);
	MCAL_TIMER_u8Init(TIMER1);
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}
#elif(SERVO_LOGIC == SERVO_DELAY)
STD_Type HAL_SERVO_u8Init(u8 LOC_u8PortNo,u8 LOC_u8PinNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PortD && LOC_u8PinNo <= Pin7)
	{
		MCAL_DIO_u8SetPinDirection(LOC_u8PortNo,LOC_u8PinNo,DIO_OUTPUT);
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif

#if(SERVO_LOGIC == SERVO_PWM)
STD_Type HAL_SERVO_u8TurnDegree(u8 LOC_u8TurnDegree)	//Doesn't give accurate angles due to fraction neglection.
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8TurnDegree <= SERVO_MAX_DEG && LOC_u8TurnDegree >= SERVO_MIN_DEG)
	{
		MCAL_TIMER_u8SetTIMER1PWM(((SERVO_MIN_PWM*LOC_u8TurnDegree/SERVO_MAX_DEG)+(SERVO_MIN_PWM)),SERVO_FREQ);
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#elif(SERVO_LOGIC == SERVO_DELAY)
STD_Type HAL_SERVO_u8TurnDegree(u8 LOC_u8PortNo,u8 LOC_u8PinNo, u8 LOC_u8TurnDegree)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8PortNo <= DIO_PortD && LOC_u8PinNo <= Pin7)
	{
		switch(LOC_u8TurnDegree)
		{
			case SERVO_HARD_LEFT:
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_HIGH);
				_delay_us(SERVO_HARD_LEFT_TIME);
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_LOW);
				_delay_us((SERVO_PERIOD_TIME)-(SERVO_HARD_LEFT_TIME));
				break;
			case SERVO_LEFT:
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_HIGH);
				_delay_us(SERVO_LEFT_TIME);
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_LOW);
				_delay_us((SERVO_PERIOD_TIME)-(SERVO_LEFT_TIME));
				break;
			case SERVO_STRAIGHT:
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_HIGH);
				_delay_us(SERVO_STRAIGHT_TIME);
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_LOW);
				_delay_us((SERVO_PERIOD_TIME)-(SERVO_STRAIGHT_TIME));
				break;
			case SERVO_RIGHT:
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_HIGH);
				_delay_us(SERVO_RIGHT_TIME);
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_LOW);
				_delay_us((SERVO_PERIOD_TIME)-(SERVO_RIGHT_TIME));
				break;
			case SERVO_HARD_RIGHT:
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_HIGH);
				_delay_us(SERVO_HARD_RIGHT_TIME);
				MCAL_DIO_u8SetPinValue(LOC_u8PortNo,LOC_u8PinNo,DIO_LOW);
				_delay_us((SERVO_PERIOD_TIME)-(SERVO_HARD_RIGHT_TIME));
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
#endif
