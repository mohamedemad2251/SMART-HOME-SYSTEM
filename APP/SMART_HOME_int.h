#ifndef SMART_HOME_INT_H_
#define SMART_HOME_INT_H_

//============================INCLUSIONS======================================
#include "../DEFS/STD_types.h"
//============================================================================

//============================DEFINITIONS=====================================
//For ACCESS_MODE
#define ADMIN_MODE		0
#define USER_MODE		1

//For SYSTEM_MODE
#define	RESET			0
#define	LOGIN_MODE		1
#define	CONTROL_MODE	2
#define LOCKED			3

//For INPUT_OVERRIDE
#define PENDING			0
#define VIRTUAL			1
#define KEYPAD			2

#define ENTER_BUTTON	'\r'		//When user/admin enters the name and password, that's the termination character to process the data.
#define NULL_CHAR		'\0'
#define MAX_VIRTUAL_USER_CHAR	10	//Max characters to be entered as username in virtual terminal = 10 (SPACE included)
#define MAX_VIRTUAL_USER_PASS	10	//Max characters to be entered as password in virtual terminal = 10 (SPACE included)
#define MAX_KEYPAD_USER_CHAR	4	//Max characters to be entered as username = 10 (SPACE included)
#define MAX_KEYPAD_USER_PASS	4	//Max characters to be entered as password = 10 (SPACE included)
#define KEYPAD_MEMORY_OFFSET	60	//Keypad memory storage comes after 60 bytes from EEPROM's start
#define MAX_EEPROM_USED			84	//Max number of bytes we'll use in the EEPROM
#define MAX_USERS				3	//Max number of users allowed in the program
#define MAX_LOGIN_TRIALS		3	//Max number of login trials the user can do before the systems locks him down
#define KEYPAD_MIN_LIMIT		'0'	//Minimum input value when adding/deleting a user from keypad login (used by admin add/delete)
#define KEYPAD_MAX_LIMIT		'9' //Maximum input value when adding/deleting a user from keypad login (used by admin add/delete)
#define KEYPAD_ENTER			'='
#define KEYPAD_CLEAR			'!'

#define USER_SELECTION_1				'1'
#define USER_SELECTION_2				'2'
#define USER_SELECTION_3				'3'
#define USER_SELECTION_4				'4'

#define LAMP_OPTION						'1'
#define DOOR_OPTION						'2'


#define USER_1			0
#define USER_2			1
#define USER_3			2

#ifndef ONE
#define ONE				1
#endif

#define MAX_LAMPS       6
#define LAMP_1			'1'
#define LAMP_2			'2'
#define LAMP_3			'3'
#define LAMP_4			'4'
#define LAMP_5			'5'
#define LAMP_6			'6'
#define OPTION_ON		'1'
#define OPTION_OFF		'2'
#define LAMP_1_PORT		DIO_PortC
#define LAMP_1_PIN		Pin2
#define LAMP_2_PORT		DIO_PortC
#define LAMP_2_PIN		Pin3
#define LAMP_3_PORT		DIO_PortC
#define LAMP_3_PIN		Pin4
#define LAMP_4_PORT		DIO_PortC
#define LAMP_4_PIN		Pin6
#define LAMP_5_PORT		DIO_PortC
#define LAMP_5_PIN		Pin7
#define LAMP_6_PORT		DIO_PortD
#define LAMP_6_PIN		Pin7
#define MAX_DIMMING_CHAR	3

#define MAX_TEMP          28
#define MAX_SENSOR_TEMP          125
#define MIN_TEMP          21
#define Unit_Conversion   4.9
#define TEN               10
#define FIVE_HUNDERED     500
#define AC_PORT			  DIO_PortD
#define AC_PIN			  Pin2
#define DEGREE_SYMBOL 0xDF  

#define ALARM_LED_PORT	DIO_PortC
#define ALARM_LED_PIN	Pin5
#define ALARM_DELAY_TIME	500

#define SERVO_DEGREE_CLOSE	0
#define SERVO_DEGREE_OPEN	90
//			ADD YOUR DEFINITIONS HERE
//==========================================================================

//============================MAIN FUNCTIONS================================
STD_Type APP_SMART_HOME_u8App(void);	//Main application function to be run in main.
//==========================================================================

#endif /* SMART_HOME_INT_H_ */
