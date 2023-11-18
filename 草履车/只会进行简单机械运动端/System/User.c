#include "stm32f10x.h" 
#include "Delay.h"
#include "Timer.h"
#include "nRF24L01.h"
#include "Motor.h"
#include "W25Q64.h"
#include "Store.h"

#define  Mode_1_WarnBit 50  //警告位，当一段时间没有收到控制信号，将自动急停。

uint8_t mode,Speed,Time,Time_Flag,Data[4],Mode_1_Cheak,Mode_1_Data,Mode,temp,BackTrack_Flag,Record_Flag;
uint16_t WarnBit,BackTrack_Num,BackTrack_Count;
uint8_t RxData0[256+16],RxData1[256];


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



void Mode_Init()             //上电模式，用于自检
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

void Mode_6()          //回溯模式，开始回溯
{
	Timer_Init();
	BackTrack_Count = RxData1[0];
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

void Mode_7()            //回溯模式，开始计时
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
			if(Mode_7_Count>256+16 | Data[1] == 0xff) 
			{
				RxData0[15] = (Mode_7_Count-16);  //-16是直接在程序上修正，按理说不需要。对，按理。  
				MyDMA_Init((uint32_t)RxData0+15,(uint32_t)RxData1);
				MyDMA_Transfer();	
				MyDMA_Init((uint32_t)RxData0+15,(uint32_t)Store_Data+2);
				MyDMA_Transfer();	
				Store_Save();				
				goto Mode_7_Break;                                     
			}
		}
		if(Mode_7_Count>=14)
		{

			Motor_State(Data[0]);			
		}
	}
	Mode_7_Break:
	Buzz_Mode(2);         //计时结束
}

void Mode_8()   //执行记录
{
	BackTrack_Count = Store_Data[2];
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
	uint8_t State,State2;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if((Data[0] & 0x1c) == 0x14)     //用于执行回溯
		{
			State = ~(RxData1[BackTrack_Count] & 0xef);
			State += (RxData1[BackTrack_Count] & 0x04);
			Motor_State(State);
			BackTrack_Count--;
			if(BackTrack_Count == 0)
			{
				Motor_State(0);
				BackTrack_Flag = 1;
				Buzz_Mode(2);    
			}
		}
		//		if((Data[0] & 0x1c) == 0x1c)     //用于执行已记录的操作  //改处理FLASH数据，它是顺序执行的。如果舍弃睡眠模式，说不定能再加个按钮，一个操控回溯，一个操控历史操作。
//		{
//			State = (Store_Data[BackTrack_Count+2] & 0xef);
//			State += (Store_Data[BackTrack_Count+2] & 0x04);
//			Motor_State(State);
//			BackTrack_Count+2;
//			if(BackTrack_Count == 0)
//			{
//				Motor_State(0);
//				Record_Flag = 1;
//				Buzz_Mode(2);    
//			}
//		}
		
		
		
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
