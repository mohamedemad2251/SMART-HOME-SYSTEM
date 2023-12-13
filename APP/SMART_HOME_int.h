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

#define ACCESS_MODE		ADMIN_MODE
#define SYSTEM_MODE		RESET

#define ENTER_BUTTON	10
//			ADD YOUR DEFINITIONS HERE
//==========================================================================

//============================MAIN FUNCTIONS================================
STD_Type APP_SMART_HOME_u8App(void);	//Main application function to be run in main.
//==========================================================================


//============================FEATURES======================================

//			ADD YOUR FUNCTIONS HERE

//==========================================================================

#endif /* SMART_HOME_INT_H_ */