#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "../../../DEFS/BIT_Math.h"
#include "../../../DEFS/STD_Types.h"
#include "../../../DEFS/MCU.h"

STD_Type MCAL_WATCHDOG_u8Init(u32 LOC_u32Prescaler);
STD_Type MCAL_WATCHDOG_u8Enable(u8 LOC_u8Enable);

#endif /* WATCHDOG_H_ */
