/*
 * ADC.c
 *
 *  Created on: Apr 30, 2025
 */
#include "ADC.h"
#include "ADC_Private.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static volatile void (*g_adcCallbackPtr)(void) = NULL_PTR;
/*static ADC_ModeType g_adcMode;*/
static volatile uint16 g_adcResult = 0;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(ADC_vect) {
	if (g_adcCallbackPtr != NULL_PTR) {
		g_adcCallbackPtr();
	}
	/* Read ADC result and store it globally */
	if (ADMUX_REG.Bits.ADLAR_Bit == 1) {
		g_adcResult = (ADCH_REG << 2); /* 8-bit left adjusted */
	} else {
		g_adcResult = ADC_REG; /* 10-bit right adjusted */
	}
}

/*******************************************************************************
 *                            Function Definitions                             *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType *Config_Ptr) {
	/* Choose ADC VOLT */
	switch (Config_Ptr->ADC_Volt) {
	case AREF:
		ADMUX_REG.Bits.REFS0_Bit = 0;
		ADMUX_REG.Bits.REFS1_Bit = 0;
		break;
	case AVCC:
		ADMUX_REG.Bits.REFS0_Bit = 1;
		ADMUX_REG.Bits.REFS1_Bit = 0;
		break;
	case INTERNAL_2_56:
		ADMUX_REG.Bits.REFS0_Bit = 1;
		ADMUX_REG.Bits.REFS1_Bit = 1;
		break;
	}
	/*	 Choose ADC Initial Channel */
	ADMUX_REG.Byte &= 0xE0;
	ADMUX_REG.Byte |= Config_Ptr->ADC_Channel;

	/* Enable ADC */
	ADCSRA_REG.Bits.ADEN_Bit = 1;

	/* Set Prescaler */
	switch (Config_Ptr->ADC_Prescaler) {
	case PRESCALER_2:
		ADCSRA_REG.Bits.ADPS0_Bit = 1;
		ADCSRA_REG.Bits.ADPS1_Bit = 0;
		ADCSRA_REG.Bits.ADPS2_Bit = 0;
		break;
	case PRESCALER_4:
		ADCSRA_REG.Bits.ADPS0_Bit = 0;
		ADCSRA_REG.Bits.ADPS1_Bit = 1;
		ADCSRA_REG.Bits.ADPS2_Bit = 0;
		break;
	case PRESCALER_8:
		ADCSRA_REG.Bits.ADPS0_Bit = 1;
		ADCSRA_REG.Bits.ADPS1_Bit = 1;
		ADCSRA_REG.Bits.ADPS2_Bit = 0;
		break;
	case PRESCALER_16:
		ADCSRA_REG.Bits.ADPS0_Bit = 0;
		ADCSRA_REG.Bits.ADPS1_Bit = 0;
		ADCSRA_REG.Bits.ADPS2_Bit = 1;
		break;
	case PRESCALER_32:
		ADCSRA_REG.Bits.ADPS0_Bit = 1;
		ADCSRA_REG.Bits.ADPS1_Bit = 0;
		ADCSRA_REG.Bits.ADPS2_Bit = 1;
		break;
	case PRESCALER_64:
		ADCSRA_REG.Bits.ADPS0_Bit = 0;
		ADCSRA_REG.Bits.ADPS1_Bit = 1;
		ADCSRA_REG.Bits.ADPS2_Bit = 1;
		break;
	case PRESCALER_128:
		ADCSRA_REG.Bits.ADPS0_Bit = 1;
		ADCSRA_REG.Bits.ADPS1_Bit = 1;
		ADCSRA_REG.Bits.ADPS2_Bit = 1;
		break;
	}

	/* ADC Adjust Left / Right */
	if (Config_Ptr->ADC_Adjust == ADC_LEFT_ADJUST) {
		ADMUX_REG.Bits.ADLAR_Bit = 1;
	} else {
		ADMUX_REG.Bits.ADLAR_Bit = 0;
	}

	/* ADC Interrupt Enable */
	if (Config_Ptr->ADC_Mode == INTERRUPT) {
		ADCSRA_REG.Bits.ADIE_Bit = 1;
		/*g_adcMode = INTERRUPT;*/
	} else {
		ADCSRA_REG.Bits.ADIE_Bit = 0;
		/*g_adcMode = POLLING;*/
	}

}

void ADC_InterruptEnable() {
	ADCSRA_REG.Bits.ADIE_Bit = 1;
}

void ADC_InterruptDisable() {
	ADCSRA_REG.Bits.ADIE_Bit = 0;
}

uint16 ADC_ReadChannelPolling(ADC_Channel Channel_num) {
	/* Choose ADC Channel */
	ADMUX_REG.Byte &= 0xE0;
	ADMUX_REG.Byte |= Channel_num;

	/* Start Conversion */
	ADCSRA_REG.Bits.ADSC_Bit = 1;

	/* Wait for conversion to complete, ADIF becomes '1' */
	while (ADCSRA_REG.Bits.ADIF_Bit == 0)
		;

	/* Clear the flag by writing 1 to it */
	ADCSRA_REG.Bits.ADIF_Bit = 1;

	/* Return ADC result (handle left/right adjust) */
	if (ADMUX_REG.Bits.ADLAR_Bit == 1) {
		/* Left-adjusted: read only high 8 bits (if needed) */
		return (ADCH_REG << 2); /* 8-bit left adjusted */
	} else {
		/* Right-adjusted: return 10-bit result */
		return ADC_REG; /* ADC is a 16-bit register combining ADCL + ADCH */
	}

}
void ADC_startConversionInterrupt(ADC_Channel Channel_num) {
	/* Choose ADC Channel */
	ADMUX_REG.Byte &= 0xE0;
	ADMUX_REG.Byte |= Channel_num;

	/* Start Conversion */
	ADCSRA_REG.Bits.ADSC_Bit = 1;
}
void ADC_setCallBack(void (*a_functionPtr)(void)) {
	g_adcCallbackPtr = a_functionPtr;
}
uint16 ADC_getResult(void) {
	return g_adcResult;
}
void ADC_deInit(void) {
	ADCSRA_REG.Bits.ADEN_Bit = 0;
	ADCSRA_REG.Bits.ADIE_Bit = 0;
	ADMUX_REG.Byte = 0;
}

