#include "DC_MOTOR_int.h"
#include "DC_MOTOR_priv.h"
#include "DC_MOTOR_config.h"

STD_Type HAL_DC_MOTOR_u8AdjustDirection(u8 LOC_u8MotorNo,u8 LOC_u8Direction)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if( (LOC_u8MotorNo <= DC_MOTOR2) && (LOC_u8Direction <= DC_MOTOR_BACKWARD) )
	{
		switch(LOC_u8MotorNo)
		{
			case DC_MOTOR0:
				switch(LOC_u8Direction)
				{
					case DC_MOTOR_FORWARD:
						MCAL_DIO_u8SetPinValue(DC_MOTOR0_IN1_PORT,DC_MOTOR0_IN1_PIN,DIO_HIGH);
						MCAL_DIO_u8SetPinValue(DC_MOTOR0_IN2_PORT,DC_MOTOR0_IN2_PIN,DIO_LOW);
						break;
					case DC_MOTOR_BACKWARD:
						MCAL_DIO_u8SetPinValue(DC_MOTOR0_IN1_PORT,DC_MOTOR0_IN1_PIN,DIO_LOW);
						MCAL_DIO_u8SetPinValue(DC_MOTOR0_IN2_PORT,DC_MOTOR0_IN2_PIN,DIO_HIGH);
						break;
				}
				break;
			case DC_MOTOR1:
				switch(LOC_u8Direction)
				{
					case DC_MOTOR_FORWARD:
						MCAL_DIO_u8SetPinValue(DC_MOTOR1_IN1_PORT,DC_MOTOR1_IN1_PIN,DIO_HIGH);
						MCAL_DIO_u8SetPinValue(DC_MOTOR1_IN2_PORT,DC_MOTOR1_IN2_PIN,DIO_LOW);
						break;
					case DC_MOTOR_BACKWARD:
						MCAL_DIO_u8SetPinValue(DC_MOTOR1_IN1_PORT,DC_MOTOR1_IN1_PIN,DIO_LOW);
						MCAL_DIO_u8SetPinValue(DC_MOTOR1_IN2_PORT,DC_MOTOR1_IN2_PIN,DIO_HIGH);
						break;
				}
				break;
			case DC_MOTOR2:
				switch(LOC_u8Direction)
				{
					case DC_MOTOR_FORWARD:
						MCAL_DIO_u8SetPinValue(DC_MOTOR2_IN1_PORT,DC_MOTOR0_IN1_PIN,DIO_HIGH);
						MCAL_DIO_u8SetPinValue(DC_MOTOR2_IN2_PORT,DC_MOTOR0_IN2_PIN,DIO_LOW);
						break;
					case DC_MOTOR_BACKWARD:
						MCAL_DIO_u8SetPinValue(DC_MOTOR2_IN1_PORT,DC_MOTOR2_IN1_PIN,DIO_LOW);
						MCAL_DIO_u8SetPinValue(DC_MOTOR2_IN2_PORT,DC_MOTOR2_IN2_PIN,DIO_HIGH);
						break;
				}
				break;
		}
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type HAL_DC_MOTOR_u8Init(u8 LOC_u8MotorNo)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8MotorNo <= DC_MOTOR2)
	{
		switch(LOC_u8MotorNo)
		{
			case DC_MOTOR0:
				MCAL_DIO_u8SetPinDirection(DC_MOTOR0_IN1_PORT,DC_MOTOR0_IN1_PIN,DIO_OUTPUT);
				MCAL_DIO_u8SetPinDirection(DC_MOTOR0_IN2_PORT,DC_MOTOR0_IN2_PIN,DIO_OUTPUT);
				MCAL_DIO_u8SetPinDirection(DC_MOTOR0_EN_PORT,DC_MOTOR0_EN_PIN,DIO_OUTPUT);
				HAL_DC_MOTOR_u8AdjustDirection(DC_MOTOR0,DC_MOTOR_FORWARD);
				MCAL_TIMER_u8Init(TIMER0);
				break;
			case DC_MOTOR1:
				MCAL_DIO_u8SetPinDirection(DC_MOTOR1_IN1_PORT,DC_MOTOR1_IN1_PIN,DIO_OUTPUT);
				MCAL_DIO_u8SetPinDirection(DC_MOTOR1_IN2_PORT,DC_MOTOR1_IN2_PIN,DIO_OUTPUT);
				MCAL_DIO_u8SetPinDirection(DC_MOTOR1_EN_PORT,DC_MOTOR1_EN_PIN,DIO_OUTPUT);
				HAL_DC_MOTOR_u8AdjustDirection(DC_MOTOR1,DC_MOTOR_FORWARD);
				MCAL_TIMER_u8Init(TIMER1);
				break;
			case DC_MOTOR2:
				MCAL_DIO_u8SetPinDirection(DC_MOTOR2_IN1_PORT,DC_MOTOR2_IN1_PIN,DIO_OUTPUT);
				MCAL_DIO_u8SetPinDirection(DC_MOTOR2_IN2_PORT,DC_MOTOR2_IN2_PIN,DIO_OUTPUT);
				MCAL_DIO_u8SetPinDirection(DC_MOTOR2_EN_PORT,DC_MOTOR2_EN_PIN,DIO_OUTPUT);
				HAL_DC_MOTOR_u8AdjustDirection(DC_MOTOR2,DC_MOTOR_FORWARD);
				MCAL_TIMER_u8Init(TIMER2);
				break;
		}
		LOC_u8ReturnValue = E_OK;
	} 
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}

STD_Type HAL_DC_MOTOR_u8AdjustSpeedPercentage(u8 LOC_u8MotorNo,u8 LOC_u8SpeedPercentage)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if( (LOC_u8MotorNo <= DC_MOTOR2) && (LOC_u8SpeedPercentage <= DC_MOTOR_MAX_SPEED_PERCENTAGE))
	{
		switch(LOC_u8MotorNo)
		{
			case DC_MOTOR0:
				MCAL_TIMER_u8SetPWM(TIMER0,LOC_u8SpeedPercentage);
				break;
			case DC_MOTOR1:
				MCAL_TIMER_u8SetTIMER1PWM(LOC_u8SpeedPercentage,DC_MOTOR1_PWM_FREQUENCY);
				break;
			case DC_MOTOR2:
				MCAL_TIMER_u8SetPWM(TIMER2,LOC_u8SpeedPercentage);
				break;
		}
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;

}