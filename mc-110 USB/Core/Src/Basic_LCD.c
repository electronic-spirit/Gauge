/*
 ******************************************************************************
 * Basic_LCD.c
 *
 * Created on: 30 июл. 2021 г.
 *     Author: Karimov
 * @brief  Оставил а отдельном файле, изночалный код, когда получилсь,
 * отобразить на дисплее картинку из массива данных в файле logo.h.
 * Ни одна из функции, написанных здесь, не будет использоваться далее
 * Настройка SPI2: 8BIT(S8) div=64
 * Использование:
 *   __HAL_SPI_ENABLE(&hspi2);
 *   LCD_Init();
 *   LCD_DisplayWindow();
 *   LCD_Display_bmp((uint8_t *) a_logo);
 * Все, теперь можно смотрет отображение изображение на экране дисплея
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @defgroup _Private_TypesDefinitions Private Types Definitions
 */

/* Private define ------------------------------------------------------------*/
/** @defgroup _Constants Private Constants
  * @{
  */
#define spi_time					0x2
//#define VlcdPM						0x0070
#define VlcdPM						0x0080

/* Private macro -------------------------------------------------------------*/
/** @defgroup _Private_Macros Private Macros
  * @{
  */
extern SPI_HandleTypeDef hspi2;
/* Private variables ---------------------------------------------------------*/
/** @defgroup _Private_Variables Private Variables
  * @{
  */ 

/* Private function prototypes -----------------------------------------------*/
/** @defgroup _Private_Functions Private Functions
  * @{
  */
void LCD_Init(void);
void LCD_DisplayWindow(void);
void LCD_Display_bmp(uint8_t *dat);

static void SPI_Send_Data(uint8_t pdata);
static void SPI_Send_Cmnd(uint8_t pdata);

/* Private functions ---------------------------------------------------------*/
/** @addtogroup _Exported_Functions
  * @{
  */
/**

/*  */
/**
  * @brief
  * @param
  * @retval
  */
static void SPI_Send_Data(uint8_t pdata)
{
	uint8_t cnt = spi_time;
	uint16_t temp_d = 0x100;
	temp_d |= (uint16_t) pdata;
	while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) == RESET);
	LCD_DATA();
	LCD_NCS_ACTIVE();
//	*((__IO uint16_t*) &hspi2.Instance->DR ) = temp_d; //9bit
	*((__IO uint8_t*) &hspi2.Instance->DR ) = pdata;
