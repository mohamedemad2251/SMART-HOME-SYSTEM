/*
 * LED.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */
#include "LED.h"

STD_Type HAL_LED_u8LEDInit(u8 LOC_u8LEDNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8LEDNo <= LED_LED2)
	{
		switch(LOC_u8LEDNo)
		{
			case LED_LED0: LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LED_LED0_PORT,LED_LED0_PIN,DIO_OUTPUT);
				break;
			case LED_LED1: LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LED_LED1_PORT,LED_LED1_PIN,DIO_OUTPUT);
				break;
			case LED_LED2: LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LED_LED2_PORT,LED_LED2_PIN,DIO_OUTPUT);
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}

STD_Type HAL_LED_u8LEDMode(u8 LOC_u8LEDNo,u8 LOC_u8LEDMode)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8LEDNo <= LED_LED2 && LOC_u8LEDMode <= LED_TOGGLE)
	{
		switch(LOC_u8LEDNo)
		{
			case LED_LED0:
				switch(LOC_u8LEDMode)
				{
					case LED_OFF:
						LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LED_LED0_PORT,LED_LED0_PIN,DIO_LOW);
						break;
					case LED_ON:
						LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LED_LED0_PORT,LED_LED0_PIN,DIO_HIGH);
						break;
					case LED_TOGGLE:
						LOC_u8ReturnValue = MCAL_DIO_u8TogglePinValue(LED_LED0_PORT,LED_LED0_PIN);
						break;
				}
				break;
			case LED_LED1:
				switch(LOC_u8LEDMode)
				{
					case LED_OFF:
						LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LED_LED1_PORT,LED_LED1_PIN,DIO_LOW);
						break;
					case LED_ON:
						LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LED_LED1_PORT,LED_LED1_PIN,DIO_HIGH);
						break;
					case LED_TOGGLE:
						LOC_u8ReturnValue = MCAL_DIO_u8TogglePinValue(LED_LED1_PORT,LED_LED1_PIN);
						break;
				}
				break;
			case LED_LED2:
				switch(LOC_u8LEDMode)
				{
					case LED_OFF:
						LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LED_LED2_PORT,LED_LED2_PIN,DIO_LOW);
						break;
					case LED_ON:
						LOC_u8ReturnValue = MCAL_DIO_u8SetPinValue(LED_LED2_PORT,LED_LED2_PIN,DIO_HIGH);
						break;
					case LED_TOGGLE:
						LOC_u8ReturnValue = MCAL_DIO_u8TogglePinValue(LED_LED2_PORT,LED_LED2_PIN);
						break;
				}
				break;
		}
	}
	else
	{
		//Do Nothing
	}
	return LOC_u8ReturnValue;
}
