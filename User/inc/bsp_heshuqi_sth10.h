#ifndef __SHT10_HESHUQI_H__
#define __SHT10_HESHUQI_H__

#include "stm32f4xx.h"
#include "bsp_sys.h"

enum {HESHUQI1_TEMP, HESHUQI1_HUMI};

extern u32 HESHUQI_M_T;
extern u32 HESHUQI_M_H;

/*****************************GPIO相关宏定义 --> 移植修改文件*/
/*
    SHT10_DATA2 --> PB7
    STH10_CLK2--> PB6
*/
#define SHT10_Heshuqi_DATA_PIN                  GPIO_Pin_7
#define SHT10_Heshuqi_DATA_PORT                 GPIOB
#define SHT10_Heshuqi_DATA_CLK                  RCC_AHB1Periph_GPIOB

#define SHT10_Heshuqi_CLK_PIN                  GPIO_Pin_6
#define SHT10_Heshuqi_CLK_PORT                 GPIOB
#define SHT10_Heshuqi_CLK_CLK                  RCC_AHB1Periph_GPIOB

#define SHT10_Heshuqi_DATA_H()				PBout(7)=1			 //拉高DATA数据线
#define SHT10_Heshuqi_DATA_L()				PBout(7)=0			 //拉低DATA数据线
#define SHT10_Heshuqi_DATA_R()				PBin(7)			    //读DATA数据线
#define SHT10_Heshuqi_SCK_H()	  			PBout(6)=1			 //拉高SCK时钟线
#define SHT10_Heshuqi_SCK_L()				  PBout(6)=0		   //拉低SCK时钟线
/*****************************************************************************/

/* 传感器相关宏定义 */
#define        noACK        0
#define      ACK             1
                                                                //addr  command         r/w
#define STATUS_REG_W        0x06        //000         0011          0          写状态寄存器
#define STATUS_REG_R        0x07        //000         0011          1          读状态寄存器
#define MEASURE_TEMP         0x03        //000         0001          1          测量温度
#define MEASURE_HUMI         0x05        //000         0010          1          测量湿度
#define SOFTRESET       0x1E        //000         1111          0          复位


void SHT10_Heshuqi_Config(void);
void SHT10_Heshuqi_ConReset(void);
u8 SHT10_Heshuqi_SoftReset(void);
u8 SHT10_Heshuqi_Measure(u16 *p_value, u8 *p_checksum, u8 mode);
void SHT10_Heshuqi_Calculate(u16 t, u16 rh,u32 *p_temperature, u32 *p_humidity);
float SHT10_Heshuqi_CalcuDewPoint(float t, float h);
u32 SHT10_Heshuqi_Get_Temp(void);
u32 SHT10_Heshuqi_Get_Humi(void);

#endif
