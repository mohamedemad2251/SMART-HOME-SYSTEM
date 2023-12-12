#ifndef UART_PRIV_H_
#define UART_PRIV_H_

#define UART_FIVE_BITS 		5
#define UART_SIX_BITS 		6
#define UART_SEVEN_BITS		7
#define UART_EIGHT_BITS 	8
#define UART_NINE_BITS 		9

#define UART_TRANSMITTER	0	//Same as making it master
#define UART_RECIEVER		1	//Same as making it slave

#define UART_MASTER			0
#define UART_SLAVE			1

#define UART_SIMPLEX		0
#define UART_DUPLEX			1

#define UART_ASYNC			0
#define UART_SYNC			1

#define UART_NO_PARITY		0
#define UART_EVEN_PARITY	2
#define UART_ODD_PARITY		3

#define UART_ONE_STOP		0
#define UART_TWO_STOPS		1

#define UART_INTERRUPT_OFF	0
#define UART_INTERRUPT_ON	1

#define UART_BAUD_9600		103
#define UART_BAUD_57600		16


#endif /* UART_PRIV_H_ */
