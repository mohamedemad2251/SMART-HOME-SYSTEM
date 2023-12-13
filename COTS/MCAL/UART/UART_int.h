#ifndef UART_INT_H_
#define UART_INT_H_

#include "../../../DEFS/STD_types.h"
#include "../../../DEFS/MCU.h"
#include "../../../DEFS/BIT_Math.h"

#include "UART_priv.h"
#include "UART_config.h"

STD_Type MCAL_UART_u8Init(void);

#if(UART_INTERRUPT == UART_INTERRUPT_OFF)

#elif(UART_INTERRUPT == UART_INTERRUPT_ON)	//When the interrupt is on and the UART is MASTER, the UDR empty interrupt starts causing headache
STD_Type MCAL_UART_u8SuspendInterrupt(void);	//Therefore, I've created two functions to suspend and resume the PIE to disable and re-enable the interrupt
STD_Type MCAL_UART_u8ResumeInterrupt(void);
#if( ((UART_DIRECTION == UART_SIMPLEX) && (UART_MASTER_SELECT == UART_MASTER)) || (UART_DIRECTION == UART_DUPLEX) )
STD_Type MCAL_UART_u8CallbackFun(u8 LOC_Pu8Data[],void (*LOC_Pu8Ptr)(void));
#elif( ((UART_DIRECTION == UART_SIMPLEX) && (UART_MASTER_SELECT == UART_SLAVE)) || (UART_DIRECTION == UART_DUPLEX) )
STD_Type MCAL_UART_u8CallbackFun(void (*LOC_Pu8Ptr)(void));
#endif
#endif

#if(UART_DATA_BITS == UART_EIGHT_BITS && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_MASTER)))
STD_Type MCAL_UART_u8SendData(u8 LOC_u8Data);
STD_Type MCAL_UART_u8SendWord(u8 LOC_u8Word[]);
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS && UART_INTERRUPT == UART_INTERRUPT_OFF && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_SLAVE)))
STD_Type MCAL_UART_u8RecieveData(u8 * LOC_Pu8Data);
#endif

#if(UART_DATA_BITS == UART_EIGHT_BITS && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_SLAVE)))
STD_Type MCAL_UART_u8GetData(u8 * LOC_Pu8Data);
#endif

#endif /* UART_INT_H_ */
