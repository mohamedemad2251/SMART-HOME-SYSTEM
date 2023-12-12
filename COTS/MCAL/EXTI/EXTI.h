#ifndef EXTI_H_
#define EXTI_H_

#include "../../../DEFS/MCU.h"
#include "../../../DEFS/STD_types.h"
#include "../../../DEFS/BIT_Math.h"

STD_Type MCAL_EXTI_u8Init(u8 LOC_u8InterruptNo,u8 LOC_u8EdgeMode);
STD_Type MCAL_EXTI_u8GlobalMode(u8 LOC_u8GlobMode);
STD_Type MCAL_EXTI_u8InterruptMode(u8 LOC_u8InterruptNo, u8 LOC_u8IntMode);
STD_Type MCAL_EXTI_u8CallbackFun(u8 LOC_u8InterruptNo, void (*LOC_Pu8Ptr)(void));

#define EXTI0 0
#define EXTI1 1
#define EXTI2 2

#define EXTI_GIE_OFF LOW
#define EXTI_GIE_ON HIGH

#define EXTI_PIE_OFF LOW
#define EXTI_PIE_ON	HIGH

#endif /* EXTI_H_ */
