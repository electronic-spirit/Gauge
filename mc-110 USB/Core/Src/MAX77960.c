#include "MAX77960.h"

extern I2C_HandleTypeDef hi2c2;


uint8_t MAX77960_Read_Reg(uint8_t REG)
{
	uint8_t tx_buf[1], rx_buf[1];
	tx_buf[0]=REG;
	rx_buf[0]=0xFF;
	HAL_I2C_Master_Transmit(&hi2c2, 0xDE, tx_buf, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c2, 0xDE, (uint8_t *)rx_buf, 1, 10000);
	return rx_buf[0];
}



