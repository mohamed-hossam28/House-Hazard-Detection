/*
 * Smoke_Sensor.c
 *
 *  Created on: May 8, 2025
 *      Author: Youssef Khaled
 */

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "ADC.h"
#include "Gas_Sensor.h"

/* Example values */
float SmokeCurve[3] = {2.3, 0.53, -0.44};
/* Example values */
float LPGCurve[3] = {2.3, 0.21, -0.47};

/* Sensor and load resistor form a voltage divider. Use analog value and load value */
float ResistanceCalculation(uint32 raw_adc) {
	/* We will find sensor resistor */
	return (((float) RL_VALUE * (ADC_MAXIMUM_VALUE - raw_adc) / raw_adc));
}

/* This function assumes that sensor is in clean air. */
float SensorCalibration(void) {
	uint32 i;
	/* Total reading value */
	float val = 0;
	const uint32 factor = 10;

	/* Take multiple samples and calculate the average value */
	for (i = 0; i < factor; i++) {
		val += ResistanceCalculation(ADC_ReadChannelPolling(0));
		_delay_ms(10);
	}

	val = val / factor;

	/* Divided by RO_CLEAN_AIR_FACTOR yields the Ro according to the chart in the datasheet */
	val = val / RO_CLEAN_AIR_FACTOR;

	return val;
}

/* Read sensor value and average multiple readings */
float ReadSensor() {
	uint32 i;
	float rs = 0;

	/* Take multiple readings and average it */
	for (i = 0; i < 10; i++) {
		/* rs changes according to gas concentration */
		rs += ResistanceCalculation(ADC_ReadChannelPolling(0));
		_delay_ms(5);
	}

	rs = rs / 10;

	return rs;
}

uint32 GetGasPercentage(float rs_ro_ratio, uint32 gas_id) {
	if (gas_id == LPG) {
		return GetPercentage(rs_ro_ratio, LPGCurve);
	}
	else if (gas_id == SMOKE) {
		return GetPercentage(rs_ro_ratio, SmokeCurve);
	}
	return 0;
}

/* Use simple polynomial or linear mapping instead of log/pow */
/* For example: assume a fixed empirical formula */
uint32 GetPercentage(float rs_ro_ratio, float *curve) {
	if (rs_ro_ratio > 5.0) return 0;
	else if (rs_ro_ratio > 3.0) return 25;
	else if (rs_ro_ratio > 2.0) return 50;
	else if (rs_ro_ratio > 1.0) return 75;
	else return 100;
}
