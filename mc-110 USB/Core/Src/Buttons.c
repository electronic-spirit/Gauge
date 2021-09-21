
#include "Buttons.h"


uint8_t Button_State = 0;
extern I2C_HandleTypeDef hi2c1;
int16_t OnOffCntr=on_off_time*2;
uint16_t buttons_press=0;
uint16_t buttons_ms=0;
uint16_t keypad_code = 0;

//--------------------------------------------------------------------------------------------------------
//									Инициализация расширителя
//--------------------------------------------------------------------------------------------------------

void Buttons_init(void)
{
	uint8_t tx_buf[2], rx_buf[2];

	// Reset
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

	tx_buf[0] = PI4IOE5V6416_REG_O_STT1; // wr to out-reg  = 0x18
	tx_buf[1] = 0x18;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, &tx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_O_STT0; // rd from out-reg port0
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, &tx_buf[0], 0x01, 1000);
	HAL_I2C_Master_Receive(&hi2c1, PI4IOE5V6416_RD, &rx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_IO_DIR1; // set pin dir to be out
	tx_buf[1] = 0x07;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, (uint8_t *)&tx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_I_PL_UD0; // pull-up port0
	tx_buf[1] = 0xFF;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, (uint8_t *)&tx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_I_PL_UD1; // pull-up port1
	tx_buf[1] = 0xFF;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, (uint8_t *)&tx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_I_PL_EN0; // pull-up enable on port0
	tx_buf[1] = 0xFF;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, (uint8_t *)&tx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_I_PL_EN1; // pull-up enable on port1
	tx_buf[1] = 0xFF;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, (uint8_t *)&tx_buf[0], 0x02, 1000);

	tx_buf[0] = PI4IOE5V6416_REG_I_MNTR0; // rd pins state port0
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, &tx_buf[0], 0x01, 1000);

	LED(0,0,0);
}


//--------------------------------------------------------------------------------------------------------
//										Выбор режима работы блютус
//--------------------------------------------------------------------------------------------------------

void BLueTooth_Configuration(uint8_t state)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	uint8_t BlueTooth_State = 0;
	// Читаем порт P1
	aTxBuffer[0] = 0x01;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, aTxBuffer, 1, 1000);
	HAL_I2C_Master_Receive(&hi2c1, PI4IOE5V6416_RD, (uint8_t *)aRxBuffer, 1, 1000);
	BlueTooth_State=aRxBuffer[0];

	// Очищаем P1_3 и P1_4
	BlueTooth_State&=0xE7;
	// Записываем значение config
	BlueTooth_State|=(state<<4);

	// Записываем состояние в порт
	aTxBuffer[0] = PI4IOE5V6416_REG_O_STT1;
	aTxBuffer[1] = BlueTooth_State;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, &aTxBuffer[0], 0x02, 1000);

	delay_ms(500);

	// Reset
	BlueTooth_State|=(0x01<<5);

	// Записываем состояние в порт
	aTxBuffer[0] = PI4IOE5V6416_REG_O_STT1;
	aTxBuffer[1] = BlueTooth_State;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, &aTxBuffer[0], 0x02, 1000);
}


//--------------------------------------------------------------------------------------------------------
//											Управление светодиодом
//--------------------------------------------------------------------------------------------------------

void LED(uint8_t RED, uint8_t GREEN, uint8_t BLUE)
{
	uint8_t color=0;
	uint8_t tx_buf[4];
	color=(RED<<7)+(GREEN<<6)+(BLUE<<5)+0x1F;
	color=~color;
	tx_buf[0] = PI4IOE5V6416_REG_O_STT1; // wr to out-reg  = 0x18
	tx_buf[1] = color;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, &tx_buf[0], 0x02, 10000);
}


//--------------------------------------------------------------------------------------------------------
//										ON / OFF
//--------------------------------------------------------------------------------------------------------

void Button_S1_press_callback(void)
{
	if(OnOffCntr==-1)
	{
		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6)==1)
		{
			old_active_string=0xFF; OnOffCntr=on_off_time; screen=Off_screen;
		}
	}
}


//--------------------------------------------------------------------------------------------------------
//										P zero
//--------------------------------------------------------------------------------------------------------

void Button_S2_press_callback(void)
{
	off_sec=off_min=0;
	light_sec=light_min=0;
	// Таррирование давления
	if(Pressure_zero==0)Pressure_zero=Pressure;
	else Pressure_zero=0;

	// Обнуление АЦП
	if(zero_ADC[0]==0)zero_ADC[0]=ADC_middle[3];
		else zero_ADC[0]=0;

}

