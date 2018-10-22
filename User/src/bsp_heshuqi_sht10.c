/*
********************************************************************************
  * @author  xdh
  * @version V1.0
  * @date    2017-xx-xx
  * @apply   3000WControl
********************************************************************************
*/
#include "bsp_heshuqi_sth10.h"
#include <math.h>
#include "bsp_sys.h"
//单独读取温湿度的变量
u32 HESHUQI_M_T=0;
u32 HESHUQI_M_H=0;

/*************************************************************
  Function   ：SHT10_Config
  Description：初始化 SHT10引脚
  Input      : none
  return     : none
*************************************************************/
void SHT10_Heshuqi_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //初始化SHT10引脚时钟
  RCC_AHB1PeriphClockCmd ( SHT10_Heshuqi_DATA_CLK|
                         SHT10_Heshuqi_CLK_CLK, ENABLE);
  //PF0   DATA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = SHT10_Heshuqi_DATA_PIN;
  GPIO_Init(SHT10_Heshuqi_DATA_PORT, &GPIO_InitStructure);

  //PF1 SCK 推挽输出
  GPIO_InitStructure.GPIO_Pin = SHT10_Heshuqi_CLK_PIN;
  GPIO_Init(SHT10_Heshuqi_CLK_PORT, &GPIO_InitStructure);

  delay_ms(50);
  SHT10_Heshuqi_ConReset();        //复位通讯
  delay_ms(20);
}


