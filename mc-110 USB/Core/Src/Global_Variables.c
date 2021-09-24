#include "Global_Variables.h"

//--------------------------------------------------------------------------------------------
//										       RAM
//--------------------------------------------------------------------------------------------

int32_t ADC_data[6];				// Мгновенные значения АЦП
double ADC_volt[6];					// Мгновенные значения АЦП в вольтах

// Кольцевые буферы для АЦП
int32_t ADC_channel[7][ADC_BUFF_SIZE];
int32_t ADC_middle[7];

double Test_Pressure;

double Temperature2 = 0;			// Температура сенсора 2
double Pressure;
double Pressure_Ag;
double Pressure_zero=0.0;			// Обнуление давления
double Voltage_zero=0.0;			// Обнуление Напряжения
double Current_zero=0.0;			// Обнуление тока
double Temperature = 0;				// Температура сенсора
double Temperature_coeff_25=1.0;	// Температурный коэффициент (отношение АЦП питающей текщей к АЦП питающей которая должна быть при 25 градусах)
uint16_t Vbat_uint[128];			// Напряжение на батарее
uint32_t ERROR_REG=0;				// Регистр ошибок
uint8_t BLE_bridge = 0;				// Включить режим моста между USB и BlueTooth
uint8_t usart1_data[100];
uint8_t usart1_counter=0;
int8_t rx_cmplt=0;
uint8_t BLE_config = 1;				// Режим работы bluetooth
double MCU_temp = 0;				// Температура кристала
uint8_t E_fun = mA;					// Что отображать во второй строке

double current_4_20mA;
double voltage_measure;
uint8_t SW_state = 0;

int32_t zero_ADC[6];

uint8_t AutoAmpCoef = 0;
uint16_t AutoOffCoef_period;
uint16_t AutoGainCoef_period;
double RefVoltage = 3.29923;
double Kdiv[6];												// Реальный коэффициент усиления


//--------------------------------------------------------------------------------------------
//										       EEPROM
//--------------------------------------------------------------------------------------------

uint8_t display_brightness = 50;
uint8_t display_contrast = 20;

uint8_t mm_peak=0;						// ПИКОВЫЕ ЗНАЧЕНИЯ
uint8_t leak_detect=0;					// ОБНАРУЖЕНИЕ УТЕЧЕК
uint8_t PV=0;							// P+V
uint8_t PI=0;							// P+I
uint8_t PE=0;							// P+E

uint8_t PGA_GAIN[7]={PGA_GAIN_OFF,PGA_GAIN1,PGA_GAIN2,PGA_GAIN4,PGA_GAIN8,PGA_GAIN16,PGA_GAIN_OFF};
uint8_t ADC_SPS = DATA_RATE_16SPS;
uint8_t adc_calibration_mode=self_calibration;		// Один из трех видов калибровки
uint16_t ADC_Buff_size=32;							// Размер окна скользящего среднего
uint16_t Auto_Off_Time = 0;							// Время автоотключения в минутах (Если 0 то не отключается)

uint32_t serial_num=4;								// Серийный номер
uint32_t gauge_range = 0;							// Диапазон
double gauge_error = 0.015;							// Погрешность

uint16_t light_delay = 3;							// Время подсветки
uint8_t logging=0;									// Логгирование да/нет
uint8_t logging_start_hour=0;						// Время старта логгирования час
uint8_t logging_start_min=0;						// Время старта логгирования мин
uint8_t logging_start_sec=0;						// Время старта логгирования сек
uint8_t logging_period = 0;							// Период логгирования
uint8_t logging_len=0;								// Длительность логгирования
uint8_t logging_num=0;								// Количество файлов

uint8_t Sound_Vol=4;
uint8_t Units = kPa;

//------------------------------------------------------------------------------------------------


uint32_t GV_LINKS[NUMBER_OF_VARIABLES]={
	[ADC_data_name]=&ADC_data,
	[ADC_volt_name]=&ADC_volt,
	[ADC_channel_0_name]=&ADC_channel[0],
	[ADC_channel_1_name]=&ADC_channel[1],
	[ADC_channel_2_name]=&ADC_channel[2],
	[ADC_channel_3_name]=&ADC_channel[3],
	[ADC_channel_4_name]=&ADC_channel[4],
	[ADC_channel_5_name]=&ADC_channel[5],
	[Temperature_name]=&Temperature,
	[Pressure_name]=&Pressure,
	[Pressure_Ag_name]=&Pressure_Ag,
	[ADC_middle_name]=&ADC_middle,
	[current_4_20mA_name]=&current_4_20mA,
	[voltage_measure_name]=&voltage_measure,
	[Kdiv_name]=&Kdiv,
	[ADC_channel_OFF_name]=&ADC_channel[6],
	[ERROR_name]=&ERROR_REG,
	[PGA_GAIN_name]=&PGA_GAIN,
};




