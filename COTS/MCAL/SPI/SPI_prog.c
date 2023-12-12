#include "../DIO/DIO.h"

#include "SPI_priv.h"
#include "SPI_config.h"
#include "SPI_address.h"
#include "SPI_int.h"

#if(SPI_INTERRUPT == SPI_INTERRUPT_OFF)
#define	SPIE_MODE	(0<<SPIE)
#elif(SPI_INTERRUPT == SPI_INTERRUPT_ON)
#define	SPIE_MODE	(1<<SPIE)
#else
#error "Wrong SPI Interrupt Mode! Please Re-configure!"
#endif

#if(SPI_SELECT == SPI_SLAVE)
#define MSTR_MODE	(0<<MSTR)
#elif(SPI_SELECT == SPI_MASTER)
#define MSTR_MODE 	(1<<MSTR)
#else
#error "Wrong SPI Master Select Mode! Please Re-configure!"
#endif

#if(SPI_CLOCK_MODE == SPI_RISING_EDGE)
#define CPOL_CPHA_MODE	((1<<CPOL) | (0<<CPHA))
#elif(SPI_CLOCK_MODE == SPI_FALLING_EDGE)
#define CPOL_CPHA_MODE	((0<<CPOL) | (0<<CPHA))
#else
#error "Wrong SPI Clock Polarity/Phase Mode! Please Re-configure!"
#endif

#if( (SPI_PRESCALER == 2) || (SPI_PRESCALER == 8) || (SPI_PRESCALER == 32) )
#define SPI2X_MODE	(1<<SPI2X)
#elif( (SPI_PRESCALER == 4) || (SPI_PRESCALER == 16) || (SPI_PRESCALER == 64) || (SPI_PRESCALER == 128) )
#define SPI2X_MODE	(0<<SPI2X)
#else
#error	"Wrong SPI Prescaler! Please Re-configure!"
#endif

#if(SPI_PRESCALER == 4)
#define	SPR_MODE	( (0<<SPR1) | (0<<SPR0) )
#elif(SPI_PRESCALER == 16)
#define	SPR_MODE	( (0<<SPR1) | (1<<SPR0) )
#elif(SPI_PRESCALER == 64)
#define	SPR_MODE	( (1<<SPR1) | (0<<SPR0) )
#elif(SPI_PRESCALER == 128)
#define	SPR_MODE	( (1<<SPR1) | (1<<SPR0) )
#elif(SPI_PRESCALER == 2)
#define	SPR_MODE	( (0<<SPR1) | (0<<SPR0) )
#elif(SPI_PRESCALER == 8)
#define	SPR_MODE	( (0<<SPR1) | (1<<SPR0) )
#elif(SPI_PRESCALER == 32)
#define	SPR_MODE	( (1<<SPR1) | (0<<SPR0) )
#endif

#if(SPI_INTERRUPT == SPI_INTERRUPT_ON)
void (*GLOB_PvoidSPIGptr)(void) = NULL;
#if(SPI_SELECT == SPI_SLAVE)
static u8 GLOB_u8SPIGetData;
static u8 GLOB_u8SPIDataRecieved = FALSE;
#elif(SPI_SELECT == SPI_MASTER)
static u8 * GLOB_u8SPISendData;
#endif
#endif

STD_Type MCAL_SPI_u8Init(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
#if(SPI_SELECT == SPI_MASTER)
	MCAL_DIO_u8SetPinDirection(SS_PORT,SS_PIN,DIO_OUTPUT);
	MCAL_DIO_u8SetPinDirection(MOSI_PORT,MOSI_PIN,DIO_OUTPUT);
	MCAL_DIO_u8SetPinDirection(MISO_PORT,MISO_PIN,DIO_INPUT);
	MCAL_DIO_u8SetPinDirection(SCK_PORT,SCK_PIN,DIO_OUTPUT);
#elif(SPI_SELECT == SPI_SLAVE)
	MCAL_DIO_u8SetPinDirection(SS_PORT,SS_PIN,DIO_INPUT);
	MCAL_DIO_u8SetPinDirection(MOSI_PORT,MOSI_PIN,DIO_INPUT);
	MCAL_DIO_u8SetPinDirection(MISO_PORT,MISO_PIN,DIO_OUTPUT);
	MCAL_DIO_u8SetPinDirection(SCK_PORT,SCK_PIN,DIO_INPUT);
#endif
	SPSR |= SPI2X_MODE;
	SPCR = (SPIE_MODE | MSTR_MODE | CPOL_CPHA_MODE | SPR_MODE);
	SPCR |= (1<<SPE);	//Finally, enable SPI Communication
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

#if(SPI_SELECT == SPI_SLAVE)
STD_Type MCAL_SPI_u8RecieveData(u8 * LOC_Pu8Data)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
#if(SPI_INTERRUPT == SPI_INTERRUPT_OFF)
	while(!GET_BIT(SPSR,SPIF));
#endif
	*LOC_Pu8Data = SPDR;
	GLOB_u8SPIDataRecieved = FALSE;
	return LOC_u8ReturnValue;
}
#if(SPI_INTERRUPT == SPI_INTERRUPT_ON)
STD_Type MCAL_SPI_u8CallbackFun(void (*LOC_Pu8Ptr)(void))
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu8Ptr != NULL)
	{
		GLOB_PvoidSPIGptr = LOC_Pu8Ptr;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
STD_Type MCAL_SPI_u8GetData(u8 * LOC_Pu8Data)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu8Data)
	{
		*LOC_Pu8Data = GLOB_u8SPIGetData;
		GLOB_u8SPIDataRecieved = TRUE;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif
#elif(SPI_SELECT == SPI_MASTER)
STD_Type MCAL_SPI_u8SendData(u8 LOC_u8Data)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_u8Data)
	{
		SPDR = LOC_u8Data;
#if(SPI_INTERRUPT == SPI_INTERRUPT_OFF)
		while(!GET_BIT(SPSR,SPIF));
#endif
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#if(SPI_INTERRUPT == SPI_INTERRUPT_ON)
STD_Type MCAL_SPI_u8CallbackFun(u8 LOC_Pu8Data[],void (*LOC_Pu8Ptr)(void))
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu8Data && (LOC_Pu8Ptr != NULL))
	{
		GLOB_u8SPISendData = LOC_Pu8Data;
		GLOB_PvoidSPIGptr = LOC_Pu8Ptr;
		if(GET_BIT(SPSR,SPIF))
		{
			MCAL_SPI_u8SendData(GLOB_u8SPISendData[ZERO]);
			GLOB_u8SPISendData++;
		}
		LOC_u8ReturnValue = E_OK;
	}
	return LOC_u8ReturnValue;
}
#endif
#endif

ISR(SPI_STC_vect)
{
#if(SPI_SELECT == SPI_SLAVE)
	MCAL_SPI_u8RecieveData(&GLOB_u8SPIGetData);
#elif(SPI_SELECT == SPI_MASTER)
	if(GLOB_u8SPISendData)
	{
		MCAL_SPI_u8SendData(*GLOB_u8SPISendData);
		GLOB_u8SPISendData++;
	}
#endif
	if(GLOB_PvoidSPIGptr != NULL)
	{
		GLOB_PvoidSPIGptr();
	}
}