/*************************************************************
  Function   ：SHT10_Heshuqi_DATAOut
  Description：设置DATA引脚为输出
  Input      : none
  return     : none
*************************************************************/
void SHT10_Heshuqi_DATAOut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //PF0 DATA 推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = SHT10_Heshuqi_DATA_PIN;
  GPIO_Init(SHT10_Heshuqi_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ：SHT10_DATAIn
  Description：设置DATA引脚为输入
  Input      : none
  return     : none
*************************************************************/
void SHT10_Heshuqi_DATAIn(void)
{
  //PF0 DATA 输入
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = SHT10_Heshuqi_DATA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//高速
  GPIO_Init(SHT10_Heshuqi_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ：SHT10_Heshuqi_WriteByte
  Description：写1字节
  Input      : value:要写入的字节
  return     : err: 0-正确  1-错误
*************************************************************/
u8 SHT10_Heshuqi_WriteByte(u8 value)
{
		u32 i, err = 0;

		SHT10_Heshuqi_DATAOut();			        //设置DATA数据线为输出

		for(i = 0x80; i > 0; i /= 2)  //写1个字节
		{
			if(i & value)
				SHT10_Heshuqi_DATA_H();
			else
				SHT10_Heshuqi_DATA_L();
			delay_us(2);
			SHT10_Heshuqi_SCK_H();
			delay_us(2);
			SHT10_Heshuqi_SCK_L();
			delay_us(2);
		}
		SHT10_Heshuqi_DATA_H();
		SHT10_Heshuqi_DATAIn();				    //设置DATA数据线为输入,释放DATA线
		SHT10_Heshuqi_SCK_H();
		delay_us(2);
		err = SHT10_Heshuqi_DATA_R();		  //读取SHT10的应答位
		SHT10_Heshuqi_SCK_L();

		return err;
}

/*************************************************************
  Function   ：SHT10_ReadByte
  Description：读1字节数据
  Input      : Ack: 0-不应答  1-应答
  return     : err: 0-正确 1-错误
*************************************************************/
u8 SHT10_Heshuqi_ReadByte(u8 Ack)
{
		u8 i, val = 0;

		SHT10_Heshuqi_DATAIn();				  //设置DATA数据线为输入
		for(i = 0x80; i > 0; i /= 2)  //读取1字节的数据
		{
			delay_us(2);
			SHT10_Heshuqi_SCK_H();
			delay_us(2);
			if(SHT10_Heshuqi_DATA_R())
				val = (val | i);
			SHT10_Heshuqi_SCK_L();
		}
		SHT10_Heshuqi_DATAOut();			  //设置DATA数据线为输出
		if(Ack)
			SHT10_Heshuqi_DATA_L();			  //应答，则会接下去读接下去的数据(校验数据)
		else
			SHT10_Heshuqi_DATA_H();			  //不应答，数据至此结束
		delay_us(2);
		SHT10_Heshuqi_SCK_H();
		delay_us(2);
		SHT10_Heshuqi_SCK_L();
		delay_us(2);

		return val;					  //返回读到的值
}


/*************************************************************
  Function   ：SHT10_TransStart
  Description：开始传输信号，时序如下：
                     _____         ________
               DATA:      |_______|
                         ___     ___
               SCK : ___|   |___|   |______
  Input      : none
  return     : none
*************************************************************/
void SHT10_Heshuqi_TransStart(void)
{
        SHT10_Heshuqi_DATAOut();                          //设置DATA数据线为输出

        SHT10_Heshuqi_DATA_H();
        SHT10_Heshuqi_SCK_L();
        delay_us(2);
        SHT10_Heshuqi_SCK_H();
        delay_us(1);
        SHT10_Heshuqi_DATA_L();
        delay_us(1);
        SHT10_Heshuqi_SCK_L();
        delay_us(2);
        SHT10_Heshuqi_SCK_H();
        delay_us(1);
        SHT10_Heshuqi_DATA_H();
        delay_us(1);
        SHT10_Heshuqi_SCK_L();
}

/*************************************************************
  Function   ：SHT10_ConReset
  Description：通讯复位，时序如下：
                     _____________________________________________________         ________
               DATA:                                                      |_______|
                        _    _    _    _    _    _    _    _    _        ___     ___
               SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
  Input      : none
  return     : none
*************************************************************/
void SHT10_Heshuqi_ConReset(void)
{
        u8 i;

        SHT10_Heshuqi_DATAOut();

        SHT10_Heshuqi_DATA_H();
        SHT10_Heshuqi_SCK_L();

        for(i = 0; i < 9; i++)                  //触发SCK时钟9c次
        {
                SHT10_Heshuqi_SCK_H();
                delay_us(2);
                SHT10_Heshuqi_SCK_L();
                delay_us(2);
        }
        SHT10_Heshuqi_TransStart();                          //启动传输
}



/*************************************************************
  Function   ：SHT10_SoftReset
  Description：软复位
  Input      : none
  return     : err: 0-正确  1-错误
*************************************************************/
u8 SHT10_Heshuqi_SoftReset(void)
{
        u8 err = 0;

        SHT10_Heshuqi_ConReset();                              //通讯复位
        err += SHT10_Heshuqi_WriteByte(SOFTRESET);//写RESET复位命令

        return err;
}


/*************************************************************
  Function   ：SHT10_ReadStatusReg
  Description：读状态寄存器
  Input      : p_value-读到的数据；p_checksun-读到的校验数据
  return     : err: 0-正确  0-错误
*************************************************************/
u8 SHT10_Heshuqi_ReadStatusReg(u8 *p_value, u8 *p_checksum)
{
        u8 err = 0;

        SHT10_Heshuqi_TransStart();                                        //开始传输
        err = SHT10_Heshuqi_WriteByte(STATUS_REG_R);//写STATUS_REG_R读取状态寄存器命令
        *p_value = SHT10_Heshuqi_ReadByte(ACK);                //读取状态数据
        *p_checksum = SHT10_Heshuqi_ReadByte(noACK);//读取检验和数据

        return err;
}



/*************************************************************
  Function   ：SHT10_WriteStatusReg
  Description：写状态寄存器
  Input      : p_value-要写入的数据值
  return     : err: 0-正确  1-错误
*************************************************************/
u8 SHT10_Heshuqi_WriteStatusReg(u8 *p_value)
{
        u8 err = 0;

        SHT10_Heshuqi_TransStart();                                         //开始传输
        err += SHT10_Heshuqi_WriteByte(STATUS_REG_W);//写STATUS_REG_W写状态寄存器命令
        err += SHT10_Heshuqi_WriteByte(*p_value);         //写入配置值

        return err;
}



/*************************************************************
  Function   ：SHT10_Measure
  Description：从温湿度传感器读取温湿度
  Input      : p_value-读到的值；p_checksum-读到的校验数
  return     : err: 0-正确 1—错误
*************************************************************/
u8 SHT10_Heshuqi_Measure(u16 *p_value, u8 *p_checksum, u8 mode)
{
        u8 err = 0;
        // u32 i;
        u8 value_H = 0;
        u8 value_L = 0;

        SHT10_Heshuqi_TransStart();                                                 //开始传输
        switch(mode)
        {
        case HESHUQI1_TEMP:                                                                 //测量温度
                err += SHT10_Heshuqi_WriteByte(MEASURE_TEMP);//写MEASURE_TEMP测量温度命令
                break;
        case HESHUQI1_HUMI:
                err += SHT10_Heshuqi_WriteByte(MEASURE_HUMI);//写MEASURE_HUMI测量湿度命令
                break;
        default:
                break;
        }
        SHT10_Heshuqi_DATAIn();
        // for(i = 0; i < 72000000; i++)                             //等待DATA信号被拉低
        // {
        //         if(SHT10_DATA_R() == 0) break;             //检测到DATA被拉低了，跳出循环
        // }
        delay_ms(320);
        if(SHT10_Heshuqi_DATA_R() == 1)                                //如果等待超时了
                err += 1;
        value_H = SHT10_Heshuqi_ReadByte(ACK);
        value_L = SHT10_Heshuqi_ReadByte(ACK);
        *p_checksum = SHT10_Heshuqi_ReadByte(noACK);           //读取校验数据
        *p_value = (value_H << 8) | value_L;
        return err;
}

/*************************************************************
  Function   ：SHT10_Calculate
  Description：计算温湿度的值
  Input      : Temp-从传感器读出的温度值；Humi-从传感器读出的湿度值
               p_humidity-计算出的实际的湿度值；p_temperature-计算出的实际温度值
  return     : none
	mode       : 14bit温度采样     12bit湿度采样
14bit温度采样: d2=0.01   d1=-40.1(5V) d1=-39.8(4V) d1=-39.7(3.5V) d1=-39.6(3V) d1=-39.4(2.5V)
12bit湿度采样: t1=0.01 t2=0.00008 c1=-2.0468  c2=0.0367 c3=-1.5955E-6
	temperature: temperature=d1+d2*t        rh_linear = c1 + c2*rh + c3 * rh*rh
		 humidity: humidity = ( temperature - 25 ) * ( t1 + t2*rh )+ rh_linear
*************************************************************/
void SHT10_Heshuqi_Calculate(u16 t, u16 rh, u32 *p_temperature, u32 *p_humidity)
{
		unsigned long int RHlinear,RHture;
		t=t&0x3fff;
		rh=rh&0xfff;
		if(t>3965)t-=3965;   	//14bit   3.3V
		else
			t=0;

		RHlinear = rh*3.67-204.68-(rh*rh*1.6/10000);
		RHture = RHlinear+(t-2500)*(0.01+0.00008*rh/100);
		if(RHture>9999)
			RHture=9999;
		*p_humidity = RHture;
		*p_temperature = t;
}
/***************************************************************
读取温度，数据稳定下来需要320ms
***************************************************************/
uint8_t SHT10_Heshuqi_Get_Temp(uint32_t *_tempvalue)
{
	u8 err=0,checksum=0;
	u16 humi_val=0,temp_val;
	err += SHT10_Heshuqi_Measure(&temp_val, &checksum, HESHUQI1_TEMP);
	if(err != 0)
	{
		err = 1;
		SHT10_Heshuqi_ConReset();
	}
	else
	{
		SHT10_Heshuqi_Calculate(temp_val, humi_val,&HESHUQI_M_T,&HESHUQI_M_H);
		*_tempvalue = HESHUQI_M_T;
	}
  return err;
}
/***************************************************************
读取湿度，数据稳定下来需要320ms
***************************************************************/
uint8_t SHT10_Heshuqi_Get_Humi(uint32_t *_tempvalue)
{
	u8 err=0,checksum=0;
	u16 humi_val,temp_val=0;
	err += SHT10_Heshuqi_Measure(&humi_val, &checksum, HESHUQI1_HUMI);
	if(err != 0)
	{
		err = 1;
		SHT10_Heshuqi_ConReset();
	}
	else
	{
		SHT10_Heshuqi_Calculate(temp_val, humi_val, &HESHUQI_M_T, &HESHUQI_M_H);
		*_tempvalue = HESHUQI_M_H;
	}
  return err;
}
/************************************************************
  Function   ：SHT10_CalcuDewPoint
  Description：计算露点
  Input      : h-实际的湿度；t-实际的温度
  return     : dew_point-露点
**************************************************************/
float SHT10_Heshuqi_CalcuDewPoint(float t, float h)
{
	float logEx, dew_point;

  logEx = (log10(h)-2)/0.4343f + (17.62f*t)/(243.12f+t);
  dew_point = 243.12f*logEx/(17.62f-logEx);

	return dew_point;
}
