#ifndef __BSP_SHT10_H__
#define __BSP_SHT10_H__
#include "stm32f4xx.h"
#include "bsp_sys.h"

enum {TEMP, HUMI};

extern uint32_t M_T;
extern uint32_t M_H;

/*
    SHT10_DATA1 --> PF0
    STH10_CLK1--> PF1
*/
/*--> 移植修改文件*/

#define SHT10_DATA_PIN                  GPIO_Pin_0
#define SHT10_DATA_PORT                 GPIOF
#define SHT10_DATA_CLK                  RCC_AHB1Periph_GPIOF

#define SHT10_CLK_PIN                  GPIO_Pin_1
#define SHT10_CLK_PORT                 GPIOF
#define SHT10_CLK_CLK                  RCC_AHB1Periph_GPIOF

#define SHT10_DATA_H()				PFout(0)=1			 //拉高DATA数据线
#define SHT10_DATA_L()				PFout(0)=0			 //拉低DATA数据线
#define SHT10_DATA_R()				PFin(0)			    //读DATA数据线
#define SHT10_SCK_H()	  			PFout(1)=1			 //拉高SCK时钟线
#define SHT10_SCK_L()				  PFout(1)=0		   //拉低SCK时钟线
/*
    SHT10_DATA2 --> PB7
    STH10_CLK2--> PB6
*/
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


void SHT10_Config(void);
void SHT10_ConReset(void);
u8 SHT10_SoftReset(void);
u8 SHT10_Measure(u16 *p_value, u8 *p_checksum, u8 mode);
void SHT10_Calculate(u16 t, u16 rh,u32 *p_temperature, u32 *p_humidity);
float SHT10_CalcuDewPoint(float t, float h);
uint8_t SHT10_Get_Temp(uint32_t *_tempvalue);
uint8_t SHT10_Get_Humi(uint32_t *_tempvalue);

#endif
