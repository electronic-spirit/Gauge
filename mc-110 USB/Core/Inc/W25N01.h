
#ifndef INC_W25N01_H_
#define INC_W25N01_H_



#include "stm32l4xx_hal.h"
#include "main.h"
#include <stdbool.h>




void W25_Init(void);
void W25_ReadBuf(uint8_t*data, uint16_t blk_addr);
void W25_WriteBuf(uint8_t*data, uint16_t blk_addr);



#endif /* INC_W25N01_H_ */
