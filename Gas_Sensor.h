/*
 * Smoke_Sensor.h
 *
 *  Created on: May 8, 2025
 *      Author: Youssef Khaled
 */

#ifndef GAS_SENSOR_H_
#define GAS_SENSOR_H_

/* Define the load resistance on the board, in kilo ohms */
#define RL_VALUE (5)

/* (Sensor resistance in clean air)/RO from the chart in datasheet */
#define RO_CLEAN_AIR_FACTOR (9.83)

/* Gas identity no. for LPG */
#define LPG (0)

/* Gas identity no. for smoke */
#define SMOKE (1)

/* Two points from LPG curve: point1:(200,1.6), point2(10000,0.26)
   log values: (lg200, lg1.6) = (2.3, 0.20), (lg10000, lg0.26) = (4, -0.58)
   slope derived from these points. Format: { x, y, slope } */
extern float LPGCurve[3];

/* Two points from smoke curve: point1:(200,3.4), point2(10000,0.62)
   log values: (lg200, lg3.4) = (2.3, 0.53), (lg10000, lg0.63) = (4, -0.20)
   slope derived from these points. Format: { x, y, slope } */
extern float SmokeCurve[3];

/* Function declarations */
uint32 GetPercentage(float rs_ro_ratio, float *curve);
uint32 GetGasPercentage(float rs_ro_ratio, uint32 gas_id);
float ReadSensor();
float ResistanceCalculation(uint32 raw_adc);
float SensorCalibration();

#endif /* GAS_SENSOR_H_ */
