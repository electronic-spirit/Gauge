
#include "Usart_Parser.h"


#define NUMBER_OF_COMMANDS 14

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
};

char rx_buf[APP_RX_DATA_SIZE];
uint16_t rxcntr=0;
int16_t USBTimeOutRX=-1;
uint16_t ANSWER=0xFFFF;
uint8_t BSFlag=0;


void clear_buf(void)
{
	uint16_t i;
	for(i=0;i<APP_RX_DATA_SIZE;i++)rx_buf[i]=0;
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
		if(rx_buf[i+offset]==compare[i]) ANSWER = com;
		else {ANSWER=0xFFFF; break;}
	}}
}

uint8_t parser(uint8_t offset)
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




const char CSV_ON_instruction[]="CSV_ON\t\tВключить формат выходных данных в формате CSV\n";
const char CSV_OFF_instruction[]="CSV_OFF\t\tВыключить формат выходных данных в формате CSV\n";
const char HELP_instruction[]="HELP\t\tВыдать это сообщение\n";
const char GV_instruction[]="GV\t\tВыдать таблицу с доступными переменными\n";
const char SET_instruction[]="SET name X\tУстановить значение Х переменной с именем name\n";
const char GET_instruction[]="GET name X\tПолучить значение переменной name с индексом X\n";
const char ALL_instruction[]="GET name ALL\tПолучить все значения переменной name\n";
const char SAVE_instruction[]="SAVE name\tСохранить значение переменной name в память\n";




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
				case 0xFFFE: CDC_Transmit_FS((unsigned char*)COMMAND_STRING[UNKNOWING_CMD], COMMAND_NUM_OF_SYM[UNKNOWING_CMD]); clear_buf(); break;

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

				case HELP:
					CDC_Transmit_FS((unsigned char*)CSV_ON_instruction, strlen(CSV_ON_instruction));
					CDC_Transmit_FS((unsigned char*)CSV_OFF_instruction, strlen(CSV_OFF_instruction));
					CDC_Transmit_FS((unsigned char*)HELP_instruction, strlen(HELP_instruction));
					CDC_Transmit_FS((unsigned char*)GV_instruction, strlen(GV_instruction));
					CDC_Transmit_FS((unsigned char*)SET_instruction, strlen(SET_instruction));
					CDC_Transmit_FS((unsigned char*)GET_instruction, strlen(GET_instruction));
					CDC_Transmit_FS((unsigned char*)ALL_instruction, strlen(ALL_instruction));
					CDC_Transmit_FS((unsigned char*)SAVE_instruction, strlen(SAVE_instruction));

					clear_buf(); break;

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








