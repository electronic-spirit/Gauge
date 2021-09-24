
#include "Usart_Parser.h"


#define NUMBER_OF_COMMANDS 38

uint8_t CSV_format = 0;
extern UART_HandleTypeDef huart1;
uint8_t bluetoothbuf[100];
uint8_t bluetoothcntr=0;

enum Main_Command{
	UNKNOWING_CMD,
	NO_CMD,
	GET_CMD,
	SET_CMD,
	SAVE_CMD,
	GV,
	CSV_ON,
	CSV_OFF,
	HELP,
	SELF_CAL,
	SYS_CAL,
	BL_BRIDGE,
	BL_CONFIG,
	CREATE,
	ID_CMD,
	A_CMD,
	B_CMD,
	FUN_CMD,
	UNITS_CMD,
	RANGE_CMD,
	UNITSXX_CMD,
	FUNXXY_CMD,
	ACCURACYST_CMD,
	ZERO_CMD,
	ZEROV_CMD,
	ZEROI_CMD,
	ZEROX_CMD,
	ZEROVX_CMD,
	ZEROIX_CMD,
	DEFAULT_CMD,
	RANGEMIN_CMD,
	RANGEMAX_CMD,
	TAREON_CMD,
	TAREOFF_CMD,
	TAREX_CMD,
	PEAKMIN_CMD,
	PEAKMAX_CMD,
	PEAKRESET_CMD,
};

const char* COMMAND_STRING[NUMBER_OF_COMMANDS]=
{
	[UNKNOWING_CMD]="ERROR CMD",
	[NO_CMD]="\r\n",
	[GET_CMD]="GET",
	[SET_CMD]="SET",
	[SAVE_CMD]="SAVE",
	[GV]="GV",
	[CSV_ON]="CSV_ON",
	[CSV_OFF]="CSV_OFF",
	[HELP]="HELP",
	[SELF_CAL]="SELF_CAL",
	[SYS_CAL]="SYS_CAL",
	[BL_BRIDGE]="BL_BRIDGE",
	[BL_CONFIG]="BL_CONFIG",
	[CREATE]="CREATE",
	[ID_CMD]="ID?",
	[A_CMD]="A?",
	[B_CMD]="B?",
	[FUN_CMD]="FUN?",
	[UNITS_CMD]="UNITS?",
	[RANGE_CMD]="RANGE?",
	[UNITSXX_CMD]="UNITS ",
	[FUNXXY_CMD]="FUN_",
	[ACCURACYST_CMD]="ACCURACYST?",
	[ZERO_CMD]="ZERO",
	[ZEROV_CMD]="ZEROV",
	[ZEROI_CMD]="ZEROI",
	[ZEROX_CMD]="ZERO?",
	[ZEROVX_CMD]="ZEROV?",
	[ZEROIX_CMD]="ZEROI?",
	[DEFAULT_CMD]="DEFAULT",
	[RANGEMIN_CMD]="RANGEMIN?",
	[RANGEMAX_CMD]="RANGEMAX?",
	[TAREON_CMD]="TAREON",
	[TAREOFF_CMD]="TAREOFF",
	[TAREX_CMD]="TARE?",
	[PEAKMIN_CMD]="PEAKMIN?",
	[PEAKMAX_CMD]="PEAKMAX?",
	[PEAKRESET_CMD]="PEAKRESET",
};

