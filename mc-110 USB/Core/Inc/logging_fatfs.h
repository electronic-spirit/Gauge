
#ifndef INC_LOGGING_FATFS_H_
#define INC_LOGGING_FATFS_H_


#include "main.h"
#include "stm32l4xx_hal.h"


#define NUMBER_OF_TAGS 34

enum Main_Tags{
	end_of_file_tag,
	display_brightness_tag,
	display_contrast_tag,
	mm_peak_tag,
	leak_detect_tag,
	PV_tag,
	PI_tag,
	PE_tag,
	PGA_GAIN_tag,
	ADC_SPS_tag,
	adc_calibration_mode_tag,
	ADC_Buff_size_tag,
	Auto_Off_Time_tag,
	serial_num_tag,
	gauge_range_tag,
	gauge_error_tag,
	light_delay_tag,
	logging_tag,
	logging_start_hour_tag,
	logging_start_min_tag,
	logging_start_sec_tag,
	logging_period_tag,
	logging_len_tag,
	logging_num_tag,
	a_current_tag,
	b_current_tag,
	c_current_tag,
	a_voltage_tag,
	b_voltage_tag,
	c_voltage_tag,
	AutoAmpCoef_tag,
	AutoOffCoef_period_tag,
	RefVoltage_tag,
	AutoGainCoef_period_tag
};



void Logging_init(void);
void Create_File(uint8_t*FileName, uint8_t NameSize, uint8_t*FileText, uint16_t FileSize);
uint8_t Read_File(uint8_t*FileName, uint8_t NameSize, uint8_t*FileText, uint16_t FileSize);
void Load_Settings(void);
void Save_Settings(uint8_t TAG, int32_t data);
double scan_double(uint16_t offset, uint8_t *buff);




#endif /* INC_LOGGING_FATFS_H_ */
