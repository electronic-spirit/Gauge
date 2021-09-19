
#ifndef INC_SOUND_H_
#define INC_SOUND_H_


#include "main.h"
#include "stm32l4xx_hal.h"

extern uint16_t buf_sin[64];
extern uint16_t buf_tr[64];

void Play_Sound(char*name);
void StartReadNextSoundBuf(void);


#endif /* INC_SOUND_H_ */