const uint8_t COMMAND_NUM_OF_SYM[NUMBER_OF_COMMANDS]=
{
	[UNKNOWING_CMD]=9,
	[NO_CMD]=2,
	[GET_CMD]=3,
	[SET_CMD]=3,
	[SAVE_CMD]=4,
	[GV]=2,
	[CSV_ON]=6,
	[CSV_OFF]=6,
	[HELP]=4,
	[SELF_CAL]=8,
	[SYS_CAL]=7,
	[BL_BRIDGE]=9,
	[BL_CONFIG]=9,
	[CREATE]=6,
	[ID_CMD]=3,
	[A_CMD]=2,
	[B_CMD]=2,
	[FUN_CMD]=4,
	[UNITS_CMD]=6,
	[RANGE_CMD]=6,
	[UNITSXX_CMD]=6,
	[FUNXXY_CMD]=4,
	[ACCURACYST_CMD]=11,
	[ZERO_CMD]=4,
	[ZEROV_CMD]=5,
	[ZEROI_CMD]=5,
	[ZEROX_CMD]=5,
	[ZEROVX_CMD]=6,
	[ZEROIX_CMD]=6,
	[DEFAULT_CMD]=7,
	[RANGEMIN_CMD]=9,
	[RANGEMAX_CMD]=9,
	[TAREON_CMD]=6,
	[TAREOFF_CMD]=7,
	[TAREX_CMD]=5,
	[PEAKMIN_CMD]=8,
	[PEAKMAX_CMD]=8,
	[PEAKRESET_CMD]=9,
};

char rx_buf[APP_RX_DATA_SIZE];
uint16_t rxcntr=0;
int16_t USBTimeOutRX=-1;
uint16_t ANSWER=0xFFFF;
uint8_t BSFlag=0;


void clear_buf(void)
{
	uint16_t i;
	for(i=0;i<APP_RX_DATA_SIZE;i++)rx_buf[i]=' ';
	rxcntr=0;
	ANSWER=0xFFFF;
}

int64_t scan_num32(uint8_t offset)
{
	int64_t res=0;
	uint8_t i=0,sign=0;
	if(rx_buf[offset]=='-')sign=1;
	for(i=0;i<15;i++){
	if((rx_buf[offset+i+sign]>=48)&&(rx_buf[offset+i+sign]<=58))res=(rx_buf[offset+i+sign]-48)+res*10;
	else {if(sign==1)return -res; else return res;}}
	if(sign==1)return -res;
	else return res;
}

float scan_float(uint8_t offset)
{
//	uint32_t res32=0, devider=0;
//	uint8_t i=0, point=0, sign=0;
//	float RES=0;
//	BSFlag=0;
//	if(rx_buf[offset]=='-')sign=1;
//	for(i=0;i<10+point;i++)
//	{
//		if(point==1){if(devider==0)devider=1;else devider*=10;}
//		if((rx_buf[offset+i+sign]>=48)&&(rx_buf[offset+i+sign]<=58))res32=(rx_buf[offset+i+sign]-48)+res32*10;
//		else if((rx_buf[offset+i+sign]==',')||(rx_buf[offset+i+sign]=='.')){ point=1; }
//		else
//		{
//			if(devider==0)devider=1;
//			RES = (float)res32/devider;
//			if(sign==1) return (float)-RES;
//			else return RES;
//		}
//	}
//	BSFlag=1;
//	if(devider==0)devider=1;
//	RES = (float)res32/devider;
//	if(sign==1) return (float)-RES;
//	else return RES;

	return scan_double(offset, (uint8_t*)rx_buf);
}

uint16_t scan_num16(uint8_t offset)
{
	uint8_t res=0,i=0;
	if((rx_buf[offset]=='A')&&(rx_buf[offset+1]=='L')&&(rx_buf[offset+2]=='L'))return 0xFFFF;
	for(i=0;i<10;i++){
	if((rx_buf[offset+i]>=48)&&(rx_buf[offset+i]<=58))res=(rx_buf[offset+i]-48)+res*10;
	else return res;}
	return res;
}

void OneCommParseCycle(uint8_t com, const char* compare, uint8_t offset, uint8_t len)
{
	uint8_t i;
	if(ANSWER==0xFFFF){
	for(i=0;i<len;i++)
	{
		if((rx_buf[i+offset]==compare[i])||(rx_buf[i+offset]==compare[i]+32)) ANSWER = com;
		else {ANSWER=0xFFFF; break;}
	}}
}

