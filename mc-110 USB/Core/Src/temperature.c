
#include "temperature.h"

#define tV_25   1.186      // Напряжение (в вольтах) на датчике при температуре 25 °C.
#define tSlope  0.0043    // Изменение напряжения (в вольтах) при изменении температуры на градус.
#define Vref    3.3       // Образцовое напряжение АЦП (в вольтах).
extern ADC_HandleTypeDef hadc1;

uint16_t temperature_buf[256];

void Temperature_process(void)
{
	uint8_t i=0;
	uint32_t temp_sum=0;
	uint16_t temperature_middle = 0;

	for(i=0;i<128;i++)temp_sum+=temperature_buf[i];
	temperature_middle = (temp_sum>>7);

	MCU_temp = (double)(temperature_middle/4096.0)*Vref;   // Напряжение в вольтах на датчике.
	MCU_temp = (tV_25-MCU_temp)/tSlope + 25.0;             // Температура в градусах.

	HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &temperature_buf, 128);	// Запуск DMA на чтение температуры MCU

	sleep(100);
}
