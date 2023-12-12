#ifndef MCU_H_
#define MCU_H_

#include "STD_types.h"

#ifndef NULL
#define NULL (void*) 0
#endif

#define ISR(vectNum) void vectNum (void) __attribute__ ((signal,used, externally_visible)) ; \
		void vectNum (void)

#define EXTI0_vect 			__vector_1
#define EXTI1_vect 			__vector_2
#define EXTI2_vect 			__vector_3
#define	TIMER2_COMP_vect 	__vector_4
#define TIMER2_OVF_vect		__vector_5
#define TIMER1_CAPT_vect	__vector_6
#define	TIMER1_COMPA_vect 	__vector_7
#define TIMER1_COMPB_vect	__vector_8
#define TIMER1_OVF_vect		__vector_9
#define	TIMER0_COMP_vect 	__vector_10
#define TIMER0_OVF_vect		__vector_11

#ifndef ZERO
#define ZERO	0
#endif

#ifndef FALSE
#define FALSE 	0
#endif
#ifndef TRUE
#define TRUE 	1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif
#ifndef ENABLE
#define	ENABLE	1
#endif

#ifndef CLEAR
#define CLEAR	0
#endif
#ifndef SET
#define SET		1
#endif

#ifndef LOW
#define LOW 	0
#endif
#ifndef HIGH
#define HIGH 	1
#endif

#ifndef RISING_EDGE
#define RISING_EDGE			0
#endif
#ifndef FALLING_EDGE
#define FALLING_EDGE		1
#endif

#ifndef INTERRUPT_OFF
#define INTERRUPT_OFF 		0
#endif

#ifndef INTERRUPT_ON
#define INTERRUPT_ON		1
#endif

#define Pin0 	0
#define Pin1 	1
#define Pin2 	2
#define Pin3 	3
#define Pin4 	4
#define Pin5 	5
#define Pin6 	6
#define Pin7 	7

//WATCHDOG TIMER DEFINITONS
#define WDTCR 	(*(volatile u8*) 0x41)
#define WDTOE	Pin4
#define WDE		Pin3
#define WDP2	Pin2
#define WDP1	Pin1
#define WDP0	Pin0

//DIO DEFINITIONS
#define PORTA 	(*(volatile u8*) 0x3B)
#define DDRA 	(*(volatile u8*) 0x3A)
#define PINA 	(*(volatile u8*) 0x39)
#define PORTB 	(*(volatile u8*) 0x38)
#define DDRB 	(*(volatile u8*) 0x37)
#define PINB 	(*(volatile u8*) 0x36)
#define PORTC 	(*(volatile u8*) 0x35)
#define DDRC 	(*(volatile u8*) 0x34)
#define PINC 	(*(volatile u8*) 0x33)
#define PORTD 	(*(volatile u8*) 0x32)
#define DDRD 	(*(volatile u8*) 0x31)
#define PIND 	(*(volatile u8*) 0x30)

//EXTI DEFINITIONS
#define SREG 	(*(volatile u8*) 0x5F)
#define C_BIT 	Pin0
#define Z_BIT 	Pin1
#define N_BIT 	Pin2
#define V_BIT 	Pin3
#define S_BIT 	Pin4
#define H_BIT 	Pin5
#define T_BIT 	Pin6
#define I_BIT 	Pin7
#define MCUCR 	(*(volatile u8*) 0x55)
#define ISC00 	Pin0
#define ISC01 	Pin1
#define ISC10 	Pin2
#define ISC11 	Pin3
#define MCUCSR 	(*(volatile u8*) 0x54)
#define ISC2 	Pin6
#define GICR 	(*(volatile u8*) 0x5B)
#define INT2 	Pin5
#define INT0 	Pin6
#define INT1 	Pin7
#define GIFR 	(*(volatile u8*) 0x5A)
#define INTF2 	Pin5
#define INTF0 	Pin6
#define INTF1 	Pin7

//TIMERS DEFINITIONS
#define TCCR0 	(*(volatile u8*) 0x53)
#define FOC0	Pin7
#define WGM00	Pin6
#define	COM01	Pin5
#define COM00	Pin4
#define WGM01	Pin3
#define CS02	Pin2
#define CS01	Pin1
#define CS00	Pin0
#define TCNT0 	(*(volatile u8*) 0x52)
#define OCR0 	(*(volatile u8*) 0x5C)
#define TIMSK 	(*(volatile u8*) 0x59)
#define OCIE2	Pin7
#define TOIE2	Pin6
#define TICIE1	Pin5
#define OCIE1A	Pin4
#define OCIE1B	Pin3
#define TOIE1	Pin2
#define OCIE0	Pin1
#define TOIE0	Pin0
#define TIFR 	(*(volatile u8*) 0x58)
#define OCF2	Pin7
#define TOV2	Pin6
#define ICF1	Pin5
#define OCF1A	Pin4
#define OCF1B	Pin3
#define TOV1	Pin2
#define OCF0	Pin1
#define TOV0	Pin0

#define TCCR1A	(*(volatile u8*) 0x4F)
#define COM1A1	Pin7
#define COM1A0	Pin6
#define COM1B1	Pin5
#define COM1B0	Pin4
#define FOC1A	Pin3
#define FOC1B	Pin2
#define WGM11	Pin1
#define WGM10	Pin0
#define TCCR1B	(*(volatile u8*) 0x4E)
#define ICNC1	Pin7
#define ICES1	Pin6
//Reserved Pin (Pin5)
#define WGM13	Pin4
#define WGM12	Pin3
#define CS12	Pin2
#define CS11	Pin1
#define CS10	Pin0
#define TCNT1	(*(volatile u16*) 0x4C)
#define TCNT1H	(*(volatile u8*) 0x4D)
#define TCNT1L	(*(volatile u8*) 0x4C)
#define OCR1A	(*(volatile u16*) 0x4A)
#define OCR1AH	(*(volatile u8*) 0x4B)
#define OCR1AL	(*(volatile u8*) 0x4A)
#define OCR1B	(*(volatile u16*) 0x48)
#define OCR1BH	(*(volatile u8*) 0x49)
#define OCR1BL	(*(volatile u8*) 0x48)
#define ICR1	(*(volatile u16*) 0x46)
#define ICR1H	(*(volatile u8*) 0x47)
#define ICR1L	(*(volatile u8*) 0x46)
#define TCCR2 	(*(volatile u8*) 0x45)
#define FOC2	Pin7
#define WGM20	Pin6
#define	COM21	Pin5
#define COM20	Pin4
#define WGM21	Pin3
#define CS22	Pin2
#define CS21	Pin1
#define CS20	Pin0
#define TCNT2 	(*(volatile u8*) 0x44)
#define OCR2 	(*(volatile u8*) 0x43)

#endif /* MCU_H_ */
