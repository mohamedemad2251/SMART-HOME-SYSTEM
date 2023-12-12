#ifndef PB_H_
#define PB_H_

#include "../../MCAL/DIO/DIO.h"

#define PB_MAX_COUNT 10
#define FALSE 0
#define TRUE 1

#define PB0 0
#define PB1 1
#define PB2 2

#define PB0_PORT DIO_PortB
#define PB1_PORT DIO_PortD
#define PB2_PORT DIO_PortD

#define PB0_PIN Pin0
#define PB1_PIN Pin6
#define PB2_PIN Pin2

STD_Type HAL_PB_u8Init(u8 LOC_u8ButtonNo);
STD_Type HAL_PB_u8GetButtonPressed(u8 LOC_u8ButtonNo,u8 * LOC_Pu8ButtonPressed);

#endif /* COTS_HAL_PB_PB_H_ */
