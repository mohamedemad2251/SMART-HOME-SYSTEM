#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#define FREQ (16)	//Making it MHz instead of Hz, easier
								//for calculations

//OPTIONS:
/*
 * ACCURACY
 * PERFORMANCE
 */

#define TIMER0_CTC_OPTIMIZE	ACCURACY

//OPTIONS:
/*
 * ACCURACY
 * PERFORMANCE
 */

#define TIMER1_CTC_OPTIMIZE	ACCURACY

//OPTIONS:
/*
 * NORMAL
 * CTC
 * PWM
 * PHASE_CORRECT
 */
#define TIMER0_MODE			CTC

//OPTIONS:
/*
 * NORMAL
 * CTC
 * PWM
 * PHASE_CORRECT
 */
#define TIMER1_MODE			PWM

//OPTIONS:
/*
 * EIGHT_BIT
 * NINE_BIT
 * TEN_BIT
 * COMPARE_MATCH	//i.e. OCR1A
 * INPUT CAPTURE	//i.e. ICR1
 */
#if(TIMER1_MODE == PWM || TIMER1_MODE == PHASE_CORRECT)
#define TIMER1_RESOLUTION COMPARE_MATCH
#endif

//OPTIONS:
/*
 * ICU_OFF
 * ICU_ON
 */
#define ICU_MODE ICU_OFF

//OPTIONS:
/*
 * NORMAL
 * CTC
 * PWM
 * PHASE_CORRECT
 */
#define TIMER2_MODE			PWM

//OPTIONS:
/*
 * OFF
 * ON
 */
#define TIMER0_INTERRUPT 	TIMER_INTERRUPT_ON	//with OCR1A only

//OPTIONS:
/*
 * OFF
 * ON
 */
#define TIMER1_INTERRUPT 	TIMER_INTERRUPT_OFF

//OPTIONS:
/*
 * OFF
 * ON
 */
#define TIMER2_INTERRUPT 	TIMER_INTERRUPT_OFF


//OPTIONS:
/*
 * 0
 * 8
 * 64
 * 256
 * 1024
 */
#define TIMER0_PRESCALER	64

//OPTIONS:
/*
 * 0
 * 8
 * 64
 * 256
 * 1024
 */
#define TIMER1_PRESCALER	1024

//OPTIONS:
/*
 * 0
 * 8
 * 64
 * 256
 * 1024
 */
#define TIMER2_PRESCALER	1024

//OPTIONS:
/*
 * OC1A_OFF
 * OC1A_TOGGLE
 * OC1A_CLEAR
 * OC1A_SET
 */
#define OC1A_MODE			OC1A_CLEAR

//OPTIONS:
/*
 * OC1B_OFF
 * OC1B_TOGGLE
 * OC1B_CLEAR
 * OC1B_SET
 */
#define OC1B_MODE			OC1B_CLEAR

//OPTIONS:
/*
 * OC0_OFF
 * OC0_TOGGLE	(NON-PWM ONLY)
 * OC0_CLEAR
 * OC0_SET
 */
#define OC0_MODE			OC0_OFF

//OPTIONS:
/*
 * OC2_OFF
 * OC2_TOGGLE	(NON-PWM ONLY)
 * OC2_CLEAR
 * OC2_SET
 */
#define OC2_MODE			OC2_CLEAR

#endif /* TIMER_CONFIG_H_ */
