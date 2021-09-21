
#include "ADC.h"
#include "math.h"
#include "u8g2_LCD.h"

#define adc_timeout_const	5


extern u8g2_t u8g2;
extern SPI_HandleTypeDef hspi1;
uint8_t adc_channel=0;
uint8_t adc_conversion_complete = 1;							// Устанавливается в 1 когда преобразование завершено
uint8_t adc_start_calibration = 0;								// Установить в 1 чтобы начать калибровку
uint16_t adc_timeout_cal=0;										// Время ожидания после калибровки
uint32_t adc_full_buff=0;										// Флаг буфер заполнен
double adc_mux[9]={1.0,1.0,2.0,4.0,8.0,16.0,32.0,64.0,128.0};	// Коррекция GAIN
int32_t adc_system_offset[9]={0,0,0,0,0,0,0,0,0};				// Коррекция смещения
uint8_t adc_timeout=adc_timeout_const;							// Таймаут на АЦП
uint8_t devider = 0;											// Делитель PGA
uint16_t ADC_counter = 0;										// Счетчик кольцевых буферов
uint8_t calibr_process = 0;										// Устанавливается в 1 когда идет калибровка
uint8_t read_off_gain = 0;
uint8_t start_packet=0;

void Measure_Kdiv(void);


uint32_t ADC_Read_REG(uint8_t REG, uint8_t len)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];

	uint32_t RES;
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xC1+((REG)<<1);
	aTxBuffer[1] = 0;
	aTxBuffer[2] = 0;
	aTxBuffer[3] = 0;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, len+1, 1000);
	ADC_NCS_NOT_ACTIVE();

	RES=aRxBuffer[1];
	RES<<=8;
	RES+=aRxBuffer[2];
	RES<<=8;
	RES+=aRxBuffer[3];
	RES = RES>>(8*(3-len));

	return RES;
}


void ADC_Conversion(uint8_t RATE)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];

	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xB0 + RATE; // DATA RATE
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	ADC_NCS_NOT_ACTIVE();
}

int32_t ADC_Read_DATA(uint8_t Channel)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	uint32_t data32;
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xC1+((14+Channel)<<1);
	aTxBuffer[1] = aTxBuffer[2] = aTxBuffer[3] = 0x00;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 4, 1000);
	ADC_NCS_NOT_ACTIVE();
	data32=aRxBuffer[1];
	data32<<=8;
	data32+=aRxBuffer[2];
	data32<<=8;
	data32+=aRxBuffer[3];
	if((data32>>23)==1) return -(8388608-(data32&0x7FFFFF));
	else return (data32&0x7FFFFF);
}

uint32_t ADC_Read_STAT(void)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	uint32_t data32;
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xC1+((STAT_REG)<<1);
	aTxBuffer[1] = aTxBuffer[2] = aTxBuffer[3] = 0x00;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 4, 1000);
	ADC_NCS_NOT_ACTIVE();
	data32=aRxBuffer[1];
	data32<<=8;
	data32+=aRxBuffer[2];
	data32<<=8;
	data32+=aRxBuffer[3];
	return data32;
}

void ADC_Write_REG(uint8_t REG, uint8_t*DATA, uint8_t len)
{
	uint8_t aTxBuffer[10], aRxBuffer[10];
	uint8_t i=0;
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xC0+((REG)<<1);
	for(i=0;i<len;i++)aTxBuffer[i+1] = DATA[i];
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, len+1, 1000);
	ADC_NCS_NOT_ACTIVE();
}


void ADC_Write_long_REG(uint8_t REG, uint32_t DATA)
{
	uint8_t aTxBuffer[10], aRxBuffer[10];
	//uint8_t i=0;
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xC0+((REG)<<1);

	aTxBuffer[1]=(DATA>>16)&0xFF;
	aTxBuffer[2]=(DATA>>8)&0xFF;
	aTxBuffer[3]=(DATA>>0)&0xFF;

	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 4, 1000);
	ADC_NCS_NOT_ACTIVE();
}


