/*
 * LED.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef LED_H_
#define LED_H_

#include "../../MCAL/DIO/DIO.h"

#define LED_LED0 0
#define LED_LED1 1
#define LED_LED2 2

#define LED_NO 	 3

#define LED_LED0_PORT DIO_PortC
#define LED_LED1_PORT DIO_PortC
#define LED_LED2_PORT DIO_PortD

#define LED_LED0_PIN Pin2
#define LED_LED1_PIN Pin7
#define LED_LED2_PIN Pin3

#define LED_OFF 0
#define LED_ON 1
#define LED_TOGGLE 2

STD_Type HAL_LED_u8LEDInit(u8 LOC_u8LEDNo);
STD_Type HAL_LED_u8LEDMode(u8 LOC_u8LEDNo,u8 LOC_u8LEDMode);

#endif /* LED_H_ */
