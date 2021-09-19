#include "sound.h"
#include "fatfs.h"

extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_dac_ch2;
void Sound_process(void);

#define WAV_BUF_SIZE 512


uint8_t sound_buf[2][WAV_BUF_SIZE];
extern char USERPath[4];	// Logical drive path
FATFS W25FatFs;
FATFS *fs;
FIL MyFile;
UINT testBytes;

uint16_t dataOffset = 0;		// Начало данных
uint8_t buf_num=0;				// Номер буфера для чтения
uint32_t wavDataSize=0;			// Размер файла в байтах
uint8_t ReadComplete=0;			// Выставляется в 1, когда прочитана очередная порция данных
uint16_t sizecntr=0; 			// Счетчик байт
uint32_t wavcounter=0;

void Play_Sound(char*name)
{
	//uint8_t path[11] = "AUDIO_5.WAV";
	uint16_t i;

	// Открываем файл

	f_mount(&W25FatFs,(TCHAR const*)USERPath,0);
	f_open(&MyFile, (char*)name, FA_READ|FA_OPEN_ALWAYS);
	f_read(&MyFile, sound_buf, WAV_BUF_SIZE, &testBytes);

	// Находим начало данных
	for (i = 0; i < (WAV_BUF_SIZE - 3); i++)
	{
	    if ((sound_buf[0][i] == 'd') && (sound_buf[0][i + 1] == 'a') &&
	        (sound_buf[0][i + 2] == 't') && (sound_buf[0][i + 3] == 'a'))
	    {
	        dataOffset = i + 8;
	        break;
	    }
	}

	//  перемещаем указатель FatFs
	f_lseek(&MyFile, dataOffset);
	// определяем количество байт данных
	wavDataSize = f_size(&MyFile) - dataOffset;

	// Заполняем оба буфера данными
	f_read(&MyFile, sound_buf[0], WAV_BUF_SIZE, &testBytes);

	ReadComplete=1;

	TURN_ON_SOUND();
	//HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start(&htim6);
	DMA1_Channel4->CCR |= (DMA_CCR_TCIE_Msk);
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, (uint32_t*)sound_buf[0], WAV_BUF_SIZE, DAC_ALIGN_8B_R);
}


void StartReadNextSoundBuf(void)
{
	if((DMA1->ISR&DMA_ISR_TCIF4_Msk)!=0)
	{

		ReadComplete=0;
		if(buf_num==1)buf_num=0;
		else buf_num=1;

		DMA1_Channel4->CCR &= ~(1 << DMA_CCR_EN_Pos);
		DMA1_Channel4->CMAR=(uint32_t)sound_buf[buf_num];
		//DMA1_Channel4->CNDTR=(uint32_t)WAV_BUF_SIZE;
		DMA1_Channel4->CCR |= (1 << DMA_CCR_EN_Pos);

		wavcounter+=WAV_BUF_SIZE;
		Sound_process();
	}
}


void Sound_process(void)
{
	uint16_t i=0;
	uint8_t sign=0;
	if(ReadComplete==0)
	{
		if(wavDataSize>wavcounter)
		{
			if(buf_num==0)
			{
				f_read(&MyFile, sound_buf[0], WAV_BUF_SIZE, &testBytes);
				for(i=0;i<WAV_BUF_SIZE;i++)
				{
					//sign=sound_buf[0][i]&0x80;
					//sound_buf[0][i]=(sound_buf[0][i]&0x7F);
					//sound_buf[0][i]|=sign;
					sound_buf[0][i]=(sound_buf[0][i])&0xFF;
				}
			}
			else
			{
				f_read(&MyFile, sound_buf[1], WAV_BUF_SIZE, &testBytes);
				for(i=0;i<WAV_BUF_SIZE;i++)
				{
					//sign=sound_buf[1][i]&0x80;
					//sound_buf[1][i]=(sound_buf[1][i]&0x7F);
					//sound_buf[1][i]|=sign;
					sound_buf[1][i]=(sound_buf[1][i])&0xFF;
				}
			}
		}
		else
		{
			TURN_OFF_SOUND();
			DMA1_Channel4->CCR &= ~(1 << DMA_CCR_EN_Pos);
			f_close(&MyFile);
		}
		ReadComplete=1;
	}
}



