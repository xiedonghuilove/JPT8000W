/**
  ******************************************************************************
  * @file    command_parse.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   命令解析
  ******************************************************************************
  */
#include "command_parse.h"
#include "bsp_usart.h"
#include "data.h"
#include "bsp_dma.h"
#include "bsp_sys.h"
#include "bsp_adc.h"
#include "bsp_os.h"

uint8_t g_ucReadWriteFlag = 0;
uint8_t g_ucWriteFlashFlag = 0;

/*
*********************************************************************************************************
*	函 数 名: Master_SendPackagetoSubcotrol
*	功能说明: 主控板发送数据到子控板 oxFF表示读取所有信息
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void Master_SendPackagetoSubcotrol(void)
{
  static uint8_t current_id = 0;
  if(g_ucReadWriteFlag)
  {
    g_ucaUSART3_TX_BUF[10] = 0x55;
    g_ucaUSART3_TX_BUF[11] = 0xaa;
    g_ucReadWriteFlag = 0;
  }
  else
  {
    g_ucaUSART3_TX_BUF[0] = 0xFE;
    g_ucaUSART3_TX_BUF[1] = 0xEF;
    g_ucaUSART3_TX_BUF[2] = g_aSubcontrolID[current_id];
    g_ucaUSART3_TX_BUF[3] = 0x03;//读功能
    g_ucaUSART3_TX_BUF[4] = 0xFF;//帧命令:读所有
    g_ucaUSART3_TX_BUF[5] = 30*2;//帧长 读取30个数据 30 *2 = 60 字节
		//数据位填充当前选择的模块。
		*(uint16_t*)(g_ucaUSART3_TX_BUF+6) = tMasterData.FlashModeSelection;
    g_ucaUSART3_TX_BUF[8] = 0;
    g_ucaUSART3_TX_BUF[9] = 0;
    g_ucaUSART3_TX_BUF[10] = 0x55;
    g_ucaUSART3_TX_BUF[11] = 0xaa;
    current_id++;
    if(current_id >= MODULE_NUM)current_id = 0;
  }
  RS485EN_TX();//485切换为发送模式
  USART_ITConfig(RS485_USART, USART_IT_RXNE, DISABLE);		/* 禁止接收中断 */
  USART_ITConfig(RS485_USART, USART_IT_TC, ENABLE);	/* 使能发送完成中断 */
  if(DMA_GetFlagStatus(RS485_USART_DMA_STREAM,DMA_FLAG_TCIF3)!=RESET)//等待DMA1_Steam6传输完成
  {
    DMA_ClearFlag(RS485_USART_DMA_STREAM,DMA_FLAG_TCIF3);//清除DMA1_Steam3传输完成标志
  }
  DMA_Enable(RS485_USART_DMA_STREAM,12);     //开始一次DMA传输！
}

