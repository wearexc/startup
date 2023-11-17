#include "stm32f10x.h"
#include "Rocker.h"
#include "Delay.h"
#include "OLED.h"
#include "Rocker.h"
#include "Key.h"
#include "nRF24L01.h"
#include "Timer.h"

#define TimeLimit   50                      //发送时间数据的上限为50秒,参数范围 0~50

uint8_t direction[2];

uint8_t demo,Data,Num,abc,Time,Time_Flag,History_Time,CountDown;
uint16_t i;
uint8_t TxData[2],RxData[4],Check[1] = {0xee};


//所有驱动进行初始化
//摇杆

void Mode_0()           //初始化
{
	OLED_Init();
	NRF24L01_Init();
	Rocker_Init();
	Key_Init();
	NRF24L01_Tx_Mode();
	CountDown = 5;
}

void Mode_1()           //实时操控
{
	TxData[0] = Rocker_Data();
	NRF24L01_TxPacket(TxData);
}

void Mode_2()           //回溯模式
{
	Timer_Init();
//	TxData[0] = Rocker_Data();
//	NRF24L01_TxPacket(TxData);
}
void Rocker_direction(void)   //0为默认，1为直行，2左转，3右转，4直退，5
{
	if(Rocker[0]>3000)
	{
		direction[0] = 1 ;
	}
	if(Rocker[0]<1000)
	{
		direction[0] = 2 ;
	}
	if(Rocker[1]>3000)
	{
		direction[1] = 1 ;
	}
	if(Rocker[1]<1000)
	{
		direction[1] = 2 ;
	}
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TxData[0] = Rocker_Data();

		NRF24L01_TxPacket(TxData);

		if(BackTime_Flag == 1)
		{
			TxData[1]++;
			Time_Flag++;
			if(Time_Flag == 5) 
			{
				Time++;
				Time_Flag = 0;
				CountDown -- ;
			}
			if(TxData[1] == TimeLimit*5) TxData[1] = 0;     //配置上限时间
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
