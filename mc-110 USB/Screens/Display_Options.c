
#include  "Display_Options.h"
#include "u8g2_LCD.h"


extern u8g2_t u8g2;
SPI_HandleTypeDef hspi2;

#define max_edit_param 3
uint8_t Vbias=1;
uint8_t light_sec=0, light_min=0;
uint16_t light_ms=0;
uint8_t temp_light=0;
uint8_t soft_onoff_cntr=0;


void Display_options_screen_func(void)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	// brightness
	u8g2_DrawUTF8(&u8g2, 20, 60, "Яркость:");
	sprintf(RAM,"%d", display_brightness);
	Blink_Edit_Parameter(170, 60, RAM, edit_param, 0);
	// contrast
	u8g2_DrawUTF8(&u8g2, 20, 80, "Контрастность:");
	sprintf(RAM,"%d", display_contrast);
	Blink_Edit_Parameter(170, 80, RAM, edit_param, 1);
	// delay
	u8g2_DrawUTF8(&u8g2, 20, 100, "Длительность:");
	sprintf(RAM,"%d", light_delay);
	Blink_Edit_Parameter(170, 100, RAM, edit_param, 2);

	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);

	TIM2->CCR1=temp_light;//display_brightness;

	// Vbias
	HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	aTxBuffer[0]=0xE8+Vbias;
	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
	// Temp compensation
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	aTxBuffer[0]=0x81;
	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, GPIO_PIN_SET);
	aTxBuffer[0]=display_contrast+125;
	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}


// MENU
void Display_options_screen_S9(void)
{
	screen=Parameter_screen;
	old_active_string=0xFF;
	Menu_num=0;
}

// DOWN
void Display_options_screen_S6(void)
{
	if(edit_param<(max_edit_param-1))edit_param++;
}

// UP
void Display_options_screen_S7(void)
{
	if(edit_param>0)edit_param--;
}

// ENTER
void Display_options_screen_S4(void)
{
	switch(edit_param)
	{
		case 0: Save_Settings(display_brightness_tag, display_brightness); break;
		case 1: Save_Settings(display_contrast_tag, display_contrast); break;
		case 2: Save_Settings(light_delay_tag, light_delay); break;
	}
}



// BACKWARD
void Display_options_screen_S8(void)
{
	switch(edit_param)
	{
		case 0: if(display_brightness>0)display_brightness--; break;
		case 1: if(display_contrast>0)display_contrast--; break;
		case 2: if(light_delay>0)light_delay--; break;
	}
}

// FORWARD
void Display_options_screen_S5(void)
{
	switch(edit_param)
	{
		case 0:if(display_brightness<50)display_brightness++; break;
		case 1: if(display_contrast<50)display_contrast++; break;
		case 2: if(light_delay<50)light_delay++; break;
	}
}



void Light_OFF_process(void)
{
	if(++light_ms>=1000)
	{
		light_ms=0;
		if(++light_sec>59){light_min++; light_sec=0;}
	}

	if(++soft_onoff_cntr>=50)
	{
		soft_onoff_cntr=0;
		if(light_min>=light_delay)
		{
			light_min=light_delay;
			if(temp_light>0)temp_light--;
			light_min=light_delay;
			TIM2->CCR1=temp_light;
		}
		else
		{
			if(temp_light<display_brightness)temp_light++;
			if(temp_light>=display_brightness)temp_light=display_brightness;
			TIM2->CCR1=temp_light;
		}
	}
}

