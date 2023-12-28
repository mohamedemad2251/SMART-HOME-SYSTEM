#include "APP/SMART_HOME_int.h"

// #include "COTS/HAL/HAL_EEPROM/HAL_EEPROM.h"
// #include "COTS/HAL/LCD/LCD.h"
// #include "COTS/MCAL/UART/UART_int.h"

int main(void)
{	
// 	MCAL_UART_u8Init();
// 	HAL_LCD_u8Init();
// 	MCAL_UART_u8SuspendInterrupt();
// 	HAL_LCD_u8WriteString("Hello, I'm Mohamed!");
// 	MCAL_UART_u8ResumeInterrupt();
	
// 	u8 data[10];
// 	HAL_LCD_u8Init();
// 	HAL_EEPROM_u8Init();
// 	
// 	HAL_EEPROM_u8ReadWord(60,4,&data);
// 	HAL_LCD_u8WriteString(data);
// 	while (1)
// 	{
// 		
// 	}
    APP_SMART_HOME_u8App();	
}