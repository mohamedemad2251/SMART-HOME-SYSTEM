#include "ULTRASONIC_priv.h"
#include "ULTRASONIC_int.h"
#include "ULTRASONIC_config.h"

#include "../../MCAL/DIO/DIO.h"
#include "../../MCAL/TIMER/TIMER_priv.h"
#define F_CPU 16000000UL
#include <util/delay.h>

#define ULTRASONIC_MAX_TIME 38
#define ULTRASONIC_SOUND_CONVERSION 29	//us per cm
#define ULTRASONIC_TRIGGER_PULSE_TIME 10	//us

static u8 GLOB_u8PulseTime;
static u8 GLOB_u8OverflowNo;
static u8 GLOB_u8TriggerPending;

void HAL_ULTRASONIC_voidTrigger(void)
{
	MCAL_DIO_u8SetPinValue(ULTRASONIC_TRIGGER_PORT,ULTRASONIC_TRIGGER_PIN,DIO_HIGH);
	_delay_us(ULTRASONIC_TRIGGER_PULSE_TIME);
	MCAL_DIO_u8SetPinValue(ULTRASONIC_TRIGGER_PORT,ULTRASONIC_TRIGGER_PIN,DIO_LOW);
}

void HAL_ULTRASONIC_voidTimerLogic(void)
{
	GLOB_u8OverflowNo++;	
}

void HAL_ULTRASONIC_voidPulseLogic(void)
{
	u8 LOC_u8CurrentTime = TCNT0;
	u16 LOC_u16Distance;
	static u8 LOC_u8PrevTime;
	static u8 LOC_u8EdgeMode = RISING_EDGE;	
	if(LOC_u8EdgeMode == RISING_EDGE)
	{
		TCNT0 = ZERO;
		LOC_u8CurrentTime = ZERO;
		MCAL_EXTI_u8Init(EXTI0,FALLING_EDGE);
		LOC_u8EdgeMode = FALLING_EDGE;
	}
	else if(LOC_u8EdgeMode == FALLING_EDGE)
	{
		if(LOC_u8CurrentTime > LOC_u8PrevTime)
		{
			GLOB_u8PulseTime = (LOC_u8CurrentTime - LOC_u8PrevTime) + (TIMER0_TICKS_MAX*GLOB_u8OverflowNo);
		}
		else
		{
			GLOB_u8PulseTime = (LOC_u8PrevTime - LOC_u8CurrentTime) + (TIMER0_TICKS_MAX*GLOB_u8OverflowNo);
		}
		LOC_u16Distance = (u16)(((u32)(GLOB_u8PulseTime)*TIMER0_PRESCALER/FREQ)/ULTRASONIC_SOUND_CONVERSION);
		LOC_u16Distance = (LOC_u16Distance) / 2;
		MCAL_EXTI_u8Init(EXTI0,RISING_EDGE);
		LOC_u8EdgeMode = RISING_EDGE;
		GLOB_u8TriggerPending = FALSE;
	}
	LOC_u8PrevTime = LOC_u8CurrentTime;
}

STD_Type HAL_ULTRASONIC_u8Init(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(ULTRASONIC_TRIGGER_PORT,ULTRASONIC_TRIGGER_PIN,DIO_OUTPUT);
	if(LOC_u8ReturnValue == E_OK)
	{
		LOC_u8ReturnValue = MCAL_EXTI_u8Init(EXTI0,RISING_EDGE);
		if(LOC_u8ReturnValue == E_OK)
		{
			LOC_u8ReturnValue = MCAL_TIMER_u8Init(TIMER0);
			if(LOC_u8ReturnValue == E_OK)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8CallbackMilliFun(TIMER0,ULTRASONIC_MAX_TIME,&HAL_ULTRASONIC_voidTimerLogic);
				TCNT0 = 0;
				if(LOC_u8ReturnValue == E_OK)
				{
					LOC_u8ReturnValue = MCAL_EXTI_u8CallbackFun(EXTI0,&HAL_ULTRASONIC_voidPulseLogic);
					HAL_ULTRASONIC_voidTrigger();
					GLOB_u8TriggerPending = TRUE;
				}
			}
		}
	}
	
	return LOC_u8ReturnValue;
}

STD_Type HAL_ULTRASONIC_u8GetDistance(u16 * LOC_u16Distance, u8 * LOC_u8TriggerPending)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	*LOC_u16Distance = (u16)(((u32)(GLOB_u8PulseTime)*TIMER0_PRESCALER/FREQ)/ULTRASONIC_SOUND_CONVERSION);
	*LOC_u16Distance = (*LOC_u16Distance) / 2;
	*LOC_u8TriggerPending = GLOB_u8TriggerPending;
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}