
#include "ADC_screen.h"
#include "u8g2_LCD.h"
#include "stdio.h"
extern u8g2_t u8g2;

#define max_edit_param 6

const char* SPS_name[16]={ "RATE_2SPS", "RATE_4SPS", "RATE_8SPS", "RATE_16SPS", "RATE_32SPS", "RATE_63SPS", "RATE_125SPS", "RATE_250SPS",
		"RATE_500SPS", "RATE_1000SPS", "RATE_2000SPS", "RATE_4000SPS", "RATE_8000SPS", "RATE_16000SPS", "RATE_32000SPS", "RATE_64000SPS"};

void ADC_options_screen_func(void)
{
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	// PGA
	u8g2_DrawUTF8(&u8g2, 10, 40, "PGA:");
	Blink_Edit_Parameter(120, 40, PGA_name[PGA_GAIN], edit_param, 0);

	// SPS
	u8g2_DrawUTF8(&u8g2, 10, 60, "SPS:");
	Blink_Edit_Parameter(120, 60,SPS_name[ADC_SPS], edit_param, 1);

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
	Blink_Edit_Parameter(160, 120, RAM, edit_param, 4);

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


