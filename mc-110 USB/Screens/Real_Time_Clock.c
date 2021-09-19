
#include "Real_Time_Clock.h"
#include "u8g2_LCD.h"
extern u8g2_t u8g2;
#include "stdio.h"

#define max_edit_param 6


RTC_HandleTypeDef hrtc;
uint8_t year, month, day, hour, minute, sec, year_temp, month_temp, day_temp, hour_temp, minute_temp, sec_temp;
const uint8_t bat_icon[]={0,24,0,12,0x3F, 0xFF, 0xFE, 0x60, 0x00, 0x03, 0x40, 0x00, 0x01, 0x40, 0x00, 0x01, 0xC0, 0x00, 0x01, 0xC0, 0x00, 0x01, 0xC0, 0x00, 0x01, 0xC0, 0x00, 0x01, 0x40, 0x00, 0x01, 0x40, 0x00, 0x01, 0x60, 0x00, 0x03, 0x3F, 0xFF, 0xFE};
const uint8_t tara_icon[]={0,16,0,16, 0x3F, 0xFC, 0x40, 0x02, 0x80, 0x01, 0x8F, 0xF1, 0x8F, 0xF1, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x80, 0x01, 0x40, 0x02, 0x3F, 0xFC};
const uint8_t eltara_icon[]={0,16,0,16, 0x3F, 0xFC, 0x40, 0x02, 0x80, 0x01, 0x87, 0xE1, 0x87, 0xE1, 0x86, 0x01, 0x86, 0x01, 0x87, 0x81, 0x87, 0x81, 0x86, 0x01, 0x86, 0x01, 0x87, 0xE1, 0x87, 0xE1, 0x80, 0x01, 0x40, 0x02, 0x3F, 0xFC};



void Battery_process(void)
{
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*) &Vbat_uint,128);
	sleep(100);
}



void RTC_view(void)
{
	uint8_t battery_percent, i;
	uint32_t bat_sum=0;
	float battery_volt;

	// date & time
	u8g2_SetFont(&u8g2, u8g2_font_9x15_t_cyrillic);
	sprintf(RAM,"%02d.%02d.%02d", day, month, year);
	u8g2_DrawUTF8(&u8g2, 0, 12, RAM);
	sprintf(RAM,"%02d:%02d:%02d", hour, minute, sec);
	u8g2_DrawUTF8(&u8g2, 81, 12, RAM);

	// Tarring
	if(Pressure_zero!=0)DrawImage(tara_icon, 194, 2, 1);

	//El tarring
	if(Electric_zero!=0)DrawImage(eltara_icon, 176, 2, 1);

	// Battery
	for(i=0;i<128;i++)bat_sum+=Vbat_uint[i];
	battery_volt = 0.0074258*(bat_sum>>7);
	DrawImage(bat_icon, 216, 4, 1);
	battery_percent = (uint8_t)((battery_volt-2.9)*100);

	u8g2_SetFont(&u8g2, u8g2_font_5x8_t_cyrillic);
	sprintf(RAM,"%.2f", battery_volt);
	//sprintf(RAM,"%d", battery_percent);
	u8g2_DrawUTF8(&u8g2, 219, 13, RAM);
}


void RTC_process(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	year=sDate.Year;
	month=sDate.Month;
	day=sDate.Date;
	hour=sTime.Hours;
	minute=sTime.Minutes;
	sec=sTime.Seconds;

	sleep(1000);
}


void RTC_set(uint8_t rtc_year, uint8_t rtc_month, uint8_t rtc_day, uint8_t rtc_hour, uint8_t rtc_min, uint8_t rtc_sec)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	HAL_RTC_DeInit(&hrtc);


	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
	Error_Handler();
	}


	sTime.Hours = rtc_hour;
	sTime.Minutes = rtc_min;
	sTime.Seconds = rtc_sec;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = rtc_month;
	sDate.Date = rtc_day;
	sDate.Year = rtc_year;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_RTCEx_EnableBypassShadow(&hrtc);
}


void RTC_screen_func(void)
{
	if(old_active_string==0xFF)
	{
		old_active_string=0;
		year_temp=year;
		month_temp=month;
		day_temp=day;
		hour_temp=hour;
		minute_temp=minute;
		sec_temp=sec;
	}

	// date time
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	// Date
	u8g2_DrawUTF8(&u8g2, 20, 50, "Дата:");
	sprintf(RAM,"%02d.", day_temp);
	Blink_Edit_Parameter(120, 50, RAM, edit_param, 0);
	sprintf(RAM,"%02d.", month_temp);
	Blink_Edit_Parameter(150, 50, RAM, edit_param, 1);
	sprintf(RAM,"%02d", year_temp);
	Blink_Edit_Parameter(180, 50, RAM, edit_param, 2);


	// Date
	u8g2_DrawUTF8(&u8g2, 20, 70, "Время:");
	sprintf(RAM,"%02d:", hour_temp);
	Blink_Edit_Parameter(120, 70, RAM, edit_param, 3);
	sprintf(RAM,"%02d:", minute_temp);
	Blink_Edit_Parameter(150, 70, RAM, edit_param, 4);
	sprintf(RAM,"%02d", sec_temp);
	Blink_Edit_Parameter(180, 70, RAM, edit_param, 5);


	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
}



// MENU
void RTC_screen_S9(void)
{
	screen=Parameter_screen;
	old_active_string=0xFF;
	Menu_num=2;
}

// DOWN
void RTC_screen_S5(void)
{
	if(edit_param<(max_edit_param-1))edit_param++;
}

// UP
void RTC_screen_S8(void)
{
	if(edit_param>0)edit_param--;
}



// BACKWARD
void RTC_screen_S6(void)
{
	switch(edit_param)
	{
		case 0: if(day_temp>0)day_temp--; break;
		case 1: if(month_temp>0)month_temp--; break;
		case 2: if(year_temp>0)year_temp--; break;
		case 3: if(hour_temp>0)hour_temp--; break;
		case 4: if(minute_temp>0)minute_temp--; break;
		case 5: if(sec_temp>0)sec_temp--; break;
	}
}

// FORWARD
void RTC_screen_S7(void)
{
	switch(edit_param)
	{
		case 0: if(day_temp<31)day_temp++; break;
		case 1: if(month_temp<12)month_temp++; break;
		case 2: if(year_temp<60)year_temp++; break;
		case 3: if(hour_temp<24)hour_temp++; break;
		case 4: if(minute_temp<60)minute_temp++; break;
		case 5: if(sec_temp<60)sec_temp++; break;
	}
}

// ENTER
void RTC_screen_S4(void)
{
	RTC_set(year_temp, month_temp, day_temp, hour_temp, minute_temp, sec_temp);
}





