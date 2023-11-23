#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*定时器初始化*/
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

/*DMA初始化*/
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB)
{
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 256;     //搬运次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
}

/*DMA传输*/
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 256);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

/*嗡鸣器初始化*/
void Buzz_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

/*嗡鸣器模式*/
void Buzz_Mode(uint8_t Sound)
{
	if(Sound == 1)    //滴滴两声，表示模式切换
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(900);
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(600);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(900);
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	
	}
	if(Sound == 2)    //滴滴一声，表示开始或完成提示
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(900);
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(600);
	}
	if(Sound == 3)    //警钟长鸣
	{
		uint8_t i;
		for(i=0;i<20;i++)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			Delay_ms(1000);
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			Delay_ms(1000);
		}
	}
//	if(Sound == 4)
//	{
//		uint8_t i;
//		for(i = 0 ;i < 30;i++)
//		{
//			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//			Delay_ms(100);
//			GPIO_SetBits(GPIOA, GPIO_Pin_0);
//			Delay_ms(100);
//		}

//	}
}



