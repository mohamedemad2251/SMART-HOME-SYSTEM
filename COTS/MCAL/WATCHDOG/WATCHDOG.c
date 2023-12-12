#include "WATCHDOG.h"

#define WATCHDOG_PRE_0_COMMAND 0
#define WATCHDOG_PRE_1_COMMAND 1
#define WATCHDOG_PRE_2_COMMAND 2
#define WATCHDOG_PRE_3_COMMAND 3
#define WATCHDOG_PRE_4_COMMAND 4
#define WATCHDOG_PRE_5_COMMAND 5
#define WATCHDOG_PRE_6_COMMAND 6
#define WATCHDOG_PRE_7_COMMAND 7


#define WATCHDOG_PRE_0	17	//Reason: I round up to the nearest ms
//e.g. 16.3 ms prescaler is 17 ms as user input
#define WATCHDOG_PRE_1	33
#define WATCHDOG_PRE_2	65
#define WATCHDOG_PRE_3	130
#define WATCHDOG_PRE_4	260
#define WATCHDOG_PRE_5	520
#define WATCHDOG_PRE_6	1000
#define WATCHDOG_PRE_7	2100

STD_Type MCAL_WATCHDOG_u8Init(u32 LOC_u32Prescaler)	//Prescaler in ms
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u32Prescaler >= WATCHDOG_PRE_0)
	{
		if(LOC_u32Prescaler < WATCHDOG_PRE_1)
		{
			WDTCR = WATCHDOG_PRE_0_COMMAND;
		}
		else if(LOC_u32Prescaler < WATCHDOG_PRE_2)
		{
			WDTCR = WATCHDOG_PRE_1_COMMAND;
		}
		else if(LOC_u32Prescaler < WATCHDOG_PRE_3)
		{
			WDTCR = WATCHDOG_PRE_2_COMMAND;
		}
		else if(LOC_u32Prescaler < WATCHDOG_PRE_4)
		{
			WDTCR = WATCHDOG_PRE_3_COMMAND;
		}
		else if(LOC_u32Prescaler < WATCHDOG_PRE_5)
		{
			WDTCR = WATCHDOG_PRE_4_COMMAND;
		}
		else if(LOC_u32Prescaler < WATCHDOG_PRE_6)
		{
			WDTCR = WATCHDOG_PRE_5_COMMAND;
		}
		else if(LOC_u32Prescaler < WATCHDOG_PRE_7)
		{
			WDTCR = WATCHDOG_PRE_6_COMMAND;
		}
		else
		{
			WDTCR = WATCHDOG_PRE_7_COMMAND;
		}
		SET_BIT(WDTCR,WDE);
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type MCAL_WATCHDOG_u8Enable(u8 LOC_u8Enable)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8Enable <= ENABLE)
	{
		switch(LOC_u8Enable)
		{
			case DISABLE:
				WDTCR |= (SET << WDTOE) | (SET << WDE);	//Must be in the same operation
				CLEAR_BIT(WDTCR,WDE);	//2-steps procedure to disable watchdog
				break;
			case ENABLE:
				SET_BIT(WDTCR,WDE);		//Not recommended! You're enabling watchdog
					//Without knowing or defining a prescaler!
				break;
		}
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

