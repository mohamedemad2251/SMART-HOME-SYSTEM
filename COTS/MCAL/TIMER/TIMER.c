#include "../../MCAL/DIO/DIO.h"
#include "TIMER_priv.h"
#include "TIMER_config.h"
#include "TIMER.h"

#ifdef TIMER0_INTERRUPT
#if(TIMER0_INTERRUPT == TIMER_INTERRUPT_ON && TIMER0_MODE == PWM)
#undef TIMER0_INTERRUPT
#define TIMER0_INTERRUPT TIMER_INTERRUPT_OFF
#endif
#endif

#ifdef TIMER1_INTERRUPT
#if(TIMER1_INTERRUPT == TIMER_INTERRUPT_ON && TIMER1_MODE == PWM)
#undef TIMER1_INTERRUPT
#define TIMER1_INTERRUPT TIMER_INTERRUPT_OFF
#endif
#endif

#ifdef TIMER2_INTERRUPT
#if(TIMER2_INTERRUPT == TIMER_INTERRUPT_ON && TIMER2_MODE == PWM)
#undef TIMER2_INTERRUPT
#define TIMER2_INTERRUPT TIMER_INTERRUPT_OFF
#endif
#endif

#ifdef TIMER1_RESOLUTION
#if(TIMER1_RESOLUTION == TEN_BIT)
#define TIMER1_PWM_TOP 0x03FF	//1023 in Decimal
#endif
#endif

#if(TIMER0_CTC_OPTIMIZE == ACCURACY)
#define TIMER0_MIN_OPTIMIZATION_VALUE 1
#elif(TIMER0_CTC_OPTIMIZE == PERFORMANCE)
#define TIMER0_MIN_OPTIMIZATION_VALUE 200
#endif

#if(TIMER1_CTC_OPTIMIZE == ACCURACY)
#define TIMER1_MIN_OPTIMIZATION_VALUE 1
#elif(TIMER1_CTC_OPTIMIZE == PERFORMANCE)
#define TIMER1_MIN_OPTIMIZATION_VALUE 51400	//(200/255)*(65535) to have
//the same ratio as TIMER0
#endif

#if(TIMER2_CTC_OPTIMIZE == ACCURACY)
#define TIMER2_MIN_OPTIMIZATION_VALUE 1
#elif(TIMER2_CTC_OPTIMIZE == PERFORMANCE)
#define TIMER2_MIN_OPTIMIZATION_VALUE 200
#endif

#if(TIMER0_PRESCALER != 0 && TIMER0_PRESCALER != 8 && TIMER0_PRESCALER != 64 && TIMER0_PRESCALER != 256 && TIMER0_PRESCALER != 1024)
#error "Wrong prescaler for TIMER0, please re-configure!"
#endif
#if(TIMER1_PRESCALER != 0 && TIMER1_PRESCALER != 8 && TIMER1_PRESCALER != 64 && TIMER1_PRESCALER != 256 && TIMER1_PRESCALER != 1024)
#error "Wrong prescaler for TIMER1, please re-configure!"
#endif
#if(TIMER2_PRESCALER != 0 && TIMER2_PRESCALER != 8 && TIMER2_PRESCALER != 64 && TIMER2_PRESCALER != 256 && TIMER2_PRESCALER != 1024)
#error "Wrong prescaler for TIMER2, please re-configure!"
#endif

#if(TIMER0_MODE == NORMAL)
#define WGM0_MODE 0b00000000
#elif(TIMER0_MODE == CTC)
#define WGM0_MODE 0b00001000
#elif(TIMER0_MODE == PHASE_CORRECT)
#define WGM0_MODE 0b01000000
#elif(TIMER0_MODE == PWM)
#define WGM0_MODE 0b01001000
#endif

#if(TIMER1_MODE == NORMAL)
#define WGM1_10_MODE 0b00000000
#define WGM1_23_MODE 0b00000000
#elif(TIMER1_MODE == CTC)
#define WGM1_10_MODE 0b00000000
#define WGM1_23_MODE 0b00001000
#elif(TIMER1_MODE == PHASE_CORRECT)
#define WGM1_10_MODE 0b00000001
#define WGM1_23_MODE 0b00000000
#elif(TIMER1_MODE == PWM)
#if(TIMER1_RESOLUTION == EIGHT_BIT)
#define WGM1_10_MODE 0b00000001
#define WGM1_23_MODE 0b00001000
#elif(TIMER1_RESOLUTION == COMPARE_MATCH)
#define WGM1_10_MODE 0b00000011
#define WGM1_23_MODE 0b00011000
#elif(TIMER1_RESOLUTION == TEN_BIT)
#define WGM1_10_MODE 0b00000011
#define WGM1_23_MODE 0b00001000
#endif
#endif