//--------------------------------------------------------------------------------------------------------
//											E zero
//--------------------------------------------------------------------------------------------------------

void Button_S3_press_callback(void)
{
	off_sec=off_min=0;
	light_sec=light_min=0;
	// Таррирование электрики
	if(Electric_zero==0)
	switch(E_fun)
	{
		case 0: Electric_zero=Test_Pressure; break;
		case 1: Electric_zero=current_4_20mA; break;
		case 2: Electric_zero=voltage_measure; break;
	}
	else Electric_zero=0;
}


//--------------------------------------------------------------------------------------------------------
//											ENTER
//--------------------------------------------------------------------------------------------------------

void Button_S4_press_callback(void)
{
	 switch(screen)
	 {
	 	 case Parameter_screen: Parameter_screen_S4(); break;
	 	 case Option_screen: Option_screen_S4(); break;
	 	 case RTC_screen: RTC_screen_S4(); break;
	 	 case ADC_options_screen: ADC_options_screen_S4(); break;
	 	 case Display_options_screen: Display_options_screen_S4(); break;
	 }
	 off_sec=off_min=0;
	 light_sec=light_min=0;
}


//--------------------------------------------------------------------------------------------------------
//											FORWARD
//--------------------------------------------------------------------------------------------------------

void Button_S5_press_callback(void)
{
	switch(screen)
	{
		case ADC_options_screen: ADC_options_screen_S5(); break;
		case Display_options_screen: Display_options_screen_S5(); break;
		case Measure_options_screen: Measure_options_screen_S5(); break;
		case RTC_screen: RTC_screen_S5(); break;
		case Auto_Off_option_screen: Auto_Off_option_screen_S5(); break;
		case Logging_options_screen: Logging_options_screen_S5(); break;
	}
	off_sec=off_min=0;
	light_sec=light_min=0;
}



//--------------------------------------------------------------------------------------------------------
//											UP
//--------------------------------------------------------------------------------------------------------

void Button_S6_press_callback(void)
{
	switch(screen)
	{
		case Parameter_screen: Parameter_screen_S6(); break;
		case Option_screen: Option_screen_S6(); break;
		case ADC_options_screen: ADC_options_screen_S6(); break;
		case Display_options_screen: Display_options_screen_S6(); break;
		case Measure_options_screen: Measure_options_screen_S6(); break;
		case RTC_screen: RTC_screen_S6(); break;
		case Logging_options_screen: Logging_options_screen_S6(); break;
	}
	off_sec=off_min=0;
	light_sec=light_min=0;
}

//--------------------------------------------------------------------------------------------------------
//										DOWN
//--------------------------------------------------------------------------------------------------------

void Button_S7_press_callback(void)
{
	switch(screen)
	{
		case Parameter_screen: Parameter_screen_S7(); break;
		case Option_screen: Option_screen_S7(); break;
		case ADC_options_screen: ADC_options_screen_S7(); break;
		case Display_options_screen: Display_options_screen_S7(); break;
		case Measure_options_screen: Measure_options_screen_S7(); break;
		case RTC_screen: RTC_screen_S7(); break;
		case Logging_options_screen: Logging_options_screen_S7(); break;
	}
	off_sec=off_min=0;
	light_sec=light_min=0;
}


//--------------------------------------------------------------------------------------------------------
//											BACKWARD
//--------------------------------------------------------------------------------------------------------

void Button_S8_press_callback(void)
{
	switch(screen)
	{
		case ADC_options_screen: ADC_options_screen_S8(); break;
		case Display_options_screen: Display_options_screen_S8(); break;
		case Measure_options_screen: Measure_options_screen_S8(); break;
		case RTC_screen: RTC_screen_S8(); break;
		case Auto_Off_option_screen: Auto_Off_option_screen_S8(); break;
		case Logging_options_screen: Logging_options_screen_S8(); break;
	}
	off_sec=off_min=0;
	light_sec=light_min=0;
}


//--------------------------------------------------------------------------------------------------------
//											MENU
//--------------------------------------------------------------------------------------------------------

void Button_S9_press_callback(void)
{
	switch(screen)
	{
		case Parameter_screen: Parameter_screen_S9(); break;
		case Option_screen: Option_screen_S9(); break;
		case ADC_options_screen: ADC_options_screen_S9(); break;
		case Display_options_screen: Display_options_screen_S9(); break;
		case Measure_options_screen: Measure_options_screen_S9(); break;
		case Main_screen: Main_screen_S9(); break;
		case RTC_screen: RTC_screen_S9(); break;
		case Auto_Off_option_screen: Auto_Off_option_screen_S9(); break;
		case About_options_screen: About_options_screen_S9(); break;
		case Logging_options_screen: Logging_options_screen_S9(); break;
	}
	off_sec=off_min=0;
	light_sec=light_min=0;
}


