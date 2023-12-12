#ifndef UART_ADDRESS_H_
#define UART_ADDRESS_H_

#define UDR 	(*(volatile u8*) 0x2C)
#define UCSRA 	(*(volatile u8*) 0x2B)
#define RXC		Pin7
#define TXC		Pin6
#define UDRE	Pin5
#define FE		Pin4
#define DOR		Pin3
#define PE		Pin2
#define U2X		Pin1
#define MPCM	Pin0
#define UCSRB 	(*(volatile u8*) 0x2A)
#define RXCIE	Pin7
#define TXCIE	Pin6
#define UDRIE	Pin5
#define RXEN	Pin4
#define TXEN	Pin3
#define UCSZ2	Pin2
#define RXB8	Pin1
#define TXB8	Pin0
#define UCSRC 	(*(volatile u8*) 0x40)
#define URSEL	Pin7
#define UMSEL	Pin6
#define UPM1	Pin5
#define UPM0	Pin4
#define USBS	Pin3
#define UCSZ1	Pin2
#define UCSZ0	Pin1
#define UCPOL	Pin0
#define UBRR 	(*(volatile u16*) 0x29)
#define UBRRH 	(*(volatile u8*) 0x40)
#define UBRRL 	(*(volatile u8*) 0x29)

#define USART_RXC_vect	__vector_13	//Rx Complete
#define	USART_UDRE_vect	__vector_14	//Data Register Empty
#define USART_TXC_vect	__vector_15	//Tx Complete


#endif /* UART_ADDRESS_H_ */
