#ifndef TIMER_PRIV_H_
#define TIMER_PRIV_H_

#define OC0_PORT			DIO_PortB
#define OC0_PIN				Pin3
#define OC1A_PORT			DIO_PortD
#define OC1A_PIN			Pin5
#define OC1B_PORT			DIO_PortD
#define OC1B_PIN			Pin4
#define OC2_PORT			DIO_PortD
#define OC2_PIN				Pin7
#define ICP1_PORT			DIO_PortD
#define ICP1_PIN			Pin6

#define ACCURACY 		 	0
#define PERFORMANCE 	 	1

#define TIMER0_TICKS_MAX 	255
#define TIMER1_TICKS_MAX 	65535
#define TIMER2_TICKS_MAX 	255

#define MILLI_CONVERSION 1000
#define MICRO_CONVERSION 1000
#define PERCENT			 100

#define TIMER_INTERRUPT_OFF	0
#define TIMER_INTERRUPT_ON	1

#define NORMAL 				0
#define CTC					1
#define PWM					2
#define PHASE_CORRECT		3

#define EIGHT_BIT			8
#define NINE_BIT			9
#define TEN_BIT				10
#define COMPARE_MATCH		0
#define INPUT_CAPTURE		1

#define OC0_OFF				0
#if(TIMER0_MODE != PWM && TIMER0_MODE != PHASE_CORRECT)
#define OC0_TOGGLE 			1
#endif
#define OC0_CLEAR			2
#define OC0_SET				3

#define OC1A_OFF			0
#define OC1A_TOGGLE 		1
#define OC1A_CLEAR			2
#define OC1A_SET			3

#define OC1B_OFF			0
#define OC1B_TOGGLE 		1
#define OC1B_CLEAR			2
#define OC1B_SET			3

#define OC2_OFF				0
#if(TIMER2_MODE != PWM && TIMER2_MODE != PHASE_CORRECT)
#define OC2_TOGGLE 			1
#endif
#define OC2_CLEAR			2
#define OC2_SET				3

#define ICU_OFF				0
#define ICU_ON				1

#endif /* TIMER_PRIV_H_ */