#if(TIMER2_MODE == NORMAL)
#define WGM2_MODE 0b00000000
#elif(TIMER2_MODE == CTC)
#define WGM2_MODE 0b00001000
#elif(TIMER2_MODE == PHASE_CORRECT)
#define WGM2_MODE 0b01000000
#elif(TIMER2_MODE == PWM)
#define WGM2_MODE 0b01001000
#endif

#if(TIMER0_PRESCALER == 0)
#define CS0_MODE 0b00000001
#elif(TIMER0_PRESCALER == 8)
#define CS0_MODE 0b00000010
#elif(TIMER0_PRESCALER == 64)
#define CS0_MODE 0b00000011
#elif(TIMER0_PRESCALER == 256)
#define CS0_MODE 0b00000100
#elif(TIMER0_PRESCALER == 1024)
#define CS0_MODE 0b00000101
#endif

#if(TIMER1_PRESCALER == 1)
#define CS1_MODE 0b00000001
#elif(TIMER1_PRESCALER == 8)
#define CS1_MODE 0b00000010
#elif(TIMER1_PRESCALER == 64)
#define CS1_MODE 0b00000011
#elif(TIMER1_PRESCALER == 256)
#define CS1_MODE 0b00000100
#elif(TIMER1_PRESCALER == 1024)
#define CS1_MODE 0b00000101
#endif

#if(TIMER2_PRESCALER == 1)
#define CS2_MODE 0b00000001
#elif(TIMER2_PRESCALER == 8)
#define CS2_MODE 0b00000010
#elif(TIMER2_PRESCALER == 32)
#define CS2_MODE 0b00000011
#elif(TIMER2_PRESCALER == 64)
#define CS2_MODE 0b00000100
#elif(TIMER2_PRESCALER == 128)
#define CS2_MODE 0b00000101
#elif(TIMER2_PRESCALER == 256)
#define CS2_MODE 0b00000110
#elif(TIMER2_PRESCALER == 1024)
#define CS2_MODE 0b00000111
#endif

#if(OC0_MODE == OC0_OFF)
#define COM0_MODE 0b00000000
#elif(TIMER0_MODE != PWM && TIMER0_MODE != PHASE_CORRECT && OC0_MODE == OC0_TOGGLE)
#define COM0_MODE 0b00010000
#elif(OC0_MODE == OC0_CLEAR)
#define COM0_MODE 0b00100000
#elif(OC0_MODE == OC0_SET)
#define COM0_MODE 0b00110000
#endif

#if(OC1A_MODE == OC1A_OFF)
#define COM1A_MODE 0b00000000
#elif(OC1A_MODE == OC1A_TOGGLE)
#define COM1A_MODE 0b01000000
#elif(OC1A_MODE == OC1A_CLEAR)
#define COM1A_MODE 0b10000000
#elif(OC1A_MODE == OC1A_SET)
#define COM1A_MODE 0b11000000
#endif

#if(OC1B_MODE == OC1B_OFF)
#define COM1B_MODE 0b00000000
#elif(OC1B_MODE == OC1B_TOGGLE)
#define COM1B_MODE 0b00010000
#elif(OC1B_MODE == OC1B_CLEAR)
#define COM1B_MODE 0b00100000
#elif(OC1B_MODE == OC1B_SET)
#define COM1B_MODE 0b00110000
#endif

#if(OC2_MODE == OC2_OFF)
#define COM2_MODE 0b00000000
#elif(TIMER2_MODE != PWM && TIMER2_MODE != PHASE_CORRECT && OC2_MODE == OC2_TOGGLE)
#define COM2_MODE 0b00010000
#elif(OC2_MODE == OC2_CLEAR)
#define COM2_MODE 0b00100000
#elif(OC2_MODE == OC2_SET)
#define COM2_MODE 0b00110000
#endif

#if( (TIMER0_INTERRUPT == TIMER_INTERRUPT_OFF) )
#define IE0_MODE 0b00000000
#elif( (TIMER0_INTERRUPT == TIMER_INTERRUPT_ON) && (TIMER0_MODE == NORMAL) )
#define IE0_MODE 0b00000001
#elif( (TIMER0_INTERRUPT == TIMER_INTERRUPT_ON) && (TIMER0_MODE == CTC) )
#define IE0_MODE 0b00000010
#endif

#if( (TIMER1_INTERRUPT == TIMER_INTERRUPT_OFF) )
#define IE1_MODE 0b00000000
#elif( (TIMER1_INTERRUPT == TIMER_INTERRUPT_ON) && (TIMER1_MODE == NORMAL) )
#define IE1_MODE 0b00000100
#elif( (TIMER1_INTERRUPT == TIMER_INTERRUPT_ON) && (TIMER1_MODE == CTC) )
#define IE1_MODE 0b00010000
#endif

