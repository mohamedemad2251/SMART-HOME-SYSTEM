#ifndef SPI_INT_H_
#define SPI_INT_H_

#include "../../../DEFS/STD_types.h"
#include "../../../DEFS/MCU.h"
#include "../../../DEFS/BIT_Math.h"

STD_Type MCAL_SPI_u8Init(void);

#if(SPI_SELECT == SPI_SLAVE)
#if(SPI_INTERRUPT == SPI_INTERRUPT_OFF)
STD_Type MCAL_SPI_u8RecieveData(u8 * LOC_Pu8Data);
#elif(SPI_INTERRUPT == SPI_INTERRUPT_ON)
STD_Type MCAL_SPI_u8CallbackFun(void (*LOC_Pu8Ptr)(void));
STD_Type MCAL_SPI_u8GetData(u8 * LOC_Pu8Data);
#endif
#elif(SPI_SELECT == SPI_MASTER)
#if(SPI_INTERRUPT == SPI_INTERRUPT_OFF)
STD_Type MCAL_SPI_u8SendData(u8 LOC_u8Data);
#elif(SPI_INTERRUPT == SPI_INTERRUPT_ON)
STD_Type MCAL_SPI_u8CallbackFun(u8 LOC_Pu8Data[],void (*LOC_Pu8Ptr)(void));
#endif
#endif

#endif /* SPI_INT_H_ */
