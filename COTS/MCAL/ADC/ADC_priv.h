#ifndef ADC_PRIV_H_
#define ADC_PRIV_H_

#define ADC_PORT	DIO_PortA

#define ADC_OFF		0
#define ADC_ON		1

#ifndef ADC_INTERRUPT_OFF
#define	ADC_INTERRUPT_OFF	0
#endif

#ifndef ADC_INTERRUPT_ON
#define	ADC_INTERRUPT_ON	1
#endif

#define ADC_REGISTER_SHIFT	8

#define AREF		0
#define AVCC		1
#define INTERNAL	3

#define	RIGHT		0
#define LEFT		1

#define CLEAR_ADC_SELECT	0b11100000

#endif /* ADC_PRIV_H_ */
