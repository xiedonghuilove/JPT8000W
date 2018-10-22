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
  g_ucaUSART3_TX_BUF[0] = 0xFE;
  g_ucaUSART3_TX_BUF[1] = 0xEF;
  g_ucaUSART3_TX_BUF[2] = g_aSubcontrolID[current_id];
  g_ucaUSART3_TX_BUF[3] = 0x03;//读功能
  g_ucaUSART3_TX_BUF[4] = 0xFF;//帧命令:读所有
  g_ucaUSART3_TX_BUF[5] = 0x17;//帧长 读取23个数据 23 *2 = 46 字节
  g_ucaUSART3_TX_BUF[6] = 0;//数据位填充0
  g_ucaUSART3_TX_BUF[7] = 0;
  g_ucaUSART3_TX_BUF[8] = 0;
  g_ucaUSART3_TX_BUF[9] = 0;
  g_ucaUSART3_TX_BUF[10] = 0x55;
  g_ucaUSART3_TX_BUF[11] = 0xaa;
  current_id++;
  if(current_id >= MODULE_NUM)current_id = 0;

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
*	函 数 名: Rs232_CommandParse
*	功能说明: RS232帧命令解析
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
//void PC_CommandParse(MasterPacket_T *uart_packet,MasterData_T *data_sp)
//{
//	uint8_t *p_send_data;
//	uint16_t *p_data;
//	uint16_t data_buff;
//
//	//将 data_sp  结构体MasterData_T 强制转换为uint16_t为的指针。(一次访问2个字节)
//	//即通过访问指针p_data 可以访问 结构体MasterData_T 中的成员，并且一次访问2字节(uint16_t*)
//	p_data = (uint16_t *)data_sp;
//	if(uart_packet->Funcode == 0x03)//读操作
//	{
//		if(uart_packet->SlaveAddr == 0xff)//丛机地址
//		{
//			if(uart_packet->Command < CMD_MASTER_LEN)//处于帧命令处
//			{
//				//将命令映射到地址，也就是在强制转换为8位的指针
//				p_send_data = (uint8_t*)&p_data[uart_packet->Command];//读地址
//				data_buff  =  uart_packet->Length;
//				PC_SendOnePackage(p_send_data,data_buff);//发送一帧数据
//			}

//		}
//	}
//	else if(uart_packet->Funcode == 0x06)//写操作
//	{
//		if(uart_packet->Command < CMD_MASTER_LEN)//处于帧命令处
//		{
//			//uart_packet->Data 包中接受的数据，赋值给结构体中
//			//p[n]  与 *(p+n) 结果一样.
//			p_data[uart_packet->Command] = uart_packet->Data;
//			//将值返回
//			data_buff = p_data[uart_packet->Command];
//			PC_SendOnePackage((u8*)(&data_buff),sizeof(data_buff));//发送一帧数据
//		}
//	}

//}

/*
*********************************************************************************************************
*	函 数 名: Rs232_CommandParse
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
	if(uart_packet->Funcode == 0x03)//读操作
	{
		//一次性读完全部主控板数据
      if(uart_packet->SlaveAddr == MASTER_ID)
      {
        *(uint16_t *)(g_ucaUSART2_TX_BUF+0) = UART2_RxData.Package.FrameHander;
        g_ucaUSART2_TX_BUF[2] = UART2_RxData.Package.SlaveAddr;
        g_ucaUSART2_TX_BUF[3] = UART2_RxData.Package.Funcode;
        g_ucaUSART2_TX_BUF[4] = UART2_RxData.Package.Command;
        g_ucaUSART2_TX_BUF[5] = UART2_RxData.Package.Length;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+6) = tMasterData.ID;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+8) = tMasterData.Mode;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+10) = tMasterData.Power;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+12) = tMasterData.ControlTemp;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+14) = tMasterData.ControlHumi;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+16) = tMasterData.HeshuqiModeTemp;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+18) = tMasterData.HeshuqiModeHumi;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+20) = tMasterData.TestSwitch;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+22) = tMasterData.WaterFlow;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+24) = tMasterData.FlashWaterFlow;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+26) = tMasterData.ModeSelection;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+28) = tMasterData.RongdianheTemp;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+30) = tMasterData.HeshuqiTemp;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+32) = tMasterData.HeshuqiWaterTemp;
        len = uart_packet->Length*2 + 6;
        crc_sum = 0x55AA;
        g_ucaUSART2_TX_BUF[len] = U16_HighByte(crc_sum);
        g_ucaUSART2_TX_BUF[len+1] = U16_LowByte(crc_sum);
        len = len+2;
      }
      else
      {
        for(i = 0;i<MODULE_NUM;i++)
        {
          if((uart_packet->SlaveAddr)&(1<<i))
          {
            module = i;
            break;
          }
        }
        *(uint16_t *)(g_ucaUSART2_TX_BUF+0) = UART2_RxData.Package.FrameHander;
        g_ucaUSART2_TX_BUF[2] = UART2_RxData.Package.SlaveAddr;
        g_ucaUSART2_TX_BUF[3] = UART2_RxData.Package.Funcode;
        g_ucaUSART2_TX_BUF[4] = UART2_RxData.Package.Command;
        g_ucaUSART2_TX_BUF[5] = UART2_RxData.Package.Length;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+6) = taModuleData[module].ID;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+8) = taModuleData[module].ElectricalTemp;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+10) = taModuleData[module].ElectricalHumi;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+12) = taModuleData[module].ElectricalWaterTemp;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+14) = taModuleData[module].OpticalWaterTemp;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+16) = taModuleData[module].I1;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+18) = taModuleData[module].I2;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+20) = taModuleData[module].I3;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+22) = taModuleData[module].I4;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+24) = taModuleData[module].I5;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+26) = taModuleData[module].I6;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+28) = taModuleData[module].I7;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+30) = taModuleData[module].I8;
				*(uint16_t*)(g_ucaUSART2_TX_BUF+32) = taModuleData[module].I9;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+34) = taModuleData[module].Version;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+36) = taModuleData[module].OverVoltage;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+38) = taModuleData[module].FullPowerAdjustment;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+40) = taModuleData[module].RedPhotocurrent;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+42) = taModuleData[module].ForwardReferenceVoltage;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+44) = taModuleData[module].OverElectricalWaterTemp;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+46) = taModuleData[module].OverOpticalWaterTemp;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+48) = taModuleData[module].BiasReferenceVoltage;
        *(uint16_t*)(g_ucaUSART2_TX_BUF+50) = taModuleData[module].GuideState;
				len = uart_packet->Length*2 + 6;
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
	else if(uart_packet->Funcode == 0x06)//写操作
	{

	}

}
