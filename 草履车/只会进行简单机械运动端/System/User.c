#include "stm32f10x.h" 
#include "Delay.h"
#include "Timer.h"
#include "nRF24L01.h"
#include "Motor.h"
#include "W25Q64.h"
#include "Store.h"

#define  Mode_1_WarnBit 50  //警告位，当一段时间没有收到控制信号，将自动急停。

uint8_t mode,Speed,Time,Time_Flag,Data[4],Mode_1_Cheak,Mode_1_Data,Mode,BackTrack_Flag,Record_Flag;
uint16_t WarnBit,BackTrack_Num,BackTrack_Count,aaaaa,Temp;
uint8_t RxData0[1020+16];


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



void Mode_Init()             //上电模式，用于初始化
{
	Motor_Init();
	W25Q64_Init();
	NRF24L01_Init();
	NRF24L01_Rx_Mode();
	Buzz_Init();
	Store_Init();
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
//		Mode_8();
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

void Mode_7()          //回溯模式，开始回溯
{
	Timer_Init();
//	if(Store_Data[1] % 2 == 1)
//	{
//		BackTrack_Count = ((Store_Data[1]/2) + 1);
//	}
//	else
//	{
		BackTrack_Count = (Store_Data[1]/2)+1;	
//}
	while(1)
	{
		if(BackTrack_Flag == 1)
		{
			BackTrack_Flag = 0;
			TIM_Cmd(TIM2,DISABLE);
			goto Mode_6_Break;
		}
	}
	Mode_6_Break:
	Buzz_Mode(3);         
	Buzz_Mode(3);         //执行完毕,提醒用户切换模式，嗡鸣结束后回溯操作将再进行一次。
	
}	

void Mode_8()            //记录操作
{
//	Timer_Init();
//	Buzz_Mode(1);
	uint16_t Mode_7_Count = 0;
//	Store_Clear();
	while(1)
	{
		NRF24L01_RxPacket(Data);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断，按理说不该在里面执行大量程序，这会加重丢包（管他呢）
		{
			RxData0[Mode_7_Count] = Data[0];
			if(Mode_7_Count == 16)
				{
					Buzz_Mode(2);    //计时开始
				}
			Mode_7_Count++;
			if(Mode_7_Count>1020+16 | Data[1] == 0xff) 
			{
				MyDMA_Init((uint32_t)RxData0+15,(uint32_t)Store_Data+2);
				MyDMA_Transfer();	
				Store_Data[1] = Mode_7_Count-16;  //-16是直接在程序上修正，按理说不需要。对，按理。  
				Store_Save();				
				goto Mode_7_Break;                                     
			}
		}
		if(Mode_7_Count>=15)
		{

			Motor_State(Data[0]);			
		}
	}
	Mode_7_Break:
	Buzz_Mode(2);         //计时结束
}

void Mode_6()   //启动记录
{
	BackTrack_Count = (Store_Data[1] >> 8);
	Temp = 0;
	Timer_Init();
	while(1)
	{
		if(Record_Flag == 1)
		{
			Record_Flag = 0;
			TIM_Cmd(TIM2,DISABLE);
			goto Mode_8_Break;			
		}
	}
	Mode_8_Break:
	Buzz_Mode(3);         
	Buzz_Mode(3);         //执行完毕,提醒用户切换模式，嗡鸣结束后操作将再进行一次。	
}

//void Mode_5()       //睡眠模式
//{
//	
//}




void TIM2_IRQHandler(void)             //没资源啊没资源，只能共用定时器中断了，希望不会出啥茬子。
{
	uint16_t State;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if((Data[0] & 0x1c) == 0x18)     //用于执行回溯
		{
			Temp ++;
			if(Temp%2 == 1)
			{
				aaaaa = ~((uint8_t)Store_Data[BackTrack_Count] & 0xe0);
				aaaaa &= 0xe0;
				aaaaa += ((uint8_t)Store_Data[BackTrack_Count] & 0x03);
				Motor_State((uint8_t)aaaaa);				
			}
			else
			{
				aaaaa = ~((uint8_t)Store_Data[BackTrack_Count] & 0xe0);
				aaaaa &= 0xe0;
				aaaaa += ((uint8_t)Store_Data[BackTrack_Count] & 0x03);
				Motor_State((uint8_t)aaaaa);;
				BackTrack_Count --;
			}
			if(BackTrack_Count == 2)
			{
				Motor_State(0);
				BackTrack_Flag = 1;
				Buzz_Mode(2);    
			}
		}
		if((Data[0] & 0x1c) == 0x14)     //用于执行已记录的操作  
		{
			Temp++;
			if(Temp%2 == 1)
			{
				aaaaa = (Store_Data[Temp+2]);
				Motor_State((uint8_t)aaaaa);
			}
			else
			{
				aaaaa = ((Store_Data[Temp+2] >> 8));
				Motor_State((uint8_t)aaaaa);
			}
			if(Temp == (uint8_t)Store_Data[1])
			{
				Motor_State(0);
				Record_Flag = 1;
				Buzz_Mode(2);    
			}
		}
		
		
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}



//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Data = ~(RxData1[Num] & 0xef);
//		Data += (RxData1[Num] & 0x04);
//		Motor_State(Data);
//		Num--;
//		if(Num == 0)
//		{
//			Motor_State(0);
//			BackTrack_Flag = 1;
//		}
//		
		
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
