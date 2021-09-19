
#include "W25N01.h"
#include "stdio.h"

void W25_WriteEnable(void);
void W25_WriteDisable(void);
void W25_Init(void);
void W25_WriteBuf(uint8_t*data, uint16_t blk_addr);
void W25_ReadBuf(uint8_t*data, uint16_t blk_addr);
uint32_t W25_Read_ID(void);
void W25_Read_Page(uint8_t* data, uint16_t page_addr, uint16_t column_addr, uint32_t sz);
void W25_Write_Page(uint8_t* data, uint16_t page_addr, uint16_t column_addr, uint32_t sz);
void W25_Block_Erase(uint32_t page_addr);
void W25_Write_Status(uint8_t StatusRegAdd, uint8_t data);
uint8_t W25_Read_Status(uint8_t StatusRegAdd);
void W25_Reset (void);


uint8_t buf[2048];				// Временный
//-------------------------------------------------------------

#define    	W25_RESET  			0xFF
#define    	PAGE_DATA_READ  	0x13
#define    	W25_FAST_READ  		0x0B
#define    	W25_GET_JEDEC_ID  	0x9f
#define   	W25_DUMMY		  	0x00
#define    	PROGRAM_DATA_LOAD	0x02
#define    	PROGRAM_EXECUTE		0x10

#define 	cs_set() 			HAL_GPIO_WritePin(MEM1_CS_GPIO_Port,MEM1_CS_Pin,GPIO_PIN_RESET)
#define 	cs_reset() 			HAL_GPIO_WritePin(MEM1_CS_GPIO_Port,MEM1_CS_Pin,GPIO_PIN_SET)

//-------------------------------------------------------------
SPI_HandleTypeDef hspi3;
//-------------------------------------------------------------
typedef struct
{
  uint16_t  PageSize;
  uint32_t  PageCount;
  uint32_t  SectorSize;
  uint32_t  SectorCount;
  uint32_t  BlockSize;
  uint32_t  BlockCount;
  uint32_t  NumKB;
  uint8_t   SR1;
  uint8_t   SR2;
  uint8_t   SR3;
  uint8_t   high_cap;
}w25_info_t;
w25_info_t  w25_info;


//-------------------------------------------------------------
void SPI1_Send (uint8_t *dt, uint16_t cnt)
{
  HAL_SPI_Transmit (&hspi3, dt, cnt, 5000);
}
//-------------------------------------------------------------
void SPI1_Recv (uint8_t *dt, uint16_t cnt)
{
  HAL_SPI_Receive (&hspi3, dt, cnt, 5000);
}
//-------------------------------------------------------------

void W25_WriteEnable(void)
{
	uint8_t command[1];
	cs_set();
	command[0]=0x06;
	SPI1_Send(command, 1);
	cs_reset();
}


void W25_WriteDisable(void)
{
	uint8_t command[1];
	cs_set();
	command[0]=0x04;
	SPI1_Send(command, 1);
	cs_reset();
}

void W25_Reset (void)
{
	uint8_t command[1];
	while ((W25_Read_Status(0xC0)&0x01)==1);
	cs_set();
	command[0] = W25_RESET;
	SPI1_Send(command, 1);
	cs_reset();
}
//-------------------------------------------------------------

uint8_t W25_Read_Status(uint8_t StatusRegAdd)
{
	uint8_t command[2];
	uint8_t data[1];
	command[0] = 0x0F; 		// Read Status Register
	command[1] = StatusRegAdd;
	cs_set();
	SPI1_Send(command, 2);
	SPI1_Recv(data, 1);
	cs_reset();
	return data[0];
}

void W25_Write_Status(uint8_t StatusRegAdd, uint8_t data)
{
	uint8_t command[3];
	command[0] = 0x1F; 		// Write Status Register
	command[1] = StatusRegAdd;
	command[2] = data;
	cs_set();
	SPI1_Send(command, 3);
	cs_reset();
}


void W25_Block_Erase(uint32_t page_addr)
{
	uint8_t command[4];

	W25_WriteEnable();

	command[0] = 0xD8;						// Block erase
	command[1] = W25_DUMMY;						// Dummy byte
	command[2] = (page_addr >> 8) & 0xFF;	// Start page address
	command[3] = page_addr & 0xFF;			// Start page address

	cs_set();
	SPI1_Send(command, 4);
	cs_reset();
	while ((W25_Read_Status(3)&0x01)==1);
}


void W25_Write_Page(uint8_t* data, uint16_t page_addr, uint16_t column_addr, uint32_t sz)
{
	uint8_t command[4];
	while ((W25_Read_Status(0xC0)&0x01)==1);
	W25_WriteEnable();
	command[0] = PROGRAM_DATA_LOAD; 							// Load program data
	command[1] = (column_addr >> 8) & 0xFF;		// Column address
	command[2] = column_addr & 0xFF;			// Column address
	cs_set();
	SPI1_Send(command, 3);
	SPI1_Send(data, sz);
	cs_reset();

	while ((W25_Read_Status(0xC0)&0x01)==1);
	//while ((W25_Read_Status(0xC0)&0x02)==0x02);

	// Пишем из буфера в память
	command[0] = PROGRAM_EXECUTE; // Program Execute
	command[1] = 0x00; // Dummy
	command[2] = (page_addr >> 8) & 0xFF;	// Адрес страницы
	command[3] = page_addr & 0xFF;			// Адрес страницы
	cs_set();
	SPI1_Send(command, 4);
	cs_reset();

	while ((W25_Read_Status(0xC0)&0x01)==1);
}


