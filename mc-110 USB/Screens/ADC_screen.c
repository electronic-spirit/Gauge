
#include "ADC_screen.h"
#include "u8g2_LCD.h"
#include "stdio.h"
extern u8g2_t u8g2;

#define max_edit_param 8

const char* SPS_name[16]={ "2SPS", "4SPS", "8SPS", "16SPS", "32SPS", "63SPS", "125SPS", "250SPS",
		"500SPS", "1000SPS", "2000SPS", "4000SPS", "8000SPS", "16000SPS", "32000SPS", "64000SPS"};
const char* CAL_name[4]={"SELF", "OFFSET", "FULL SCALE", "NEW"};

void ADC_options_screen_func(void)
{
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	// PGA 0
	u8g2_DrawUTF8(&u8g2, 10, 40, "PGA 0:");
	Blink_Edit_Parameter(120, 40, PGA_name[PGA_GAIN[0]], edit_param, 0);
	// PGA 1
	u8g2_DrawUTF8(&u8g2, 10, 58, "PGA 1:");
	Blink_Edit_Parameter(120, 58, PGA_name[PGA_GAIN[1]], edit_param, 1);
	// PGA 2
	u8g2_DrawUTF8(&u8g2, 10, 76, "PGA 2:");
	Blink_Edit_Parameter(120, 76, PGA_name[PGA_GAIN[2]], edit_param, 2);
	// PGA 3
	u8g2_DrawUTF8(&u8g2, 10, 94, "PGA 3:");
	Blink_Edit_Parameter(120, 94, PGA_name[PGA_GAIN[3]], edit_param, 3);
	// PGA 4
	u8g2_DrawUTF8(&u8g2, 10, 112, "PGA 4:");
	Blink_Edit_Parameter(120, 112, PGA_name[PGA_GAIN[4]], edit_param, 4);
	// PGA 5
	u8g2_DrawUTF8(&u8g2, 10, 130, "PGA 5:");
	Blink_Edit_Parameter(120, 130, PGA_name[PGA_GAIN[5]], edit_param, 5);
	// SPS
	u8g2_DrawUTF8(&u8g2, 10, 155, "RATE:");
	Blink_Edit_Parameter(65, 155, SPS_name[ADC_SPS], edit_param, 6);
	// Size
	u8g2_DrawUTF8(&u8g2, 155, 155, "Size:");
	sprintf((char*)RAM,"%d", ADC_Buff_size);
	Blink_Edit_Parameter(205, 155, (char*)RAM, edit_param, 7);

	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
}




// DOWN
void ADC_options_screen_S6(void)
{
	if(edit_param<(max_edit_param-1))edit_param++;
	else edit_param=0;
}

// UP
void ADC_options_screen_S7(void)
{
	if(edit_param>0)edit_param--;
	else edit_param=max_edit_param-1;
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
		case 0: Save_Settings(PGA_GAIN0_tag, PGA_GAIN[0]); break;
		case 1: Save_Settings(PGA_GAIN1_tag, PGA_GAIN[1]); break;
		case 2: Save_Settings(PGA_GAIN2_tag, PGA_GAIN[2]); break;
		case 3: Save_Settings(PGA_GAIN3_tag, PGA_GAIN[3]); break;
		case 4: Save_Settings(PGA_GAIN4_tag, PGA_GAIN[4]); break;
		case 5: Save_Settings(PGA_GAIN5_tag, PGA_GAIN[5]); break;
		case 6: Save_Settings(ADC_SPS_tag, ADC_SPS); break;
		case 7: Save_Settings(ADC_Buff_size_tag, ADC_Buff_size); break;
	}
}



// BACKWARD
void ADC_options_screen_S8(void)
{
	switch(edit_param)
	{
		case 0: if(PGA_GAIN[0]>0)PGA_GAIN[0]--; break;
		case 1: if(PGA_GAIN[1]>0)PGA_GAIN[1]--; break;
		case 2: if(PGA_GAIN[2]>0)PGA_GAIN[2]--; break;
		case 3: if(PGA_GAIN[3]>0)PGA_GAIN[3]--; break;
		case 4: if(PGA_GAIN[4]>0)PGA_GAIN[4]--; break;
		case 5: if(PGA_GAIN[5]>0)PGA_GAIN[5]--; break;
		case 6: if(ADC_SPS>0)ADC_SPS--; break;
		case 7: if(ADC_Buff_size>0)ADC_Buff_size--; break;
	}
}


// FORWARD
void ADC_options_screen_S5(void)
{
	switch(edit_param)
	{
		case 0: if(PGA_GAIN[0]<8)PGA_GAIN[0]++; break;
		case 1: if(PGA_GAIN[1]<8)PGA_GAIN[1]++; break;
		case 2: if(PGA_GAIN[2]<8)PGA_GAIN[2]++; break;
		case 3: if(PGA_GAIN[3]<8)PGA_GAIN[3]++; break;
		case 4: if(PGA_GAIN[4]<8)PGA_GAIN[4]++; break;
		case 5: if(PGA_GAIN[5]<8)PGA_GAIN[5]++; break;
		case 6: if(ADC_SPS<15)ADC_SPS++; break;
		case 7: if(ADC_Buff_size<511)ADC_Buff_size++; break;
	}
}


