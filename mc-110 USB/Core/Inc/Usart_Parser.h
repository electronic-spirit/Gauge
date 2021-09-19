
#ifndef INC_USART_PARSER_H_
#define INC_USART_PARSER_H_


#ifndef USART_PARSER_H
#define USART_PARSER_H

#include "stm32l4xx_hal.h"
#include "main.h"
#include "usbd_cdc_if.h"




#define timeout_const 2


void Parser_process(void);


extern char rx_buf[APP_RX_DATA_SIZE];
extern uint16_t rxcntr;
extern int16_t USBTimeOutRX;

#endif



#endif /* INC_USART_PARSER_H_ */
