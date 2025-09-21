/*
 * ADC.h
 *
 *  Created on: Apr 30, 2025
 */

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   5

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	AREF, AVCC, INTERNAL_2_56 = 3
} ADC_ReferenceVoltage;

typedef enum {
	ADC_CHANNEL_0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7
} ADC_Channel;

typedef enum {
	PRESCALER_2 = 1,
	PRESCALER_4,
	PRESCALER_8,
	PRESCALER_16,
	PRESCALER_32,
	PRESCALER_64,
	PRESCALER_128
} ADC_PrescalerSelections;

typedef enum {
	ADC_RIGHT_ADJUST, ADC_LEFT_ADJUST
} ADC_ResultAdjust;

typedef enum {
	POLLING, INTERRUPT
} ADC_ModeType;

typedef struct {
	ADC_ReferenceVoltage ADC_Volt;
	ADC_Channel ADC_Channel;
	ADC_PrescalerSelections ADC_Prescaler;
	ADC_ResultAdjust ADC_Adjust;
	ADC_ModeType ADC_Mode;
} ADC_ConfigType;

/*******************************************************************************
 *                                Function Prototypes                         *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType *Config_Ptr);
void ADC_InterruptEnable();
void ADC_InterruptDisable();
uint16 ADC_ReadChannelPolling(ADC_Channel Channel_num);
void ADC_startConversionInterrupt(ADC_Channel Channel_num);
void ADC_setCallBack(void (*a_ptr)(void));
uint16 ADC_getResult(void);
void ADC_deInit(void);
#endif /* ADC_H_ */

