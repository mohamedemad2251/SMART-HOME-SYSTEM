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

#define ENTER_BUTTON	'\r'		//When user/admin enters the name and password, that's the termination character to process the data.
#define MAX_VIRTUAL_USER_CHAR	10	//Max characters to be entered as username in virtual terminal = 10 (SPACE included)
#define MAX_VIRTUAL_USER_PASS	10	//Max characters to be entered as password in virtual terminal = 10 (SPACE included)
#define MAX_KEYPAD_USER_CHAR	4	//Max characters to be entered as username = 10 (SPACE included)
#define MAX_KEYPAD_USER_PASS	4	//Max characters to be entered as password = 10 (SPACE included)
#define MAX_USERS				3	//Max number of users allowed in the program
#define MAX_LOGIN_TRIALS		3	//Max number of login trials the user can do before the systems locks him down

#define USER_SELECTION_1				'1'
#define USER_SELECTION_2				'2'
#define USER_SELECTION_3				'3'


#define MAX_LAMPS       6
#define LAMP_1			0
#define LAMP_2			1
#define LAMP_3			2
#define LAMP_4			3
#define LAMP_5			4
#define LAMP_6			5


//			ADD YOUR DEFINITIONS HERE
//==========================================================================

//============================MAIN FUNCTIONS================================
STD_Type APP_SMART_HOME_u8App(void);	//Main application function to be run in main.
//==========================================================================


//============================FEATURES======================================

//			ADD YOUR FUNCTIONS HERE

//==========================================================================

#endif /* SMART_HOME_INT_H_ */
