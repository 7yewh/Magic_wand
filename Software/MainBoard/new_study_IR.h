#ifndef	_NEW_STUDY_IR_H_
#define	_NEW_STUDY_IR_H_
#include "stm32f10x.h"    // Device header
#include "type.h"
#define   BAUD_RATE     115200
void USART2_Config(void);
void Uart_Send(uint8_t *data, uint16_t len);

#endif	//_NEW_STUDY_IR_H_

