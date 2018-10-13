/**
  ******************************************************************************
  * @file    bsp_publicIO.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   公共的IO定义
  ******************************************************************************
  */
  #include "bsp_spi.h"

  /*
  *********************************************************************************************************
  *	函 数 名: SPI2_Init
  *	功能说明: 以下是SPI模块的初始化代码，配置成主机模式
  *           SPI口初始化
  *           这里针是对SPI2的初始化
  *	形    参:  无
  *	返 回 值: 无
  *********************************************************************************************************
  */
void Bsp_SPI_Init(SPI_TypeDef* SPIx)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  //检查参数
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  //时钟使能
  RCC_AHB1PeriphClockCmd(SPI_SCK_GPIO_CLK|
                          SPI_MISO_GPIO_CLK|
                          SPI_MOSI_GPIO_CLK|
						  SPI_CS_GPIO_CLK, ENABLE);//使能GPIOB时钟


  if((SPIx == SPI1)||(SPIx == SPI4)||(SPIx == SPI5)||(SPIx == SPI6))
  {
    RCC_APB2PeriphClockCmd(SPI_CLK, ENABLE);//使能SPI1时钟
  }
  else
  {
    RCC_APB1PeriphClockCmd(SPI_CLK, ENABLE);//使能SPI2时钟
  }

  //引脚复用
  GPIO_PinAFConfig(SPI_SCK_GPIO_PORT,SPI_SCK_PINSOURCE,SPI_SCK_AF);
  GPIO_PinAFConfig(SPI_MISO_GPIO_PORT,SPI_MISO_PINSOURCE,SPI_MISO_AF);
  GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT,SPI_MOSI_PINSOURCE,SPI_MOSI_AF);

  //引脚配置SPI复用
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉

  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStructure);//初始化

  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStructure);//初始化

  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);//初始化

  //推挽输出
  GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  //这里只针对SPI口初始化
  if((SPIx == SPI1)||(SPIx == SPI4)||(SPIx == SPI5)||(SPIx == SPI6))
  {
	   RCC_APB2PeriphResetCmd(SPI_CLK,ENABLE);//复位SPI
	   RCC_APB2PeriphResetCmd(SPI_CLK,DISABLE);//停止复位SPI
  }
  else
  {
     RCC_APB1PeriphResetCmd(SPI_CLK,ENABLE);
     RCC_APB1PeriphResetCmd(SPI_CLK,DISABLE);
  }
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPIx, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPIx, ENABLE); //使能SPI外设

	SPI_ReadWriteByte(SPIx,0xff);//启动传输

}

/*
*********************************************************************************************************
*	函 数 名: SPI_SetSpeed
*	功能说明: SPI速度设置函数
*          SPI速度=fAPB2/分频系数
*          fAPB2时钟一般为84Mhz：
*	形    参:  @ref SPIx
*           @ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPIx->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPIx->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度
	SPI_Cmd(SPIx,ENABLE); //使能SPI
}
/*
*********************************************************************************************************
*	函 数 名: SPI_ReadWriteByte
*	功能说明: SPI读写一个字节
*          SPI速度=fAPB2/分频系数
*          fAPB2时钟一般为84Mhz：
*	形    参:  @ref SPIx
*           @ref TxData:要写入的字节
*	返 回 值: 读取到的字节
*********************************************************************************************************
*/
uint8_t SPI_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t TxData)
{

  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空

  SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个byte  数据

  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte

  return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据

}
