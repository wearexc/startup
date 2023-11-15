#include "stm32f10x.h" 
#include "Delay.h"
#include "Timer.h"
#include "nRF24L01.h"
#include "Motor.h"
#include "W25Q64.h"

#define  Mode_1_WarnBit 50  //警告位，当一段时间没有收到控制信号，将自动急停。

uint8_t mode,Speed,Time,Time_Flag,Data[4],Mode_1_Cheak,Mode_1_Data,Mode;
uint8_t Mode_Flag,BackTime_Flag,BackTrack_Flag,temp,Num;
uint16_t WarnBit;
uint8_t RxData1[256],RxData2[256];


void Status(uint8_t Data)  //处理小车状态
{
//	uint8_t Temp;
	temp = Data & 0x1c;
	if(temp == 0x00) Mode = 0;
		else
		{
			Mode = temp/4;
		}
}

void SignWarn()     //信号警告，失去控制时紧急制动。
{
	
}



void Mode_0()             //上电模式，用于自检
{
	Motor_Init();
	W25Q64_Init();
	NRF24L01_Init();
	NRF24L01_Rx_Mode();
	Mode_1_Cheak = 0;
	Mode = 0;
	WarnBit = Mode_1_WarnBit;
	//嗡鸣器响起1s;
}

void Mode_1()            //实时遥控模式
{
	NRF24L01_RxPacket(Data);
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

void Mode_2()            //回溯模式
{
	Timer_Init();
	NRF24L01_RxPacket(Data);

}






void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Motor_State(Data[0]);
		if(temp != Data[1])
		{
			Status(Data[0]);
		}
		temp = Data[1];
		RxData1[Num] = Data[1];
		Num++;
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