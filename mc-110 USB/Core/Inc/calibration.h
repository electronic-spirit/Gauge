
#ifndef INC_CALIBRATION_H_
#define INC_CALIBRATION_H_


#include "stm32l4xx_hal.h"
#include "main.h"

void Approximation_MC110(int32_t meas_data, int32_t power_data);
void Calculate(int32_t adc_press, int32_t adc_pow);
double calculate_current(double data);
double calculate_voltage(double data);


extern double a_current, b_current, c_current, a_voltage, b_voltage, c_voltage;


#endif /* INC_CALIBRATION_H_ */
