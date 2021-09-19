
#include "ADC_screen.h"
#include "u8g2_LCD.h"
extern u8g2_t u8g2;

#define max_edit_param 6


void ADC_options_screen_func(void)
{
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	// PGA
	u8g2_DrawUTF8(&u8g2, 10, 40, "PGA:");
	switch(PGA_GAIN)
	{
		case 0: sprintf(RAM,"OFF"); break;
		case 1: sprintf(RAM,"%d", 1); break;
		case 2: sprintf(RAM,"%d", 2); break;
		case 3: sprintf(RAM,"%d", 4); break;
		case 4: sprintf(RAM,"%d", 8); break;
		case 5: sprintf(RAM,"%d", 16); break;
		case 6: sprintf(RAM,"%d", 32); break;
		case 7: sprintf(RAM,"%d", 64); break;
		case 8: sprintf(RAM,"%d", 128); break;
	}
	Blink_Edit_Parameter(120, 40, RAM, edit_param, 0);

	// SPS
	u8g2_DrawUTF8(&u8g2, 10, 60, "SPS:");
	switch(ADC_SPS)
	{
		case 0: Blink_Edit_Parameter(120, 60, "RATE_2SPS", edit_param, 1); break;
		case 1: Blink_Edit_Parameter(120, 60, "RATE_4SPS", edit_param, 1); break;
		case 2: Blink_Edit_Parameter(120, 60, "RATE_8SPS", edit_param, 1); break;
		case 3: Blink_Edit_Parameter(120, 60, "RATE_16SPS", edit_param, 1); break;
		case 4: Blink_Edit_Parameter(120, 60, "RATE_32SPS", edit_param, 1); break;
		case 5: Blink_Edit_Parameter(120, 60, "RATE_63SPS", edit_param, 1); break;
		case 6: Blink_Edit_Parameter(120, 60, "RATE_125SPS", edit_param, 1); break;
		case 7: Blink_Edit_Parameter(120, 60, "RATE_250SPS", edit_param, 1); break;
		case 8: Blink_Edit_Parameter(120, 60, "RATE_500SPS", edit_param, 1); break;
		case 9: Blink_Edit_Parameter(120, 60, "RATE_1000SPS", edit_param, 1); break;
		case 10: Blink_Edit_Parameter(120, 60, "RATE_2000SPS", edit_param, 1); break;
		case 11: Blink_Edit_Parameter(120, 60, "RATE_4000SPS", edit_param, 1); break;
		case 12: Blink_Edit_Parameter(120, 60, "RATE_8000SPS", edit_param, 1); break;
		case 13: Blink_Edit_Parameter(120, 60, "RATE_16000SPS", edit_param, 1); break;
		case 14: Blink_Edit_Parameter(120, 60, "RATE_32000SPS", edit_param, 1); break;
		case 15: Blink_Edit_Parameter(120, 60, "RATE_64000SPS", edit_param, 1); break;
	}

	// Calibration
	u8g2_DrawUTF8(&u8g2, 10, 80, "CAL:");
	switch(adc_calibration_mode)
	{
		case 0: Blink_Edit_Parameter(120, 80, "SELF", edit_param, 2); break;
		case 1: Blink_Edit_Parameter(120, 80, "OFFSET", edit_param, 2); break;
		case 2: Blink_Edit_Parameter(120, 80, "FULL SCALE", edit_param, 2); break;
	}

	// Window size
	u8g2_DrawUTF8(&u8g2, 10, 100, "SIZE:");
	sprintf(RAM,"%d", ADC_Buff_size);
	Blink_Edit_Parameter(120, 100, RAM, edit_param, 3);

	// Period
	u8g2_DrawUTF8(&u8g2, 10, 120, "OFF PERIOD:");
	sprintf(RAM,"%d", AutoOffCoef_period);
	Blink_Edit_Parameter(120, 120, RAM, edit_param, 4);

	// Period
	u8g2_DrawUTF8(&u8g2, 10, 140, "GAIN PERIOD:");
	sprintf(RAM,"%d", AutoGainCoef_period);
	Blink_Edit_Parameter(160, 140, RAM, edit_param, 5);


	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
}




// DOWN
void ADC_options_screen_S6(void)
{
	if(edit_param<(max_edit_param-1))edit_param++;
}

// UP
void ADC_options_screen_S7(void)
{
	if(edit_param>0)edit_param--;
}


// MENU
void ADC_options_screen_S9(void)
{
	screen=Parameter_screen;
	old_active_string=0xFF;
	Menu_num=5;
}





// ENTER
void ADC_options_screen_S4(void)
{
	switch(edit_param)
	{
		case 0: Save_Settings(PGA_GAIN_tag, PGA_GAIN); break;
		case 1: Save_Settings(ADC_SPS_tag, ADC_SPS); break;
		case 2: Save_Settings(adc_calibration_mode_tag, adc_calibration_mode); adc_start_calibration=1; break;
		case 3: Save_Settings(ADC_Buff_size_tag, ADC_Buff_size); break;
		case 4: Save_Settings(AutoOffCoef_period_tag, AutoOffCoef_period); break;
		case 5: Save_Settings(AutoGainCoef_period_tag, AutoGainCoef_period); break;
	}
}



// BACKWARD
void ADC_options_screen_S8(void)
{
	switch(edit_param)
	{
		case 0: if(PGA_GAIN>0)PGA_GAIN--;
		ADC_PGA_Set(PGA_GAIN);
		ADC_Conversion(ADC_SPS);
		break;
		case 1: if(ADC_SPS>0)ADC_SPS--; break;
		case 2: if(adc_calibration_mode>0)adc_calibration_mode--; break;
		case 3: if(ADC_Buff_size>1)ADC_Buff_size--; break;
		case 4: if(AutoOffCoef_period>5)AutoOffCoef_period--; break;
		case 5: if(AutoGainCoef_period>5)AutoGainCoef_period--; break;
	}
}


// FORWARD
void ADC_options_screen_S5(void)
{
	switch(edit_param)
	{
		case 0: if(PGA_GAIN<8)PGA_GAIN++;
		ADC_PGA_Set(PGA_GAIN);
		ADC_Conversion(ADC_SPS);
		break;
		case 1: if(ADC_SPS<15)ADC_SPS++; break;
		case 2: if(adc_calibration_mode<2)adc_calibration_mode++; break;
		case 3: if(ADC_Buff_size<ADC_BUFF_SIZE)ADC_Buff_size++; break;
		case 4: if(AutoOffCoef_period<ADC_BUFF_SIZE)AutoOffCoef_period++; break;
		case 5: if(AutoGainCoef_period<ADC_BUFF_SIZE*100)AutoGainCoef_period++; break;
	}
}