/*
*********************************************************************************************************
*	函 数 名: PC_CommandParse
*	功能说明: RS232帧命令解析
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void PC_CommandParse(MasterPacket_T *uart_packet,MasterData_T *data_sp)
{
    uint8_t len=0;
	uint16_t crc_sum = 0;
    uint8_t i=0,module = 0;
	if(uart_packet->Funcode == 0x03)//读操作 读所有
	{
		//一次性读完全部主控板数据
        if(uart_packet->SlaveAddr == MASTER_ID)
        {
                //根据不同模式显示的功率数组不同
            if(tMasterData.Mode == 1) tMasterData.Power = (g_usPowerValue*100)>>12;
            g_ucaUSART2_TX_BUF[0] = U16_HighByte(UART2_RxData.Package.FrameHander);
            g_ucaUSART2_TX_BUF[1] = U16_LowByte(UART2_RxData.Package.FrameHander);
            g_ucaUSART2_TX_BUF[2] = UART2_RxData.Package.SlaveAddr;
            g_ucaUSART2_TX_BUF[3] = UART2_RxData.Package.Funcode;
            g_ucaUSART2_TX_BUF[4] = UART2_RxData.Package.Command;
            g_ucaUSART2_TX_BUF[5] = UART2_RxData.Package.Length;

            DataCopyByPointer(&g_ucaUSART2_TX_BUF[6], UART2_RxData.Package.Length/2, (uint16_t*)&tMasterData);
            len = uart_packet->Length + 6;
            crc_sum = 0x55AA;
            g_ucaUSART2_TX_BUF[len] = U16_HighByte(crc_sum);
            g_ucaUSART2_TX_BUF[len+1] = U16_LowByte(crc_sum);
            len = len+2;
        }
        else
        {
            for(i = 1;i<=MODULE_NUM;i++)
            {
                if(uart_packet->SlaveAddr == i)
                {
                module = i-1;
                break;
                }
            }
            g_ucaUSART2_TX_BUF[0] = U16_HighByte(UART2_RxData.Package.FrameHander);
            g_ucaUSART2_TX_BUF[1] = U16_LowByte(UART2_RxData.Package.FrameHander);
            g_ucaUSART2_TX_BUF[2] = UART2_RxData.Package.SlaveAddr;
            g_ucaUSART2_TX_BUF[3] = UART2_RxData.Package.Funcode;
            g_ucaUSART2_TX_BUF[4] = UART2_RxData.Package.Command;
            g_ucaUSART2_TX_BUF[5] = UART2_RxData.Package.Length;
            DataCopyByPointer(&g_ucaUSART2_TX_BUF[6], UART2_RxData.Package.Length/2, (uint16_t*)&taModuleData[module]);
            len = uart_packet->Length + 6;
            crc_sum = 0x55AA;
            g_ucaUSART2_TX_BUF[len] = U16_HighByte(crc_sum);
            g_ucaUSART2_TX_BUF[len+1] = U16_LowByte(crc_sum);
            len = len+2;
        }
        if(DMA_GetFlagStatus(RS232_USART_DMA_STREAM,DMA_FLAG_TCIF6)!=RESET)//等待DMA1_Steam6传输完成
        {
            DMA_ClearFlag(RS232_USART_DMA_STREAM,DMA_FLAG_TCIF6);//清除DMA1_Steam6传输完成标志
        }
        DMA_Enable(RS232_USART_DMA_STREAM,len);     //开始一次DMA传输！
    }
	else if((uart_packet->Funcode == 0x06)&&(uart_packet->Length ==  0x04))//写操作 长度为4字节
	{
        //这里需要区分的是，读取主控板的数据，还是模块的数据。使用switch 还是if else
        if(uart_packet->SlaveAddr == MASTER_ID)
        {
            switch (uart_packet->Command)
            {
            case CMD_Erase_FULL:
                if(uart_packet->Data == 0x01) g_ucWriteFlashFlag = 1;
                break;
            case CMD_Select_Module:
                tMasterData.FlashModeSelection = (uint16_t)uart_packet->Data;
                g_ucWriteFlashFlag = 2;
                break;
            case CMD_Switch_Laser:
                tMasterData.TestSwitch = (uint16_t)uart_packet->Data;
                if((tMasterData.TestSwitch == 0x01)&&(tMasterData.Power>9)&&(tMasterData.Mode == 0)&&(g_ulUserFlag & USER_LASER_ON))
                {
                    STOP_CTRL_ON();//STOP打开
                    INSIDE_PWM_ON();//内控PWM打开
                    INSIDE_EN_ON();//内控EN打开
                }
                else
                {
                    tMasterData.TestSwitch = 0;
                    g_ulUserFlag &= USER_LASER_OFF;//关闭出光预备状态
                    PANEL_STAR_OFF();
                    STOP_CTRL_OFF();
                    INSIDE_PWM_OFF();//内控PWM打开
                    INSIDE_EN_OFF();//内控EN打开
                }
                break;
            case CMD_Write_Water:
                tMasterData.FlashWaterFlow = (uint16_t)uart_packet->Data;
                g_ucWriteFlashFlag = 2;
                break;
            case CMD_Write_QBHWatre:
                tMasterData.FlashQBHWaterFlow = (uint16_t)uart_packet->Data;
                g_ucWriteFlashFlag = 2;
                break;
            case CMD_Write_HeshuqiTemp:
                tMasterData.FlashHeshuqiTemp = (uint16_t)uart_packet->Data;
                g_ucWriteFlashFlag = 2;
                break;
            case CMD_Write_HeshuqiWaterTemp:
                tMasterData.FlashHeshuqiWaterTemp = (uint16_t)uart_packet->Data;
                g_ucWriteFlashFlag = 2;
                break;
            case CMD_Write_RedUserState:
                tMasterData.FlashRedUserState = (uint16_t)uart_packet->Data;
                g_ucWriteFlashFlag = 2;
                break;
            case CMD_Guide_UserControl:
                g_ulGuideUserControl = (uint16_t)uart_packet->Data;
                break;
            default:
                break;
            }
        }
        else //广播地址 + 模块地址需要转发写下去
        {
            g_ucReadWriteFlag = 1;
            g_ucaUSART3_TX_BUF[0] = U16_HighByte(UART2_RxData.Package.FrameHander);
            g_ucaUSART3_TX_BUF[1] = U16_LowByte(UART2_RxData.Package.FrameHander);
            g_ucaUSART3_TX_BUF[2] = UART2_RxData.Package.SlaveAddr;
            g_ucaUSART3_TX_BUF[3] = UART2_RxData.Package.Funcode;
            g_ucaUSART3_TX_BUF[4] = UART2_RxData.Package.Command;
            g_ucaUSART3_TX_BUF[5] = UART2_RxData.Package.Length;
            *(uint16_t *)(g_ucaUSART3_TX_BUF+6) = UART2_RxData.Package.Data;//6 7 8 9

        }
	}

}

/*
*********************************************************************************************************
*	函 数 名: RS485_CommandParse
*	功能说明: RS232帧命令解析
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void RS485_CommandParse(ModulePacket_T *_uart_packet,uint8_t _datalen,uint8_t *_pdata,ModuleData_T *_ModuleData)
{
    uint8_t i = 0;
    uint16_t *pdata;
    pdata = (uint16_t*)_ModuleData;//强制转换为16位指针，每一次指向一个数据

    for(i = 0;i<_datalen;i++)
    {
        *(pdata+i) = *(uint16_t*)(_pdata+i*2);
    }

}
/*
*********************************************************************************************************
*	函 数 名: RS485_CommandParse
*	功能说明: RS232帧命令解析
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void DataCopyByPointer(uint8_t *_pdata,uint8_t _datalen,uint16_t *_MasterData)
{
    uint8_t i = 0;
    uint16_t *pdata;
    pdata = (uint16_t*)_MasterData;//强制转换为16位指针，每一次指向一个数据

    for(i = 0;i<_datalen;i++)
    {
        *(uint16_t*)(_pdata+i*2) = *(pdata+i);
    }
}