#if( (TIMER2_INTERRUPT == TIMER_INTERRUPT_OFF) )
#define IE2_MODE 0b00000000
#elif( (TIMER2_INTERRUPT == TIMER_INTERRUPT_ON) && (TIMER2_MODE == NORMAL) )
#define IE2_MODE 0b00000001
#elif( (TIMER2_INTERRUPT == TIMER_INTERRUPT_ON) && (TIMER2_MODE == CTC) )
#define IE2_MODE 0b00000010
#endif

#define TCCR0_COMMAND (WGM0_MODE|CS0_MODE|COM0_MODE)
#define TCCR1A_COMMAND (COM1A_MODE|COM1B_MODE|WGM1_10_MODE)
#define TCCR1B_COMMAND (WGM1_23_MODE|CS1_MODE)
#define TCCR2_COMMAND (WGM2_MODE|CS2_MODE|COM2_MODE)
#define TIMSK_COMMAND (IE2_MODE|IE1_MODE|IE0_MODE)	//Will Need Work for Timer2

#if(TIMER0_INTERRUPT == TIMER_INTERRUPT_ON || TIMER1_INTERRUPT == TIMER_INTERRUPT_ON || TIMER2_INTERRUPT == TIMER_INTERRUPT_ON)
void (*GLOB_voidTIMER0Gptr)(void) = NULL;
void (*GLOB_voidTIMER1Gptr)(void) = NULL;
void (*GLOB_voidTIMER2Gptr)(void) = NULL;

volatile static u32 GLOB_u32TIMER0OverflowNo;
volatile static u32 GLOB_u32TIMER1OverflowNo;
volatile static u32 GLOB_u32TIMER2OverflowNo;
#endif

#if(ICU_MODE == ICU_ON)
volatile static u8 GLOB_u8DutyCyclePercentage;
volatile static u32 GLOB_u32Frequency;
#endif

#if(TIMER0_MODE == CTC || TIMER1_MODE == CTC || TIMER2_MODE == CTC)
STD_Type MCAL_TIMER_u8Optimize(u8 LOC_u8TimerNo,u32 LOC_u32Value)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
#if(TIMER0_MODE == CTC || TIMER2_MODE == CTC)
	u8 LOC_u8OptimizedValue;
	u8 LOC_u8MinRemainder;
#endif
#if(TIMER1_MODE == CTC)
	u16 LOC_u16OptimizedValue;
	u16 LOC_u16MinRemainder;
