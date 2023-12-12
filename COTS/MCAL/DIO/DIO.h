#ifndef DIO_H_INCLUDED  //File guard
#define DIO_H_INCLUDED

#include "../../../DEFS/MCU.h"
#include "../../../DEFS/STD_types.h"
#include "../../../DEFS/BIT_Math.h"

#define DIO_PortA 0
#define DIO_PortB 1
#define DIO_PortC 2
#define DIO_PortD 3
#define DIO_PinA 0
#define DIO_PinB 1
#define DIO_PinC 2
#define DIO_PinD 3
#define DIO_DDRA 0
#define DIO_DDRB 1
#define DIO_DDRC 2
#define DIO_DDRD 3

#define DIO_LOW 0
#define DIO_HIGH 1

#define DIO_INPUT 0
#define DIO_OUTPUT 1

#define DIO_ALL_INPUTS 0x00
#define DIO_ALL_OUTPUTS 0xFF

#define DIO_ALL_LOW 0x00
#define DIO_ALL_HIGH 0xFF

STD_Type MCAL_DIO_u8SetPinDirection(u8 LOC_u8PortNo, u8 LOC_u8PinNo, u8 LOC_u8PinDirection);
STD_Type MCAL_DIO_u8SetPortDirection(u8 LOC_u8PortNo, u8 LOC_u8PortDirection);
STD_Type MCAL_DIO_u8SetPinValue(u8 LOC_u8PortNo, u8 LOC_u8PinNo, u8 LOC_u8PinValue);
STD_Type MCAL_DIO_u8SetPortValue(u8 LOC_u8PortNo, u8 LOC_u8PortValue);
STD_Type MCAL_DIO_u8GetPinValue(u8 LOC_u8PortNo, u8 LOC_u8PinNo, u8 * LOC_Pu8PinValue);
STD_Type MCAL_DIO_u8GetPortValue(u8 LOC_u8PortNo, u8 * LOC_Pu8PortValue);
STD_Type MCAL_DIO_u8TogglePinValue(u8 LOC_u8PortNo, u8 LOC_u8PinNo);
STD_Type MCAL_DIO_u8TogglePortValue(u8 LOC_u8PortNo);

#endif