void Auto_System_Calibration(void)
{
	uint16_t i,j;
	int32_t DYNAMIC_BAND_MAX[9]={0,0,0,0,0,0,0,0,0};
	int32_t DYNAMIC_BAND_MIN[9]={0,0,0,0,0,0,0,0,0};
	uint8_t RES=0;
	int64_t SUM[6]={0,0,0,0,0,0};

	// Читаем калибровочные данные из внешнего регистра
//	aTxBuffer[0] = 0x10;
//	ADC_Write_REG(CTRL3_REG, aTxBuffer, 1);

	calibr_process=1;
//	for(i=0;i<9;i++)
//	{
//		ADC_PGA_Set(i);
//		ADC_Conversion(ADC_SPS);
//		adc_full_buff=0;
//		while(adc_full_buff<ADC_Buff_size)
//		{
//			Gauge_loading((uint8_t*)"Калибровка АЦП..",  (uint8_t)((((i<<4)|adc_full_buff)*100)/143));
//		}
//
//		// Скользящее среднее
//		for(j=0;j<ADC_Buff_size;j++)
//		{
//			SUM[0]+=ADC_channel_0[j];
//			SUM[1]+=ADC_channel_1[j];
//			SUM[2]+=ADC_channel_2[j];
//			SUM[3]+=ADC_channel_3[j];
//			SUM[4]+=ADC_channel_4[j];
//			SUM[5]+=ADC_channel_5[j];
//		}
//		for(j=0;j<6;j++) ADC_middle[j]=SUM[j]/ADC_Buff_size;
//
//		DYNAMIC_BAND_MAX[i]=ADC_middle[5];
//		DYNAMIC_BAND_MIN[i]=ADC_middle[0];
//	}


	ADC_PGA_Set(PGA_GAIN128);
	ADC_Conversion(ADC_SPS);
	ADC_Buff_size=256;
	adc_full_buff=0;
	while(adc_full_buff<ADC_Buff_size)
	{
		Gauge_loading((uint8_t*)"Калибровка АЦП..",  (uint8_t)((adc_full_buff*100)/256));
	}

//	// Скользящее среднее
//	for(j=0;j<ADC_Buff_size;j++)
//	{
//		SUM[0]+=ADC_channel_5[j];
//	}
//	ADC_middle[5]=SUM[0]/ADC_Buff_size;
//
//	ADC_Buff_size=32;
//	OldData=ADC_middle[5];



	// Расчитываем коэффициенты для SYSTEM калибровки GAIN и OFFSET
	adc_mux[0]=1.0;
	//for(i=1;i<9;i++)adc_mux[i]=(double)(DYNAMIC_BAND_MAX[0]*(1<<(i-1)))/(double)(DYNAMIC_BAND_MAX[i]-DYNAMIC_BAND_MIN[i]);
	for(i=1;i<9;i++)adc_mux[i]=(double)DYNAMIC_BAND_MAX[i]/(double)DYNAMIC_BAND_MAX[1];
	for(i=0;i<9;i++)adc_system_offset[i]=DYNAMIC_BAND_MIN[i];

	// Проверяем правильность показаний усиления
	for(i=2;i<9;i++) {if(DYNAMIC_BAND_MAX[i-1]<DYNAMIC_BAND_MAX[i])RES=1;}
	if(RES==1)ERROR_REG|=(0x01<<ADC_cal_err);

	//OldData=(double)DYNAMIC_BAND_MAX[1];

	calibr_process=0;
}


void ADC_Calibration(uint8_t CAL)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	aTxBuffer[0]=((CAL)<<6)+0x02;
	ADC_Write_REG(CTRL1_REG, aTxBuffer, 1);

	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xA0;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	ADC_NCS_NOT_ACTIVE();
}

