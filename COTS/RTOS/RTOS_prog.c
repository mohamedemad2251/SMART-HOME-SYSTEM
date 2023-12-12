#include "RTOS_priv.h"
#include "RTOS_config.h"
#include "RTOS_int.h"

#include "../MCAL/TIMER/TIMER.h"

typedef struct
{
	void (*voidPtr)(void);
	u16 periodicity;
}Task_Type;

Task_Type GLOB_Task_TypeARR[RTOS_TASK_NUM] = {{NULL},{0}};

static void RTOS_voidScheduler(void)
{
	u16 LOC_u16Index = ZERO;
	static u32 LOC_u32Count;
	LOC_u32Count++;		//Since count = 0 will be a corner case
	for(LOC_u16Index;LOC_u16Index < RTOS_TASK_NUM;LOC_u16Index++)
	{
		if( LOC_u32Count % (GLOB_Task_TypeARR[LOC_u16Index].periodicity) == ZERO )
		{
			if(GLOB_Task_TypeARR[LOC_u16Index].voidPtr != NULL)
			{
				GLOB_Task_TypeARR[LOC_u16Index].voidPtr();
			}
		}
	}
}

STD_Type RTOS_u8CreateTask(void (*LOC_PvoidFun)(void),u8 LOC_u8Priority, u16 LOC_u16Periodicity)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if( (LOC_PvoidFun != NULL) && (LOC_u8Priority < RTOS_TASK_NUM) )
	{
		GLOB_Task_TypeARR[LOC_u8Priority].voidPtr = LOC_PvoidFun;
		GLOB_Task_TypeARR[LOC_u8Priority].periodicity = LOC_u16Periodicity;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type RTOS_u8Init(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	LOC_u8ReturnValue = MCAL_TIMER_u8Init(TIMER0);
	LOC_u8ReturnValue = MCAL_TIMER_u8CallbackMilliFun(TIMER0,RTOS_TICK,&RTOS_voidScheduler);
	return LOC_u8ReturnValue;
}
