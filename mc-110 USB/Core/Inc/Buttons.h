

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_


#include "stm32l4xx_hal.h"
#include "main.h"

#define on_off_time	200

#define PI4IOE5V6416_I2C_ADDR          0x40
#define PI4IOE5V6416_RD                0x41
#define PI4IOE5V6416_WR                0x40

#define PI4IOE5V6416_REG_I_MNTR             0x00  // Monitor
#define PI4IOE5V6416_REG_I_MNTR0            0x00
#define PI4IOE5V6416_REG_I_MNTR1            0x01
#define PI4IOE5V6416_REG_O_STT              0x02  // State
#define PI4IOE5V6416_REG_O_STT0             0x02
#define PI4IOE5V6416_REG_O_STT1             0x03
#define PI4IOE5V6416_REG_I_PIN              0x04  // Polarity inversion
#define PI4IOE5V6416_REG_I_PIN0             0x04
#define PI4IOE5V6416_REG_I_PIN1             0x05
#define PI4IOE5V6416_REG_IO_DIR             0x06  // Direction
#define PI4IOE5V6416_REG_IO_DIR0            0x06
#define PI4IOE5V6416_REG_IO_DIR1            0x07
#define PI4IOE5V6416_REG_O_STRNTH           0x40
#define PI4IOE5V6416_REG_O_STRNTH0          0x40
#define PI4IOE5V6416_REG_O_STRNTH1          0x41
#define PI4IOE5V6416_REG_O_STRNTH2          0x42
#define PI4IOE5V6416_REG_O_STRNTH3          0x43
#define PI4IOE5V6416_REG_I_LTCH             0x44
#define PI4IOE5V6416_REG_I_LTCH0            0x44
#define PI4IOE5V6416_REG_I_LTCH1            0x45
#define PI4IOE5V6416_REG_I_PL_EN            0x46
#define PI4IOE5V6416_REG_I_PL_EN0           0x46
#define PI4IOE5V6416_REG_I_PL_EN1           0x47
#define PI4IOE5V6416_REG_I_PL_UD            0x48
#define PI4IOE5V6416_REG_I_PL_UD0           0x48
#define PI4IOE5V6416_REG_I_PL_UD1           0x49
#define PI4IOE5V6416_REG_INT_MSK            0x4A
#define PI4IOE5V6416_REG_INT_MSK0           0x4A
#define PI4IOE5V6416_REG_INT_MSK1           0x4B
#define PI4IOE5V6416_REG_INT_STS            0x4C
#define PI4IOE5V6416_REG_INT_STS0           0x4C
#define PI4IOE5V6416_REG_INT_STS1           0x4D
#define PI4IOE5V6416_REG_O_CFG              0x4F

#define PI4IOE5V6416_DIRECTION_IN              ((uint8_t)0x1)
#define PI4IOE5V6416_DIRECTION_OUT             ((uint8_t)0x0)
#define PI4IOE5V6416_I_WITHOUT_PULL_RESISTOR   ((uint8_t)0x0)
#define PI4IOE5V6416_I_WITH_PULL_RESISTOR      ((uint8_t)0x1)
#define PI4IOE5V6416_I_PULL_DOWN               ((uint8_t)0x0)
#define PI4IOE5V6416_I_PULL_UP                 ((uint8_t)0x1)
#define PI4IOE5V6416_I_PULL_DISABLE            ((uint8_t)0x0)
#define PI4IOE5V6416_I_PULL_ENABLE             ((uint8_t)0x1)
#define PI4IOE5V6416_I_POL_INV_DISABLE         ((uint8_t)0x0)
#define PI4IOE5V6416_I_POL_INV_ENABLE          ((uint8_t)0x1)
#define PI4IOE5V6416_I_INT_DISABLE             ((uint8_t)0x1)
#define PI4IOE5V6416_I_INT_ENABLE              ((uint8_t)0x0)

#define PI4IOE5V6416_PIN_0             0x0001
#define PI4IOE5V6416_PIN_1             0x0002
#define PI4IOE5V6416_PIN_2             0x0004
#define PI4IOE5V6416_PIN_3             0x0008
#define PI4IOE5V6416_PIN_4             0x0010
#define PI4IOE5V6416_PIN_5             0x0020
#define PI4IOE5V6416_PIN_6             0x0040
#define PI4IOE5V6416_PIN_7             0x0080
#define PI4IOE5V6416_PIN_8             0x0100
#define PI4IOE5V6416_PIN_9             0x0200
#define PI4IOE5V6416_PIN_10            0x0400
#define PI4IOE5V6416_PIN_11            0x0800
#define PI4IOE5V6416_PIN_12            0x1000
#define PI4IOE5V6416_PIN_13            0x2000
#define PI4IOE5V6416_PIN_14            0x4000
#define PI4IOE5V6416_PIN_15            0x8000
#define PI4IOE5V6416_PIN_ALL           0xFFFF
// Начиная с PIN_8 это выводы порта №1
#define PI4IOE5V6416_PORT1             0x0100



void Buttons_init(void);
void Buttons_process(void);
void LED(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
void Buttons_timer(void);
void BLueTooth_Configuration(uint8_t state);

extern uint16_t keypad_code;
extern int16_t OnOffCntr;

#endif /* INC_BUTTONS_H_ */
