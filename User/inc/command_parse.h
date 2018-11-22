#ifndef __COMMAND_PARSE_H
#define __COMMAND_PARSE_H

#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "data.h"

extern uint8_t g_ucReadWriteFlag;
extern uint8_t g_ucWriteFlashFlag;

enum CMD_Type{
    CMD_Version = 0,
    CMD_Erase_FULL,
    CMD_Select_Module,
    CMD_Power,
    CMD_Switch_Laser,
    CMD_Write_Water,
    CMD_Write_QBHWatre,
    CMD_Write_HeshuqiTemp,
    CMD_Write_HeshuqiWaterTemp,
    CMD_Write_HeshuqiRedCurrent,
    CMD_Write_RedUserState,
    CMD_Guide_UserControl,
    CMD_NUM,
};

void Master_SendPackagetoSubcotrol(void);
void PC_CommandParse(MasterPacket_T *uart_packet,MasterData_T *data_sp);
void RS485_CommandParse(ModulePacket_T *_uart_packet,uint8_t _datalen,uint8_t *_pdata,ModuleData_T *_ModuleData);
void DataCopyByPointer(uint8_t *_pdata,uint8_t _datalen,uint16_t *_MasterData);
#endif
