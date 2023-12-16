/*
 * MCAL_I2C.h
 *
 *  Created on: Nov 14, 2023
 *      Author: 20155
 */

#include "../../STD_Type.h"

#ifndef MCAL_MCAL_I2C_MCAL_I2C_H_
#define MCAL_MCAL_I2C_MCAL_I2C_H_

#define TWBR    (*(volatile u8*)0x20)   /*Two-wire Serial Interface Bit Rate Register*/
#define TWSR    (*(volatile u8*)0x21)   /*Two-wire Serial Interface Status Register*/
#define TWCR    (*(volatile u8*)0x56)   /*Two-wire Serial Interface Control Register*/
#define TWDR    (*(volatile u8*)0x23)   /*Two-wire Serial Interface Data Register*/

/*TWCR_bits*/
#define TWIE 0
#define TWEN 2
#define TWWC 3
#define TWSTO 4
#define TWSTA 5
#define TWEA 6
#define TWINT 7

/*Prescaler_Values*/
#define _1_  0x00
#define _4_  0x01
#define _16_ 0x02
#define _64_ 0x03


u8 MCAL_TWI_u8Init(void);
u8 MCAL_TWI_u8Start(void);
u8 MCAL_TWI_u8Repeated_Start(void);
u8 MCAL_TWI_u8Write_Byte(u8 LOC_u8data);
u8 MCAL_TWI_u8Read_Byte(void);
u8 MCAL_TWI_u8Send_Slave_Address_With_Read_Req(u8 LOC_u8Slave_Address);
u8 MCAL_TWI_u8Send_Slave_Address_With_Write_Req(u8 LOC_u8Slave_Address);
u8 MCAL_TWI_u8Stop(void);

#endif /* MCAL_MCAL_I2C_MCAL_I2C_H_ */
