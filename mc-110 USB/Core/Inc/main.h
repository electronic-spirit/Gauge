/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "u8g2.h"
#include "u8g2_LCD.h"
#include "Global_Variables.h"
#include "ELSPIRE_OS.h"
#include "Buttons.h"
#include "ADC.h"
#include "W25N01.h"
#include "Real_Time_Clock.h"
#include "Display.h"
#include "Display_Options.h"
#include "Measure_screen.h"
#include "ADC_screen.h"
#include "Measure_options.h"
#include "menu.h"
#include "Auto_Off_option.h"
#include "about_screen_options.h"
#include "temperature.h"
#include "logging_option_screen.h"
#include "calibration.h"
#include "logging_fatfs.h"
#include "sound.h"
#include "MAX77960.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc1;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_ARK_Pin GPIO_PIN_2
#define LCD_ARK_GPIO_Port GPIOE
#define Reserved_Pin GPIO_PIN_3
#define Reserved_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_4
#define LCD_RST_GPIO_Port GPIOE
#define LCD_MOSI_Pin GPIO_PIN_1
#define LCD_MOSI_GPIO_Port GPIOC
#define LCD_MISO_Pin GPIO_PIN_2
#define LCD_MISO_GPIO_Port GPIOC
#define LCD_PWM_Pin GPIO_PIN_0
#define LCD_PWM_GPIO_Port GPIOA
#define ADC_SCK_Pin GPIO_PIN_1
#define ADC_SCK_GPIO_Port GPIOA
#define ADC_RDYB_Pin GPIO_PIN_4
#define ADC_RDYB_GPIO_Port GPIOA
#define ADC_RDYB_EXTI_IRQn EXTI4_IRQn
#define SOUND_Pin GPIO_PIN_5
#define SOUND_GPIO_Port GPIOA
#define ADC_MISO_Pin GPIO_PIN_6
#define ADC_MISO_GPIO_Port GPIOA
#define ADC_MOSI_Pin GPIO_PIN_7
#define ADC_MOSI_GPIO_Port GPIOA
#define ADC_CS_Pin GPIO_PIN_4
#define ADC_CS_GPIO_Port GPIOC
#define SW_P_Pin GPIO_PIN_5
#define SW_P_GPIO_Port GPIOC
#define I_P_ERROR_Pin GPIO_PIN_0
#define I_P_ERROR_GPIO_Port GPIOB
#define Audio_EN_Pin GPIO_PIN_1
#define Audio_EN_GPIO_Port GPIOB
#define INT_CHARGER_Pin GPIO_PIN_2
#define INT_CHARGER_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_10
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_SPI_CS_Pin GPIO_PIN_12
#define LCD_SPI_CS_GPIO_Port GPIOB
#define POWER_SCL_Pin GPIO_PIN_13
#define POWER_SCL_GPIO_Port GPIOB
#define POWER_SDA_Pin GPIO_PIN_14
#define POWER_SDA_GPIO_Port GPIOB
#define P_meter_UGen_Pin GPIO_PIN_13
#define P_meter_UGen_GPIO_Port GPIOD
#define EN_LCD_reserv_Pin GPIO_PIN_14
#define EN_LCD_reserv_GPIO_Port GPIOD
#define EN_AV6_reserv_Pin GPIO_PIN_15
#define EN_AV6_reserv_GPIO_Port GPIOD
#define EN_AV6_Pin GPIO_PIN_6
#define EN_AV6_GPIO_Port GPIOC
#define EN_V24_Pin GPIO_PIN_7
#define EN_V24_GPIO_Port GPIOC
#define EN_LCD_Pin GPIO_PIN_8
#define EN_LCD_GPIO_Port GPIOC
#define MEM_SCK_Pin GPIO_PIN_10
#define MEM_SCK_GPIO_Port GPIOC
#define MEM_MISO_Pin GPIO_PIN_11
#define MEM_MISO_GPIO_Port GPIOC
#define MEM_MOSI_Pin GPIO_PIN_12
#define MEM_MOSI_GPIO_Port GPIOC
#define MEM1_CS_Pin GPIO_PIN_2
#define MEM1_CS_GPIO_Port GPIOD
#define MEM2_CS_Pin GPIO_PIN_3
#define MEM2_CS_GPIO_Port GPIOD
#define LCD_BL_EN2_Pin GPIO_PIN_6
#define LCD_BL_EN2_GPIO_Port GPIOD
#define KEYPAD_SCL_Pin GPIO_PIN_6
#define KEYPAD_SCL_GPIO_Port GPIOB
#define KEYPAD_SDA_Pin GPIO_PIN_7
#define KEYPAD_SDA_GPIO_Port GPIOB
#define I2C_ADDR_Pin GPIO_PIN_8
#define I2C_ADDR_GPIO_Port GPIOB
#define LCD_CD_Pin GPIO_PIN_9
#define LCD_CD_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define TURN_ON_ADC_POWER()            HAL_GPIO_WritePin(EN_AV6_GPIO_Port, EN_AV6_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(EN_AV6_reserv_GPIO_Port, EN_AV6_reserv_Pin, GPIO_PIN_SET);
#define TURN_OFF_ADC_POWER()           HAL_GPIO_WritePin(EN_AV6_GPIO_Port, EN_AV6_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(EN_AV6_reserv_GPIO_Port, EN_AV6_reserv_Pin, GPIO_PIN_RESET);

#define TURN_ON_SOUND()			HAL_GPIO_WritePin(Audio_EN_GPIO_Port, Audio_EN_Pin, GPIO_PIN_SET);
#define TURN_OFF_SOUND()        HAL_GPIO_WritePin(Audio_EN_GPIO_Port, Audio_EN_Pin, GPIO_PIN_RESET);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
