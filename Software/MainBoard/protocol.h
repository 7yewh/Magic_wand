//********************************************************************************
#ifndef	_PROTOCOL_H_
#define	_PROTOCOL_H_
#include "type.h"
#include "CyberryPotter.h"
#include "stm32f10x.h"

//红外内码学习指令组包
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index);

//红外内码发送指令组包
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index);

#endif	//_PROTOCOL_H_

