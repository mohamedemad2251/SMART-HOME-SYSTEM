/*
 * MCAL_I2C.c
 *
 *  Created on: Nov 14, 2023
 *      Author: 20155
 */

#include "../MCAL_TWI/MCAL_TWI_Config.h"
#include "../MCAL_TWI/MCAL_TWI_Private.h"

u8 MCAL_TWI_u8Init(void)
{
  STD_Type LOC_u8ReturnValue = E_NOT_OK;

  if(Prescaler_Value>=_1_ && Prescaler_Value<=_64_ && Bit_Rate>0)
  {
  TWBR = Bit_Rate;
  TWSR = Prescaler_Value;
  TWCR = (1<<TWEN);
  LOC_u8ReturnValue = E_OK;
  }
  return LOC_u8ReturnValue;
}


u8 MCAL_TWI_u8Start(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	LOC_u8ReturnValue = E_OK;

	return LOC_u8ReturnValue;
}


u8 MCAL_TWI_u8Repeated_Start(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	LOC_u8ReturnValue = E_OK;

	return LOC_u8ReturnValue;
}


u8 MCAL_TWI_u8Send_Slave_Address_With_Read_Req(u8 LOC_u8Slave_Address)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	if(LOC_u8Slave_Address>=0)
	{
		 TWDR=(LOC_u8Slave_Address<<1);
		 TWDR|=(1<<0);                  /*Read_Instruction*/
		 TWCR = (1<<TWINT)|(1<<TWEN);
		 while (!(TWCR & (1<<TWINT)));
		 LOC_u8ReturnValue = E_OK;
	}
	return LOC_u8ReturnValue;
}


u8 MCAL_TWI_u8Send_Slave_Address_With_Write_Req(u8 LOC_u8Slave_Address)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	if(LOC_u8Slave_Address>=0)
	{
        TWDR=(LOC_u8Slave_Address<<1);
        TWCR = (1<<TWINT)|(1<<TWEN);
        while (!(TWCR & (1<<TWINT)));
        LOC_u8ReturnValue = E_OK;
    }
	return LOC_u8ReturnValue;
}


u8  MCAL_TWI_u8Write_Byte(u8 LOC_u8data)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

   TWDR = LOC_u8data;
   TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	LOC_u8ReturnValue = E_OK;

	return LOC_u8ReturnValue;
}


u8 MCAL_TWI_u8Read_Byte(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	  TWCR = (1<<TWINT)|(1<<TWEN);
	  while (!(TWCR & (1<<TWINT)));
	  LOC_u8ReturnValue = E_OK;

	  if( LOC_u8ReturnValue == E_OK) {return TWDR;}

	  else{return LOC_u8ReturnValue;}
}


u8 MCAL_TWI_u8Stop(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	LOC_u8ReturnValue = E_OK;

	return LOC_u8ReturnValue;
}

