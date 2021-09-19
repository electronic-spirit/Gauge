#include "logging_fatfs.h"
#include "fatfs.h"


#define file_size	2000			// Размер файла
uint8_t ReadFileBuf[file_size];

extern char USERPath[4];	// Logical drive path
FATFS W25FatFs;
FATFS *fs;
FIL MyFile;
UINT testBytes;





const char* TAG_STRING[NUMBER_OF_TAGS]=
{
	[end_of_file_tag]="end_of_file",
	[display_brightness_tag]="display_brightness",
	[display_contrast_tag]=  "display_contrast",
	[mm_peak_tag]="mm_peak",
	[leak_detect_tag]="leak_detect",
	[PV_tag]="PV",
	[PI_tag]="PI",
	[PE_tag]="PE",
	[PGA_GAIN_tag]="PGA_GAIN",
	[ADC_SPS_tag]="ADC_SPS",
	[adc_calibration_mode_tag]="adc_calibration_mode",
	[ADC_Buff_size_tag]="ADC_Buff_size",
	[Auto_Off_Time_tag]="Auto_Off_Time",
	[serial_num_tag]="serial_num",
	[gauge_range_tag]="gauge_range",
	[gauge_error_tag]="gauge_error",
	[light_delay_tag]="light_delay",
	[logging_tag]="logging",
	[logging_start_hour_tag]="logging_start_hour",
	[logging_start_min_tag]="logging_start_min",
	[logging_start_sec_tag]="logging_start_sec",
	[logging_period_tag]="logging_period",
	[logging_len_tag]="logging_len",
	[logging_num_tag]="logging_num",
	[a_current_tag]="a_current",
	[b_current_tag]="b_current",
	[c_current_tag]="c_current",
	[a_voltage_tag]="a_voltage",
	[b_voltage_tag]="b_voltage",
	[c_voltage_tag]="c_voltage",
	[AutoAmpCoef_tag]="AutoAmpCoef",
	[AutoOffCoef_period_tag]="AutoOffCoef_period",
	[RefVoltage_tag]="RefVoltage",
	[AutoGainCoef_period_tag]="AutoGainCoef_period",
};

const uint8_t TAG_NUM_OF_SYM[NUMBER_OF_TAGS]=
{
	[end_of_file_tag]=11,
	[display_brightness_tag]=18,
	[display_contrast_tag]=16,
	[mm_peak_tag]=7,
	[leak_detect_tag]=11,
	[PV_tag]=2,
	[PI_tag]=2,
	[PE_tag]=2,
	[PGA_GAIN_tag]=8,
	[ADC_SPS_tag]=7,
	[adc_calibration_mode_tag]=20,
	[ADC_Buff_size_tag]=13,
	[Auto_Off_Time_tag]=13,
	[serial_num_tag]=10,
	[gauge_range_tag]=11,
	[gauge_error_tag]=11,
	[light_delay_tag]=11,
	[logging_tag]=7,
	[logging_start_hour_tag]=18,
	[logging_start_min_tag]=17,
	[logging_start_sec_tag]=17,
	[logging_period_tag]=14,
	[logging_len_tag]=11,
	[logging_num_tag]=11,
	[a_current_tag]=9,
	[b_current_tag]=9,
	[c_current_tag]=9,
	[a_voltage_tag]=9,
	[b_voltage_tag]=9,
	[c_voltage_tag]=9,
	[AutoAmpCoef_tag]=11,
	[AutoOffCoef_period_tag]=18,
	[RefVoltage_tag]=10,
	[AutoGainCoef_period_tag]=19,
};

void Logging_init(void)
{
	disk_initialize(W25FatFs.drv);
}



void Create_File(uint8_t*FileName, uint8_t NameSize, uint8_t*FileText, uint16_t FileSize)
{
	f_mount(&W25FatFs,(TCHAR const*)USERPath,0);
	f_open(&MyFile, (char*)FileName, FA_WRITE|FA_CREATE_ALWAYS);
	f_write(&MyFile, FileText, FileSize, &testBytes);
	f_close(&MyFile);
}