//-------------------------------------------------------------
void W25_Read_Page(uint8_t* data, uint16_t page_addr, uint16_t column_addr, uint32_t sz)
{
	uint8_t command[4];
	while ((W25_Read_Status(0xC0)&0x01)==1);
	//W25_WriteEnable();
	command[0] = 0x13; 		// Page Data Read
	command[1] = 0x00;		// Dummy
	command[2] = (page_addr >> 8) & 0xFF;
	command[3] = page_addr & 0xFF;
	cs_set();
	SPI1_Send(command, 4);
	cs_reset();

	while ((W25_Read_Status(0xC0)&0x01)==1);

	command[0] = 0x0B; 		// Fast Read
	command[1] = (column_addr >> 8) & 0xFF;
	command[2] = column_addr & 0xFF;
	command[3] = 0x00;		// Dummy
	cs_set();
	SPI1_Send(command, 4);
	// Читаем в буфер с необходимой позиции
	SPI1_Recv(data, sz);
	cs_reset();
}
//-------------------------------------------------------------
uint32_t W25_Read_ID(void)
{
	uint8_t command[4];
	uint8_t dt[3];
	command[0] = W25_GET_JEDEC_ID;
	command[1] = 0x00;
	cs_set();
	SPI1_Send(command, 2);
	SPI1_Recv(dt,3);
	cs_reset();
	return (dt[0] << 16) | (dt[1] << 8) | dt[2];
}


void W25_ReadBuf(uint8_t*data, uint16_t blk_addr)
{
	uint8_t command[4];
	uint16_t address=(blk_addr>>2)*64;
	uint16_t column=(blk_addr&0x03)*512;

	while ((W25_Read_Status(0xC0)&0x01)==1);
	// Transfer from memory to buffer
	command[0] = PAGE_DATA_READ;
	command[1] = W25_DUMMY;
	command[2] = (address >> 8) & 0xFF;
	command[3] = address & 0xFF;
	cs_set();
	SPI1_Send(command, 4);
	cs_reset();
	while ((W25_Read_Status(0xC0)&0x01)==1);

	// Read from buffer
	command[0] = W25_FAST_READ;
	command[1] = (column >> 8) & 0xFF;
	command[2] = column & 0xFF;
	command[3] = W25_DUMMY;
	cs_set();
	SPI1_Send(command, 4);
	SPI1_Recv(data, 512);
	cs_reset();
	while ((W25_Read_Status(0xC0)&0x01)==1);
}

void W25_WriteBuf(uint8_t*data, uint16_t blk_addr)
{
	uint8_t command[4];
	uint16_t i;
	uint16_t address=(blk_addr>>2)*64;
	uint16_t column=(blk_addr&0x03)*512;

	// Прочитаем блок 2048 прежде чем стереть
	while ((W25_Read_Status(0xC0)&0x01)==1);
	// Transfer from memory to buffer
	command[0] = PAGE_DATA_READ;
	command[1] = W25_DUMMY;
	command[2] = (address >> 8) & 0xFF;
	command[3] = address & 0xFF;
	cs_set();
	SPI1_Send(command, 4);
	cs_reset();
	while ((W25_Read_Status(0xC0)&0x01)==1);
	// Read from buffer
	command[0] = W25_FAST_READ;
	command[1] = 0;
	command[2] = 0;
	command[3] = W25_DUMMY;
	cs_set();
	SPI1_Send(command, 4);
	SPI1_Recv(buf, 2048);
	cs_reset();
	while ((W25_Read_Status(0xC0)&0x01)==1);

	for(i=0;i<512;i++)buf[column+i]=data[i];

	//memcpy(buf[column], data, 512);

	// Сотрем блок 128кБ
	W25_Block_Erase(address);

	while ((W25_Read_Status(0xC0)&0x01)==1);
	// Пишем в буфер
	W25_WriteEnable();
	command[0] = PROGRAM_DATA_LOAD;
	command[1] = 0;
	command[2] = 0;
	cs_set();
	SPI1_Send(command, 3);
	SPI1_Send(buf, 2048);
	cs_reset();
	while ((W25_Read_Status(0xC0)&0x01)==1);

	// Переносим во внутреннюю память
	W25_WriteEnable();
	command[0] = PROGRAM_EXECUTE;
	command[1] = W25_DUMMY;
	command[2] = (address >> 8) & 0xFF;	// Адрес страницы
	command[3] = address & 0xFF;			// Адрес страницы
	cs_set();
	SPI1_Send(command, 4);
	cs_reset();
	while ((W25_Read_Status(0xC0)&0x01)==1);
}

//-------------------------------------------------------------
void W25_Init(void)
{
	HAL_Delay(100);
	W25_Reset();
	HAL_Delay(100);
	unsigned int id = W25_Read_ID();

	if(id!=0xEFAA21)ERROR_REG|=(0x01<<Memory_err);

	w25_info.PageSize=2048;
	w25_info.PageCount=0xFFFF;
	w25_info.BlockSize=w25_info.PageSize*64;
	w25_info.NumKB=(w25_info.BlockSize*128)/8;

	W25_Write_Status(0xA0, 0x00);
}



//-------------------------------------------------------------
