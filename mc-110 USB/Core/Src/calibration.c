#include "calibration.h"

#define num_of_cal_points	11
#define num_of_temperature_cal_points	4


// Коэффициенты Тимура
double a_current, b_current, c_current, a_voltage, b_voltage, c_voltage;


// Калибровочные точки
double Pressure_cal_points[num_of_cal_points]={0, 0.7, 1.4, 2.1, 2.8, 3.5, 4.2, 4.9, 5.6, 6.3, 7.0};
int32_t ADC_measure_cal_points[num_of_cal_points]={98717, 594404, 1089976, 1585548, 2080942, 2576264, 3071423, 3566551, 4061479, 4556289, 5051115};
int32_t ADC_power_cal_points[num_of_cal_points]={3381422,3381431,3381501,3381593,3381896,3382039,3382234,3382476,3382791,3383102,3383419};

double T_coef_cal[num_of_temperature_cal_points]={0.994262, 1.005968, 1.0154, 1.1}; // В порядке возрастания
double Temperature_cal_points[num_of_temperature_cal_points] = {15.0, 21.3, 24.57, 30.0};

double cal_temperature = 15.0;				    // Температура калибровки
double affect=0;// -0.0000418240259295;		// Влияние температуры на расход Па/градус (прямое влияние положительное значение, обратное влияние отрицательное)



double Approximation_pressure(int32_t current_ADC_measure);
double Approximation_Tcoef(int32_t current_ADC_measure, int32_t current_ADC_power);
double Approximation_temperature(double T_coef);
double Temperature_correction(double uncorrect_Pressure, double affect_temp);



void Approximation_MC110(int32_t meas_data, int32_t power_data)
{
	double LSB;
	double temp_diff;

	int32_t ADC_MEAS_ZERO = meas_data-zero_ADC[0];

	// Считаем LSB
	LSB=0.00000000028857241176*(double)ADC_MEAS_ZERO+1.41200835380823000000;
	// Находим температуру
	Temperature2=0.000128471469464*(double)power_data-420.277295988542;
	// Дифференциал температуры
	temp_diff=Temperature2-cal_temperature;
	// Находим термостабилизированное LSB
	LSB=LSB-(temp_diff*0.0000418240259295);
	// Находим давление
	Pressure=(LSB*(double)ADC_MEAS_ZERO)/1000000.0;
}



void Calculate(int32_t adc_press, int32_t adc_pow)
{
	static double Press_old=0;
	// Считаем давлени
	Test_Pressure = Approximation_pressure(adc_press);
	// Считаем температурный коэффициент
	Temperature_coeff_25 = Approximation_Tcoef(adc_press, adc_pow);
	// Считаем температуру
	Temperature = Approximation_temperature(Temperature_coeff_25);
	// Корректируем давление по температуре
	Test_Pressure = (Temperature_correction(Test_Pressure, Temperature) + Press_old)/2.0;
	Press_old = Test_Pressure;
}



//---------------------------------------------------------------------------------------------------------------------------------------------------------
//													Аппроксимация давления
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double Approximation_pressure(int32_t current_ADC_measure)
{
	double RES=0, a, b;
	int index = 0;
	int32_t ADC_MEAS_ZERO=0;

	if(zero_ADC[0]!=0)ADC_MEAS_ZERO = ADC_measure_cal_points[0]+current_ADC_measure-zero_ADC[0];
	else ADC_MEAS_ZERO = current_ADC_measure;

	for (int i = 0; i < (num_of_cal_points-1); i++) if(ADC_MEAS_ZERO>ADC_measure_cal_points[i])index++;

	if(index==0) a = (Pressure_cal_points[index+1] - Pressure_cal_points[index]) / (double)(ADC_measure_cal_points[index+1] - ADC_measure_cal_points[index]);
	else a = (Pressure_cal_points[index]-Pressure_cal_points[index-1]) / ((double)ADC_measure_cal_points[index]-(double)ADC_measure_cal_points[index-1]);

	b = (double)Pressure_cal_points[index] - (double)ADC_measure_cal_points[index] * a;

	RES = b + a * (double)ADC_MEAS_ZERO;

	return RES;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------
//													Аппроксимация температурного коэффициента
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double Approximation_Tcoef(int32_t current_ADC_measure, int32_t current_ADC_power)
{
	double RES = 0, a, b;
	int index = 0;
	int32_t ADC_MEAS_ZERO=0;

	if(zero_ADC[0]!=0)ADC_MEAS_ZERO = ADC_measure_cal_points[0]+current_ADC_measure-zero_ADC[0];
	else ADC_MEAS_ZERO = current_ADC_measure;

	for (int i = 0; i < (num_of_cal_points-1); i++) if (ADC_MEAS_ZERO > ADC_measure_cal_points[i]) index++;

	if (index == 0) a = (double)(ADC_power_cal_points[index + 1] - ADC_power_cal_points[index]) / (double)(ADC_measure_cal_points[index + 1] - ADC_measure_cal_points[index]);
	else a = (double)(ADC_power_cal_points[index] - ADC_power_cal_points[index - 1]) / (double)(ADC_measure_cal_points[index] - ADC_measure_cal_points[index - 1]);
	b = (double)ADC_power_cal_points[index] - (double)ADC_measure_cal_points[index] * a;

	RES = b + a * (double)ADC_MEAS_ZERO;

	return RES/current_ADC_power;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//													Аппроксимация температуры
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double Approximation_temperature(double T_coef)
{
	double RES = 0, a, b;
	int index = 0;

	for (int i = 0; i < (num_of_temperature_cal_points-1); i++) if (T_coef > T_coef_cal[i]) index++;

	if (index == 0) a = (double)(Temperature_cal_points[index + 1] - Temperature_cal_points[index]) / (double)(T_coef_cal[index + 1] - T_coef_cal[index]);
	else a = (double)(Temperature_cal_points[index] - Temperature_cal_points[index - 1]) / (double)(T_coef_cal[index] - T_coef_cal[index - 1]);
	b = (double)Temperature_cal_points[index] - (double)T_coef_cal[index] * a;

	RES = b + a * T_coef;

	return RES;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//													Коррекция давления по температуре
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double Temperature_correction(double uncorrect_Pressure, double affect_temp)
{
	return (uncorrect_Pressure + (cal_temperature - affect_temp) * affect);
}



//---------------------------------------------------------------------------------------------------------------------------------------------------------
//													Расчет тока и напряжения, формулы Тимура
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double calculate_current(double data)
{
	double RES;
	//a_current = -0.0000741921796439735;
	//b_current = 9.9684133071094100000;
	//c_current = 0.0080740815705555100;
	RES = (-b_current+(sqrt((b_current*b_current)-(4*a_current*(c_current-data)))))/(2*a_current);

	return RES;
}

double calculate_voltage(double data)
{
	double RES;
	//a_voltage = 0.0000160989032721161;
	//b_voltage = 9.6660275246403800000;
	//c_voltage = 0.0066157945677234600;
	RES = (-b_voltage+(sqrt((b_voltage*b_voltage)-(4*a_voltage*(c_voltage-data)))))/(2*a_voltage);

	return RES;
}