void ADC_Start_Conversion(uint8_t channel, uint8_t RATE)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	if(channel>6)channel=6;
	aTxBuffer[0]=channel<<5;
	// Выбираем канал
	ADC_Write_REG(SEQ_REG, aTxBuffer, 1);
	// Запускаем преобразование
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0xB0 + RATE; // DATA RATE
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	ADC_NCS_NOT_ACTIVE();

}



void ADC_init(void)
{
	uint8_t aTxBuffer[4], aRxBuffer[4];
	uint32_t data=0xFFFFFFFF;
	uint16_t i;

	// Reset
	aTxBuffer[0] = 0x30;
	ADC_Write_REG(CTRL1_REG, aTxBuffer, 1);
	ADC_NCS_ACTIVE();
	aTxBuffer[0]=0x90;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, 1, 1000);
	ADC_NCS_NOT_ACTIVE();

	while(i<10000)
	{
		data=ADC_Read_STAT();
		i++;
	}
	if(data!=152) ERROR_REG=(1<<ADC_err);

	// Настройка АЦП
	aTxBuffer[0] = 0x09;
	ADC_Write_REG(SEQ_REG, aTxBuffer, 1);

	aTxBuffer[0] = 0x0E;
	aTxBuffer[1] = 0x0A;
	aTxBuffer[2] = 0x06;
	ADC_Write_REG(CHMAP0_REG, aTxBuffer, 3);

	aTxBuffer[0] = 0x1A;
	aTxBuffer[1] = 0x16;
	aTxBuffer[2] = 0x12;
	ADC_Write_REG(CHMAP1_REG, aTxBuffer, 3);

	aTxBuffer[0] = 0x02;
	ADC_Write_REG(CTRL1_REG, aTxBuffer, 1);

	// Self calibration
	LED(1,0,0);
	aTxBuffer[0] = 0x01;
	ADC_Write_REG(SEQ_REG, aTxBuffer, 1);
	ADC_Calibration(adc_calibration_mode);
	delay_ms(300);
	aTxBuffer[0] = 0x09;
	ADC_Write_REG(SEQ_REG, aTxBuffer, 1);

	ADC_PGA_Set(PGA_GAIN);
	ADC_Conversion(ADC_SPS);

	delay_ms(1500);
	//Auto_System_Calibration();

	ADC_PGA_Set(PGA_GAIN);
	ADC_Conversion(ADC_SPS);

	LED(0,0,0);
}


void ADC_PGA_Set(uint8_t pga_data)
{
	uint8_t aTxBuffer[4];
	uint8_t CTRL=ADC_Read_REG(CTRL2_REG, 1);
	CTRL&=0xF0;
	if(pga_data==0) aTxBuffer[0]=CTRL;
	else aTxBuffer[0]=CTRL+0x08+(pga_data-1);
	ADC_Write_REG(CTRL2_REG, aTxBuffer, 1);
}


