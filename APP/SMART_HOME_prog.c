//============================INCLUSIONS======================================
#include "../COTS/MCAL/ADC/ADC_int.h"
#include "../COTS/MCAL/UART/UART_int.h"

#include "../COTS/HAL/DC_MOTOR/DC_MOTOR_int.h"
#include "../COTS/HAL/KEYPAD/KEYPAD.h"
#include "../COTS/HAL/LCD/LCD.h"
#include "../COTS/HAL/SERVO/SERVO_int.h"
#include "../COTS/HAL/HAL_EEPROM/HAL_EEPROM.h"

#include "SMART_HOME_int.h"
//============================================================================

//============================GLOBAL VARIABLES================================
static u8 GLOB_u8AccessMode = ADMIN_MODE;
static u8 GLOB_u8SystemMode = RESET;
static u8 GLOB_u8InputOverride = PENDING;
static u8 GLOB_u8FreeSpaceExists = FALSE;
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

void callback_AlarmLED_fun(void)
{
	MCAL_DIO_u8TogglePinValue(ALARM_LED_PORT,ALARM_LED_PIN);
}

//NOTE: Please make sure you add callback functions for EVERY peripheral used that uses an interrupt, makes it easier.
//You don't need to write anything inside the function, but it's better than NULL in my implementation for the drivers.
//============================================================================

