#ifndef ULTRASONIC_INT_H_
#define ULTRASONIC_INT_H_

#include "../../MCAL/EXTI/EXTI.h"
#include "../../MCAL/TIMER/TIMER.h"
#include "ULTRASONIC_config.h"

STD_Type HAL_ULTRASONIC_u8Init(void);
STD_Type HAL_ULTRASONIC_u8GetDistance(u16 * LOC_u16Distance, u8 * LOC_u8TriggerPending);
void HAL_ULTRASONIC_voidTrigger(void);


#endif	/* ULTRASONIC_INT_H_ */