void ADC_EXT(void)
{
	uint16_t i=0;

//	if(adc_full_buff>2)
//	{
		if(read_off_gain==0)
		{
			for(i=0;i<6;i++)ADC_data[i]=ADC_Read_DATA(i);

			if(adc_full_buff!=0)
			{
				for(i=0;i<ADC_Buff_size-1;i++)
				{
					ADC_channel_0[i]=ADC_channel_0[i+1];
					ADC_channel_1[i]=ADC_channel_1[i+1];
					ADC_channel_2[i]=ADC_channel_2[i+1];
					ADC_channel_3[i]=ADC_channel_3[i+1];
					ADC_channel_4[i]=ADC_channel_4[i+1];
					ADC_channel_5[i]=ADC_channel_5[i+1];
				}

				ADC_channel_0[ADC_Buff_size-1]=ADC_data[0];
				ADC_channel_1[ADC_Buff_size-1]=ADC_data[1];
				ADC_channel_2[ADC_Buff_size-1]=ADC_data[2];
				ADC_channel_3[ADC_Buff_size-1]=ADC_data[3];
				ADC_channel_4[ADC_Buff_size-1]=ADC_data[4];
				ADC_channel_5[ADC_Buff_size-1]=ADC_data[5];
			}
			adc_full_buff++;
//			else
//			{
////				ADC_channel_0[ADC_Buff_size-1]=12345;
////				ADC_channel_1[ADC_Buff_size-1]=12345;
////				ADC_channel_2[ADC_Buff_size-1]=12345;
////				ADC_channel_3[ADC_Buff_size-1]=12345;
////				ADC_channel_4[ADC_Buff_size-1]=12345;
////				ADC_channel_5[ADC_Buff_size-1]=12345;
////				start_packet=0;
//			}
		}
		else
		{
//			if(read_off_gain==1)
//			{
//				read_off_gain=2;
//			}
//			else
//			{
				for(i=1;i<ADC_Buff_size;i++)
				{ ADC_channel_OFF[i-1]=ADC_channel_OFF[i]; }
				for(i=0;i<6;i++)ADC_data[i]=ADC_Read_DATA(i);
				ADC_channel_OFF[ADC_Buff_size-1]=ADC_data[5];
//			}
		}
//	}

//	if(adc_full_buff<ADC_Buff_size)adc_full_buff++;
//	else{if(cal_ag<10)cal_ag++;}
//	if(cal_ag==10){cal_ag=20; for(i=0;i<6;i++) Kag[i]=(double)CALag[i]/(double)ADC_middle[i];}

	//if(start_packet==0)
		adc_full_buff++;
	//else start_packet=0;

	// Расчитываем Kdiv
	//Kdiv = 128.0/(OldData/(double)ADC_middle[5]);
	if(AutoAmpCoef==1)Measure_Kdiv();
	else Kdiv=devider;

	adc_timeout=adc_timeout_const;
	ADC_Conversion(ADC_SPS);
}


void Measure_Kdiv(void)
{
	// Gain
	if(read_off_gain==0)
	{
		if(adc_full_buff>=(AutoGainCoef_period+1))
		{
			ADC_PGA_Set(PGA_GAIN_OFF);
			//ADC_Conversion(ADC_SPS);
			read_off_gain=1;
			adc_full_buff=0;
			start_packet=1;
		}
	}
	// Gain OFF
	else
	{
		if(adc_full_buff>=(AutoOffCoef_period+1))
		{
			ADC_PGA_Set(PGA_GAIN);
			//ADC_Conversion(ADC_SPS);
			adc_full_buff=0;
			read_off_gain=0;
			start_packet=1;
		}
	}
	Kdiv=(double)ADC_middle[5]/(double)ADC_middle[6];
}


