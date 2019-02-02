#include "main.h"
char tt=0;
volatile int8_t x,y,z;


/* Private function prototypes -----------------------------------------------*/

//cau hinh xung clock he thong chay 168 Mhz
void cauhinhrcc()
{
	RCC_DeInit();							//xoa cau hinh cu
	RCC_HSICmd(DISABLE);			//tat hsi
	RCC_HSEConfig(RCC_HSE_ON);		//bat che do hse
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY)==RESET);		//kiem tra co trang thai cua hse 
	FLASH_PrefetchBufferCmd(ENABLE);										//bat flash
	FLASH_SetLatency(FLASH_Latency_5);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);								//ahb pre
	RCC_PCLK1Config(RCC_HCLK_Div4);						//apb1
	RCC_PCLK2Config(RCC_HCLK_Div4);
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,4);			//cau hinh bo nhan pll
	RCC_PLLCmd(ENABLE);				//cho phep pll hoat dong
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);		//cho trang thai cua pll
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);					//chon nguon xung vao sysclk
	while(RCC_GetSYSCLKSource()!=0x08);
}

void spi_tx(uint8_t add,uint8_t data)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	//Dang la che do SPI write
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,add);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,data);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
}

uint8_t spi_rx(uint8_t add)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	add=0x80|add;//chuyen qua che do SPI read
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,add);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,0x00);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	return SPI_I2S_ReceiveData(SPI1);
}

void cauhinhchan()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOC, ENABLE);//xung cap tu ahb1 168Mhz cho portD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   // cau hinh ngo ra
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//kieu day keo
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//toc do toi da 100Mhz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//khong dien tro keo
  GPIO_Init(GPIOD, &GPIO_InitStructure);						//set cau hinh cho ngo ra theo bien GPIO_InitStructure
	
	//Cau hinh chan A5,6,7 dung chuc nang thay the AF5 datasheet 272
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	/* GPIOC Configuration: TIM3 CH1 (PC6), TIM3 CH2 (PC7), TIM3 CH3 (PC8) and TIM3 CH4 (PC9) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	/* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	//Cau hinh AF chan 5 6 7 o che do SPI
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	//khoi tao SPI
	SPI_InitTypeDef spi;
	spi.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	spi.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	spi.SPI_Mode=SPI_Mode_Master;
	spi.SPI_DataSize=SPI_DataSize_8b;
	spi.SPI_FirstBit=SPI_FirstBit_MSB;
	spi.SPI_CPOL=SPI_CPOL_High;
	spi.SPI_CPHA=SPI_CPHA_2Edge;
	spi.SPI_NSS=SPI_NSS_Soft|SPI_NSSInternalSoft_Set;
	SPI_Init(SPI1,&spi);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	SPI_Cmd(SPI1,ENABLE);
	delay_ms_systick(1000);
}

int main(void)
{
	DutyCycle aProp;
  cauhinhrcc();
	cauhinhchan();
	InitialisePWM();
	spi_tx(0x20,0x67);
	aProp = InitialisePWMChannel(GPIOC, 6, GPIO_PinSource6, 1);
	delay_ms_systick(2000);
	for(int g=0;g<100;g++){
		aProp.update(10);
		delay_ms_systick(1000);
	}
  while (1)
  {
		x=spi_rx(0x29); 				//tra datasheet cua lis302dl
		y=spi_rx(0x2b);
		z=spi_rx(0x2d);
		if(x<-20){GPIO_SetBits(GPIOD,GPIO_Pin_12);aProp.update(1200);}
		else GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		if(x>20) GPIO_SetBits(GPIOD,GPIO_Pin_14);
		else GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		if(y<-20) GPIO_SetBits(GPIOD,GPIO_Pin_15);
		else GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		if(y>20) GPIO_SetBits(GPIOD,GPIO_Pin_13);
		else GPIO_ResetBits(GPIOD,GPIO_Pin_13);
  }
}









#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