uint8_t Read_File(uint8_t*FileName, uint8_t NameSize, uint8_t*FileText, uint16_t FileSize)
{
	uint8_t RES;
	f_mount(&W25FatFs,(TCHAR const*)USERPath,0);
	f_open(&MyFile, (char*)FileName, FA_READ|FA_OPEN_ALWAYS);
	RES=f_read(&MyFile, FileText, FileSize, &testBytes);
	f_close(&MyFile);
	return RES;
}

uint16_t FileParser(uint8_t tag)
{
	uint16_t i=0;
	uint16_t RES=0, Sym_cntr=0;

	for(i=0;i<file_size;i++)
	{
		if(ReadFileBuf[i]==TAG_STRING[tag][Sym_cntr])
		{
			for(;Sym_cntr<TAG_NUM_OF_SYM[tag];Sym_cntr++)
			{
				if(ReadFileBuf[i+Sym_cntr]==TAG_STRING[tag][Sym_cntr])
				{RES++;}
			}
			if(RES==TAG_NUM_OF_SYM[tag])
			{RES=i+TAG_NUM_OF_SYM[tag]+1; i=file_size;}
			else {RES=Sym_cntr=0;}
		}
	}

	return RES;
}



void my_itoa(double n, unsigned char s[], unsigned char index, unsigned char quantity)
{
	int i = 0;
	unsigned char arr[20] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
	uint8_t sign=0, before_size, after_size;
	uint32_t before_data, after_data;

	if(n<0){sign=1; n=-n;}
	before_data=(uint32_t)n;
	after_data=(uint32_t)((n-before_data)*10000000.0);

	do { arr[i++] = before_data % 10 + '0'; }
	while ((before_data /= 10) > 0);
	before_size=i;

	if(sign==0)s[index]='-';

	for(i = 0; i < before_size; i++)
	{ s[i+index+sign] = arr[before_size-i-1]; }

	if(after_data>0)
	{
		s[before_size+sign+index]=',';

		do { arr[i++] = after_data % 10 + '0'; }
		while ((after_data /= 10) > 0);
		after_size=i;

		for(i = 0; i < after_size; i++)
		{ s[i+before_size+sign+index+1] = arr[after_size-i-1]; }
	}
}


double scan_double(uint16_t offset, uint8_t *buff)
{
	uint64_t res64=0;
	uint16_t i=0, point=0, sign=0, after_size=0;
	double RES=0;

	if(buff[offset]=='-')sign=1;

	// Find point
	for(i=0;i<30;i++)
	{
		if((buff[i+sign+offset]==',')||(buff[i+sign+offset]=='.')) point=i;
		if((buff[i+sign+offset]==0x0D)||(buff[i+sign+offset]==0x0A)) {after_size=i; i=30;}
	}

	if(after_size>19)after_size=19;
	for(i=0;i<after_size;i++)
	{
		if((buff[i+sign+offset]==0x0D)||(buff[i+sign+offset]==0x0A)) break;
		if((i!=point)||(point==0))
		{
			res64*=10;
			res64+=buff[i+sign+offset]-48;
		}
	}

	if(point!=0){after_size-=1;
	RES = (double)res64;
	for(i=0;i<(after_size-point);i++)RES *= 0.1;}
	else RES = (double)res64;
	if(sign==1)RES=-RES;
	return RES;
}

void Save_Settings(uint8_t TAG, int32_t data)
{
	uint8_t Name[11]="CONFIG.CSV\0";
	uint16_t index, i, end_index;
	while(Read_File(Name, 11, ReadFileBuf, file_size)!=FR_OK){};
	//{
		index=FileParser(TAG);
		for(i=index;i<file_size-1;i++)
		{
			if((ReadFileBuf[i]==0x0D)&&(ReadFileBuf[i+1]==0x0A))
			{
				end_index=i+2;
				i=file_size-1;
			}
		}

		for(i=(index-TAG_NUM_OF_SYM[TAG]-1); i<file_size-end_index; i++)
		{
			ReadFileBuf[i]=ReadFileBuf[i+end_index-(index-TAG_NUM_OF_SYM[TAG]-1)];
		}

		index=FileParser(end_of_file_tag)-TAG_NUM_OF_SYM[end_of_file_tag]-1;

		for(i=0;i<TAG_NUM_OF_SYM[TAG];i++)ReadFileBuf[i+index]=TAG_STRING[TAG][i];
		sprintf(ReadFileBuf+index+TAG_NUM_OF_SYM[TAG],";%d\r\nend_of_file;\r\n", data);
		ReadFileBuf[file_size-1]="\0";
		Create_File(Name, 11, ReadFileBuf, file_size);
	//}
}



