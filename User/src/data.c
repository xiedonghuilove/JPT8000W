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
#include "bsp_eeprom_24xx.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "bsp_publicIO.h"

//结构体变量定义 8字节对齐 后面需要访问
__align(8) MasterData_T tMasterData;

ModuleData_T taModuleData[MODULE_NUM];
uint32_t g_aSubcontrolID[MODULE_NUM] = {1,2,3,4,5,6,7,8};//子控板ID

uint16_t g_ulaBufFlash[INDEX_FLASH_NUM] = {0};

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
    LASER_MODE_OUTSIDE();
  }
  else if(ucRobotCnt>1)
  {
    tMasterData.Mode=1;    //外控
    LASER_MODE_OUTSIDE();
  }
  else if(ucTestCnt>8)
  {
    tMasterData.Mode=0;   //测试
    LASER_MODE_INSIDE();
  }
}
/*
*********************************************************************************************************
*	函 数 名: EE_Flash_Set_Data
*	功能说明: 开机读取一次eeprom数据
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void EE_Flash_Set_Data(void)
{
  int i = 0;
  //读取flash中的数据
  ee_ReadBytes((uint8_t*)g_ulaBufFlash, 0, sizeof(g_ulaBufFlash));
  for(i=0;i<INDEX_FLASH_NUM;i++)
  {
      printf(" %04X", g_ulaBufFlash[i]);
  }
  //校验正确
  if(g_ulaBufFlash[Index_CheckData] == 0x55AA)
  {
    //将flash中的数据赋值给结构体变量
    tMasterData.FlashWaterFlow = g_ulaBufFlash[Index_FlashWaterFlow];
    tMasterData.FlashModeSelection = g_ulaBufFlash[Index_FlashModeSelection];
    tMasterData.FlashHeshuqiTemp = g_ulaBufFlash[Index_FlashHeshuqiTemp];
    tMasterData.FlashHeshuqiWaterTemp = g_ulaBufFlash[Index_FlashHeshuqiWaterTemp];
    tMasterData.FlashRongdianheTemp = g_ulaBufFlash[Index_FlashRongdianheTemp];
    tMasterData.FlashRedUserState = g_ulaBufFlash[Index_FlashRedUserState];
    tMasterData.FlashHeshuqiRedCurrent = g_ulaBufFlash[Index_FlashHeshuqiRedCurrent];
  }
  else
  {
    tMasterData.FlashWaterFlow = 4500;
    tMasterData.FlashModeSelection = 0xFF;//默认开启8个
    tMasterData.FlashHeshuqiTemp = 45;
    tMasterData.FlashHeshuqiWaterTemp = 45;
    tMasterData.FlashRongdianheTemp = 45;
    tMasterData.FlashRedUserState = 0xBB;//默认状态
    tMasterData.FlashHeshuqiRedCurrent = 90;//合束器红光电流
  }

}