uint16_t parser(uint8_t offset)
{
	uint8_t i;
	ANSWER=0xFFFF;
	// Commands
	for(i=2;i<NUMBER_OF_COMMANDS;i++) OneCommParseCycle(i, COMMAND_STRING[i], offset, COMMAND_NUM_OF_SYM[i]);
	// Variables
	for(i=0;i<NUMBER_OF_VARIABLES;i++) OneCommParseCycle(i, GV_STRING[i], offset, GV_NUM_OF_SYM[i]);
	// UNKNOWING_CMD
	if((rx_buf[offset]!=0)&&(ANSWER==0xFFFF))ANSWER=0xFFFE;
	return ANSWER;
}


void Set_uint8(uint8_t*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	uint32_t data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_num32(equal);
	if(data>255)CDC_Transmit_FS((unsigned char*)"Argument Is Too Big!!\r\n", 23);
	else
	{
		arr[index]=data;
		CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
	}
}

void Set_uint16(uint16_t*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	uint32_t data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_num32(equal);
	if(data>65535)CDC_Transmit_FS((unsigned char*)"Argument Is Too Big!!\r\n", 23);
	else
	{
		arr[index]=data;
		CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
	}
}

void Set_int8(int8_t*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	uint32_t data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_num32(equal);
	if((data>127)||(data<-127))CDC_Transmit_FS((unsigned char*)"Argument Is Too Big!!\r\n", 23);
	else
	{
		arr[index]=data;
		CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
	}
}

void Set_int16(int16_t*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	uint32_t data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_num32(equal);
	if((data>32767)||(data<-32767))CDC_Transmit_FS((unsigned char*)"Argument Is Too Big!!\r\n", 23);
	else
	{
		arr[index]=data;
		CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
	}
}

void Set_int32(int32_t*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	int64_t data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_num32(equal);
	if((data>2147483647)||(data<-2147483647))CDC_Transmit_FS((unsigned char*)"Argument Is Too Big!!\r\n", 23);
	else
	{
		arr[index]=(int32_t)data;
		CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
	}

//	arr[index]=data;
//	CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
}

void Set_uint32(uint32_t*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	int32_t data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_num32(equal);
	arr[index]=data;
	CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);
}

void Set_float(float*arr, uint8_t offset)
{
	uint16_t index,i,equal;
	float data;
	index=scan_num16(offset);
	for(i=0;i<100;i++) if((rx_buf[offset+i]==' ')||(rx_buf[offset+i]=='=')){equal=offset+i+1;i=100;}
	data=scan_float(equal);
	if(!BSFlag){
	arr[index]=data;
	CDC_Transmit_FS((unsigned char*)"OK\r\n", 4);}
	else CDC_Transmit_FS((unsigned char*)"Argument Is Too Big!!\r\n", 23);
}





