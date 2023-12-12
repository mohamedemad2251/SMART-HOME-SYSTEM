#ifndef SEV_SEG_H_
#define SEV_SEG_H_

#include "../../MCAL/DIO/DIO.h"
#include <util/delay.h>

#define MAX_SEV_SEG_NO 99
#define MIN_SEV_SEG_NO 0
#define DIGIT_DIVIDEND 10
#define SEGMENT_SHIFT 4

#define ALL_ONES 0xFF

#define SEV_SEG_SEGMENT_WRITE_PORT DIO_PortA
#define SEV_SEG_SEGMENT_1_ENABLE_PORT DIO_PortB
#define SEV_SEG_SEGMENT_2_ENABLE_PORT DIO_PortB
#define SEV_SEG_DP_ENABLE_PORT DIO_PortB 		//DP = Decimal Point
#define SEV_SEG_SEGMENT_1_ENABLE_PIN Pin1
#define SEV_SEG_SEGMENT_2_ENABLE_PIN Pin2
#define SEV_SEG_DP_ENABLE_PIN Pin3



STD_Type HAL_SEV_SEG_u8DisplayNo(u8 LOC_u8No);	//NOTE: DELAY = 2 MS (Max 2 Digits AKA 99)
STD_Type HAL_SEV_SEG_u8EnableDP(u8 LOC_u8SegmentChoice);

#endif /* SEV_SEG_H_ */
