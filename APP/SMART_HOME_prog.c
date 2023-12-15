//============================INCLUSIONS======================================
#include "../COTS/MCAL/ADC/ADC_int.h"
#include "../COTS/MCAL/UART/UART_int.h"

#include "../COTS/HAL/DC_MOTOR/DC_MOTOR_int.h"
#include "../COTS/HAL/KEYPAD/KEYPAD.h"
#include "../COTS/HAL/LCD/LCD.h"
#include "../COTS/HAL/SERVO/SERVO_int.h"

#include "SMART_HOME_int.h"
//============================================================================

//============================GLOBAL VARIABLES================================
static u8 GLOB_u8AccessMode = ADMIN_MODE;
typedef struct 
{
	u8 username[MAX_VIRTUAL_USER_CHAR];
	u8 password[MAX_VIRTUAL_USER_PASS];
}VirtualUser;
typedef struct  
{
	u8 username[MAX_KEYPAD_USER_CHAR];
	u8 password[MAX_KEYPAD_USER_PASS];
}KeypadUser;
VirtualUser GLOB_VirtualUserProfile[MAX_USERS];
KeypadUser GLOB_KeypadUserProfile[MAX_USERS];
//============================================================================

void InitiateCredentialsTest(void)	//Should be removed on release
{
	GLOB_VirtualUserProfile[0] = (const VirtualUser){"Hamada","123"};	//Change in these for testing
	GLOB_VirtualUserProfile[1] = (const VirtualUser){"Helal","100100300"};
	GLOB_VirtualUserProfile[2] = (const VirtualUser){"Abo3amo","Ta3meya"};
}

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

