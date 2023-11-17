#include "stm32f10x.h"                  // Device header
#include "Key.h" 
#include "User.h" 

//摇杆的键程，取值为0~4095，摇杆不动时为2030左右。
#define TopLimit         3000
#define LowerLimit       1000

uint16_t Rocker[2];  //Rocker[0]为y轴，1为x轴

void Rocker_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //开启APB2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
	
//配置ADC工作模式和DMA模式
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_Init(ADC1, &ADC_InitStructure);
// 配置DMA通道，用于传输ADC数据
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rocker;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
// 启用DMA通道
    DMA_Cmd(DMA1_Channel1, ENABLE);
// 启用ADC的DMA传输
    ADC_DMACmd(ADC1, ENABLE);
// 启用ADC
    ADC_Cmd(ADC1, ENABLE);
// 复位ADC校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
// 开始ADC校准
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
// 启动ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint8_t Rocker_Data()   //摇杆按键数据处理，注释为xy轴方向。
{
	uint8_t temp;
	//摇杆处理
	if(Rocker[0] < LowerLimit && Rocker[1] < TopLimit && Rocker[1] > LowerLimit) (temp = 0x20);          //y+
	else if(Rocker[0] > TopLimit && Rocker[1] < TopLimit && Rocker[1] > LowerLimit) (temp = 0xc0);       //y-
	else if(Rocker[0] < LowerLimit && Rocker[1] < LowerLimit ) (temp = 0x40);   				    	 //x+,y+
	else if(Rocker[0] > TopLimit && Rocker[1] > TopLimit ) (temp = 0xa0); 						         //x-,y-
	else if(Rocker[0] > TopLimit && Rocker[1] < LowerLimit ) (temp = 0x60); 					    	 //x+,y-
	else if(Rocker[0] < LowerLimit && Rocker[1] > TopLimit ) (temp = 0x80); 					    	 //x-,y+
	else (temp = 0xe0);
	//按键处理
	if(Speed == 40) (temp += 1);
	else if(Speed == 70) (temp += 2);
	else if(Speed == 100) (temp += 3);
	
	
	if(BackTrack_Flag == 1)  //启动回溯
	{
		temp += 28;
		TxData[1] = 0;
//		BackTrack = 0;
	}
	else if(BackTime_Flag == 1)    //开始回溯计时
	{
		temp += 20;
	}
	else if(BackTime_Flag == 2)    //回溯计时结束
	{
		temp += 24;
		BackTime_Flag = 0;
//		BackTrack = 1;
	}
	else 
	{
		temp += 4*Mode;           //置模式位
	}
	
	return temp;
}
