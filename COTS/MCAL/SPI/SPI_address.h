#ifndef SPI_ADDRESS_H_
#define SPI_ADDRESS_H_

#if(SPI_INTERRUPT == SPI_INTERRUPT_ON)
#define SPI_STC_vect 		__vector_12		//Serial Transfer Complete (SPI)
#endif

#define SS_PORT		DIO_PortB
#define	SS_PIN		Pin4
#define	MOSI_PORT	DIO_PortB
#define	MOSI_PIN	Pin5
#define MISO_PORT	DIO_PortB
#define	MISO_PIN	Pin6
#define	SCK_PORT	DIO_PortB
#define	SCK_PIN		Pin7

#define SPCR	(*(volatile u8*) 0x2D)
#define SPIE	Pin7
#define SPE		Pin6
#define	DORD	Pin5
#define MSTR	Pin4
#define CPOL	Pin3
#define CPHA	Pin2
#define SPR1	Pin1
#define SPR0	Pin0

#define SPSR	(*(volatile u8*) 0x2E)
#define SPIF	Pin7
#define WCOL	Pin6
/*
 * The rest is reserved
 */
#define SPI2X	Pin0

#define SPDR	(*(volatile u8*) 0x2F)


#endif /* SPI_ADDRESS_H_ */
