/**
  ******************************************************************************
  * @file    date.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   数据定义与数据处理
  ******************************************************************************
  */
#include "data.h"

//结构体变量定义 8字节对齐 后面需要访问
__align(8) MasterData_T tMasterData;

ModuleData_T taModuleData[MODULE_NUM];
uint32_t g_aSubcontrolID[MODULE_NUM] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//子控板ID
/*
*********************************************************************************************************
*	函 数 名: Scan_Mode
*	功能说明: 扫描模式
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void Scan_Mode(void)
{
  uint8_t i,ucRobotCnt=0,ucTestCnt=0,ucRS232Cnt=0;
  for (i = 0; i < 10; i++)
  {
    delay_ms(100);
    if(INTERLOCKB_In()==1)//互锁信号短接
    {
      if(MODE_In()==1)//RS232模式
      {
        ucRS232Cnt++;
      }
      else//测试模式
      {
        ucTestCnt++;
      }
    }
    else//互锁信号断开  正常模式
    {
      if(MODE_In()==1)//外控模式
      {
        ucRobotCnt++;
      }
      else//测试模式
      {
        ucTestCnt++;
      }
    }
    LED3_Out() = !LED3_Out();
  }
  if(ucRS232Cnt>1)
  {
    tMasterData.Mode=2;    //RS232
  }
  else if(ucRobotCnt>1)
  {
    tMasterData.Mode=1;    //外控
  }
  else if(ucTestCnt>8)
  {
    tMasterData.Mode=0;   //测试
  }
}