// Текстовые имена переменных
const char* GV_STRING[NUMBER_OF_VARIABLES]=
{
	[ADC_data_name]="ADC_data",
	[ADC_volt_name]="ADC_volt",
	[ADC_channel_0_name]="ADC_channel_0",
	[ADC_channel_1_name]="ADC_channel_1",
	[ADC_channel_2_name]="ADC_channel_2",
	[ADC_channel_3_name]="ADC_channel_3",
	[ADC_channel_4_name]="ADC_channel_4",
	[ADC_channel_5_name]="ADC_channel_5",
	[Temperature_name]="Temperature",
	[Pressure_name]="Pressure",
	[Pressure_Ag_name]="Pressure_Ag",
	[ADC_middle_name]="ADC_middle",
	[current_4_20mA_name]="current_4_20mA",
	[voltage_measure_name]="voltage_measure",
	[Kdiv_name]="Kdiv",
	[ADC_channel_OFF_name]="ADC_channel_OFF",
	[ERROR_name]="Error",
	[PGA_GAIN_name]="PGA_GAIN",
};


// Количество символов в имени переменной для комманд
const uint8_t GV_NUM_OF_SYM[NUMBER_OF_VARIABLES]=
{
	[ADC_data_name]=8,
	[ADC_volt_name]=8,
	[ADC_channel_0_name]=13,
	[ADC_channel_1_name]=13,
	[ADC_channel_2_name]=13,
	[ADC_channel_3_name]=13,
	[ADC_channel_4_name]=13,
	[ADC_channel_5_name]=13,
	[Temperature_name]=11,
	[Pressure_name]=8,
	[Pressure_Ag_name]=11,
	[ADC_middle_name]=10,
	[current_4_20mA_name]=14,
	[voltage_measure_name]=15,
	[Kdiv_name]=4,
	[ADC_channel_OFF_name]=15,
	[ERROR_name]=5,
	[PGA_GAIN_name]=8,
};

// Тип переменных
const uint8_t GV_TYPE[NUMBER_OF_VARIABLES]=
{
	[ADC_data_name]=type_INT32,
	[ADC_volt_name]=type_DOUBLE,
	[ADC_channel_0_name]=type_INT32,
	[ADC_channel_1_name]=type_INT32,
	[ADC_channel_2_name]=type_INT32,
	[ADC_channel_3_name]=type_INT32,
	[ADC_channel_4_name]=type_INT32,
	[ADC_channel_5_name]=type_INT32,
	[Temperature_name]=type_DOUBLE,
	[Pressure_name]=type_DOUBLE,
	[Pressure_Ag_name]=type_DOUBLE,
	[ADC_middle_name]=type_INT32,
	[current_4_20mA_name]=type_DOUBLE,
	[voltage_measure_name]=type_DOUBLE,
	[Kdiv_name]=type_DOUBLE,
	[ADC_channel_OFF_name]=type_INT32,
	[ERROR_name]=type_INT32,
	[PGA_GAIN_name]=type_UINT8,
};


// Размеры массивов
const uint16_t GV_LENGTH[NUMBER_OF_VARIABLES]=
{
	[ADC_data_name]=6,
	[ADC_volt_name]=6,
	[ADC_channel_0_name]=ADC_BUFF_SIZE,
	[ADC_channel_1_name]=ADC_BUFF_SIZE,
	[ADC_channel_2_name]=ADC_BUFF_SIZE,
	[ADC_channel_3_name]=ADC_BUFF_SIZE,
	[ADC_channel_4_name]=ADC_BUFF_SIZE,
	[ADC_channel_5_name]=ADC_BUFF_SIZE,
	[Temperature_name]=1,
	[Pressure_name]=1,
	[Pressure_Ag_name]=1,
	[ADC_middle_name]=6,
	[current_4_20mA_name]=1,
	[voltage_measure_name]=1,
	[Kdiv_name]=1,
	[ADC_channel_OFF_name]=ADC_BUFF_SIZE,
	[ERROR_name]=1,
	[PGA_GAIN_name]=7,
};




const char* TYPE_STRING[NUMBER_OF_TYPES]=
{
	[type_INT8]=	"type_INT8  ",
	[type_INT16]=	"type_INT16 ",
	[type_INT32]=	"type_INT32 ",
	[type_UINT8]=	"type_UINT8 ",
	[type_UINT16]=	"type_UINT16",
	[type_UINT32]=	"type_UINT32",
	[type_FLOAT]=	"type_FLOAT ",
	[type_DOUBLE]=	"type_DOUBLE",
};





