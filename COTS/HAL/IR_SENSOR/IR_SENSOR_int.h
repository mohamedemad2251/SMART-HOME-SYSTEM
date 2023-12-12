#ifndef IR_SENSOR_INT_H_
#define IR_SENSOR_INT_H_

#include "../../MCAL/DIO/DIO.h"

STD_Type HAL_IR_SENSOR_u8Init(u8 LOC_PortNo,u8 LOC_PinNo);

STD_Type HAL_IR_SENSOR_u8GetDetection(u8 LOC_PortNo,u8 LOC_PinNo,u8 * LOC_Pu8Detected);


#endif /* IR_SENSOR_INT_H_ */