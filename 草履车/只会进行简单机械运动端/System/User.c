#include "stm32f10x.h" 
#include "Delay.h"
#include "Timer.h"
#include "nRF24L01.h"
#include "Motor.h"
#include "W25Q64.h"

#define  Mode_1_WarnBit 50  //警告位，当一段时间没有收到控制信号，将自动急停。

uint8_t mode,Speed,Time,Time_Flag,Data[4],Mode_1_Cheak,Mode_1_Data,Mode,temp,BackTrack_Flag;
uint16_t WarnBit,Num;
uint8_t RxData0[256+15],RxData1[256];


void Status(uint8_t Data)  //处理小车状态
{
//	uint8_t Temp;
//	temp = Data & 0x1c;
//	if(temp == 0x00) Mode = 0;
//		else
//		{
//			Mode = temp/4;
//		}
}

void SignWarn()     //信号警告，失去控制时紧急制动。
{
	
}

void Mode_3()          //回溯模式，开始回溯
{
	Timer_Init();
	while(1)
	{
		
	}
	
}

void Mode_0()             //上电模式，用于自检
{
	Motor_Init();
	W25Q64_Init();
	NRF24L01_Init();
	NRF24L01_Rx_Mode();
	Buzz_Init();
	Mode_1_Cheak = 0;
	Mode = 0;
	WarnBit = Mode_1_WarnBit;
//	W25Q64_Init();
//	W25Q64_SectorErase(0x000000);
	//嗡鸣器响起1s;
}

void Mode_1()            //实时遥控模式
{
	NRF24L01_RxPacket(Data);
	if((Data[0] & 0x1c) == 0x1c)
	{
		Mode_3();
	}
	if(Mode_1_Cheak == 0)
	{
		Motor_State(Data[0]);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断
	{
		WarnBit = Mode_1_WarnBit;
		//关闭嗡鸣器警告
	}
	else WarnBit--;
	if(WarnBit == 0)
	{
		Mode_1_Cheak = 1;
		Motor_State(0);
		//嗡鸣器警告
	}
}

	

void Mode_2()            //回溯模式，开始计时
{
//	Timer_Init();
//	Buzz_Mode(1);
	uint16_t abc = 0;
	while(1)
	{
		NRF24L01_RxPacket(Data);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断
		{
			RxData0[abc] = Data[0];
			if(abc == 15)
				{
					Buzz_Mode(2);
				}
			abc++;
			if(abc>256+15 | Data[1] == 0xff) 
			{
				MyDMA_Init((uint32_t)RxData0+15,(uint32_t)RxData1);
				MyDMA_Transfer();
				Num = abc;
				Mode_1();//go to Mode 1 
			}
		}
		if(abc>=14)
		{

			Motor_State(Data[0]);			
		}
	}

}






void TIM2_IRQHandler(void)
{
	uint8_t Data;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Data = ~(RxData1[Num] & 0xef);
		Data += (RxData1[Num] & 0x04);
		Motor_State(Data);
		Num--;
		if(Num == 0)
		{
			Motor_State(0);
			BackTrack_Flag = 1;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

/*
uint8_t Mode_2()            //观察模式
{
	
}

uint8_t Mode_3()            //回溯模式
{
	
}

uint8_t Mode_4()            //自动避障模式
{
	
}

uint8_t Mode_5()            //跟随模式
{
	
}

uint8_t Mode_6()            //休眠模式
{
	
}
*/