#endif
	if(LOC_u8TimerNo <= TIMER2)
	{
		switch(LOC_u8TimerNo)
		{
		case TIMER0:
#if(TIMER0_MODE == CTC)
			for(LOC_u8OptimizedValue=TIMER0_TICKS_MAX;LOC_u8OptimizedValue>=TIMER0_MIN_OPTIMIZATION_VALUE;LOC_u8OptimizedValue--)
			{
				if(LOC_u8OptimizedValue == TIMER0_TICKS_MAX)
				{
					LOC_u8MinRemainder = LOC_u32Value%TIMER0_TICKS_MAX;
				}
				if(!(LOC_u32Value%LOC_u8OptimizedValue))
				{
					OCR0 = LOC_u8OptimizedValue;
					return E_OK;		//I broke MESRA xD
				}
				else
				{
					if((LOC_u32Value%LOC_u8OptimizedValue) < LOC_u8MinRemainder)
					{
						LOC_u8MinRemainder = LOC_u32Value%LOC_u8OptimizedValue;
					}
				}
			}
			for(LOC_u8OptimizedValue=TIMER0_TICKS_MAX;LOC_u8OptimizedValue>=TIMER0_MIN_OPTIMIZATION_VALUE;LOC_u8OptimizedValue--)
			{
				if((LOC_u32Value%LOC_u8OptimizedValue) == LOC_u8MinRemainder)
				{
					OCR0 = LOC_u8OptimizedValue;
					LOC_u8ReturnValue = E_OK;
					break;
				}
			}
#endif
			break;
		case TIMER1:
#if(TIMER1_MODE == CTC)
			for(LOC_u16OptimizedValue=TIMER1_TICKS_MAX;LOC_u16OptimizedValue>=TIMER1_MIN_OPTIMIZATION_VALUE;LOC_u16OptimizedValue--)
			{
				if(LOC_u16OptimizedValue == TIMER1_TICKS_MAX)
				{
					LOC_u16MinRemainder = LOC_u32Value%TIMER1_TICKS_MAX;
				}
				if(!(LOC_u32Value%LOC_u16OptimizedValue))
				{
					OCR1A = LOC_u16OptimizedValue;
					return E_OK;		//I broke MESRA xD
				}
				else
				{
					if((LOC_u32Value%LOC_u16OptimizedValue) < LOC_u16MinRemainder)
					{
						LOC_u16MinRemainder = LOC_u32Value%LOC_u16OptimizedValue;
					}
				}
			}
			for(LOC_u16OptimizedValue=TIMER1_TICKS_MAX;LOC_u16OptimizedValue>=TIMER1_MIN_OPTIMIZATION_VALUE;LOC_u16OptimizedValue--)
			{
				if((LOC_u32Value%LOC_u16OptimizedValue) == LOC_u16MinRemainder)
				{
					OCR1A = LOC_u16OptimizedValue;
					LOC_u8ReturnValue = E_OK;
					break;
				}
			}
#endif
			break;
		case TIMER2:
#if(TIMER2_MODE == CTC)
			for(LOC_u8OptimizedValue=TIMER2_TICKS_MAX;LOC_u8OptimizedValue>=TIMER2_MIN_OPTIMIZATION_VALUE;LOC_u8OptimizedValue--)
			{
				if(LOC_u8OptimizedValue == TIMER2_TICKS_MAX)
				{
					LOC_u8MinRemainder = LOC_u32Value%TIMER2_TICKS_MAX;
				}
				if(!(LOC_u32Value%LOC_u8OptimizedValue))
				{
					OCR2 = LOC_u8OptimizedValue;
					return E_OK;		//I broke MESRA xD
				}
				else
				{
					if((LOC_u32Value%LOC_u8OptimizedValue) < LOC_u8MinRemainder)
					{
						LOC_u8MinRemainder = LOC_u32Value%LOC_u8OptimizedValue;
					}
				}
			}
			for(LOC_u8OptimizedValue=TIMER2_TICKS_MAX;LOC_u8OptimizedValue>=TIMER2_MIN_OPTIMIZATION_VALUE;LOC_u8OptimizedValue--)
			{
				if((LOC_u32Value%LOC_u8OptimizedValue) == LOC_u8MinRemainder)
				{
					OCR2 = LOC_u8OptimizedValue;
					LOC_u8ReturnValue = E_OK;
					break;
				}
			}
#endif
			break;
		}
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif

STD_Type MCAL_TIMER_u8Init(u8 LOC_u8TimerNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8TimerNo <= TIMER2)
	{
		switch(LOC_u8TimerNo)
		{
		case TIMER0:
			TCCR0 = TCCR0_COMMAND;
#if(TIMER0_MODE == PWM && OC0_MODE != OC0_OFF)
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(OC0_PORT,OC0_PIN,DIO_OUTPUT);
#endif
			break;
		case TIMER1:
			TCCR1A = TCCR1A_COMMAND;
			TCCR1B = TCCR1B_COMMAND;
#if(TIMER1_MODE == PWM && OC1A_MODE != OC1A_OFF)
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(OC1A_PORT,OC1A_PIN,DIO_OUTPUT);
#endif
#if(TIMER1_MODE == PWM && OC1B_MODE != OC1A_OFF)
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(OC1B_PORT,OC1B_PIN,DIO_OUTPUT);
#endif
			break;
		case TIMER2:
			TCCR2 = TCCR2_COMMAND;
#if(TIMER2_MODE == PWM && OC2_MODE != OC2_OFF)
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(OC2_PORT,OC2_PIN,DIO_OUTPUT);
#endif
			break;
		}
		TIMSK = TIMSK_COMMAND;
#if(TIMER0_INTERRUPT == TIMER_INTERRUPT_ON || TIMER1_INTERRUPT == TIMER_INTERRUPT_ON || TIMER2_INTERRUPT == TIMER_INTERRUPT_ON)
		SET_BIT(SREG,I_BIT);
#endif
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#if( TIMER0_INTERRUPT == TIMER_INTERRUPT_ON || TIMER1_INTERRUPT == TIMER_INTERRUPT_ON || TIMER2_INTERRUPT == TIMER_INTERRUPT_ON)
STD_Type MCAL_TIMER_u8CallbackMilliFun(u8 LOC_u8TimerNo,u32 LOC_u32MilliSeconds,void (*LOC_Pu8Ptr)(void))
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u32 LOC_u32TotalTicks;		
	
	if(LOC_Pu8Ptr != NULL && LOC_u8TimerNo <= TIMER2)
	{
		switch(LOC_u8TimerNo)
		{
			case TIMER0:
			LOC_u32TotalTicks = (LOC_u32MilliSeconds*MICRO_CONVERSION)/(TIMER0_PRESCALER/FREQ);

			GLOB_u32TIMER0OverflowNo = LOC_u32TotalTicks/(TIMER0_TICKS_MAX);
			GLOB_voidTIMER0Gptr = LOC_Pu8Ptr;

			#if( TIMER0_MODE == NORMAL)
			if(LOC_u32TotalTicks > TIMER0_TICKS_MAX)
			{
				TCNT0 = (TIMER0_TICKS_MAX)-(u8)(((LOC_u32TotalTicks)%(TIMER0_TICKS_MAX))*(TIMER0_TICKS_MAX));
			}
			else
			{
				TCNT0 = TIMER0_TICKS_MAX - (u8) LOC_u32TotalTicks;
			}
			#elif( TIMER0_MODE == CTC)
			if(LOC_u32TotalTicks > TIMER0_TICKS_MAX)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER0,LOC_u32TotalTicks);
			}
			else
			{
				OCR0 = (u8) LOC_u32TotalTicks;
			}
			#endif
			break;
			case TIMER1:
			LOC_u32TotalTicks = (LOC_u32MilliSeconds*MICRO_CONVERSION)/(TIMER1_PRESCALER/FREQ);
			GLOB_u32TIMER1OverflowNo = LOC_u32TotalTicks/(TIMER1_TICKS_MAX);
			GLOB_voidTIMER1Gptr = LOC_Pu8Ptr;
			#if( TIMER1_MODE == NORMAL)
			if(LOC_u32TotalTicks > TIMER1_TICKS_MAX)
			{
				TCNT1H = (u8)(((TIMER1_TICKS_MAX)-(u16)(((LOC_u32TotalTicks)%(TIMER1_TICKS_MAX))*(TIMER1_TICKS_MAX)))>>8);
				TCNT1L = (u8)(((TIMER1_TICKS_MAX)-(u16)(((LOC_u32TotalTicks)%(TIMER1_TICKS_MAX))*(TIMER1_TICKS_MAX))));				//TCNT1 = (TIMER1_TICKS_MAX)-(u16)(((LOC_u32TotalTicks)%(TIMER1_TICKS_MAX))*(TIMER1_TICKS_MAX));
			}
			else
			{
				TCNT1 = (u16) (TIMER1_TICKS_MAX - LOC_u32TotalTicks);
			}
			#elif( TIMER1_MODE == CTC)
			if(LOC_u32TotalTicks > TIMER1_TICKS_MAX)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER1,LOC_u32TotalTicks);
			}
			else
			{
				OCR1A = (u16) LOC_u32TotalTicks;
			}
			#endif
			break;
			case TIMER2:
			LOC_u32TotalTicks = (LOC_u32MilliSeconds*MICRO_CONVERSION)/(TIMER2_PRESCALER/FREQ);
			GLOB_u32TIMER2OverflowNo = LOC_u32TotalTicks/(TIMER2_TICKS_MAX);
			GLOB_voidTIMER2Gptr = LOC_Pu8Ptr;

			#if( TIMER2_MODE == NORMAL)
			if(LOC_u32TotalTicks > TIMER2_TICKS_MAX)
			{
				TCNT2 = (TIMER2_TICKS_MAX)-(u8)(((LOC_u32TotalTicks)%(TIMER2_TICKS_MAX))*(TIMER2_TICKS_MAX));
			}
			else
			{
				TCNT2 = TIMER2_TICKS_MAX - (u8) LOC_u32TotalTicks;
			}
			#elif( TIMER2_MODE == CTC)
			if(LOC_u32TotalTicks > TIMER2_TICKS_MAX)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER2,LOC_u32TotalTicks);
			}
			else
			{
				OCR2 = (u8) LOC_u32TotalTicks;
			}
			#endif
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

