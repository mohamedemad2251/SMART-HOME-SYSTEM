/*
 * HAL_EEPROM.c
 *
 *  Created on: Nov 17, 2023
 *      Author: 20155
 */
#include"HAL_EEPROM.h"
#include <util/delay.h>

u8 HAL_EEPROM_u8Init(void)
{
	 STD_Type LOC_u8ReturnValue = E_NOT_OK;

	 LOC_u8ReturnValue = MCAL_TWI_u8Init();

	 return LOC_u8ReturnValue;
}


u8 HAL_EEPROM_u8WriteByte(u16 LOC_u16EEPROMDataAddress, u8 LOC_u8data)  /*Write char*/
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	if(LOC_u16EEPROMDataAddress>=ZERO && LOC_u16EEPROMDataAddress<=EEPROM_LAST_ADDRESS)
	{
	MCAL_TWI_u8Start();
	MCAL_TWI_u8Send_Slave_Address_With_Write_Req(CONTROL_CODE | ((LOC_u16EEPROMDataAddress & PAGE_CONTROL) >> EIGHT_BITS_SHIFTING) );
	MCAL_TWI_u8Write_Byte((u8) LOC_u16EEPROMDataAddress);
	MCAL_TWI_u8Write_Byte(LOC_u8data);
	LOC_u8ReturnValue = MCAL_TWI_u8Stop();
	_delay_ms(EEPROM_DELAY);
	}

	 return LOC_u8ReturnValue;
}


u8 HAL_EEPROM_u8ReadByte(u16 LOC_u16EEPROMDataAddress, u8 * LOC_u8data)   /*Read Char*/
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

	if(LOC_u16EEPROMDataAddress>=ZERO && LOC_u16EEPROMDataAddress<=EEPROM_LAST_ADDRESS)
	{
	 MCAL_TWI_u8Start();
	 MCAL_TWI_u8Send_Slave_Address_With_Write_Req(CONTROL_CODE | ((LOC_u16EEPROMDataAddress & PAGE_CONTROL) >> EIGHT_BITS_SHIFTING) );
	 MCAL_TWI_u8Write_Byte((u8) LOC_u16EEPROMDataAddress);
	 MCAL_TWI_u8Repeated_Start();
	 MCAL_TWI_u8Send_Slave_Address_With_Read_Req(CONTROL_CODE | ((LOC_u16EEPROMDataAddress & PAGE_CONTROL) >> EIGHT_BITS_SHIFTING) );
	*LOC_u8data =  MCAL_TWI_u8Read_Byte();
	 LOC_u8ReturnValue = MCAL_TWI_u8Stop();
	 _delay_ms(EEPROM_DELAY);
	}

 return LOC_u8ReturnValue;

}


u8 HAL_EEPROM_u8WordLength(u8 LOC_u8Str[], u8 * LOC_Pu8WordLength)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;

    u8 LOC_u8Count;

    for (LOC_u8Count=ZERO; LOC_u8Str[LOC_u8Count] != '\0'; ++LOC_u8Count);

    *LOC_Pu8WordLength = LOC_u8Count;

    LOC_u8ReturnValue = E_OK;

    return LOC_u8ReturnValue;
}


u8 HAL_EEPROM_u8WriteWord(u16 LOC_u16StartingLocation, u8 LOC_u8Word[])  /*Write String only*/
{
  STD_Type LOC_u8ReturnValue = E_NOT_OK;

  u8 LOC_u8WordLength;
  u16 LOC_u16ByteNum;
  u8 LOC_u8Count;

  HAL_EEPROM_u8WordLength(LOC_u8Word, &LOC_u8WordLength);
  LOC_u16ByteNum = LOC_u8WordLength;

  if((LOC_u16StartingLocation+LOC_u16ByteNum)<=EEPROM_SIZE && LOC_u16ByteNum>0 && LOC_u16StartingLocation>=0)
  {

  for(LOC_u8Count=ZERO; LOC_u8Count<LOC_u8WordLength; LOC_u8Count++)
    {
	  LOC_u8ReturnValue = HAL_EEPROM_u8WriteByte(LOC_u16StartingLocation, LOC_u8Word[LOC_u8Count]);
	  LOC_u16StartingLocation++;
    }

  }

    return LOC_u8ReturnValue;
}


u8 HAL_EEPROM_u8ReadWord (u16 LOC_u16StartingLocation, u16 LOC_u16ByteNum, u8 (*LOC_Pu8WordArray)[])  /*Read String only*/
{
   STD_Type LOC_u8ReturnValue = E_NOT_OK;

  u8 LOC_u8Count;
  u8 LOC_u8Temp;
  u16 LOC_u16WordLastAddress;

  LOC_u8Count = ZERO;
  LOC_u16WordLastAddress = LOC_u16ByteNum+LOC_u16StartingLocation;

 if((LOC_u16StartingLocation+LOC_u16ByteNum)<=EEPROM_SIZE && LOC_u16ByteNum>0 && LOC_u16StartingLocation>=0)
 {

  while(LOC_u16StartingLocation!=LOC_u16WordLastAddress)
  {
	  HAL_EEPROM_u8ReadByte(LOC_u16StartingLocation, &LOC_u8Temp);
	  (*LOC_Pu8WordArray)[LOC_u8Count] = LOC_u8Temp;
	  LOC_u16StartingLocation++;
	  LOC_u8Count++;
  }

   LOC_u8ReturnValue = E_OK;

 }
   return LOC_u8ReturnValue;
}


u8 HAL_EEPROM_u8ClearAllBytes(void)
{
    STD_Type LOC_u8ReturnValue = E_NOT_OK;

	 u16 LOC_u16Count;

	 LOC_u16Count = ZERO;

	 while(LOC_u16Count!=EEPROM_SIZE)
	 {
		 LOC_u8ReturnValue = HAL_EEPROM_u8WriteByte(LOC_u16Count, '\0');
		LOC_u16Count++;
	 }

	 return LOC_u8ReturnValue;
}


u8 HAL_EEPROM_u8ClearBytes(u16 LOC_u16StartingLocation, u16 LOC_u16ByteNum)
{
    STD_Type LOC_u8ReturnValue = E_NOT_OK;

	 u16 LOC_u16Count;

	 LOC_u16Count = ZERO;

	 if((LOC_u16StartingLocation+LOC_u16ByteNum)<=EEPROM_SIZE && LOC_u16ByteNum>0 && LOC_u16StartingLocation>=0)
	  {

	    for (; LOC_u16Count<LOC_u16ByteNum; LOC_u16Count++)
	     {
		   LOC_u8ReturnValue = HAL_EEPROM_u8WriteByte(LOC_u16StartingLocation, '\0');
		   LOC_u16StartingLocation++;
	     }

	  }

	 return LOC_u8ReturnValue;
}
