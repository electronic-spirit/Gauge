
#ifndef INC_ADC_H_
#define INC_ADC_H_


#include "stm32l4xx_hal.h"
#include "main.h"


#define STAT_REG		0x00
#define CTRL1_REG		0x01
#define CTRL2_REG		0x02
#define CTRL3_REG		0x03
#define SEQ_REG			0x08
#define CHMAP0_REG		0x07
#define CHMAP1_REG		0x06
#define SOC_REG			0x0A
#define SGC_REG			0x0B
#define SCOC_REG		0x0C
#define SCGC_REG		0x0D
#define DELAY_REG		0x05


#define PGA_GAIN_OFF   		0x00
#define PGA_GAIN1   		0x01
#define PGA_GAIN2   		0x02
#define PGA_GAIN4   		0x03
#define PGA_GAIN8   		0x04
#define PGA_GAIN16  		0x05
#define PGA_GAIN32  		0x06
#define PGA_GAIN64  		0x07
#define PGA_GAIN128 		0x08

#define DATA_RATE_2SPS 			0x00
#define DATA_RATE_4SPS 			0x01
#define DATA_RATE_8SPS 			0x02
#define DATA_RATE_16SPS 		0x03
#define DATA_RATE_32SPS 		0x04
#define DATA_RATE_63SPS 		0x05
#define DATA_RATE_125SPS 		0x06
#define DATA_RATE_250SPS 		0x07
#define DATA_RATE_500SPS 		0x08
#define DATA_RATE_1000SPS 		0x09
#define DATA_RATE_2000SPS 		0x0A
#define DATA_RATE_4000SPS 		0x0B
#define DATA_RATE_8000SPS 		0x0C
#define DATA_RATE_16000SPS 		0x0D
#define DATA_RATE_32000SPS 		0x0E
#define DATA_RATE_64000SPS 		0x0F


#define self_calibration 		0x00
#define offset_calibration 		0x01
#define full_scale_calibration 	0x02


#define ADC_NCS_ACTIVE()      HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET)
#define ADC_NCS_NOT_ACTIVE()  HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET)


uint32_t ADC_Read_REG(uint8_t REG, uint8_t len);
int32_t ADC_Read_DATA(uint8_t Channel);
int32_t ADC_UINT2INT_Convertation(uint32_t data);
void ADC_Write_REG(uint8_t REG, uint8_t *DATA, uint8_t len);
void ADC_Calibration(uint8_t CAL);
void ADC_Start_Conversion(uint8_t channel, uint8_t RATE);
void ADC_process(void);
void Auto_System_Calibration(void);
void HEX2BIN(uint32_t data, uint8_t*text, uint8_t ofset);
float HEX2FLOAT(uint32_t hex);

void ADC_init(void);
void ADC_Conversion(uint8_t RATE);
void ADC_EXT(void);
void ADC_PGA_Set(uint8_t pga_data);

extern uint8_t devider;
extern uint8_t adc_conversion_complete;
extern uint8_t adc_calibration_mode;
extern uint8_t adc_start_calibration;
extern uint8_t adc_timeout;


#endif /* INC_ADC_H_ */
