#ifndef ADC_INT_H_
#define ADC_INT_H_

#include "../../../DEFS/BIT_Math.h"
#include "../../../DEFS/MCU.h"
#include "../../../DEFS/STD_types.h"

#include "ADC_config.h"

#define	ADC0	0
#define	ADC1	1
#define	ADC2	2
#define	ADC3	3
#define	ADC4	4
#define	ADC5	5
#define	ADC6	6
#define	ADC7	7

#define ADC_MAX	0x3FF	//1023	(Usable by the user)

STD_Type MCAL_ADC_u8Init(u8 LOC_u8ADCNo);
STD_Type MCAL_ADC_u8ChangeChannel(u8 LOC_u8ADCNo);
#if(ADC_AUTO_TRIGGER == ADC_OFF)
STD_Type MCAL_ADC_u8StartConversion(void);
#endif
#if( ADC_INTERRUPT == ADC_INTERRUPT_ON)
STD_Type MCAL_ADC_u8GetConversion(u16 * LOC_Pu16ConversionValue,u8 * LOC_Pu8ADCDataChannel);
#elif( ADC_INTERRUPT == ADC_INTERRUPT_OFF)
STD_Type MCAL_ADC_u8GetConversion(u16 * LOC_Pu16ConversionValue);
#endif
#if( ADC_INTERRUPT == ADC_INTERRUPT_ON)
STD_Type MCAL_ADC_u8CallbackFun(void (*LOC_PvoidPtr)(void));
#endif


#endif /* ADC_INT_H_ */