STD_Type MCAL_TIMER_u8CallbackMicroFun(u8 LOC_u8TimerNo,u32 LOC_u32MicroSeconds,void (*LOC_Pu8Ptr)(void))
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u32 LOC_u32TotalTicks;
	if(LOC_Pu8Ptr != NULL && LOC_u8TimerNo <= TIMER2)
	{
		switch(LOC_u8TimerNo)
		{
		case TIMER0:
			LOC_u32TotalTicks = (LOC_u32MicroSeconds)/(TIMER0_PRESCALER/FREQ);
			GLOB_u32TIMER0OverflowNo = LOC_u32TotalTicks/(TIMER0_TICKS_MAX);
			GLOB_voidTIMER0Gptr = LOC_Pu8Ptr;

#if( TIMER0_MODE == NORMAL)
			if(LOC_u32TotalTicks > TIMER0_TICKS_MAX)
			{
				TCNT0 = (TIMER0_TICKS_MAX)-(u8)(((LOC_u32TotalTicks)%(TIMER0_TICKS_MAX))*(TIMER0_TICKS_MAX));
			}
			else
			{
				TCNT0 = TIMER0_TICKS_MAX - (u8) LOC_u32TotalTicks;
			}
#elif( TIMER0_MODE == CTC)
			if(LOC_u32TotalTicks > TIMER0_TICKS_MAX)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER0,LOC_u32TotalTicks);
			}
			else
			{
				OCR0 = (u8) LOC_u32TotalTicks;
			}
