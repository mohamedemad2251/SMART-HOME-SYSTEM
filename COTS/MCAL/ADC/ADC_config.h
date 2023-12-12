#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

//OPTIONS:
/*
 * AREF
 * AVCC
 * INTERNAL
 */
#define ADC_VOLTAGE_SOURCE 	AVCC

//OPTIONS:
/*
 * RIGHT
 * LEFT
 */
#define ADC_ADJUSTMENT 		RIGHT

//OPTIONS:
/*
 * ADC_OFF
 * ADC_ON
 */
#define ADC_AUTO_TRIGGER 	ADC_OFF

#if(ADC_AUTO_TRIGGER == ADC_ON)

#endif

//OPTIONS:
/*
 * ADC_INTERRUPT_OFF
 * ADC_INTERRUPT_ON
 */
#define ADC_INTERRUPT 	 	ADC_INTERRUPT_ON

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
#define ADC_PRESCALER 		128

#endif /* ADC_CONFIG_H_ */
