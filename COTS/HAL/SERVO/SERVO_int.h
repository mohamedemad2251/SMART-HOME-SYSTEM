#ifndef SERVO_INT_H_
#define SERVO_INT_H_

#include "../../MCAL/DIO/DIO.h"
#include "../../MCAL/TIMER/TIMER.h"
#include "../../MCAL/TIMER/TIMER_config.h"

#include "SERVO_priv.h"
#include "SERVO_config.h"

#define SERVO_PORT DIO_PortD
#define SERVO_PIN  Pin4			//This config for OC1B connection, leave it as it is

#if( SERVO_LOGIC == SERVO_DELAY)
#define SERVO_HARD_LEFT		0
#define SERVO_LEFT			45
#define SERVO_STRAIGHT		90
#define SERVO_RIGHT			135
#define SERVO_HARD_RIGHT	180
#endif

#if( SERVO_LOGIC == SERVO_PWM)
STD_Type HAL_SERVO_u8Init(void);
#elif( SERVO_LOGIC == SERVO_DELAY)
STD_Type HAL_SERVO_u8Init(u8 LOC_u8PortNo,u8 LOC_u8PinNo);
#endif
#if( SERVO_LOGIC == SERVO_PWM)
STD_Type HAL_SERVO_u8TurnDegree(u8 LOC_u8TurnDegree);	//Doesn't give accurate angles due to fraction neglection.
#elif( SERVO_LOGIC == SERVO_DELAY)
STD_Type HAL_SERVO_u8TurnDegree(u8 LOC_u8PortNo,u8 LOC_u8PinNo, u8 LOC_u8TurnDegree);
#endif
#endif /* SERVO_INT_H_ */
