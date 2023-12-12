#include "IR_SENSOR_int.h"

STD_Type HAL_IR_SENSOR_u8Init(u8 LOC_PortNo,u8 LOC_PinNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_PortNo <= DIO_PortD && LOC_PinNo <= Pin7)
	{
		LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LOC_PortNo,LOC_PinNo,DIO_INPUT);
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type HAL_IR_SENSOR_u8GetDetection(u8 LOC_PortNo,u8 LOC_PinNo,u8 * LOC_Pu8Detected)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_PortNo <= DIO_PortD && LOC_PinNo <= Pin7)
	{
		LOC_u8ReturnValue = MCAL_DIO_u8GetPinValue(LOC_PortNo,LOC_PinNo,LOC_Pu8Detected);
	} 
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}