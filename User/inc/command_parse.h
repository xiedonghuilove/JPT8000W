#ifndef __COMMAND_PARSE_H
#define __COMMAND_PARSE_H

#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "data.h"
void Master_SendPackagetoSubcotrol(void);
void PC_CommandParse(MasterPacket_T *uart_packet,MasterData_T *data_sp);
#endif
