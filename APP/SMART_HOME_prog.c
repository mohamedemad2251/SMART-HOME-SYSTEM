//============================INCLUSIONS======================================
#include "../COTS/MCAL/ADC/ADC_int.h"
#include "../COTS/MCAL/UART/UART_int.h"

#include "../COTS/HAL/DC_MOTOR/DC_MOTOR_int.h"
#include "../COTS/HAL/KEYPAD/KEYPAD.h"
#include "../COTS/HAL/LCD/LCD.h"
#include "../COTS/HAL/SERVO/SERVO_int.h"

#include "SMART_HOME_int.h"
//============================================================================

//============================CALLBACK FUNCTIONS==============================	//These functions don't follow the naming convention
void callback_send_uart_fun(void)	//These functions are used for when an interrupt happens and the CPU is executing the ISR
{
	//Nothing at all
}

void callback_recieve_uart_fun(void)
{
	//Nothing at all
}

void callback_adc_fun(void)
{
	//Nothing at all
}

//NOTE: Please make sure you add callback functions for EVERY peripheral used that uses an interrupt, makes it easier.
//You don't need to write anything inside the function, but it's better than NULL in my implementation for the drivers.
//============================================================================

//============================MAIN FUNCTIONS==================================
static STD_Type APP_SMART_HOME_u8FeatureSelect(u8 LOC_u8AppMode)	//The selector of which feature is running.
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;	//Initially, we assume an error will be returned till the function fixes it.
	//UNFINISHED LOGIC HERE
	return LOC_u8ReturnValue;				//Return the error status
}

STD_Type APP_SMART_HOME_u8App(void)									//Main application function to be run in main.
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;	//Initially, we assume an error will be returned till the function fixes it.
	static u8 LOC_u8Initialized = FALSE;	//Initialization variable, once everything starts normally, the variable will be TRUE.
	while(1)
	{
		if(LOC_u8Initialized == FALSE)			//If it's the first time
		{
			LOC_u8ReturnValue = MCAL_ADC_u8Init(ADC0);				//Initialize the ADC on channel ADC0 (for now)
			LOC_u8ReturnValue = HAL_LCD_u8Init();					//Initialize LCD	(Please make sure you change the configuration in .h file)
			LOC_u8ReturnValue = MCAL_UART_u8Init();					//Initialize UART communication (Bluetooth)
			LOC_u8ReturnValue = HAL_DC_MOTOR_u8Init(DC_MOTOR2);		//Initialize DC Motor (Please avoid using DC_MOTOR1, as it is used for the servo)
																	//DC_MOTOR0---->TIMER0 (OC0 output), DC_MOTOR1---->TIMER1 (OC1B output), DC_MOTOR2---->TIMER2 (OC2 output)
			LOC_u8ReturnValue = HAL_KEYPAD_u8Init();				//Initialize Keypad (Please make sure you change the configuration in .h file)
			LOC_u8ReturnValue = HAL_SERVO_u8Init();					//Initialize Servo Motor
			
			LOC_u8ReturnValue = MCAL_UART_u8CallbackFun(UART_TERMINATION_CHAR,&callback_send_uart_fun);	//Send nothing for now
			LOC_u8ReturnValue = MCAL_ADC_u8CallbackFun(&callback_adc_fun);
			//Add the rest of callback functions here
			
			LOC_u8Initialized = TRUE;								//Now that we initialized the peripherals, this bit of code is no longer useful			
		}
	}
	return LOC_u8ReturnValue;				//Return the error status
}
//============================================================================