double Read_Variable_From_File(uint8_t name)
{
	return scan_double(FileParser(name), ReadFileBuf);
}



void Load_Settings(void)
{
	uint8_t Name_settings[11]="CONFIG.CSV\0";
	uint8_t Name_calibr[8]="CAL.CSV\0";

	// Читаем конфигурационный файл
	while(Read_File(Name_settings, 11, ReadFileBuf, file_size)!=FR_OK){};
	//{
		gauge_error = Read_Variable_From_File(gauge_error_tag);
		display_brightness = (uint16_t)Read_Variable_From_File(display_brightness_tag);
		display_contrast = (uint16_t)Read_Variable_From_File(display_contrast_tag);
		mm_peak = (uint16_t)Read_Variable_From_File(mm_peak_tag);
		leak_detect = (uint16_t)Read_Variable_From_File(leak_detect_tag);
		PV = (uint16_t)Read_Variable_From_File(PV_tag);
		PI = (uint16_t)Read_Variable_From_File(PI_tag);
		PE = (uint16_t)Read_Variable_From_File(PE_tag);
		PGA_GAIN = (uint16_t)Read_Variable_From_File(PGA_GAIN_tag);
		ADC_SPS = (uint16_t)Read_Variable_From_File(ADC_SPS_tag);
		adc_calibration_mode = (uint16_t)Read_Variable_From_File(adc_calibration_mode_tag);
		ADC_Buff_size = (uint16_t)Read_Variable_From_File(ADC_Buff_size_tag);
		Auto_Off_Time = (uint16_t)Read_Variable_From_File(Auto_Off_Time_tag);
		serial_num = (uint16_t)Read_Variable_From_File(serial_num_tag);
		gauge_range = (uint16_t)Read_Variable_From_File(gauge_range_tag);
		light_delay = (uint16_t)Read_Variable_From_File(light_delay_tag);
		logging = (uint16_t)Read_Variable_From_File(logging_tag);
		logging_start_hour = (uint16_t)Read_Variable_From_File(logging_start_hour_tag);
		logging_start_min = (uint16_t)Read_Variable_From_File(logging_start_min_tag);
		logging_start_sec = (uint16_t)Read_Variable_From_File(logging_start_sec_tag);
		logging_period = (uint16_t)Read_Variable_From_File(logging_period_tag);
		logging_len = (uint16_t)Read_Variable_From_File(logging_len_tag);
		logging_num = (uint16_t)Read_Variable_From_File(logging_num_tag);
		AutoAmpCoef=(uint8_t)Read_Variable_From_File(AutoAmpCoef_tag);
		AutoOffCoef_period=(uint16_t)Read_Variable_From_File(AutoOffCoef_period_tag);
		AutoGainCoef_period=(uint16_t)Read_Variable_From_File(AutoGainCoef_period_tag);

	//}

		// Читаем калибровочный файл
		while(Read_File(Name_calibr, 8, ReadFileBuf, file_size)!=FR_OK){};
		a_current=Read_Variable_From_File(a_current_tag);
		b_current=Read_Variable_From_File(b_current_tag);
		c_current=Read_Variable_From_File(c_current_tag);
		a_voltage=Read_Variable_From_File(a_voltage_tag);
		b_voltage=Read_Variable_From_File(b_voltage_tag);
		c_voltage=Read_Variable_From_File(c_voltage_tag);
		RefVoltage=Read_Variable_From_File(RefVoltage_tag);


//		my_itoa(123456.654321, RAM, 0, 20);
//		my_itoa(-654321.123456, RAM, 0, 20);
//		my_itoa(555555, RAM, 0, 20);
//		my_itoa(-444444, RAM, 0, 20);
//		RAM[0]=0;
}