#endif
			break;
		case TIMER1:
			LOC_u32TotalTicks = (LOC_u32MicroSeconds)/(TIMER1_PRESCALER/FREQ);
			GLOB_u32TIMER1OverflowNo = LOC_u32TotalTicks/(TIMER1_TICKS_MAX);
			GLOB_voidTIMER1Gptr = LOC_Pu8Ptr;
#if( TIMER1_MODE == NORMAL)
			if(LOC_u32TotalTicks > TIMER1_TICKS_MAX)
			{
				TCNT1 = (TIMER1_TICKS_MAX)-(u16)(((LOC_u32TotalTicks)%(TIMER1_TICKS_MAX))*(TIMER1_TICKS_MAX));
			}
			else
			{
				TCNT1 = TIMER1_TICKS_MAX - (u16) LOC_u32TotalTicks;
			}
#elif( TIMER1_MODE == CTC)
			if(LOC_u32TotalTicks > TIMER1_TICKS_MAX)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER1,LOC_u32TotalTicks);
			}
			else
			{
				OCR1A = (u16) LOC_u32TotalTicks;
			}
#endif
			break;
		case TIMER2:
			LOC_u32TotalTicks = (LOC_u32MicroSeconds)/(TIMER2_PRESCALER/FREQ);
			GLOB_u32TIMER2OverflowNo = LOC_u32TotalTicks/(TIMER2_TICKS_MAX);
			GLOB_voidTIMER2Gptr = LOC_Pu8Ptr;

#if( TIMER2_MODE == NORMAL)
			if(LOC_u32TotalTicks > TIMER2_TICKS_MAX)
			{
				TCNT2 = (TIMER2_TICKS_MAX)-(u8)(((LOC_u32TotalTicks)%(TIMER2_TICKS_MAX))*(TIMER2_TICKS_MAX));
			}
			else
			{
				TCNT2 = TIMER2_TICKS_MAX - (u8) LOC_u32TotalTicks;
			}
#elif( TIMER2_MODE == CTC)
			if(LOC_u32TotalTicks > TIMER2_TICKS_MAX)
			{
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER2,LOC_u32TotalTicks);
			}
			else
			{
				OCR2 = (u8) LOC_u32TotalTicks;
			}
#endif
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

#if( TIMER0_MODE == NORMAL && TIMER0_INTERRUPT == TIMER_INTERRUPT_ON)
ISR(TIMER0_OVF_vect)
{
	static u32 LOC_u32Count;
	LOC_u32Count++;
	if(LOC_u32Count == (GLOB_u32TIMER0OverflowNo+1))
	{
		LOC_u32Count = ZERO;
		if(GLOB_voidTIMER0Gptr != NULL)
		{
			GLOB_voidTIMER0Gptr();
		}

	}
}
#elif( TIMER0_MODE == CTC && TIMER0_INTERRUPT == TIMER_INTERRUPT_ON)
ISR(TIMER0_COMP_vect)
{
	static u32 LOC_u32Count;
	LOC_u32Count++;
	if(LOC_u32Count == (GLOB_u32TIMER0OverflowNo+1))
	{
		LOC_u32Count = ZERO;
		if(GLOB_voidTIMER0Gptr != NULL)
		{
			GLOB_voidTIMER0Gptr();
		}
	}
}
#endif

#if( TIMER1_MODE == NORMAL && TIMER1_INTERRUPT == TIMER_INTERRUPT_ON)
ISR(TIMER1_OVF_vect)
{
	static u32 LOC_u32Count;
	LOC_u32Count++;
	if(LOC_u32Count == (GLOB_u32TIMER1OverflowNo+1))
	{
		LOC_u32Count = ZERO;
		if(GLOB_voidTIMER1Gptr != NULL)
		{
			GLOB_voidTIMER1Gptr();
		}

	}
}
#elif( TIMER1_MODE == CTC && TIMER1_INTERRUPT == TIMER_INTERRUPT_ON)
ISR(TIMER1_COMPA_vect)
{
	static u32 LOC_u32Count;
	LOC_u32Count++;
	if(LOC_u32Count == (GLOB_u32TIMER1OverflowNo+1))
	{
		LOC_u32Count = ZERO;
		if(GLOB_voidTIMER1Gptr != NULL)
		{
			GLOB_voidTIMER1Gptr();
		}
	}
}
#endif