//--------------------------------------------------------------------------------------------------------
//											E func
//--------------------------------------------------------------------------------------------------------

void Button_S10_press_callback(void)
{
	off_sec=off_min=0;
	light_sec=light_min=0;
	// Переключение отображения второй строки
	if(++E_fun==3)E_fun=0;
	Electric_zero=0;
}

//--------------------------------------------------------------------------------------------------------
//											P units
//--------------------------------------------------------------------------------------------------------

void Button_S11_press_callback(void)
{
	if(screen==Main_screen) screen=Test_screen;
	else if(screen==Test_screen)screen=ADC_options_screen;
	else screen=Main_screen;

	off_sec=off_min=0;
	light_sec=light_min=0;
}



//--------------------------------------------------------------------------------------------------------
//											Обработчик кнопок
//--------------------------------------------------------------------------------------------------------

void Buttons_process(void)
{
	uint16_t keypad = 0;
	uint8_t aTxBuffer[4], aRxBuffer[4];

	// reset if bug
	aTxBuffer[0] = PI4IOE5V6416_REG_INT_MSK0;
	aRxBuffer[0] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, aTxBuffer, 1, 1000);
	HAL_I2C_Master_Receive(&hi2c1, PI4IOE5V6416_RD, (uint8_t *)aRxBuffer, 1, 1000);
	if(aRxBuffer[0] == 0x00){ERROR_REG|=(1<<KeyPad_err);HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); delay_us(100); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);}

	aTxBuffer[0] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, aTxBuffer, 1, 1000);
	HAL_I2C_Master_Receive(&hi2c1, PI4IOE5V6416_RD, (uint8_t *)aRxBuffer, 1, 1000);
	keypad=aRxBuffer[0];
	keypad<<=8;
	aTxBuffer[0] = 0x01;
	HAL_I2C_Master_Transmit(&hi2c1, PI4IOE5V6416_WR, aTxBuffer, 1, 1000);
	HAL_I2C_Master_Receive(&hi2c1, PI4IOE5V6416_RD, (uint8_t *)aRxBuffer, 1, 1000);
	keypad+=(aRxBuffer[0]&0x07);

	keypad_code=keypad;
	switch(keypad)
	{
		case 64519: if(Button_State==0){Button_State=1; Button_S2_press_callback();} break;
		case 64007: if(Button_State==0){Button_State=1; Button_S3_press_callback();} break;
		case 62983: if(Button_State==0){Button_State=1; Button_S4_press_callback();} break;
		case 60935: if(Button_State==0){Button_State=1; Button_S5_press_callback();} break;
		case 56839: if(Button_State==0){Button_State=1; Button_S6_press_callback();} break;
		case 48647: if(Button_State==0){Button_State=1; Button_S7_press_callback();} break;
		case 32263: if(Button_State==0){Button_State=1; Button_S8_press_callback();} break;
		case 65030: if(Button_State==0){Button_State=1; Button_S9_press_callback();} break;
		case 65029: if(Button_State==0){Button_State=1; Button_S10_press_callback();} break;
		case 65027: if(Button_State==0){Button_State=1; Button_S11_press_callback();} break;
		case 65287: if(Button_State==0){Button_State=1; Button_S1_press_callback();} break; // Выключение
		default: buttons_press=Button_State=0; break;
	}

	if((Button_State==1)&&(buttons_press<20))buttons_press++;

	// Задержка на обработку кнопки включения
	if(OnOffCntr>on_off_time){ if(--OnOffCntr==on_off_time) OnOffCntr=-1; }
	if((OnOffCntr>0)&&(OnOffCntr<=on_off_time)) if(--OnOffCntr==0)
	{
		// Выключение
		TURN_OFF_ADC_POWER();
		delay_ms(200);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EN_LCD_reserv_GPIO_Port, EN_LCD_reserv_Pin, GPIO_PIN_RESET);
	}

	sleep(5);
}

//--------------------------------------------------------------------------------------------------------
//											Таймер долгого нажатия
//--------------------------------------------------------------------------------------------------------

void Buttons_timer(void)
{
	if(buttons_press==20)
	{
		if(++buttons_ms>=1000)
		{
			buttons_ms=1000-100;
			Button_State=0;
		}
	}
	else {buttons_ms=0;}
}




