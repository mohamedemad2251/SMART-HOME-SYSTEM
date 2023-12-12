#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

//OPTIONS:
/*
 * SPI_INTERRUPT_OFF
 * SPI_INTERRUPT_ON
 */
#define SPI_INTERRUPT	SPI_INTERRUPT_ON

//OPTIONS:
/*
 * SPI_SLAVE
 * SPI_MASTER
 */
#define SPI_SELECT		SPI_SLAVE

//OPTIONS:
/*
 * SPI_FALLING_EDGE
 * SPI_RISING_EDGE
 */
#define SPI_CLOCK_MODE	SPI_FALLING_EDGE

//OPTIONS:
/*
 * 2
 * 4
 * 8
 * 16
 * 32
 * 64
 * 128
 */
#define SPI_PRESCALER	128

#endif /* SPI_CONFIG_H_ */
