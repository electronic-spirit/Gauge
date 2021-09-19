

#include "u8g2_LCD.h"


uint16_t spitimeout = 0;

#ifdef  USE_BSP_SPI2
  extern SPI_HandleTypeDef huc1638_hspi;
#else
  extern SPI_HandleTypeDef hspi2;
#endif
static SPI_HandleTypeDef hdisplayspi;



uint8_t spi_timeout(void)
{
	if(++spitimeout==100) return 1;
	else return 0;
}


/* Private functions ---------------------------------------------------------*/
/** @addtogroup _Exported_Functions
  * @{
  */
uint8_t u8x8_gpio_and_delay_cb_stm32l4 (U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg,
		U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	  switch (msg)
	  {
	  case U8X8_MSG_GPIO_AND_DELAY_INIT:
		  LCD_NRST_NOT_ACTIVE();
		  LCD_NCS_NOT_ACTIVE();
	    break;
	  case U8X8_MSG_DELAY_MILLI:
		  delay_ms(arg_int);
	    break;
	  case U8X8_MSG_GPIO_DC:
	    HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, arg_int); //?
	    break;
	  case U8X8_MSG_GPIO_CS:
	    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, arg_int); //?
	    break;
	  case U8X8_MSG_GPIO_RESET:
	    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, arg_int);
	    break;
	  }
	  return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t *data;//, a_flag;
#ifdef  USE_BSP_SPI2
 hdisplayspi.Instance = huc1638_hspi.Instance;
#else
 hdisplayspi.Instance = hspi2.Instance;
#endif

  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
	data = (uint8_t *)arg_ptr;
    while (arg_int > 0)
    {
    	/* check TXE flag */
    	while((hdisplayspi.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE)
    	{
    		spitimeout = 0;
    		if(spi_timeout()==1) break;
    	}
    	/* Write the data */
    	*((__IO uint8_t*) &hdisplayspi.Instance->DR ) = (uint8_t) * data;
    	/* Wait BSY flag */
    	while ((hdisplayspi.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY)
    		{
    			spitimeout = 0;
    		    if(spi_timeout()==1) break;
    		}
    	while ((hdisplayspi.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY)
    	{
    		spitimeout = 0;
    		if(spi_timeout()==1) break;
    	}
        data++;
        arg_int--;
    }
//	HAL_SPI_Transmit(&hspi2, (uint8_t*)data, arg_int, 1000);
    break;
  case U8X8_MSG_BYTE_INIT:
	  u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
    break;
  case U8X8_MSG_BYTE_SET_DC:
	  u8x8_gpio_SetDC(u8x8, arg_int);
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
	  __HAL_SPI_ENABLE(&hdisplayspi);
	  u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
	  u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
	  __HAL_SPI_DISABLE(&hdisplayspi);
    break;
  default:
    return 0;
  }
  return 1;
}

/**
  * @}
  */

/************************ (C) COPYRIGHT Microelectronics *****END OF FILE****/
