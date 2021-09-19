
#ifndef INC_MAX77960_H_
#define INC_MAX77960_H_

#include "main.h"
#include "stm32l4xx_hal.h"



#define	MAX77960_CID				0x00
#define	MAX77960_SWRST				0x01
#define	MAX77960_TOP_INT			0x02
#define	MAX77960_TOP_INT_MASK		0x03
#define	MAX77960_TOP_INT_OK			0x04
#define	MAX77960_CHG_INT			0x10
#define	MAX77960_CHG_INT_MASK		0x11
#define	MAX77960_CHG_INT_OK			0x12
#define	MAX77960_CHG_DETAILS_00		0x13
#define	MAX77960_CHG_DETAILS_01		0x14
#define	MAX77960_CHG_DETAILS_02		0x15
#define	MAX77960_CHG_CNFG_00		0x16
#define	MAX77960_CHG_CNFG_01		0x17
#define	MAX77960_CHG_CNFG_02		0x18
#define	MAX77960_CHG_CNFG_03		0x19
#define	MAX77960_CHG_CNFG_04		0x1A
#define	MAX77960_CHG_CNFG_05		0x1B
#define	MAX77960_CHG_CNFG_06		0x1C
#define	MAX77960_CHG_CNFG_07		0x1D
#define	MAX77960_CHG_CNFG_08		0x1E
#define	MAX77960_CHG_CNFG_09		0x1F
#define	MAX77960_CHG_CNFG_10		0x20

#define MAX77960_I2C_ADDR           0xD2
#define MAX77960_WR					0xD2
#define MAX77960_RD					0xD3
#define MAX77960_7B_ADDR       	 	0x69

#define A_REGISTER                                0x1
#define ATEMPTS_TO_SEE_CHARGER                    0x9
#define CHARGER_I2C_TIMEOUT_MAX                   100
#define NO_INITIALIZATION                         0x00
#define INITIALIZED                               0x01

uint8_t MAX77960_Read_Reg(uint8_t REG);


#endif /* INC_MAX77960_H_ */