//===============================FEATURES=====================================
//Your functions will go here, like reset and such. Please make them STATIC.
static STD_Type APP_SMART_HOME_u8Reset(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8AdminLogin(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8LoginError = TRUE;	//We initially assume an error exists, we clear that error when the credentials are correct
	static u8 LOC_u8Iterations;	//For using for loops, to avoid magical numbers
	static u8 LOC_u8UARTData;	//For receiving and sending data using UART
	static u8 LOC_u8CharCount;	//To keep count of number of characters entered by user
	static u8 LOC_u8LoginTrials;	//How many times did the user fail the login
	static u8 LOC_u8ProfileNo;		//Which profile the algorithm is searching for now
	VirtualUser LOC_VirtualUserProfile;	//User credentials entered, which will be compared with the saved ones
	
	while(LOC_u8LoginError == TRUE && LOC_u8LoginTrials < MAX_LOGIN_TRIALS)
	{
		MCAL_UART_u8SendWord("Please enter your username: ");
		while( (LOC_u8UARTData != ENTER_BUTTON) )
		{
			if(LOC_u8CharCount < MAX_VIRTUAL_USER_CHAR)
			{
				MCAL_UART_u8GetData(&LOC_u8UARTData);
				MCAL_UART_u8SendData(LOC_u8UARTData);
				if(LOC_u8UARTData == ENTER_BUTTON)
				{
					LOC_VirtualUserProfile.username[LOC_u8CharCount] = NULL;
				}
				else if(LOC_u8UARTData)
				{
					LOC_VirtualUserProfile.username[LOC_u8CharCount] = LOC_u8UARTData;
					LOC_u8CharCount++;
				}
			}
			else
			{
				LOC_u8UARTData = ENTER_BUTTON;
			}
		}
		while(LOC_u8ProfileNo < MAX_USERS && LOC_u8LoginError == TRUE)
		{
			for(LOC_u8Iterations;LOC_u8Iterations<LOC_u8CharCount;LOC_u8Iterations++)
			{
				if(LOC_VirtualUserProfile.username[LOC_u8Iterations] != GLOB_VirtualUserProfile[LOC_u8ProfileNo].username[LOC_u8Iterations])
				{
					LOC_u8LoginError = TRUE;
					LOC_u8ProfileNo++;
					break;
				}
				else
				{
					LOC_u8LoginError = FALSE;
				}
			}
			LOC_u8Iterations = ZERO;
		}
		if(LOC_u8LoginError == TRUE)
		{
			MCAL_UART_u8SendWord("Error!\rUsername doesn't exist!\rTrials left: ");
			LOC_u8LoginTrials++;
			MCAL_UART_u8SendData((MAX_LOGIN_TRIALS - LOC_u8LoginTrials)+ASCII);
			MCAL_UART_u8SendData(ENTER_BUTTON);
			LOC_u8UARTData = ZERO;
			LOC_u8LoginError = TRUE;
			LOC_u8Iterations = ZERO;
			LOC_u8CharCount = ZERO;
			LOC_u8ProfileNo = ZERO;
		}
	}
	LOC_u8UARTData = ZERO;
	LOC_u8LoginError = TRUE;
	LOC_u8Iterations = ZERO;
	LOC_u8CharCount = ZERO;
	//LOC_u8ProfileNo = ZERO;	//We want to keep the profile number because password has to match username
	while(LOC_u8LoginError == TRUE && LOC_u8LoginTrials < MAX_LOGIN_TRIALS)
	{
		MCAL_UART_u8SendWord("Please enter your password: ");
		while( (LOC_u8UARTData != ENTER_BUTTON) )
		{
			if(LOC_u8CharCount < MAX_VIRTUAL_USER_PASS)
			{
				MCAL_UART_u8GetData(&LOC_u8UARTData);
				MCAL_UART_u8SendData(LOC_u8UARTData);
				if(LOC_u8UARTData == ENTER_BUTTON)
				{
					LOC_VirtualUserProfile.password[LOC_u8CharCount] = NULL;
				}
				else if(LOC_u8UARTData)
				{
					LOC_VirtualUserProfile.password[LOC_u8CharCount] = LOC_u8UARTData;
					LOC_u8CharCount++;
				}
			}
			else
			{
				LOC_u8UARTData = ENTER_BUTTON;
			}
		}
		if(LOC_u8LoginError == TRUE)
		{
			for(LOC_u8Iterations;LOC_u8Iterations<LOC_u8CharCount;LOC_u8Iterations++)
			{
				if(LOC_VirtualUserProfile.password[LOC_u8Iterations] != GLOB_VirtualUserProfile[LOC_u8ProfileNo].password[LOC_u8Iterations])
				{
					LOC_u8LoginError = TRUE;
					break;
				}
				else
				{
					LOC_u8LoginError = FALSE;
				}
			}
		}
		if(LOC_u8LoginError == TRUE)
		{
			MCAL_UART_u8SendWord("Error!\rPassword is incorrect!\rTrials left: ");
			LOC_u8LoginTrials++;
			MCAL_UART_u8SendData((MAX_LOGIN_TRIALS - LOC_u8LoginTrials)+ASCII);
			MCAL_UART_u8SendData(ENTER_BUTTON);
			LOC_u8UARTData = ZERO;
			LOC_u8LoginError = TRUE;
			LOC_u8Iterations = ZERO;
			LOC_u8CharCount = ZERO;
		}
	}
	if(LOC_u8LoginError == FALSE)
	{	
		MCAL_UART_u8SendWord("\r\rLogged In Successfully!\r");
		
		MCAL_UART_u8SuspendInterrupt();
		HAL_LCD_u8WriteString("Welcome, ");
		HAL_LCD_u8WriteString(LOC_VirtualUserProfile.username);
		HAL_LCD_u8WriteChar('!');
		MCAL_UART_u8ResumeInterrupt();
		
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		MCAL_UART_u8SendWord("\rYOU HAVE BEEN LOCKED OUT!\r");
		MCAL_UART_u8SuspendInterrupt();
		//Alarm logic goes here!	(Elham's job)
		MCAL_UART_u8ResumeInterrupt();
	}
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8UserLogin()
{
	
}

static STD_Type APP_SMART_HOME_u8SelectAccess(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8UARTData;
	MCAL_UART_u8SendWord("Please select access mode\r[1]Admin\r[2]User: ");
	while( (LOC_u8UARTData != ENTER_BUTTON) && (LOC_u8UARTData != USER_SELECTION_1) && (USER_SELECTION_2))
	{
		MCAL_UART_u8GetData(&LOC_u8UARTData);
		MCAL_UART_u8SendData(LOC_u8UARTData);
	}
	MCAL_UART_u8SendData(ENTER_BUTTON);
	MCAL_UART_u8SendWord("\rYou chose: ");
	switch(LOC_u8UARTData)
	{
		case USER_SELECTION_1:
		MCAL_UART_u8SendWord("Admin\r");
		LOC_u8ReturnValue = APP_SMART_HOME_u8AdminLogin();
		break;
		case USER_SELECTION_2:
		MCAL_UART_u8SendWord("User\r");
		LOC_u8ReturnValue = APP_SMART_HOME_u8UserLogin();
		break;
	}
	return LOC_u8ReturnValue;
}
//============================================================================

//============================MAIN FUNCTIONS==================================
static STD_Type APP_SMART_HOME_u8FeatureSelect(u8 LOC_u8AppMode)	//The selector of which feature is running.
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;	//Initially, we assume an error will be returned till the function fixes it.
	if(LOC_u8AppMode <= CONTROL_MODE)
	{
		switch(LOC_u8AppMode)
		{
			case RESET:	//Reset function goes here
				MCAL_UART_u8SuspendInterrupt();
				APP_SMART_HOME_u8Reset();
				MCAL_UART_u8ResumeInterrupt();
				break;
			case LOGIN_MODE:
				APP_SMART_HOME_u8SelectAccess();
				break;
			case CONTROL_MODE:
				break;
		}
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;				//Return the error status
}

STD_Type APP_SMART_HOME_u8App(void)									//Main application function to be run in main.
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;	//Initially, we assume an error will be returned till the function fixes it.
	static u8 LOC_u8Initialized = FALSE;	//Initialization variable, once everything starts normally, the variable will be TRUE.
	u8 LOC_u8SystemMode = RESET;
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
			
			InitiateCredentialsTest();	//FOR TESTING
			
			APP_SMART_HOME_u8FeatureSelect(LOC_u8SystemMode);
			LOC_u8Initialized = TRUE;								//Now that we initialized the peripherals, this bit of code is no longer useful			
		}
		if(LOC_u8SystemMode == RESET)
		{
			LOC_u8SystemMode = LOGIN_MODE;
		}
		else if(LOC_u8SystemMode == LOGIN_MODE)
		{
			LOC_u8SystemMode = CONTROL_MODE;
		}
		APP_SMART_HOME_u8FeatureSelect(LOC_u8SystemMode);
	}
	return LOC_u8ReturnValue;				//Return the error status
}
//============================================================================