//===============================FEATURES=====================================
//Your functions will go here, like reset and such. Please make them STATIC.
static STD_Type APP_SMART_HOME_u8BlockState(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8Initialized = FALSE;
	
	if(LOC_u8Initialized == FALSE)
	{
		MCAL_TIMER_u8CallbackMilliFun(TIMER0,ALARM_DELAY_TIME,&callback_AlarmLED_fun);
		LOC_u8Initialized = TRUE;
	}
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8DoorAccess(void)
{

	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UARTData = ZERO;
	MCAL_UART_u8SendWord("Please select DOOR mode\r[1]Open\r[2]Close: ");
	while( (LOC_u8UARTData != USER_SELECTION_1) && (LOC_u8UARTData != USER_SELECTION_2))
	{
		MCAL_UART_u8GetData(&LOC_u8UARTData);
		MCAL_UART_u8SendData(LOC_u8UARTData);
	}
	MCAL_UART_u8SendData(ENTER_BUTTON);
	MCAL_UART_u8SendWord("\rYou chose: ");
	switch(LOC_u8UARTData)
	{
		case USER_SELECTION_1:
		MCAL_UART_u8SendWord("Open\r");
		LOC_u8ReturnValue =HAL_SERVO_u8TurnDegree(SERVO_DEGREE_OPEN);
		break;
		case USER_SELECTION_2:
		MCAL_UART_u8SendWord("Close\r");
		LOC_u8ReturnValue =HAL_SERVO_u8TurnDegree(SERVO_DEGREE_CLOSE);
		break;
	}
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8UpdateProfiles(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UserCount = ZERO;
	
	while(LOC_u8UserCount < MAX_USERS)
	{
		HAL_EEPROM_u8ReadWord( (LOC_u8UserCount*(MAX_VIRTUAL_USER_CHAR + MAX_VIRTUAL_USER_PASS) ),MAX_VIRTUAL_USER_CHAR,&(GLOB_VirtualUserProfile[LOC_u8UserCount].username));
		HAL_EEPROM_u8ReadWord( (LOC_u8UserCount*(MAX_VIRTUAL_USER_CHAR + MAX_VIRTUAL_USER_PASS)) + MAX_VIRTUAL_USER_CHAR,MAX_VIRTUAL_USER_PASS,&(GLOB_VirtualUserProfile[LOC_u8UserCount].password));
		HAL_EEPROM_u8ReadWord( (LOC_u8UserCount*(MAX_KEYPAD_USER_CHAR + MAX_KEYPAD_USER_PASS) ) + KEYPAD_MEMORY_OFFSET,MAX_KEYPAD_USER_CHAR,&(GLOB_KeypadUserProfile[LOC_u8UserCount].username));
		HAL_EEPROM_u8ReadWord( (LOC_u8UserCount*(MAX_KEYPAD_USER_CHAR + MAX_KEYPAD_USER_PASS)) + KEYPAD_MEMORY_OFFSET + MAX_KEYPAD_USER_CHAR,MAX_KEYPAD_USER_PASS,&(GLOB_KeypadUserProfile[LOC_u8UserCount].password));
		LOC_u8UserCount++;
	}
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}
static STD_Type APP_SMART_HOME_u8Reset(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8ACControl(void)
{
	STD_Type LOC_u8ReturnValue= E_NOT_OK;
	u16 LOC_u16SensorRead;
	u16 LOC_u16Temperature;

	MCAL_ADC_u8StartConversion();
	MCAL_ADC_u8GetConversion(&LOC_u16SensorRead,ADC0);
	LOC_u16Temperature = ((LOC_u16SensorRead*Unit_Conversion)-FIVE_HUNDERED)/TEN ;             //convert the sensor read to degree Celsius
	if(LOC_u16Temperature)
	{		
		if(LOC_u16Temperature>MAX_TEMP)                                                       //max temp is 28
		{
			MCAL_DIO_u8SetPinValue(AC_PORT,AC_PIN,DIO_HIGH);
			LOC_u8ReturnValue= E_OK;
		}
		else if (LOC_u16Temperature<=MIN_TEMP)                                                 //min temp is 21
		{
			MCAL_DIO_u8SetPinValue(AC_PORT,AC_PIN,DIO_LOW);
			LOC_u8ReturnValue= E_OK;
		}
		else
		{
			//wait for user input?
		}
		LOC_u8ReturnValue= E_OK;
	}
	else
	{
		//Do Nothing
	}

	LOC_u8ReturnValue= E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8LightLamp(u8 LOC_u8Choice,u8 LOC_u8LampState)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8Choice < LAMP_6 && LOC_u8LampState >= OPTION_ON && LOC_u8LampState <= OPTION_OFF)
	{
		switch(LOC_u8Choice)
		{
			case LAMP_1:
				switch(LOC_u8LampState)
				{
					case OPTION_OFF:
						MCAL_DIO_u8SetPinValue(LAMP_1_PORT,LAMP_1_PIN,DIO_LOW);
						break;
					case OPTION_ON:
						MCAL_DIO_u8SetPinValue(LAMP_1_PORT,LAMP_1_PIN,DIO_HIGH);
						break;
				}
				break;
			case LAMP_2:
				switch(LOC_u8LampState)
				{
					case OPTION_OFF:
						MCAL_DIO_u8SetPinValue(LAMP_2_PORT,LAMP_2_PIN,DIO_LOW);
						break;
					case OPTION_ON:
						MCAL_DIO_u8SetPinValue(LAMP_2_PORT,LAMP_2_PIN,DIO_HIGH);
						break;
				}
				break;
			case LAMP_3:
				switch(LOC_u8LampState)
				{
					case OPTION_OFF:
						MCAL_DIO_u8SetPinValue(LAMP_3_PORT,LAMP_3_PIN,DIO_LOW);
						break;
					case OPTION_ON:
						MCAL_DIO_u8SetPinValue(LAMP_3_PORT,LAMP_3_PIN,DIO_HIGH);
						break;
				}
				break;
			case LAMP_4:
				switch(LOC_u8LampState)
				{
					case OPTION_OFF:
						MCAL_DIO_u8SetPinValue(LAMP_4_PORT,LAMP_4_PIN,DIO_LOW);
						break;
					case OPTION_ON:
						MCAL_DIO_u8SetPinValue(LAMP_4_PORT,LAMP_4_PIN,DIO_HIGH);
						break;
				}
				break;
			case LAMP_5:
				switch(LOC_u8LampState)
				{
					case OPTION_OFF:
						MCAL_DIO_u8SetPinValue(LAMP_5_PORT,LAMP_5_PIN,DIO_LOW);
						break;
					case OPTION_ON:
						MCAL_DIO_u8SetPinValue(LAMP_5_PORT,LAMP_5_PIN,DIO_HIGH);
						break;
				}
				break;
		}
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//do nothing
	}
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8KeypadDimmingLamp(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8ButtonPressed = KEYPAD_CLEAR;
	u8 LOC_u8Base = ONE;
	u8 LOC_u8Digits = ZERO;
	u8 LOC_u8Percentage = ZERO;
	u8 LOC_u8DimmingDigits[MAX_DIMMING_CHAR] = {};
 
	while(LOC_u8ButtonPressed == KEYPAD_CLEAR)
	{
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8WriteString("Percentage: ");
		while(LOC_u8ButtonPressed != KEYPAD_ENTER && LOC_u8Digits < MAX_DIMMING_CHAR)
		{
			HAL_KEYPAD_u8GetButtonPressed(&LOC_u8ButtonPressed);
			if(LOC_u8ButtonPressed >= KEYPAD_MIN_LIMIT && LOC_u8ButtonPressed <= KEYPAD_MAX_LIMIT)
			{
				HAL_LCD_u8WriteChar(LOC_u8ButtonPressed);
				LOC_u8DimmingDigits[LOC_u8Digits] = (LOC_u8ButtonPressed - ASCII);
				LOC_u8Digits++;
			}
			else if(LOC_u8ButtonPressed == KEYPAD_ENTER && LOC_u8Digits == ZERO)
			{
				LOC_u8ButtonPressed = NULL_CHAR;
			}
			else if(LOC_u8ButtonPressed == KEYPAD_CLEAR)
			{
				break;
			}
		}
		if( (LOC_u8ButtonPressed >= KEYPAD_MIN_LIMIT && LOC_u8ButtonPressed <= KEYPAD_MAX_LIMIT) || (LOC_u8ButtonPressed == KEYPAD_ENTER) )
		{
			LOC_u8Digits = LOC_u8Digits - ONE;
			for(LOC_u8Digits;LOC_u8Digits >= ZERO;LOC_u8Digits--)
			{
				LOC_u8Percentage = LOC_u8Percentage + (LOC_u8Base) * (LOC_u8DimmingDigits[LOC_u8Digits]);
				LOC_u8Base = LOC_u8Base * BASE_TEN;
				if(LOC_u8Digits == ZERO)
				{
					break;
				}
			}
			if(LOC_u8Percentage > (BASE_TEN * BASE_TEN))
			{
				LOC_u8Percentage = (BASE_TEN * BASE_TEN);	//100%
			}
			MCAL_TIMER_u8SetPWM(TIMER2,LOC_u8Percentage);
		}
		LOC_u8Digits = ZERO;
		LOC_u8ButtonPressed = NULL_CHAR;
	}
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualDimmingLamp(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UARTData = NULL_CHAR;
	u8 LOC_u8Base = ONE;
	u8 LOC_u8Digits = ZERO;
	u8 LOC_u8Percentage = ZERO;
	u8 LOC_u8DimmingDigits[MAX_DIMMING_CHAR] = {};
		
	MCAL_UART_u8SendWord("Percentage: ");
	MCAL_UART_u8ResumeInterrupt();
	while(LOC_u8UARTData != ENTER_BUTTON && LOC_u8Digits < MAX_DIMMING_CHAR)
	{
		MCAL_UART_u8GetData(&LOC_u8UARTData);
		MCAL_UART_u8SendData(LOC_u8UARTData);
		if(LOC_u8UARTData >= KEYPAD_MIN_LIMIT && LOC_u8UARTData <= KEYPAD_MAX_LIMIT)
		{
			LOC_u8DimmingDigits[LOC_u8Digits] = (LOC_u8UARTData - ASCII);
			LOC_u8Digits++;
		}
		else if(LOC_u8UARTData == ENTER_BUTTON && LOC_u8Digits == ZERO)
		{
			LOC_u8UARTData = NULL_CHAR;
		}
	}
	MCAL_UART_u8SendData(ENTER_BUTTON);
	MCAL_UART_u8SuspendInterrupt();
	if( (LOC_u8UARTData >= KEYPAD_MIN_LIMIT && LOC_u8UARTData <= KEYPAD_MAX_LIMIT) || (LOC_u8UARTData == ENTER_BUTTON) )
	{
		LOC_u8Digits = LOC_u8Digits - ONE;
		for(LOC_u8Digits;LOC_u8Digits >= ZERO;LOC_u8Digits--)
		{
			LOC_u8Percentage = LOC_u8Percentage + (LOC_u8Base) * (LOC_u8DimmingDigits[LOC_u8Digits]);
			LOC_u8Base = LOC_u8Base * BASE_TEN;
			if(LOC_u8Digits == ZERO)
			{
				break;
			}
		}
		if(LOC_u8Percentage > (BASE_TEN * BASE_TEN))
		{
			LOC_u8Percentage = (BASE_TEN * BASE_TEN);	//100%
		}
		MCAL_TIMER_u8SetPWM(TIMER2,LOC_u8Percentage);
	}
// 	LOC_u8Digits = ZERO;
// 	LOC_u8UARTData = NULL_CHAR;
	MCAL_UART_u8ResumeInterrupt();
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8KeypadLamps(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8ButtonPressed = NULL_CHAR;
	u8 LOC_u8LampChoice = NULL_CHAR;
	u8 LOC_u8LampState = NULL_CHAR;
	
	HAL_LCD_u8ClearScreen();
	HAL_LCD_u8WriteString("Select a lamp!");
	HAL_LCD_u8GotoCursor(MIN_X_COOR,MAX_Y_COOR);
	HAL_LCD_u8WriteString("[1-6]Lamp 1-6");
	while(LOC_u8ButtonPressed < LAMP_1 || LOC_u8ButtonPressed > LAMP_6)
	{
		HAL_KEYPAD_u8GetButtonPressed(&LOC_u8ButtonPressed);
	}
	if(LOC_u8ButtonPressed >= LAMP_1 && LOC_u8ButtonPressed <= LAMP_5)
	{
		LOC_u8LampChoice = LOC_u8ButtonPressed;
		LOC_u8ButtonPressed = NULL_CHAR;
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8WriteString("Select an option");
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MAX_Y_COOR);
		HAL_LCD_u8WriteString("[1]ON [2]OFF");
		while(LOC_u8ButtonPressed != OPTION_OFF && LOC_u8ButtonPressed != OPTION_ON)
		{
			HAL_KEYPAD_u8GetButtonPressed(&LOC_u8ButtonPressed);
		}
		LOC_u8LampState = LOC_u8ButtonPressed;
		APP_SMART_HOME_u8LightLamp(LOC_u8LampChoice,LOC_u8LampState);
	}
	else if(LOC_u8ButtonPressed == LAMP_6)
	{
		HAL_LCD_u8ClearScreen();
		APP_SMART_HOME_u8KeypadDimmingLamp();
	}
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualLamps(void)
{
		STD_Type LOC_u8ReturnValue = E_NOT_OK;
		u8 LOC_u8UARTData = NULL_CHAR;
		u8 LOC_u8LampChoice = NULL_CHAR;
		u8 LOC_u8LampState = NULL_CHAR;
		
		MCAL_UART_u8SendWord("Select a lamp!   ");
		MCAL_UART_u8SendWord("[1-6]Lamp 1-6\rYour choice: ");
		MCAL_UART_u8ResumeInterrupt();
		while(LOC_u8UARTData < LAMP_1 || LOC_u8UARTData > LAMP_6)
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);
		}
		MCAL_UART_u8SuspendInterrupt();
		if(LOC_u8UARTData >= LAMP_1 && LOC_u8UARTData <= LAMP_5)
		{
			LOC_u8LampChoice = LOC_u8UARTData;
			LOC_u8UARTData = NULL_CHAR;
			MCAL_UART_u8SendWord("\rSelect an option!\r");
			MCAL_UART_u8SendWord("[1]ON     [2]OFF\rYour choice: ");
			MCAL_UART_u8ResumeInterrupt();
			while(LOC_u8UARTData != OPTION_OFF && LOC_u8UARTData != OPTION_ON)
			{
				MCAL_UART_u8GetData(&LOC_u8UARTData);
				MCAL_UART_u8SendData(LOC_u8UARTData);
			}
			LOC_u8LampState = LOC_u8UARTData;
			APP_SMART_HOME_u8LightLamp(LOC_u8LampChoice,LOC_u8LampState);
		}
		else if(LOC_u8UARTData == LAMP_6)
		{
			MCAL_UART_u8SendWord("\r\r");
			APP_SMART_HOME_u8VirtualDimmingLamp();
		}
		LOC_u8ReturnValue = E_OK;
		return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualControl(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8Initialized = FALSE;
	static u8 LOC_u8MessageSent = FALSE;
	u8 LOC_u8UARTData = ZERO;
	if(LOC_u8Initialized == FALSE)
	{
		MCAL_UART_u8SendWord("\r\r\r\r\r\r\r\r\r\r                         CONTROL MODE\r");
		LOC_u8Initialized = TRUE;
	}
	if(GLOB_u8AccessMode == ADMIN_MODE)
	{
		if(LOC_u8MessageSent == FALSE)
		{
			MCAL_UART_u8SendWord("\rSelect your option: [1]Lamps        [2]Door\rYour choice: ");
			LOC_u8MessageSent = TRUE;
		}
		if(LOC_u8UARTData != LAMP_OPTION && LOC_u8UARTData != DOOR_OPTION)
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);
		}
		//MCAL_UART_u8SendData(ENTER_BUTTON);
		switch(LOC_u8UARTData)
		{
			case LAMP_OPTION:
				APP_SMART_HOME_u8VirtualLamps();
				LOC_u8MessageSent = FALSE;
				break;
			case DOOR_OPTION:
				APP_SMART_HOME_u8DoorAccess();
				LOC_u8MessageSent = FALSE;
				break;
		}
	}
	else if(GLOB_u8AccessMode == USER_MODE)
	{
		if(LOC_u8MessageSent == FALSE)
		{
			MCAL_UART_u8SendWord("\rSelect your option: [1]Lamps\rYour choice: ");
			LOC_u8MessageSent = TRUE;
		}
		if(LOC_u8UARTData != LAMP_OPTION)
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);
		}
		//MCAL_UART_u8SendData(ENTER_BUTTON);
		switch(LOC_u8UARTData)
		{
			case LAMP_OPTION:
			APP_SMART_HOME_u8VirtualLamps();
			LOC_u8MessageSent = FALSE;
			break;
		}
	}
	APP_SMART_HOME_u8ACControl();
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8KeypadControl(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8Initialized = FALSE;
	u8 LOC_u8ButtonPressed = NULL_CHAR;
	static u8 LOC_u8MessageWritten = FALSE;
	if(LOC_u8Initialized == FALSE)
	{
		HAL_LCD_u8ClearScreen();
		LOC_u8Initialized = TRUE;
	}
	if(LOC_u8MessageWritten == FALSE)
	{
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8WriteString("Choose an option");
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MAX_Y_COOR);
		HAL_LCD_u8WriteString("[1] Lamps");
		LOC_u8MessageWritten = TRUE;
	}
	HAL_KEYPAD_u8GetButtonPressed(&LOC_u8ButtonPressed);
	if(LOC_u8ButtonPressed == LAMP_OPTION)
	{
		APP_SMART_HOME_u8KeypadLamps();
		LOC_u8MessageWritten = FALSE;
	}
	APP_SMART_HOME_u8ACControl();
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualLogin(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8LoginError = TRUE;	//We initially assume an error exists, we clear that error when the credentials are correct
	static u8 LOC_u8Iterations;	//For using for loops, to avoid magical numbers
	static u8 LOC_u8UARTData;	//For receiving and sending data using UART
	static u8 LOC_u8CharCount;	//To keep count of number of characters entered by user
	static u8 LOC_u8LoginTrials;	//How many times did the user fail the login
	static u8 LOC_u8ProfileNo;		//Which profile the algorithm is searching for now
	VirtualUser LOC_VirtualUserProfile = {{},{}};	//User credentials entered, which will be compared with the saved ones
			
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
					if(LOC_u8CharCount != ZERO)
					{
						LOC_VirtualUserProfile.username[LOC_u8CharCount] = NULL_CHAR;
					}
					else
					{
						LOC_u8UARTData = NULL_CHAR;
					}
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
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_VIRTUAL_USER_CHAR;LOC_u8Iterations++)
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
					if(LOC_u8CharCount != ZERO)
					{
						LOC_VirtualUserProfile.password[LOC_u8CharCount] = NULL_CHAR;
					}
					else
					{
						LOC_u8UARTData = NULL_CHAR;
					}
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
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_VIRTUAL_USER_PASS;LOC_u8Iterations++)
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
			MCAL_UART_u8SendWord("\rError!\rPassword is incorrect!\rTrials left: ");
			LOC_u8LoginTrials++;
			MCAL_UART_u8SendData((MAX_LOGIN_TRIALS - LOC_u8LoginTrials)+ASCII);
			MCAL_UART_u8SendData(ENTER_BUTTON);
			LOC_u8UARTData = ZERO;
			LOC_u8LoginError = TRUE;
			LOC_u8Iterations = ZERO;
			LOC_u8CharCount = ZERO;
		}
	}
	LOC_u8UARTData = ENTER_BUTTON;
	if(LOC_u8LoginError == FALSE)
	{	
		MCAL_UART_u8SendWord("\r\rLogged In Successfully!\r");
		GLOB_u8SystemMode = CONTROL_MODE;
		MCAL_UART_u8ResumeInterrupt();
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		MCAL_UART_u8SendWord("\rYOU HAVE BEEN LOCKED OUT!\r");
		MCAL_UART_u8SuspendInterrupt();
		GLOB_u8SystemMode = LOCKED;
		MCAL_UART_u8ResumeInterrupt();
	}
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8KeypadUserLogin(u8 LOC_u8KeypadData)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8LoginError = TRUE;	//We initially assume an error exists, we clear that error when the credentials are correct
	u8 LOC_u8Iterations = ZERO;	//For using for loops, to avoid magical numbers
	//LOC_u8KeypadData = KEYPAD_CLEAR;	//For receiving data using Keypad
	u8 LOC_u8CharCount = ZERO;	//To keep count of number of characters entered by user
	static u8 LOC_u8LoginTrials;	//How many times did the user fail the login
	static u8 LOC_u8ProfileNo;		//Which profile the algorithm is searching for now
	KeypadUser LOC_KeypadUserProfile = {{},{}};	//User credentials entered, which will be compared with the saved ones

	while(LOC_u8LoginError == TRUE && LOC_u8LoginTrials < MAX_LOGIN_TRIALS)
	{
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MAX_Y_COOR);
		HAL_LCD_u8WriteString("Trials Left: ");
		HAL_LCD_u8WriteInteger(MAX_LOGIN_TRIALS - LOC_u8LoginTrials);
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MIN_Y_COOR);
		HAL_LCD_u8WriteString("Username: ");
		while( (LOC_u8KeypadData != KEYPAD_ENTER) )
		{
			if(LOC_u8CharCount < MAX_KEYPAD_USER_CHAR)
			{
				HAL_KEYPAD_u8GetButtonPressed(&LOC_u8KeypadData);
				if(LOC_u8KeypadData >= KEYPAD_MIN_LIMIT && LOC_u8KeypadData <= KEYPAD_MAX_LIMIT)
				{
					HAL_LCD_u8WriteChar(LOC_u8KeypadData);
				}
				if(LOC_u8KeypadData == KEYPAD_CLEAR)
				{
					return E_OK;
				}
				else if(LOC_u8KeypadData == KEYPAD_ENTER)
				{
					if(LOC_u8CharCount != ZERO)
					{
						LOC_KeypadUserProfile.username[LOC_u8CharCount] = NULL_CHAR;
					}
					else
					{
						LOC_u8KeypadData = NULL_CHAR;
					}
				}
				else if(LOC_u8KeypadData >= KEYPAD_MIN_LIMIT && LOC_u8KeypadData <= KEYPAD_MAX_LIMIT)
				{
					LOC_KeypadUserProfile.username[LOC_u8CharCount] = LOC_u8KeypadData;
					LOC_u8CharCount++;
				}
			}
			else
			{
				LOC_u8KeypadData = KEYPAD_ENTER;
			}
		}
		while(LOC_u8ProfileNo < MAX_USERS && LOC_u8LoginError == TRUE)
		{
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_KEYPAD_USER_CHAR;LOC_u8Iterations++)
			{
				if(LOC_KeypadUserProfile.username[LOC_u8Iterations] != GLOB_KeypadUserProfile[LOC_u8ProfileNo].username[LOC_u8Iterations])
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
			LOC_u8LoginTrials++;
			LOC_u8KeypadData = ZERO;
			LOC_u8LoginError = TRUE;
			LOC_u8Iterations = ZERO;
			LOC_u8CharCount = ZERO;
			LOC_u8ProfileNo = ZERO;
		}
	}

	LOC_u8KeypadData = ZERO;
	LOC_u8LoginError = TRUE;
	LOC_u8Iterations = ZERO;
	LOC_u8CharCount = ZERO;
	//LOC_u8ProfileNo = ZERO;	//We want to keep the profile number because password has to match username
	while(LOC_u8LoginError == TRUE && LOC_u8LoginTrials < MAX_LOGIN_TRIALS)
	{
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MAX_Y_COOR);
		HAL_LCD_u8WriteString("Trials left: ");
		HAL_LCD_u8WriteInteger(MAX_LOGIN_TRIALS - LOC_u8LoginTrials);
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MIN_Y_COOR);
		HAL_LCD_u8WriteString("Password: ");
		while( (LOC_u8KeypadData != KEYPAD_ENTER) )
		{
			if(LOC_u8CharCount < MAX_KEYPAD_USER_PASS)
			{
				HAL_KEYPAD_u8GetButtonPressed(&LOC_u8KeypadData);
				if(LOC_u8KeypadData >= KEYPAD_MIN_LIMIT && LOC_u8KeypadData <= KEYPAD_MAX_LIMIT)
				{
					HAL_LCD_u8WriteChar(LOC_u8KeypadData);
				}
				if(LOC_u8KeypadData == KEYPAD_CLEAR)
				{
					return E_OK;
				}
				else if(LOC_u8KeypadData == KEYPAD_ENTER)
				{
					if(LOC_u8CharCount != ZERO)
					{
						LOC_KeypadUserProfile.password[LOC_u8CharCount] = NULL_CHAR;
					}
					else
					{
						LOC_u8KeypadData = NULL_CHAR;
					}
				}
				else if(LOC_u8KeypadData <= KEYPAD_MAX_LIMIT && LOC_u8KeypadData >= KEYPAD_MIN_LIMIT)
				{
					LOC_KeypadUserProfile.password[LOC_u8CharCount] = LOC_u8KeypadData;
					LOC_u8CharCount++;
				}
			}
			else
			{
				LOC_u8KeypadData = KEYPAD_ENTER;
			}
		}
		if(LOC_u8LoginError == TRUE)
		{
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_KEYPAD_USER_PASS;LOC_u8Iterations++)
			{
				if(LOC_KeypadUserProfile.password[LOC_u8Iterations] != GLOB_KeypadUserProfile[LOC_u8ProfileNo].password[LOC_u8Iterations])
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
			LOC_u8LoginTrials++;
			LOC_u8KeypadData = ZERO;
			LOC_u8LoginError = TRUE;
			LOC_u8Iterations = ZERO;
			LOC_u8CharCount = ZERO;
		}
	}
	LOC_u8KeypadData = KEYPAD_ENTER;
	if(LOC_u8LoginError == FALSE)
	{
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8WriteString("Welcome!");
		HAL_LCD_u8GotoCursor(MIN_X_COOR,MAX_Y_COOR);
		HAL_LCD_u8WriteString("Press ON/C!");
		while(LOC_u8KeypadData != KEYPAD_CLEAR)
		{
			HAL_KEYPAD_u8GetButtonPressed(&LOC_u8KeypadData);
		}
		GLOB_u8SystemMode = CONTROL_MODE;
		MCAL_UART_u8ResumeInterrupt();
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		HAL_LCD_u8ClearScreen();
		HAL_LCD_u8WriteString("YOU ARE LOCKED");
		MCAL_UART_u8SuspendInterrupt();
		GLOB_u8SystemMode = LOCKED;
		MCAL_UART_u8ResumeInterrupt();
	}
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualAdminAdd(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UARTData = ZERO;
	u8 LOC_u8UserCount = ZERO;
	u8 LOC_u8Iterations = ZERO;
	u8 LOC_u8CheckCounter = ZERO;
	MCAL_UART_u8SuspendInterrupt();
	
	for(LOC_u8UserCount;LOC_u8UserCount < MAX_USERS; LOC_u8UserCount++)
	{
		for(LOC_u8Iterations;LOC_u8Iterations < (MAX_VIRTUAL_USER_CHAR);LOC_u8Iterations++)
		{
			if(GLOB_VirtualUserProfile[LOC_u8UserCount].username[LOC_u8Iterations] == NULL_CHAR)
			{
				LOC_u8CheckCounter++;
			}
		}
		LOC_u8Iterations = ZERO;
		for(LOC_u8Iterations;LOC_u8Iterations < (MAX_VIRTUAL_USER_PASS);LOC_u8Iterations++)
		{
			if(GLOB_VirtualUserProfile[LOC_u8UserCount].password[LOC_u8Iterations] == NULL_CHAR)
			{
				LOC_u8CheckCounter++;
			}
		}
		if(LOC_u8CheckCounter == (MAX_VIRTUAL_USER_CHAR + MAX_VIRTUAL_USER_PASS) )
		{
			GLOB_u8FreeSpaceExists = TRUE;
			break;
		}
		else
		{
			LOC_u8CheckCounter = ZERO;
			LOC_u8Iterations = ZERO;
		}
	}
// 	HAL_LCD_u8ClearScreen();
// 	HAL_LCD_u8WriteInteger(LOC_u8UserCount);
	if(GLOB_u8FreeSpaceExists == TRUE)
	{
		LOC_u8CheckCounter = ZERO;
		MCAL_UART_u8SendWord("\rEnter the username to add: ");
		MCAL_UART_u8ResumeInterrupt();
		while(LOC_u8CheckCounter < MAX_VIRTUAL_USER_CHAR && LOC_u8UARTData != ENTER_BUTTON )
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);
			if(LOC_u8UARTData != ENTER_BUTTON && LOC_u8UARTData != NULL_CHAR)
			{
				GLOB_VirtualUserProfile[LOC_u8UserCount].username[LOC_u8CheckCounter] = LOC_u8UARTData;
				LOC_u8CheckCounter++;
			}
			else if(LOC_u8UARTData == ENTER_BUTTON)
			{
				if(LOC_u8CheckCounter == ZERO)
				{
					LOC_u8UARTData = ZERO;
				}
			}
		}
		LOC_u8CheckCounter = ZERO;
		LOC_u8UARTData = ZERO;
		MCAL_UART_u8SendWord("\rEnter the password to add: ");
		while(LOC_u8CheckCounter < MAX_VIRTUAL_USER_PASS && LOC_u8UARTData != ENTER_BUTTON)
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);
			if(LOC_u8UARTData != ENTER_BUTTON && LOC_u8UARTData != NULL_CHAR)
			{
				GLOB_VirtualUserProfile[LOC_u8UserCount].password[LOC_u8CheckCounter] = LOC_u8UARTData;
				LOC_u8CheckCounter++;
			}
			else if(LOC_u8UARTData == ENTER_BUTTON)
			{
				if(LOC_u8CheckCounter == ZERO)
				{
					LOC_u8UARTData = ZERO;
				}
			}
		}
		MCAL_UART_u8SendData(ENTER_BUTTON);
		MCAL_UART_u8SuspendInterrupt();
		HAL_EEPROM_u8WriteWord((LOC_u8UserCount * (MAX_VIRTUAL_USER_CHAR + MAX_VIRTUAL_USER_PASS)),GLOB_VirtualUserProfile[LOC_u8UserCount].username);
		HAL_EEPROM_u8WriteWord( (LOC_u8UserCount * (MAX_VIRTUAL_USER_CHAR + MAX_VIRTUAL_USER_PASS)) + MAX_VIRTUAL_USER_PASS,GLOB_VirtualUserProfile[LOC_u8UserCount].password);
		//MCAL_UART_u8ResumeInterrupt();
	}
	else
	{
		MCAL_UART_u8SendWord("Can't add more users!\rPlease delete users first!\r");
	}
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualAdminDelete(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UserCount = ZERO;
	u8 LOC_u8Iterations = ZERO;
	u8 LOC_u8Interface = USER_SELECTION_1;
	u8 LOC_u8UARTData = ZERO;
	u8 LOC_u8Digits = ZERO;
	
	MCAL_UART_u8SuspendInterrupt();
	for(LOC_u8Iterations;LOC_u8Iterations<MAX_USERS;LOC_u8Iterations++)
	{
		if(GLOB_VirtualUserProfile[LOC_u8Iterations].username[ZERO] != NULL_CHAR)	//User exists
		{
			LOC_u8UserCount++;
		}	
	}
	LOC_u8Iterations = ZERO;
	if(LOC_u8UserCount > ZERO)
	{
		MCAL_UART_u8SendWord("\rChoose a user to delete from these:\r");
		for(LOC_u8Iterations;LOC_u8Iterations<MAX_USERS;LOC_u8Iterations++)
		{
			if(GLOB_VirtualUserProfile[LOC_u8Iterations].username[ZERO] != NULL_CHAR)	//User exists
			{
				MCAL_UART_u8SendData('[');
				MCAL_UART_u8SendData(LOC_u8Interface);
				MCAL_UART_u8SendWord("] ");
				for(LOC_u8Digits;LOC_u8Digits<MAX_VIRTUAL_USER_CHAR;LOC_u8Digits++)
				{
					MCAL_UART_u8SendData(GLOB_VirtualUserProfile[LOC_u8Iterations].username[LOC_u8Digits]);	
				}
				MCAL_UART_u8SendData(ENTER_BUTTON);
			}
			else
			{
				MCAL_UART_u8SendData('[');
				MCAL_UART_u8SendData(LOC_u8Interface);
				MCAL_UART_u8SendWord("] ");
				MCAL_UART_u8SendWord("NOT FOUND");
				MCAL_UART_u8SendData(ENTER_BUTTON);
			}
			LOC_u8Interface++;
			LOC_u8Digits = ZERO;
		}
		MCAL_UART_u8SendWord("\rYour choice: ");
		MCAL_UART_u8ResumeInterrupt();
		while(LOC_u8UARTData < USER_SELECTION_1 || LOC_u8UARTData >= LOC_u8Interface)	//Open range depending on the number of users existing
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);	//Sending UDR after receiving it serves as emptying UDR for no data ignorance.
		}
		MCAL_UART_u8SuspendInterrupt();
		LOC_u8UARTData = LOC_u8UARTData - ASCII;	//This will now be either 1, 2 or 3, it's now a number not ASCII
		if(GLOB_VirtualUserProfile[LOC_u8UARTData-ONE].username[ZERO] != NULL_CHAR)
		{
			MCAL_UART_u8SendWord("\rDeleted Successfully!\r");
			LOC_u8Iterations = ZERO;
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_VIRTUAL_USER_CHAR;LOC_u8Iterations++)
			{
				GLOB_VirtualUserProfile[LOC_u8UARTData-ONE].username[LOC_u8Iterations] = NULL_CHAR;
			}
			LOC_u8Iterations = ZERO;
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_VIRTUAL_USER_PASS;LOC_u8Iterations++)
			{
				GLOB_VirtualUserProfile[LOC_u8UARTData-ONE].password[LOC_u8Iterations] = NULL_CHAR;
			}
			HAL_EEPROM_u8ClearBytes( ((LOC_u8UARTData-ONE)*(MAX_VIRTUAL_USER_CHAR+MAX_VIRTUAL_USER_PASS)) ,MAX_VIRTUAL_USER_CHAR);
			HAL_EEPROM_u8ClearBytes( ((LOC_u8UARTData-ONE)*(MAX_VIRTUAL_USER_CHAR+MAX_VIRTUAL_USER_PASS)) + MAX_VIRTUAL_USER_CHAR ,MAX_VIRTUAL_USER_PASS);
			MCAL_UART_u8ResumeInterrupt();
		}
		else
		{
			MCAL_UART_u8SendWord("\rCan't delete a user that doesn't exist!\r\r");
		}
	}
	else
	{
		MCAL_UART_u8SendWord("\rNo users exist yet!, Add users first!\r");
	}
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualUserAdd(void)	//Still need to handle password and EEPROM here!
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UARTData = ZERO;
	u8 LOC_u8UserCount = ZERO;
	u8 LOC_u8Iterations = ZERO;
	u8 LOC_u8CheckCounter = ZERO;
	MCAL_UART_u8SuspendInterrupt();
	
	for(LOC_u8UserCount;LOC_u8UserCount < MAX_USERS; LOC_u8UserCount++)
	{
		for(LOC_u8Iterations;LOC_u8Iterations < (MAX_KEYPAD_USER_CHAR);LOC_u8Iterations++)
		{
			if(GLOB_KeypadUserProfile[LOC_u8UserCount].username[LOC_u8Iterations] == NULL_CHAR)
			{
				LOC_u8CheckCounter++;
			}
		}
		LOC_u8Iterations = ZERO;
		for(LOC_u8Iterations;LOC_u8Iterations < (MAX_KEYPAD_USER_PASS);LOC_u8Iterations++)
		{
			if(GLOB_KeypadUserProfile[LOC_u8UserCount].password[LOC_u8Iterations] == NULL_CHAR)
			{
				LOC_u8CheckCounter++;
			}
		}
		if(LOC_u8CheckCounter == (MAX_KEYPAD_USER_CHAR + MAX_KEYPAD_USER_PASS) )
		{
			GLOB_u8FreeSpaceExists = TRUE;
			break;
		}
		else
		{
			LOC_u8CheckCounter = ZERO;
			LOC_u8Iterations = ZERO;
		}
	}
	if(GLOB_u8FreeSpaceExists == TRUE)
	{
		LOC_u8CheckCounter = ZERO;
		LOC_u8UARTData = ZERO;
		while( (LOC_u8UARTData < KEYPAD_MIN_LIMIT || LOC_u8UARTData > KEYPAD_MAX_LIMIT) && (LOC_u8UARTData != ENTER_BUTTON) )
		{
			LOC_u8Iterations = ZERO;
			LOC_u8CheckCounter = ZERO;
			LOC_u8UARTData = ZERO;
			MCAL_UART_u8SendWord("\rEnter the username to add: ");
			MCAL_UART_u8ResumeInterrupt();
			while(LOC_u8CheckCounter < MAX_KEYPAD_USER_CHAR && LOC_u8UARTData != ENTER_BUTTON)
			{
				MCAL_UART_u8GetData(&LOC_u8UARTData);
				MCAL_UART_u8SendData(LOC_u8UARTData);
				if(LOC_u8UARTData != ENTER_BUTTON && LOC_u8UARTData != NULL_CHAR && LOC_u8UARTData >= KEYPAD_MIN_LIMIT && LOC_u8UARTData <= KEYPAD_MAX_LIMIT)
				{
					GLOB_KeypadUserProfile[LOC_u8UserCount].username[LOC_u8CheckCounter] = LOC_u8UARTData;
					LOC_u8CheckCounter++;
				}
				else if(LOC_u8UARTData == ENTER_BUTTON)
				{
					if(LOC_u8CheckCounter == ZERO)
					{
						LOC_u8UARTData = ZERO;
					}
				}
				else if(LOC_u8UARTData && (LOC_u8UARTData < KEYPAD_MIN_LIMIT || LOC_u8UARTData > KEYPAD_MAX_LIMIT) )
				{
					MCAL_UART_u8SendWord("\rERROR! You cannot send anything but numbers! Try again!\r");
					for(LOC_u8Iterations;LOC_u8Iterations<MAX_KEYPAD_USER_CHAR;LOC_u8Iterations++)
					{
						GLOB_KeypadUserProfile[LOC_u8UserCount].username[LOC_u8Iterations] = NULL_CHAR; 
					}
					//LOC_u8UARTData = ENTER_BUTTON;
					LOC_u8CheckCounter = ZERO;
					break;
				}
			}
		}
		LOC_u8CheckCounter = ZERO;
		LOC_u8UARTData = ZERO;
		while( (LOC_u8UARTData < KEYPAD_MIN_LIMIT || LOC_u8UARTData > KEYPAD_MAX_LIMIT) && (LOC_u8UARTData != ENTER_BUTTON) )
		{
			LOC_u8CheckCounter = ZERO;
			LOC_u8UARTData = ZERO;
			LOC_u8Iterations = ZERO;
			MCAL_UART_u8SendWord("\rEnter the password to add: ");
			MCAL_UART_u8ResumeInterrupt();
			while(LOC_u8CheckCounter < MAX_KEYPAD_USER_PASS && LOC_u8UARTData != ENTER_BUTTON)
			{
				MCAL_UART_u8GetData(&LOC_u8UARTData);
				MCAL_UART_u8SendData(LOC_u8UARTData);
				if(LOC_u8UARTData != ENTER_BUTTON && LOC_u8UARTData != NULL_CHAR && LOC_u8UARTData >= KEYPAD_MIN_LIMIT && LOC_u8UARTData <= KEYPAD_MAX_LIMIT)
				{
					GLOB_KeypadUserProfile[LOC_u8UserCount].password[LOC_u8CheckCounter] = LOC_u8UARTData;
					LOC_u8CheckCounter++;
				}
				else if(LOC_u8UARTData == ENTER_BUTTON)
				{
					if(LOC_u8CheckCounter == ZERO)
					{
						LOC_u8UARTData = ZERO;
					}
				}
				else if(LOC_u8UARTData && (LOC_u8UARTData < KEYPAD_MIN_LIMIT || LOC_u8UARTData > KEYPAD_MAX_LIMIT) )
				{
					MCAL_UART_u8SendWord("\rERROR! You cannot send anything but numbers! Try again!\r");
					for(LOC_u8Iterations;LOC_u8Iterations<MAX_KEYPAD_USER_PASS;LOC_u8Iterations++)
					{
						GLOB_KeypadUserProfile[LOC_u8UserCount].password[LOC_u8Iterations] = NULL_CHAR;
					}
					LOC_u8CheckCounter = ZERO;
					break;
				}
			}
		}
		MCAL_UART_u8SuspendInterrupt();
		HAL_EEPROM_u8WriteWord((LOC_u8UserCount * (MAX_KEYPAD_USER_CHAR + MAX_KEYPAD_USER_PASS)) + KEYPAD_MEMORY_OFFSET,GLOB_KeypadUserProfile[LOC_u8UserCount].username);
		HAL_EEPROM_u8WriteWord( (LOC_u8UserCount * (MAX_KEYPAD_USER_CHAR + MAX_KEYPAD_USER_PASS)) + KEYPAD_MEMORY_OFFSET + MAX_KEYPAD_USER_PASS,GLOB_KeypadUserProfile[LOC_u8UserCount].password);
		//MCAL_UART_u8ResumeInterrupt();
	}
	else
	{
		MCAL_UART_u8SendWord("Can't add more users!\rPlease delete users first!\r");
	}
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue; 
}