//	if (HAL_SPI_Transmit(&hspi2, data, 2, HAL_MAX_DELAY) != HAL_OK)
//	{
//		HAL_GPIO_WritePin(LCD_NCS_GPIO_Port, LCD_NCS_Pin, GPIO_PIN_SET);
//		Error_Handler();
//	}
	while ((hspi2.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
	while ((hspi2.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
	while(cnt)
	{
		cnt--;
	}
	LCD_NCS_NOT_ACTIVE();
}

/**
  * @brief
  * @param
  * @retval
  */
static void SPI_Send_Cmnd(uint8_t pdata)
{
	uint8_t cnt = spi_time;
	uint16_t temp_d = 0x000;
	temp_d |= (uint16_t)pdata;

	while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) == RESET);
	LCD_COMMAND();
	LCD_NCS_ACTIVE();
//	*((__IO uint16_t*) &hspi2.Instance->DR ) = temp_d;
	*((__IO uint8_t*) &hspi2.Instance->DR ) = pdata;

	while ((hspi2.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
	while ((hspi2.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
	while(cnt)
	{
		cnt--;
	}
	LCD_NCS_NOT_ACTIVE();
}

void LCD_Init(void)
{
	osDelay(5);
	LCD_NRST_ACTIVE();
	osDelay(10);
	LCD_NRST_NOT_ACTIVE();
	osDelay(150);
	SPI_Send_Cmnd(0xE1);  // (23) system reset
	SPI_Send_Data(0xE2);

	osDelay(5);
	SPI_Send_Cmnd(0x24);  // (5) Temp. Comp -0.05%
	SPI_Send_Cmnd(0xC4);  //(18) set ram mapping control  MY=1, MX=1 (18) LC[1:0]= MY:MX= 10'b
//	SPI_Send_Cmnd(0xC2);  //(18) set ram mapping control  MY=1, MX=1 (18) LC[1:0]= MY:MX= 10'b
	SPI_Send_Cmnd(0xa2);  //(15) LC[3:2]00:14.1/01:17.4/10:20.8/11:25.4    // LC[3:2]=10'b

    osDelay(3);
    SPI_Send_Cmnd(0xeb);    //(26) LCD bias rate BR[1:0]= 11b = 1/12 //standby mode off   0xea	//0xeb  1//11 bais//
    SPI_Send_Cmnd(0x81);    //(10) set EV Vbias PM[7:0] = 104
    SPI_Send_Data(VlcdPM); // 15V set constr=104 default and must be adjustable lot by lot
    // Страница 27 Vlcd quick reference BR=12, PM=104, на рисунке см. голубую линию

	osDelay(3);
	SPI_Send_Cmnd(0x2d);   // (6) PC[1:0] internal booster enabled / pump control. Vlcd

	osDelay(3);
	SPI_Send_Cmnd(0x89);   // (13) set RAM address control
	SPI_Send_Cmnd(0xa0);   // (15) LC[3:2] = 00
	SPI_Send_Cmnd(0x95);   // (14) set 1bit Display mode DC[5]=0, DC[4]=1. Pattern 0, 2 bits per pixel.

    osDelay(3);
    SPI_Send_Cmnd(0xf1);   //(29) Set COM electrode End
    SPI_Send_Data(0x9f);   // COM End = 159
    SPI_Send_Cmnd(0x84);   // (11) disable partial display LC[8]=0
    SPI_Send_Cmnd(0x60);   // (7) Set avanced Program Control APC[R][7:0] = ? Do not Use ?
    SPI_Send_Data(0x00); // It is Double byte comand !!! For UltraChip's only
    SPI_Send_Cmnd(0x70);   // (9) Set page address PA[5:4]=00'b

    SPI_Send_Cmnd(0x04);   // (4) set column address
    SPI_Send_Data(0x00);   // CA[7:0] = 0x00

    SPI_Send_Cmnd(0xf4);   // (32) Set Window Progr. starting column addr
	SPI_Send_Data(0x00);   // WPC0[7:0] = 0x00

	SPI_Send_Cmnd(0xf5);   // (33) Set Window Prorg. starting Page address
	SPI_Send_Data(0x00);   // WPP0[7:0] = 0x00

	SPI_Send_Cmnd(0xf6);   // (34) Set Window Progr. ending column address
	SPI_Send_Data(0xef);   // WPC1[7:0] = 0xEF / 239'dec

	SPI_Send_Cmnd(0xf7);   // (35) Set Window Prorg. ending Page address
	SPI_Send_Data(0x13);   // WPP1[7:0] = 0x13 / 19'dec

	SPI_Send_Cmnd(0xc9);   // (20) Set Display Enable
	SPI_Send_Data(0xad);   // B/W MODE=0, exit sleep mode=1 DC[3]=0 DC[2]=1
}

/* */
void LCD_DisplayWindow(void)
{
	SPI_Send_Cmnd(0x60);	// (7) Set avanced Program Control APC[R][7:0] = ? Do not Use ?
	SPI_Send_Cmnd(0x70);	// (9) Set page address PA[5:4]=00'b

	SPI_Send_Cmnd(0x04);	// (4) set column address
	SPI_Send_Data(0x00);	// CA[7:0] = 0x00

	SPI_Send_Cmnd(0xf4);	// (32) Set Window Progr. starting column addr
	SPI_Send_Data(0x00);	// WPC0[7:0] = 0x00

	SPI_Send_Cmnd(0xf5);	// (33) Set Window Prorg. starting Page address
	SPI_Send_Data(0x00);	// WPP0[7:0] = 0x00

	SPI_Send_Cmnd(0xf6);	// (34) Set Window Progr. ending column address
	SPI_Send_Data(0xef);	// WPC1[7:0] = 0xEF / 239'dec

	SPI_Send_Cmnd(0xf7);	// (35) Set Window Prorg. ending Page address
	SPI_Send_Data(0x13);	// WPP1[7:0] = 0x13 / 19'dec

	SPI_Send_Cmnd(0xf9);    // (36) AC[3] = 1 Set Window Program Enable
}

/* */
void LCD_Display_bmp(uint8_t *dat) //write a picture data
{
	uint8_t ii, jj;
	LCD_DisplayWindow();
	SPI_Send_Cmnd(0x01);		  //write datap

	for(ii = 0; ii < 20; ii++)
	{
		for(jj = 0; jj < 240; jj++)
		{
			SPI_Send_Data(*(dat++));
		}
	}
}

/**
  * @}
  */

/************************ (C) COPYRIGHT Microelectronics *****END OF FILE****/