#if( TIMER2_MODE == NORMAL && TIMER2_INTERRUPT == TIMER_INTERRUPT_ON)
ISR(TIMER2_OVF_vect)
{
	static u32 LOC_u32Count;
	LOC_u32Count++;
	if(LOC_u32Count == (GLOB_u32TIMER2OverflowNo+1))
	{
		LOC_u32Count = ZERO;
		if(GLOB_voidTIMER2Gptr != NULL)
		{
			GLOB_voidTIMER2Gptr();
		}

	}
}
#elif( TIMER2_MODE == CTC && TIMER2_INTERRUPT == TIMER_INTERRUPT_ON)
ISR(TIMER2_COMP_vect)
{
	static u32 LOC_u32Count;
	LOC_u32Count++;
	if(LOC_u32Count == (GLOB_u32TIMER2OverflowNo+1))
	{
		LOC_u32Count = ZERO;
		if(GLOB_voidTIMER2Gptr != NULL)
		{
			GLOB_voidTIMER2Gptr();
		}
	}
}
#endif

//The following function is not updated!
#elif( TIMER0_INTERRUPT == TIMER_INTERRUPT_OFF && TIMER1_INTERRUPT == TIMER_INTERRUPT_OFF && TIMER2_INTERRUPT == TIMER_INTERRUPT_OFF)
STD_Type MCAL_TIMER_u8DelayMilliSeconds(u8 LOC_u8TimerNo,u32 LOC_u32MilliSeconds,u8 * LOC_Pu8TimeDone)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	*LOC_Pu8TimeDone = FALSE;
	static u32 LOC_u32OVCounts;
	static u32 LOC_u32TotalTicksPrev;	//Since we don't want to have busy waiting, we'll check if the prev
	//ticks are equal to the current or they changed. So that the flow of code
	//does not stop.
#if( TIMER0_PRESCALER != 0)
	u32 LOC_u32TotalTicksCurrent = (LOC_u32MilliSeconds*MICRO_CONVERSION*FREQ)/(TIMER0_PRESCALER);
#else
	u32 LOC_u32TotalTicksCurrent = (LOC_u32MilliSeconds*MICRO_CONVERSION*FREQ);

