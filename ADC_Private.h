/*
 * ADC_Private.h
 *
 *  Created on: Apr 30, 2025
 */

#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

#include "std_types.h"

/*******************************************************************************
 *                 ADC Registers type structure declarations                *
 *******************************************************************************/

/* Bitmap structure for ADMUX register */

typedef union {
	uint8 Byte;/* All register bits */
	struct {
		uint8 MUX0_Bit :1; /* Analog Channel and Gain Selection Bit 0 */
		uint8 MUX1_Bit :1; /* Analog Channel and Gain Selection Bit 1 */
		uint8 MUX2_Bit :1; /* Analog Channel and Gain Selection Bit 2 */
		uint8 MUX3_Bit :1; /* Analog Channel and Gain Selection Bit 3 */
		uint8 MUX4_Bit :1; /* Analog Channel and Gain Selection Bit 4 */
		uint8 ADLAR_Bit :1; /* ADC Left Adjust Result */
		uint8 REFS0_Bit :1; /* Reference Selection Bit 0 */
		uint8 REFS1_Bit :1; /* Reference Selection Bit 1 */
	} Bits;
} ADC_ADMUX;

/* Bitmap structure for ADCSRA register */
typedef union {
	uint8 Byte;/* All register bits */
	struct {
		uint8 ADPS0_Bit :1; /* ADC Prescaler Select Bit 0 */
		uint8 ADPS1_Bit :1; /* ADC Prescaler Select Bit 1 */
		uint8 ADPS2_Bit :1; /* ADC Prescaler Select Bit 2 */
		uint8 ADIE_Bit :1; /* ADC Interrupt Enable */
		uint8 ADIF_Bit :1; /* ADC Interrupt Flag */
		uint8 ADATE_Bit :1; /* ADC Auto Trigger Enable */
		uint8 ADSC_Bit :1; /* ADC Start Conversion */
		uint8 ADEN_Bit :1; /* ADC Enable */
	} Bits;
} ADC_ADCSRA;

/* Bitmap structure for SFIOR register */
typedef union {
	uint8 Byte;/* All register bits */
	struct {
		uint8 PSR10_Bit :1; /* Prescaler Reset Timer/Counter1 and Timer/Counter0 */
		uint8 PSR2_Bit :1; /* Prescaler Reset Timer/Counter2 */
		uint8 PUD_Bit :1; /* Pull-up disable */
		uint8 ACME_Bit :1; /* Analog Comparator Multiplexer Enable */
		uint8 :1; /* Reserved Bit */
		uint8 ADTS0_Bit :1; /* ADC Auto Trigger Source */
		uint8 ADTS1_Bit :1; /* ADC Auto Trigger Source */
		uint8 ADTS2_Bit :1; /* ADC Auto Trigger Source */
	} Bits;
} ADC_SFIOR;

/*******************************************************************************
 *                         ADX Registers Definitions                           *
 *******************************************************************************/

#define ADMUX_REG (*(volatile ADC_ADMUX *const)   0x27) /* ADC Multiplexer Selection Register */
#define ADCSRA_REG (*(volatile ADC_ADCSRA *const)   0x26) /* ADC Control and Status Register A */
#define ADCH_REG (*(volatile uint8 *const)   0x25) /* The ADC Higher Data Register */
#define ADCL_REG (*(volatile uint8 *const)   0x24) /* The ADC Lower Data Register */
#define ADC_REG (*(volatile uint16 *const)   0x24) /* The ADC Data Register */
#define SFIOR_REG (*(volatile uint8 *const)  0x50) /*Special FunctionIO Register */


#endif /* ADC_PRIVATE_H_ */

