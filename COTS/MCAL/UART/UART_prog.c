#include "UART_priv.h"
#include "UART_config.h"
#include "UART_int.h"
#include "UART_address.h"

#include "../../HAL/LCD/LCD.h"

#if( UART_DATA_BITS == UART_EIGHT_BITS)
#define UCSZ2_MODE	(0<<UCSZ2)
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS)
#define	UCSZ10_MODE	((1<<UCSZ1) | (1<<UCSZ0))
#endif

#if( UART_DIRECTION == UART_SIMPLEX)
#if( UART_MASTER_SELECT == UART_MASTER)
#define RX_TX_EN_MODE	(1<<TXEN)
#elif( UART_MASTER_SELECT == UART_SLAVE)
#define RX_TX_EN_MODE	(1<<RXEN)
#endif
#elif( UART_DIRECTION == UART_DUPLEX)
#define RX_TX_EN_MODE	((1<<RXEN) | (1<<TXEN))
#endif

#if( UART_INTERRUPT == UART_INTERRUPT_OFF)
#define RX_TX_UDR_IE_MODE	0b00000000
#elif( UART_INTERRUPT == UART_INTERRUPT_ON)
#if( UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_MASTER)
#define RX_TX_UDR_IE_MODE	(1<<UDRIE)
#elif( UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_SLAVE)
#define RX_TX_UDR_IE_MODE	(1<<RXCIE)
#elif( UART_DIRECTION == UART_DUPLEX)
#define RX_TX_UDR_IE_MODE	((1<<UDRIE) | (1<<RXCIE))
#endif
#endif

#if( UART_MODE == UART_ASYNC)
#define UMSEL_MODE	(0<<UMSEL)
#ifdef UART_BAUD_RATE
#if( UART_BAUD_RATE == 9600)
#define UBRR_MODE	UART_BAUD_9600	//USE UBRRL!
#elif( UART_BAUD_RATE == 57600)
#define UBRR_MODE	UART_BAUD_57600
#endif
#endif
#elif( UART_MODE == UART_SYNC)
#define UMSEL_MODE	(1<<UMSEL)
#endif

#if( UART_PARITY == UART_NO_PARITY)
#define UPM_MODE	((0<<UPM1) | (0<<UPM0))
#elif( UART_PARITY == UART_EVEN_PARITY)
#define UPM_MODE	((1<<UPM1) | (0<<UPM0))
#elif( UART_PARITY == UART_ODD_PARITY)
#define UPM_MODE	((1<<UPM1) | (1<<UPM0))
#endif

#if( UART_STOP == UART_ONE_STOP)
#define USBS_MODE	(0<<USBS)
#elif(UART_STOP == UART_TWO_STOPS)
#define USBS_MODE	(1<<USBS)
#endif

#if( UART_INTERRUPT == UART_INTERRUPT_ON)
void (*GLOB_PvoidUARTGptr)(void) = NULL;
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS && UART_INTERRUPT == UART_INTERRUPT_ON && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == SIMPLEX && UART_MASTER_SELECT == UART_MASTER)))
static u8 * GLOB_u8UARTSendData;
static u8 GLOB_u8UARTDataSent = FALSE;
static u8 GLOB_u8UARTDataPending = FALSE;
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS && UART_INTERRUPT == UART_INTERRUPT_ON && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == SIMPLEX && UART_MASTER_SELECT == UART_SLAVE)))
static u8 GLOB_u8UARTGetData;
static u8 GLOB_u8UARTDataRecieved = FALSE;
#endif

STD_Type MCAL_UART_u8Init(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	UCSRB |= (RX_TX_EN_MODE);
	SET_BIT(UCSRC,URSEL);
	UCSRC |= (UMSEL_MODE|UPM_MODE|USBS_MODE|UCSZ10_MODE);
	UBRRL = (UBRR_MODE);
	UCSRB |= (RX_TX_UDR_IE_MODE|UCSZ2_MODE);
	#if( UART_INTERRUPT == UART_INTERRUPT_ON)
	SET_BIT(SREG,I_BIT);
	#endif
	LOC_u8ReturnValue = E_OK;
	return LOC_u8ReturnValue;
}

#if( UART_DATA_BITS == UART_EIGHT_BITS && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_MASTER)))
STD_Type MCAL_UART_u8SendData(u8 LOC_u8Data)
{
	//HAL_LCD_u8WriteString("Data Processed!");
//	#if(UART_INTERRUPT == UART_INTERRUPT_OFF)
	while(!GET_BIT(UCSRA,UDRE));
//	#endif
	UDR = LOC_u8Data;

	return E_OK;
}

STD_Type MCAL_UART_u8SendWord(u8 LOC_u8Word[])
{
	u8 LOC_u8Count = ZERO;
	while(LOC_u8Word[LOC_u8Count] != '\0')
	{
		MCAL_UART_u8SendData(LOC_u8Word[LOC_u8Count]);
		LOC_u8Count++;
	}
	return E_OK;
}
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_SLAVE)))
STD_Type MCAL_UART_u8RecieveData(u8 * LOC_Pu8Data)
{
#if( UART_INTERRUPT == UART_INTERRUPT_OFF )
	while(!GET_BIT(UCSRA,RXC));
#endif
	*LOC_Pu8Data = UDR;
	return E_OK;
}
#endif

#if( UART_INTERRUPT == UART_INTERRUPT_OFF)