static STD_Type APP_SMART_HOME_u8VirtualUserDelete(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	u8 LOC_u8UserCount = ZERO;
	u8 LOC_u8Iterations = ZERO;
	u8 LOC_u8Interface = USER_SELECTION_1;
	u8 LOC_u8UARTData = ZERO;
	u8 LOC_u8Digit = ZERO;
	
	MCAL_UART_u8SuspendInterrupt();
	for(LOC_u8Iterations;LOC_u8Iterations<MAX_USERS;LOC_u8Iterations++)
	{
		if(GLOB_KeypadUserProfile[LOC_u8Iterations].username[ZERO] != NULL_CHAR)	//User exists
		{
			LOC_u8UserCount++;
		}
	}
	LOC_u8Iterations = ZERO;
	if(LOC_u8UserCount > ZERO)
	{
		MCAL_UART_u8SendWord("\rChoose a user to delete from these:\r");
		for(LOC_u8Iterations;LOC_u8Iterations<MAX_USERS;LOC_u8Iterations++)
		{
			if(GLOB_KeypadUserProfile[LOC_u8Iterations].username[ZERO] != NULL_CHAR)	//User exists
			{
				MCAL_UART_u8SendData('[');
				MCAL_UART_u8SendData(LOC_u8Interface);
				MCAL_UART_u8SendWord("] ");
				for(LOC_u8Digit;LOC_u8Digit < MAX_KEYPAD_USER_CHAR;LOC_u8Digit++)
				{
					MCAL_UART_u8SendData(GLOB_KeypadUserProfile[LOC_u8Iterations].username[LOC_u8Digit]);
				}
				MCAL_UART_u8SendData(ENTER_BUTTON);
			}
			else
			{
				MCAL_UART_u8SendData('[');
				MCAL_UART_u8SendData(LOC_u8Interface);
				MCAL_UART_u8SendWord("] ");
				MCAL_UART_u8SendWord("NOT FOUND");
				MCAL_UART_u8SendData(ENTER_BUTTON);
			}
			LOC_u8Interface++;
			LOC_u8Digit = ZERO;
		}
		MCAL_UART_u8SendWord("\rYour choice: ");
		MCAL_UART_u8ResumeInterrupt();
		while(LOC_u8UARTData < USER_SELECTION_1 || LOC_u8UARTData >= LOC_u8Interface)	//Open range depending on the number of users existing
		{
			MCAL_UART_u8GetData(&LOC_u8UARTData);
			MCAL_UART_u8SendData(LOC_u8UARTData);	//Sending UDR after receiving it serves as emptying UDR for no data ignorance.
		}
		MCAL_UART_u8SuspendInterrupt();
		LOC_u8UARTData = LOC_u8UARTData - ASCII;	//This will now be either 1, 2 or 3, it's now a number not ASCII
		if(GLOB_KeypadUserProfile[LOC_u8UARTData-ONE].username[ZERO] != NULL_CHAR)
		{
			MCAL_UART_u8SendWord("\rDeleted Successfully!\r");
			LOC_u8Iterations = ZERO;
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_KEYPAD_USER_CHAR;LOC_u8Iterations++)
			{
				GLOB_KeypadUserProfile[LOC_u8UARTData-ONE].username[LOC_u8Iterations] = NULL_CHAR;
			}
			LOC_u8Iterations = ZERO;
			for(LOC_u8Iterations;LOC_u8Iterations<MAX_KEYPAD_USER_PASS;LOC_u8Iterations++)
			{
				GLOB_KeypadUserProfile[LOC_u8UARTData-ONE].password[LOC_u8Iterations] = NULL_CHAR;
			}
			HAL_EEPROM_u8ClearBytes( ((LOC_u8UARTData-ONE)*(MAX_KEYPAD_USER_CHAR+MAX_KEYPAD_USER_PASS)) + KEYPAD_MEMORY_OFFSET ,MAX_KEYPAD_USER_CHAR);
			HAL_EEPROM_u8ClearBytes( ((LOC_u8UARTData-ONE)*(MAX_KEYPAD_USER_CHAR+MAX_KEYPAD_USER_PASS)) + KEYPAD_MEMORY_OFFSET + MAX_KEYPAD_USER_CHAR ,MAX_KEYPAD_USER_PASS);
			MCAL_UART_u8ResumeInterrupt();
		}
		else
		{
			MCAL_UART_u8SendWord("\rCan't delete a user that doesn't exist!\r\r");
		}
	}
	else
	{
		MCAL_UART_u8SendWord("\rNo users exist yet!, Add users first!\r");
	}
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8VirtualAdminOptions(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8UARTData;
	u8 LOC_u8OptionSelected = FALSE;
	
	if(GLOB_u8FreeSpaceExists == TRUE)
	{
		MCAL_UART_u8SendWord("\rUser added successfully!\r");
		GLOB_u8FreeSpaceExists = FALSE;
	}
	MCAL_UART_u8SendWord("\rSelect an option!\r[1]Add User\r[2]Delete User\r[3]Login: ");
	MCAL_UART_u8ResumeInterrupt();

	while(LOC_u8OptionSelected == FALSE)
	{
		MCAL_UART_u8GetData(&LOC_u8UARTData);
		MCAL_UART_u8SendData(LOC_u8UARTData);
		if(LOC_u8UARTData == USER_SELECTION_1 || LOC_u8UARTData == USER_SELECTION_2 || LOC_u8UARTData == USER_SELECTION_3)
		{
			MCAL_UART_u8SendWord("\r\r");
			switch(LOC_u8UARTData)
			{
				case USER_SELECTION_1:
					LOC_u8UARTData = ZERO;
					MCAL_UART_u8SendWord("Select the user type to add\r[1] Virtual (This screen)\r[2] Keypad (LCD)\rYour choice: ");
					while(LOC_u8UARTData != USER_SELECTION_1 && LOC_u8UARTData != USER_SELECTION_2)
					{
						MCAL_UART_u8GetData(&LOC_u8UARTData);
						MCAL_UART_u8SendData(LOC_u8UARTData);
					}
					MCAL_UART_u8SendData(ENTER_BUTTON);
					switch(LOC_u8UARTData)
					{
						case USER_SELECTION_1:
							APP_SMART_HOME_u8VirtualAdminAdd();
							break;
						case USER_SELECTION_2:
							APP_SMART_HOME_u8VirtualUserAdd();
							break;
					}
					break;
				case USER_SELECTION_2:
					LOC_u8UARTData = ZERO;
					MCAL_UART_u8SendWord("Select the user type to delete\r[1] Virtual (This screen)\r[2] Keypad (LCD)\rYour choice: ");
					while(LOC_u8UARTData != USER_SELECTION_1 && LOC_u8UARTData != USER_SELECTION_2)
					{
						MCAL_UART_u8GetData(&LOC_u8UARTData);
						MCAL_UART_u8SendData(LOC_u8UARTData);
					}
					switch(LOC_u8UARTData)
					{
						case USER_SELECTION_1:
						APP_SMART_HOME_u8VirtualAdminDelete();
						break;
						case USER_SELECTION_2:
						APP_SMART_HOME_u8VirtualUserDelete();
						break;
					}
					break;
				case USER_SELECTION_3:
					APP_SMART_HOME_u8VirtualLogin();
					break;
			}
			LOC_u8OptionSelected = TRUE;
		}
	}
	
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

static STD_Type APP_SMART_HOME_u8SelectAccess(void)		//Something is wrong with the timing of sending/recieving in UART
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	static u8 LOC_u8UARTData;
	static u8 LOC_u8KeypadData;
	static u8 LOC_u8SendOnce = FALSE;
	static u8 LOC_u8TerminateVirtual = FALSE;
	switch(GLOB_u8InputOverride)
	{
		case PENDING:
			if(LOC_u8SendOnce == FALSE)
			{
				MCAL_UART_u8SuspendInterrupt();
				HAL_LCD_u8WriteString("Keypad Logs You Here!");
				MCAL_UART_u8SendWord("Select your access mode!\r[1]Admin     [2]User: ");
				MCAL_UART_u8ResumeInterrupt();
				LOC_u8SendOnce = TRUE;
			}	
			MCAL_UART_u8ResumeInterrupt();
			if(LOC_u8UARTData != USER_SELECTION_1 && LOC_u8UARTData != USER_SELECTION_2)
			{
				MCAL_UART_u8GetData(&LOC_u8UARTData);
				MCAL_UART_u8SendData(LOC_u8UARTData);
			}
			MCAL_UART_u8SuspendInterrupt();
			if(LOC_u8UARTData == USER_SELECTION_1 || LOC_u8UARTData == USER_SELECTION_2)
			{
				MCAL_UART_u8SendWord("\rYou chose: ");
				LOC_u8SendOnce = FALSE;
				GLOB_u8InputOverride = VIRTUAL;
			}
			HAL_KEYPAD_u8GetButtonPressed(&LOC_u8KeypadData);
			if(LOC_u8KeypadData)
			{
				GLOB_u8InputOverride = KEYPAD;
			}
			break;
		case VIRTUAL:
			switch(LOC_u8UARTData)
			{
				case USER_SELECTION_1:
					if(LOC_u8SendOnce == FALSE)
					{
						MCAL_UART_u8SendWord("Admin!\r");
						MCAL_UART_u8SendWord("NOTE: You no longer have access to KEYPAD!\r\r");
						MCAL_UART_u8ResumeInterrupt();
						GLOB_u8AccessMode = ADMIN_MODE;
						LOC_u8SendOnce = TRUE;
					}
					APP_SMART_HOME_u8VirtualAdminOptions();
					break;
				case USER_SELECTION_2:
					if(LOC_u8SendOnce == FALSE)
					{
						MCAL_UART_u8SendWord("User!\r");
						MCAL_UART_u8SendWord("NOTE: You no longer have access to KEYPAD!\r\r");
						MCAL_UART_u8ResumeInterrupt();
						GLOB_u8AccessMode = USER_MODE;
						LOC_u8SendOnce = TRUE;
					}
					APP_SMART_HOME_u8VirtualLogin();
					break;
			}
			break;
		case KEYPAD:
			if(LOC_u8TerminateVirtual == FALSE)
			{
				MCAL_UART_u8SendWord("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\rYou no longer have access to VIRTUAL TERMINAL!");
				LOC_u8TerminateVirtual = TRUE;
			}
			MCAL_UART_u8SuspendInterrupt();
			APP_SMART_HOME_u8KeypadUserLogin(LOC_u8KeypadData);
			MCAL_UART_u8ResumeInterrupt();
			GLOB_u8AccessMode = USER_MODE;
			break;
	}
	return LOC_u8ReturnValue;
}
//============================================================================

