
#ifndef INC_U8G2_LCD_H_
#define INC_U8G2_LCD_H_

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32l4xx_hal.h"



#define LCD_NRST_NOT_ACTIVE() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)
#define LCD_NRST_ACTIVE()     HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)

#define LCD_NCS_ACTIVE()      HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_NCS_NOT_ACTIVE()  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define LCD_DATA()            HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, GPIO_PIN_SET)
#define LCD_COMMAND()         HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, GPIO_PIN_RESET)


uint8_t u8x8_gpio_and_delay_cb_stm32l4 (U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t spi_timeout(void);






#endif /* INC_U8G2_LCD_H_ */