#elif( UART_INTERRUPT == UART_INTERRUPT_ON)
STD_Type MCAL_UART_u8SuspendInterrupt(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
#if( UART_DIRECTION == UART_DUPLEX)
	CLEAR_BIT(UCSRB,RXCIE);
	CLEAR_BIT(UCSRB,UDRIE);
	LOC_u8ReturnValue = E_OK;
#elif( (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_SLAVE))
	CLEAR_BIT(UCSRB,RXCIE);
	LOC_u8ReturnValue = E_OK;
#elif( (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_MASTER))
	CLEAR_BIT(UCSRB,UDRIE);
	LOC_u8ReturnValue = E_OK;
#endif
	return LOC_u8ReturnValue;
}
STD_Type MCAL_UART_u8ResumeInterrupt(void)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
#if( UART_DIRECTION == UART_DUPLEX)
	SET_BIT(UCSRB,RXCIE);
	SET_BIT(UCSRB,UDRIE);
	LOC_u8ReturnValue = E_OK;
#elif( (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_SLAVE))
	SET_BIT(UCSRB,RXCIE);
	LOC_u8ReturnValue = E_OK;
#elif( (UART_DIRECTION == UART_SIMPLEX && UART_MASTER_SELECT == UART_MASTER))
	SET_BIT(UCSRB,UDRIE);
	LOC_u8ReturnValue = E_OK;
#endif
	return LOC_u8ReturnValue;
}
#if( ((UART_DIRECTION == UART_SIMPLEX) && (UART_MASTER_SELECT == UART_MASTER)) || (UART_DIRECTION == UART_DUPLEX) )
STD_Type MCAL_UART_u8CallbackFun(u8 LOC_Pu8Data[],void (*LOC_Pu8Ptr)(void))
#elif( ((UART_DIRECTION == UART_SIMPLEX) && (UART_MASTER_SELECT == UART_SLAVE)) || (UART_DIRECTION == UART_DUPLEX) )
STD_Type MCAL_UART_u8CallbackFun(void (*LOC_Pu8Ptr)(void))
#endif
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	//HAL_LCD_u8WriteString("Data Sent!");
	if(LOC_Pu8Ptr != NULL)
	{
#if( ((UART_DIRECTION == UART_SIMPLEX) && (UART_MASTER_SELECT == UART_MASTER)) || (UART_DIRECTION == UART_DUPLEX) )
		if(LOC_Pu8Data && (GLOB_u8UARTDataPending == FALSE) )
		{
			GLOB_u8UARTSendData = LOC_Pu8Data;
			GLOB_u8UARTDataSent = FALSE;	//We still didn't send the new data once, even if we sent the same data in this function before, we'll send it one more time only.
			GLOB_u8UARTDataPending = TRUE;
		}
#endif
		GLOB_PvoidUARTGptr = LOC_Pu8Ptr;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		//Do Nothing.
	}
	return LOC_u8ReturnValue;
}
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS && UART_INTERRUPT == UART_INTERRUPT_ON && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == SIMPLEX && UART_MASTER_SELECT == UART_MASTER)))
ISR(USART_UDRE_vect)
{
	static u8 LOC_u8Count;
	if(GLOB_u8UARTDataSent == FALSE)
	{
		if(GLOB_u8UARTSendData)
		{
			if(GLOB_u8UARTSendData[LOC_u8Count] != UART_TERMINATION_CHAR)
			{
				MCAL_UART_u8SendData(GLOB_u8UARTSendData[LOC_u8Count]);
				LOC_u8Count++;
			}
			else if(GLOB_u8UARTSendData[LOC_u8Count] == UART_TERMINATION_CHAR)
			{
				//MCAL_UART_u8SendData(UART_TERMINATION_CHAR);
				GLOB_u8UARTSendData = (u8*) NULL;
				GLOB_u8UARTDataSent = TRUE;
				GLOB_u8UARTDataPending = FALSE;
				LOC_u8Count = ZERO;
			}
		}
		if(GLOB_PvoidUARTGptr != NULL)
		{
			GLOB_PvoidUARTGptr();
		}
	}
}
#endif

#if( UART_DATA_BITS == UART_EIGHT_BITS && UART_INTERRUPT == UART_INTERRUPT_ON && (UART_DIRECTION == UART_DUPLEX || (UART_DIRECTION == SIMPLEX && UART_MASTER_SELECT == UART_SLAVE)))
ISR(USART_RXC_vect)
{
	if(GLOB_u8UARTDataRecieved)
	{
		MCAL_UART_u8RecieveData(&GLOB_u8UARTGetData);
		GLOB_u8UARTDataRecieved = FALSE;
	}
	if(GLOB_PvoidUARTGptr != NULL)
	{
		GLOB_PvoidUARTGptr();
	}
}

STD_Type MCAL_UART_u8GetData(u8 * LOC_Pu8Data)
{
	STD_Type LOC_u8ReturnValue = E_NOT_OK;
	if(LOC_Pu8Data && (GLOB_u8UARTDataRecieved == FALSE) )
	{
		*LOC_Pu8Data = GLOB_u8UARTGetData;
		GLOB_u8UARTDataRecieved = TRUE;
		LOC_u8ReturnValue = E_OK;
	}
	else
	{
		*LOC_Pu8Data = ZERO;
	}
	return LOC_u8ReturnValue;
}
#endif