//============================MAIN FUNCTIONS==================================
static STD_Type APP_SMART_HOME_u8FeatureSelect(u8 LOC_u8AppMode)	//The selector of which feature is running.
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;	//Initially, we assume an error will be returned till the function fixes it.
	if(LOC_u8AppMode <= LOCKED)
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
				switch(GLOB_u8InputOverride)
				{
					case VIRTUAL:
						APP_SMART_HOME_u8VirtualControl();
						break;
					case KEYPAD:
						MCAL_UART_u8SuspendInterrupt();
						APP_SMART_HOME_u8KeypadControl();
						MCAL_UART_u8ResumeInterrupt();
						break;
				}
				break;
			case LOCKED:
				APP_SMART_HOME_u8BlockState();
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
	
	while(1)
	{
		if(LOC_u8Initialized == FALSE)			//If it's the first time
		{
			LOC_u8ReturnValue = MCAL_ADC_u8Init(ADC0);				//Initialize the ADC on channel ADC0 (for now)
			LOC_u8ReturnValue = HAL_LCD_u8Init();					//Initialize LCD	(Please make sure you change the configuration in .h file)
			LOC_u8ReturnValue = MCAL_UART_u8Init();					//Initialize UART communication (Bluetooth)
			//LOC_u8ReturnValue = HAL_DC_MOTOR_u8Init(DC_MOTOR2);		//Initialize DC Motor (Please avoid using DC_MOTOR1, as it is used for the servo)
			LOC_u8ReturnValue = MCAL_TIMER_u8Init(TIMER0);			//DC_MOTOR0---->TIMER0 (OC0 output), DC_MOTOR1---->TIMER1 (OC1B output), DC_MOTOR2---->TIMER2 (OC2 output)
			LOC_u8ReturnValue = MCAL_TIMER_u8Init(TIMER2);
			LOC_u8ReturnValue = HAL_KEYPAD_u8Init();				//Initialize Keypad (Please make sure you change the configuration in .h file)
			LOC_u8ReturnValue = HAL_SERVO_u8Init();					//Initialize Servo Motor
			LOC_u8ReturnValue = HAL_EEPROM_u8Init();				//Initalize EEPROM
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(ALARM_LED_PORT,ALARM_LED_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LAMP_1_PORT,LAMP_1_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LAMP_2_PORT,LAMP_2_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LAMP_3_PORT,LAMP_3_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LAMP_4_PORT,LAMP_4_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LAMP_5_PORT,LAMP_5_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(LAMP_6_PORT,LAMP_6_PIN,DIO_OUTPUT);
			LOC_u8ReturnValue = MCAL_DIO_u8SetPinDirection(AC_PORT,AC_PIN,DIO_OUTPUT);

			LOC_u8ReturnValue = MCAL_UART_u8CallbackFun(UART_TERMINATION_CHAR,&callback_send_uart_fun);	//Send nothing for now
			LOC_u8ReturnValue = MCAL_ADC_u8CallbackFun(&callback_adc_fun);
			//Add the rest of callback functions here
			
			MCAL_UART_u8SuspendInterrupt();
			//HAL_EEPROM_u8ClearBytes(ZERO,MAX_EEPROM_USED);
			APP_SMART_HOME_u8UpdateProfiles();
			MCAL_UART_u8ResumeInterrupt();
			
			APP_SMART_HOME_u8FeatureSelect(GLOB_u8SystemMode);
			LOC_u8Initialized = TRUE;								//Now that we initialized the peripherals, this bit of code is no longer useful			
		}
		if(GLOB_u8SystemMode == RESET)
		{
			GLOB_u8SystemMode = LOGIN_MODE;
		}
		
		APP_SMART_HOME_u8FeatureSelect(GLOB_u8SystemMode);
	}
	return LOC_u8ReturnValue;				//Return the error status
}
//============================================================================