#endif
	u32 LOC_u32TIMER0OverflowNo = LOC_u32TotalTicksCurrent/(TIMER0_TICKS_MAX);
	if(LOC_u32TotalTicksCurrent != LOC_u32TotalTicksPrev)
	{
		LOC_u32OVCounts = 0;
	}
	if(LOC_u8TimerNo <= TIMER2)
	{
		switch(LOC_u8TimerNo)
		{
		case TIMER0:
			if(LOC_u32TotalTicksCurrent != LOC_u32TotalTicksPrev)
			{
#if( TIMER0_MODE == NORMAL)
				TCNT0 = (TIMER0_TICKS_MAX)-(u8)(((LOC_u32TotalTicksCurrent)%(TIMER0_TICKS_MAX))*(TIMER0_TICKS_MAX));
#elif( TIMER0_MODE == CTC)
				LOC_u8ReturnValue = MCAL_TIMER_u8Optimize(TIMER0,LOC_u32TotalTicksCurrent);
#endif
			}
#if( TIMER0_MODE == NORMAL)
			if(GET_BIT(TIFR,TOV0))
			{
				LOC_u32OVCounts++;
				if(LOC_u32OVCounts == (LOC_u32TIMER0OverflowNo+1))
				{
					*LOC_Pu8TimeDone = TRUE;
					LOC_u32OVCounts = ZERO;
				}
				SET_BIT(TIFR,TOV0);	//Clear the flag.
			}
#elif( TIMER0_MODE == CTC)
			if(GET_BIT(TIFR,OCF0))
			{
				LOC_u32OVCounts++;
				if(LOC_u32OVCounts == (LOC_u32TIMER0OverflowNo+1))
				{
					*LOC_Pu8TimeDone = TRUE;
					LOC_u32OVCounts = ZERO;
				}
				SET_BIT(TIFR,OCF0);	//Clear the flag.
			}
#endif
			LOC_u32TotalTicksPrev = LOC_u32TotalTicksCurrent;
			LOC_u8ReturnValue = E_OK;
			break;
		}
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif

#if( TIMER1_MODE == PWM)
STD_Type MCAL_TIMER_u8SetTIMER1PWM(u8 LOC_u8Percentage,u8 LOC_u8Frequency)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8Percentage >= ZERO && LOC_u8Percentage <= PERCENT && LOC_u8Frequency > ZERO)
	{
#if( TIMER1_RESOLUTION == COMPARE_MATCH)
#if( OC1B_MODE == OC1B_CLEAR)
		OCR1A =	(u16)((u32)((u32)MILLI_CONVERSION*MICRO_CONVERSION*FREQ)/((u32)TIMER1_PRESCALER*LOC_u8Frequency));
		OCR1B = (u16)((u32)LOC_u8Percentage*((u32)MILLI_CONVERSION*MICRO_CONVERSION*FREQ)/((u32)TIMER1_PRESCALER*LOC_u8Frequency*PERCENT));
		LOC_u8ReturnValue = E_OK;
#endif
#elif( TIMER1_RESOLUTION == TEN_BIT)
#if( OC1A_MODE == OC1A_CLEAR)
		OCR1A = (u16)((u32)LOC_u8Percentage*(TIMER1_PWM_TOP)/PERCENT);
		LOC_u8ReturnValue = E_OK;
#endif
#endif
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif

#if( TIMER0_MODE == PWM || TIMER2_MODE == PWM)
STD_Type MCAL_TIMER_u8SetPWM(u8 LOC_u8TimerNo,u8 LOC_u8Percentage)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8TimerNo <= TIMER2 && LOC_u8Percentage >= ZERO && LOC_u8Percentage <= PERCENT)
	{
		switch(LOC_u8TimerNo)
		{
		case TIMER0:
#if( OC0_MODE == OC0_CLEAR)
			OCR0 = (u8)((u32)LOC_u8Percentage*(TIMER0_TICKS_MAX)/PERCENT);
			LOC_u8ReturnValue = E_OK;
#endif
			break;
		case TIMER2:
#if( OC2_MODE == OC2_CLEAR)
			OCR2 = (u8)((u32)LOC_u8Percentage*(TIMER2_TICKS_MAX)/PERCENT);
			LOC_u8ReturnValue = E_OK;
#endif
			break;
		}
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif

#if( ICU_MODE == ICU_ON)
STD_Type MCAL_TIMER_u8ICUInit(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	MCAL_DIO_u8SetPinDirection(ICP1_PORT,ICP1_PIN,DIO_INPUT);
#ifdef TIMER1_RESOLUTION
#if( TIMER1_RESOLUTION == COMPARE_MATCH)
	OCR1A = 0xFFFF;		//TOP = 65,535
#endif
#endif
	CLEAR_BIT(TCCR1B,ICNC1);	//No noise canceler (takes 4 clocks of delay)
	SET_BIT(TCCR1B,ICES1);		//Start with rising edge
	SET_BIT(TIMSK,TICIE1);
	SET_BIT(SREG,I_BIT);
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}
STD_Type MCAL_TIMER_u8ICUGetDutyFreq(u8 * LOC_Pu8DutyCyclePercentage,u32 * LOC_Pu32Frequency)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu8DutyCyclePercentage && LOC_Pu32Frequency)
	{
		*LOC_Pu8DutyCyclePercentage = GLOB_u8DutyCyclePercentage;
		*LOC_Pu32Frequency = GLOB_u32Frequency;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

ISR(TIMER1_CAPT_vect)
{
	u16 LOC_u16ICRValueCurrent = ICR1;
	//u16 LOC_u16ICRValueCurrent = ICR1H << 8;
	//LOC_u16ICRValueCurrent |= ICR1L;
	static u16 LOC_u16TimeOn;
	static u16 LOC_u16TotalTime;
	static u16 LOC_u16ICRValuePrev;
	static u8 LOC_u8State = RISING_EDGE;
	static u8 LOC_u8FirstTime = TRUE;
	if(LOC_u8FirstTime)
	{
		LOC_u16ICRValuePrev = ZERO;
		LOC_u8FirstTime = FALSE;
		//Ignore this time for more accurate results.
	}
	else if(LOC_u8State == RISING_EDGE)
	{
		if(LOC_u16ICRValuePrev != ZERO)
		{
			LOC_u16TotalTime = LOC_u16TimeOn + (LOC_u16ICRValueCurrent - LOC_u16ICRValuePrev);
			GLOB_u8DutyCyclePercentage = (u8)((u32)(LOC_u16TimeOn*PERCENT)/LOC_u16TotalTime);
			GLOB_u32Frequency = (u32)(((u32)MILLI_CONVERSION*MICRO_CONVERSION)/(LOC_u16TotalTime*(TIMER1_PRESCALER/FREQ)));
			LOC_u8FirstTime = TRUE;
			LOC_u16TotalTime = ZERO;
			LOC_u16ICRValuePrev = ZERO;
			LOC_u8State = RISING_EDGE;
			//TCNT1 = ZERO;
		}
		else
		{
			LOC_u8State = FALLING_EDGE;
			CLEAR_BIT(TCCR1B,ICES1);		//Make it falling edge
			LOC_u16ICRValuePrev = LOC_u16ICRValueCurrent;
		}
	}
	else if(LOC_u8State == FALLING_EDGE)
	{
		LOC_u16TimeOn = LOC_u16ICRValueCurrent - LOC_u16ICRValuePrev;
		LOC_u8State = RISING_EDGE;
		SET_BIT(TCCR1B,ICES1);		//Make it rising edge
		LOC_u16ICRValuePrev = LOC_u16ICRValueCurrent;
	}
}

#endif

