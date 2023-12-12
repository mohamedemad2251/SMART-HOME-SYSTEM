#ifndef RTOS_INT_H_
#define RTOS_INT_H_

#include "../../DEFS/MCU.h"

STD_Type RTOS_u8Init(void);
STD_Type RTOS_u8CreateTask(void (*LOC_PvoidFun)(void),u8 LOC_u8Priority, u16 LOC_u16Periodicity);

#endif /* RTOS_INT_H_ */
