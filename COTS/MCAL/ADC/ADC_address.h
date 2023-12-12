#ifndef INCFILE1_H_
#define INCFILE1_H_

#define	ADC_vect	__vector_16

#define ADMUX 	(*(volatile u8*) 0x27)
#define REFS1	Pin7
#define REFS0	Pin6
#define	ADLAR	Pin5
#define MUX4	Pin4
#define MUX3	Pin3
#define MUX2	Pin2
#define MUX1	Pin1
#define MUX0	Pin0
#define ADCSRA 	(*(volatile u8*) 0x26)
#define ADEN	Pin7
#define ADSC	Pin6
#define	ADATE	Pin5
#define ADIF	Pin4
#define ADIE	Pin3
#define ADPS2	Pin2
#define ADPS1	Pin1
#define ADPS0	Pin0
#define ADC		(*(volatile u16*) 0x24)
#define ADCH 	(*(volatile u8*) 0x25)
#define ADCL 	(*(volatile u8*) 0x24)
#define SFIOR 	(*(volatile u8*) 0x50)
#define ADTS2	Pin7
#define ADTS1	Pin6
#define	ADTS0	Pin5

#endif /* INCFILE1_H_ */