void ADC_process(void)
{
	uint16_t i=0;
	uint8_t aTxBuffer[4];
	int64_t SUM[7]={0,0,0,0,0,0};

	// Скользящее среднее
	for(i=0;i<ADC_Buff_size;i++)
	{
		SUM[0]+=ADC_channel_0[i];
		SUM[1]+=ADC_channel_1[i];
		SUM[2]+=ADC_channel_2[i];
		SUM[3]+=ADC_channel_3[i];
		SUM[4]+=ADC_channel_4[i];
		SUM[5]+=ADC_channel_5[i];
		SUM[6]+=ADC_channel_OFF[i];
	}
	for(i=0;i<7;i++) ADC_middle[i]=SUM[i]/ADC_Buff_size;

	//for(i=0;i<7;i++) ADC_middle[i]*=Kag[i];

	// Перезапуск АЦП если нет ответа в течении adc_timeout_const
	if(adc_timeout>0)adc_timeout--;
	else
	{
		// Настройка АЦП
		aTxBuffer[0] = 0x09;
		ADC_Write_REG(SEQ_REG, aTxBuffer, 1);
		aTxBuffer[0] = 0x0E;
		aTxBuffer[1] = 0x0A;
		aTxBuffer[2] = 0x06;
		ADC_Write_REG(CHMAP0_REG, aTxBuffer, 3);
		aTxBuffer[0] = 0x1A;
		aTxBuffer[1] = 0x16;
		aTxBuffer[2] = 0x12;
		ADC_Write_REG(CHMAP1_REG, aTxBuffer, 3);
		aTxBuffer[0] = 0x02;
		ADC_Write_REG(CTRL1_REG, aTxBuffer, 1);
		// Self calibration
		aTxBuffer[0] = 0x01;
		ADC_Write_REG(SEQ_REG, aTxBuffer, 1);
		ADC_Calibration(adc_calibration_mode);
		delay_ms(300);
		aTxBuffer[0] = 0x09;
		ADC_Write_REG(SEQ_REG, aTxBuffer, 1);

		ADC_Conversion(ADC_SPS);
		adc_timeout=adc_timeout_const;
		ERROR_REG|=(0x01<<ADC_err);
	}


	if(adc_start_calibration==0)
	{
		if(adc_timeout_cal>0)
		{
			adc_timeout_cal--;
			if(adc_timeout_cal==0)
			{
				// Стартуем преобразование
				aTxBuffer[0] = 0x09;
				ADC_Write_REG(SEQ_REG, aTxBuffer, 1);
				ADC_Conversion(DATA_RATE_16SPS);
				LED(0,0,0);
			}
		}
	}
	// Калибровка
	else
	{
		LED(1,0,0);
		adc_start_calibration=0;
		aTxBuffer[0] = 0x01;
		if(adc_calibration_mode==offset_calibration)aTxBuffer[0]+=(0<<5); 		// Ноль это канал по которому производится калибровка
		if(adc_calibration_mode==full_scale_calibration)aTxBuffer[0]+=(5<<5);	// Ноль это канал по которому производится калибровка
		ADC_Write_REG(SEQ_REG, aTxBuffer, 1);
		ADC_Calibration(adc_calibration_mode);
		adc_timeout_cal=50;
	}
	// Делитель для PGA
	if(PGA_GAIN==0)devider=1;
	else devider= (0x01<<(PGA_GAIN-1));


	ADC_volt[0]=ADC_middle[0]*((RefVoltage*1000)/(8388608*Kdiv));
	ADC_volt[1]=ADC_middle[1]*((RefVoltage*1000)/(8388608*Kdiv));
	ADC_volt[2]=ADC_middle[2]*((RefVoltage*1000)/(8388608*Kdiv));
	ADC_volt[3]=ADC_middle[3]*((RefVoltage*1000)/(8388608*Kdiv));
	ADC_volt[4]=ADC_middle[4]*((RefVoltage*1000)/(8388608*Kdiv));
	ADC_volt[5]=ADC_middle[5]*((RefVoltage*1000)/(8388608*Kdiv));

	// Заводской номер 001
	current_4_20mA = calculate_current(ADC_volt[0]);
	voltage_measure = calculate_voltage(ADC_volt[1]);

	Approximation_MC110(ADC_middle[3], ADC_middle[4]);
	Calculate(ADC_middle[3], ADC_middle[4]);

	sleep(100);
}



float HEX2FLOAT(uint32_t hex)
{
	int8_t s=1; // знак
	int8_t e; 	// Порядок
	int32_t m;	// Мантисса
	float m1;

	if((hex>>31)==1)s=-1;
	e = (hex >> 23) & 0xFF;


	if(e!=0) m = ((hex & 0x7FFFFF ) | 0x800000);
	else     m = ((hex & 0x7FFFFF ) << 1);

	m1 = m*0.00000011920928955078;                            // # Мантисса в float
	return s*m1*(pow(2,(e-127)));
}

void HEX2BIN(uint32_t data, uint8_t*text, uint8_t ofset)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		if((data&(1<<i))!=0)text[7-i+ofset]='1';
		else text[7-i+ofset]='0';
	}
	text[ofset+8]='\0';
}



