
#ifndef INC_GLOBAL_VARIABLES_H_
#define INC_GLOBAL_VARIABLES_H_


#include "main.h"
#include "stm32l4xx_hal.h"


#define ADC_BUFF_SIZE			512		// Размеры буферов
#define NUMBER_OF_VARIABLES		17		// Доступные глобальные переменные
#define NUMBER_OF_TYPES			8		// Количество типов данных

enum Errors{
	ADC_cal_err,				// Ошибка калибровки ADC
	ADC_err,					// Отказ АЦП
	Memory_err,					// Отсутствует или повреждена FLASH
	USB_CVC_err,				// Ошибка отправки по USB
	KeyPad_err,					// Ошибки клавиатуры
};



enum GlobalVariables{
	ADC_data_name,
	ADC_volt_name,
	ADC_channel_0_name,
	ADC_channel_1_name,
	ADC_channel_2_name,
	ADC_channel_3_name,
	ADC_channel_4_name,
	ADC_channel_5_name,
	Temperature_name,
	Pressure_name,
	Pressure_Ag_name,
	ADC_middle_name,
	current_4_20mA_name,
	voltage_measure_name,
	Kdiv_name,
	ADC_channel_OFF_name,
	ERROR_name,
};


enum TypeVariables{
	type_INT8,
	type_INT16,
	type_INT32,
	type_UINT8,
	type_UINT16,
	type_UINT32,
	type_FLOAT,
	type_DOUBLE,
};


//--------------------------------------------------------------------------------------------
//										       RAM
//--------------------------------------------------------------------------------------------

extern int32_t ADC_data[6];
extern double ADC_volt[6];
// Кольцевые буферы для АЦП
extern int32_t ADC_channel_1[ADC_BUFF_SIZE];
extern int32_t ADC_channel_2[ADC_BUFF_SIZE];
extern int32_t ADC_channel_3[ADC_BUFF_SIZE];
extern int32_t ADC_channel_4[ADC_BUFF_SIZE];
extern int32_t ADC_channel_5[ADC_BUFF_SIZE];
extern int32_t ADC_channel_0[ADC_BUFF_SIZE];
extern int32_t ADC_channel_OFF[ADC_BUFF_SIZE];
extern int32_t ADC_middle[7];
extern double Pressure_zero;
extern double Pressure;
extern double Pressure_Ag;


extern uint16_t Vbat_uint[128];
extern uint32_t ERROR_REG;

extern uint8_t BLE_bridge;
extern uint8_t usart1_data[100];
extern uint8_t usart1_counter;
extern int8_t rx_cmplt;
extern uint8_t BLE_config;

extern double MCU_temp;
extern uint8_t E_fun;
extern double Electric_zero;

extern double Temperature;
extern double Temperature2;
extern double Temperature_coeff_25;

extern double Test_Pressure;

extern int32_t zero_ADC[6];
extern double current_4_20mA;
extern double voltage_measure;

extern uint8_t AutoAmpCoef; 	// Пересчет реального делителя для Тимура
extern uint16_t AutoOffCoef_period;
extern uint16_t AutoGainCoef_period;
extern double RefVoltage;
extern double Kdiv;

//--------------------------------------------------------------------------------------------
//										       EEPROM
//--------------------------------------------------------------------------------------------

extern uint8_t display_brightness;
extern uint8_t display_contrast;

extern uint8_t mm_peak;						// ПИКОВЫЕ ЗНАЧЕНИЯ
extern uint8_t leak_detect;					// ОБНАРУЖЕНИЕ УТЕЧЕК
extern uint8_t PV;							// P+V
extern uint8_t PI;							// P+I
extern uint8_t PE;							// P+E

extern uint8_t PGA_GAIN;
extern uint8_t ADC_SPS;
extern uint8_t adc_calibration_mode;
extern uint16_t ADC_Buff_size;

extern uint16_t Auto_Off_Time;
extern uint32_t serial_num;
extern uint32_t gauge_range;
extern double gauge_error;
extern uint16_t light_delay;

extern uint8_t logging;
extern uint8_t logging_start_hour;
extern uint8_t logging_start_min;
extern uint8_t logging_start_sec;
extern uint8_t logging_period;
extern uint8_t logging_len;
extern uint8_t logging_num;

extern uint8_t Sound_Vol;

//--------------------------------------------------------------------------------------------
//										   Parser Variables
//--------------------------------------------------------------------------------------------

extern uint32_t GV_LINKS[NUMBER_OF_VARIABLES];
extern const char* GV_STRING[NUMBER_OF_VARIABLES];
extern const uint8_t GV_NUM_OF_SYM[NUMBER_OF_VARIABLES];
extern const uint16_t GV_LENGTH[NUMBER_OF_VARIABLES];
extern const uint8_t GV_NUM_OF_SYM[NUMBER_OF_VARIABLES];
extern const uint8_t GV_TYPE[NUMBER_OF_VARIABLES];
extern const char* TYPE_STRING[NUMBER_OF_TYPES];



#endif /* INC_GLOBAL_VARIABLES_H_ */