void Get_uint8(uint8_t*arr, uint8_t offset, uint16_t size)
{
	uint16_t i = 0,index;
	index=scan_num16(offset);
	// Get All
	if(index==0xFFFF)
	{
		CDC_Transmit_FS((unsigned char*)"Get All Array\n", 14);
		for(i=0;i<size;i++)
		{
			if(!CSV_format)sprintf(RAM, "%d\t", arr[i]);
			else sprintf(RAM, "%d;", arr[i]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
		CDC_Transmit_FS((unsigned char*)"\r\n", 2);
	}
	// Get one
	else
	{
		if(index>size-1)CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
		else
		{
			sprintf(RAM, "%d\r\n", arr[index]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
	}
}


void Get_int32(int32_t*arr, uint8_t offset, uint16_t size)
{
	uint16_t i = 0,index;
	index=scan_num16(offset);
	// Get All
	if(index==0xFFFF)
	{
		CDC_Transmit_FS((unsigned char*)"Get All Array\n", 14);
		for(i=0;i<size;i++)
		{
			if(!CSV_format)sprintf(RAM, "%i\t", arr[i]);
			else sprintf(RAM, "%i;", arr[i]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
		CDC_Transmit_FS((unsigned char*)"\r\n", 2);
	}
	// Get one
	else
	{
		if(index>size-1)CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
		else
		{
			sprintf(RAM, "%i\r\n", arr[index]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
	}
}

void Get_float(float*arr, uint8_t offset, uint16_t size)
{
	uint16_t i = 0,index;
	index=scan_num16(offset);
	// Get All
	if(index==0xFFFF)
	{
		CDC_Transmit_FS((unsigned char*)"Get All Array\n", 14);
		for(i=0;i<size;i++)
		{
			if(!CSV_format)sprintf(RAM, "%f\t", arr[i]);
			else sprintf(RAM, "%f;", arr[i]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
		CDC_Transmit_FS((unsigned char*)"\r\n", 2);
	}
	// Get one
	else
	{
		if(index>size-1)
		{
			CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
		}
		else
		{
			sprintf(RAM, "%f\r\n", (double)arr[index]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
	}
}


void Get_double(double*arr, uint8_t offset, uint16_t size)
{
	uint16_t i = 0,index;
	index=scan_num16(offset);
	// Get All
	if(index==0xFFFF)
	{
		CDC_Transmit_FS((unsigned char*)"Get All Array\n", 14);
		for(i=0;i<size;i++)
		{
			if(!CSV_format)sprintf(RAM, "%f\t", arr[i]);
			else sprintf(RAM, "%f;", (double)arr[i]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
		CDC_Transmit_FS((unsigned char*)"\r\n", 2);
	}
	// Get one
	else
	{
		if(index>size-1)
		{
			CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
		}
		else
		{
			sprintf(RAM, "%f\r\n", (double)arr[index]);
			CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
		}
	}
}


uint16_t Scan_Uint16(uint8_t ofset, uint8_t*data)
{
	uint16_t i=0;
	uint16_t result=0;

	for(i=ofset;i<ofset+10;i++)
	{
		if((data[i]>=48)&&(data[i]<=58))
		{
			result*=10;
			result+=(data[i]-48);
		}
		else return result;
	}
	return result;
}



void Parser_process(void)
{
	uint16_t RES=0,i;

	if(BLE_bridge!=0)
	{
		if(usart1_counter!=0)
		{
			if(rx_cmplt==0)
			{
				CDC_Transmit_FS((unsigned char*)usart1_data, usart1_counter);
				usart1_counter=0;
				rx_cmplt=-1;
			}
		}
	}

	if(USBTimeOutRX>0)USBTimeOutRX--;
	if(USBTimeOutRX==0)
	{
		// Bluetooth bridge
		if(BLE_bridge!=0)
		{
			for(i=0;i<rxcntr;i++)bluetoothbuf[i]=rx_buf[i];
			bluetoothcntr=rxcntr;
			HAL_UART_Transmit(&huart1, (uint8_t*)rx_buf, rxcntr, 1000);
			clear_buf();
			USBTimeOutRX=-1;
		}
		// Parser
		else
		{
			LED(0,1,0);
			switch(parser(0))
			{
				case 0xFFFF: break;
				case 0xFFFE: CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], (uint8_t)COMMAND_NUM_OF_SYM[UNKNOWING_CMD]); clear_buf(); break;

				// Информация о модели
				case ID_CMD: CDC_Transmit_FS((unsigned char*)"METROL 110 USB", 14); clear_buf(); break;
				// Получить текущее значение давления
				case A_CMD: sprintf((char*)RAM, "%f ", Pressure);
				CDC_Transmit_FS((unsigned char*)RAM, strlen((char*)RAM));
				CDC_Transmit_FS((unsigned char*)UNITS_NAME[Units], strlen(UNITS_NAME[Units]));
				clear_buf(); break;
				// Получить текущее значение электрического параметра
				case B_CMD:
				switch(E_fun)
				{
					case mA: sprintf((char*)RAM, "%f ", current_4_20mA-Current_zero); break;
					case V: sprintf((char*)RAM, "%f ", voltage_measure-Voltage_zero); break;
					case SW: sprintf((char*)RAM, "%d ", SW_state); break;
				}
				CDC_Transmit_FS((unsigned char*)RAM, strlen((char*)RAM));
				CDC_Transmit_FS((unsigned char*)E_UNITS_NAME[E_fun], strlen(E_UNITS_NAME[E_fun]));
				clear_buf(); break;
				// Получить наименование электрического параметра
				case FUN_CMD:
					CDC_Transmit_FS((unsigned char*)E_UNITS_NAME[E_fun], strlen(E_UNITS_NAME[E_fun]));
					clear_buf(); break;
				// Текущая единица измерения
				case UNITS_CMD:
					CDC_Transmit_FS((unsigned char*)UNITS_NAME[Units], strlen(UNITS_NAME[Units]));
					clear_buf(); break;
				// Получить количество диапазонов измерения давления и значения этих диапазонов
				case RANGE_CMD: clear_buf(); break;
				// Смена единицы измерения давления
				case UNITSXX_CMD: RES=Scan_Uint16(COMMAND_NUM_OF_SYM[UNITSXX_CMD], (uint8_t*)rx_buf);
				if(RES<11) {Units=RES; CDC_Transmit_FS((unsigned char*)"OK", 2);}
				else CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], (uint8_t)COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
				clear_buf(); break;
				// Смена функции измерения напряжения, тока, контроля внешних контактов
				case FUNXXY_CMD: clear_buf(); break;
				// Класс точности манометра
				case ACCURACYST_CMD: clear_buf(); break;
				// Обнуление показаний давления
				case ZERO_CMD: clear_buf(); break;
				// Обнуление показаний напряжения
				case ZEROV_CMD: clear_buf(); break;
				// Обнуление показаний тока
				case ZEROI_CMD: clear_buf(); break;
				// Показать смещение нуля давления
				case ZEROX_CMD: clear_buf(); break;
				// Показать смещение нуля напряжения
				case ZEROVX_CMD: clear_buf(); break;
				// Показать смещение нуля тока
				case ZEROIX_CMD: clear_buf(); break;
				// Сброс на заводские настройки
				case DEFAULT_CMD: clear_buf(); break;
				// Нижний предел измерения давления
				case RANGEMIN_CMD: clear_buf(); break;
				// Верхний предел измерения давления
				case RANGEMAX_CMD: clear_buf(); break;
				// Включение функции относительного измерения давления
				case TAREON_CMD: clear_buf(); break;
				// Выключение функции относительного измерения давления
				case TAREOFF_CMD: clear_buf(); break;
				// Получить значение точки отсчета давления для функции TARE
				case TAREX_CMD: clear_buf(); break;
				// Получить минимальное измеренное значение давления за сеанс
				case PEAKMIN_CMD: clear_buf(); break;
				// Получить максимальное измеренное значение давления за сеанс
				case PEAKMAX_CMD: clear_buf(); break;
				// Сбросить макс. и мин. значения давления за сеанс
				case PEAKRESET_CMD: clear_buf(); break;

				case BL_BRIDGE:
					CDC_Transmit_FS((unsigned char*)"BlueTooth Bridge Enabled", 24);
					BLE_bridge=1;
					usart1_counter=0;
					rx_cmplt=-1;
					clear_buf();
					break;

				case CREATE:

					clear_buf();
					break;

				case BL_CONFIG:
					if(BLE_config==1)BLE_config=0;
					else BLE_config=1;
					BLueTooth_Configuration(BLE_config);
					sprintf(RAM, "BlueTooth Config = %d", BLE_config);
					CDC_Transmit_FS((unsigned char*)RAM, 20);
					HAL_UART_Transmit(&huart1, (unsigned char*)RAM, 20, 1000);
					clear_buf();
					break;

				case SELF_CAL:
					clear_buf();
					adc_calibration_mode=self_calibration;
					adc_start_calibration=1;
					CDC_Transmit_FS((unsigned char*)"OK", 2);
				break;
				case SYS_CAL:
					RES=screen;
					Auto_System_Calibration();
						ADC_Conversion(ADC_SPS);
						LED(0,0,0);
						screen=RES;
					clear_buf();
					CDC_Transmit_FS((unsigned char*)"OK", 2);
					break;

				case CSV_ON: CSV_format=1; clear_buf(); CDC_Transmit_FS((unsigned char*)"CSV enabled", 11); HAL_UART_Transmit(&huart1, (unsigned char*)"CSV enabled", 11, 1000); break;
				case CSV_OFF: CSV_format=0; clear_buf(); CDC_Transmit_FS((unsigned char*)"CSV disabled", 12); HAL_UART_Transmit(&huart1, (unsigned char*)"CSV disabled", 12, 1000); break;

				case GV:
					if(CSV_format==0)
					{
						CDC_Transmit_FS((unsigned char*)"Available Global Variables:\n", 28);
						for(i=0;i<NUMBER_OF_VARIABLES;i++)
						{
							CDC_Transmit_FS((unsigned char*)TYPE_STRING[GV_TYPE[i]], 11);
							CDC_Transmit_FS((unsigned char*)"\t\t", 1);
							sprintf(RAM, "%i ", GV_LENGTH[i]);
							CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
							CDC_Transmit_FS((unsigned char*)"\t\t", 1);
							CDC_Transmit_FS((unsigned char*)GV_STRING[i], GV_NUM_OF_SYM[i]);
							CDC_Transmit_FS((unsigned char*)"\n", 1);
						}
					}
					else
					{
						for(i=0;i<NUMBER_OF_VARIABLES;i++)
						{
							CDC_Transmit_FS((unsigned char*)TYPE_STRING[GV_TYPE[i]], 11);
							CDC_Transmit_FS((unsigned char*)";", 1);
							sprintf(RAM, "%i ", GV_LENGTH[i]);
							CDC_Transmit_FS((unsigned char*)RAM, strlen(RAM));
							CDC_Transmit_FS((unsigned char*)";", 1);
							CDC_Transmit_FS((unsigned char*)GV_STRING[i], GV_NUM_OF_SYM[i]);
							CDC_Transmit_FS((unsigned char*)"\n", 1);
						}
					}

					CDC_Transmit_FS((unsigned char*)"\r\n", 1);
					clear_buf();
					break;

				case GET_CMD:
					RES=parser(COMMAND_NUM_OF_SYM[GET_CMD]+1);
					if(RES<0xFFFE)
					{
						switch(GV_TYPE[RES])
						{
							case type_INT8: Get_uint8((uint8_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2, GV_LENGTH[RES]); break;
							case type_INT16: break;
							case type_INT32: Get_int32((int32_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2, GV_LENGTH[RES]); break;
							case type_UINT8: Get_uint8((uint8_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2, GV_LENGTH[RES]); break;
							case type_UINT16: break;
							case type_UINT32: Get_int32((int32_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2, GV_LENGTH[RES]); break;
							case type_DOUBLE: Get_double((double*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2, GV_LENGTH[RES]); break;
							case type_FLOAT:  Get_float((float*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2, GV_LENGTH[RES]); break;
							default: CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]); clear_buf(); break;
						}
					}
					else
					{
						CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
					}
					clear_buf(); break;


				case SET_CMD:
					RES=parser(COMMAND_NUM_OF_SYM[GET_CMD]+1);
					if(RES<0xFFFE)
					{
						switch(GV_TYPE[RES])
						{
							case type_INT8: Set_int8((int8_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2); break;
							case type_INT16: Set_int16((int16_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2); break;
							case type_INT32: Set_int32((int32_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2); break;
							case type_UINT8: Set_uint8((uint8_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2); break;
							case type_UINT16: break;
							case type_UINT32: Set_uint32((uint32_t*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2); break;
							case type_DOUBLE: break;
							case type_FLOAT: Set_float((float*)GV_LINKS[RES], COMMAND_NUM_OF_SYM[GET_CMD]+GV_NUM_OF_SYM[RES]+2); break;
							default: CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]); clear_buf(); break;
						}
					}
					else
					{
						CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]);
					}
					clear_buf(); break;


				case SAVE_CMD:
					CDC_Transmit_FS((unsigned char*)"FLASH MEMORY IS BROKEN!!", 24);
					clear_buf(); break;

				default: CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]); clear_buf(); break;

			}
			CDC_Transmit_FS((unsigned char*)"\r\n", 2);
			USBTimeOutRX=-1;
		}
	}
	sleep(5);
}








