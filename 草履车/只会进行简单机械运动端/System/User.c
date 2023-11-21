#include "stm32f10x.h" 
#include "Delay.h"
#include "Timer.h"
#include "nRF24L01.h"
#include "Motor.h"
#include "W25Q64.h"
#include "Store.h"
#include "HC_SR04.h"

#define  Mode_1_WarnBit 60000  //警告位，当一段时间没有收到控制信号，将自动急停。建议取值高些
#define	 Keep_Length  40      //跟随模式，应该保持的距离，取值0~450cm。			

uint8_t mode,Speed,Time,Time_Flag,Data[4],Mode_1_Cheak,Mode_1_Data,Mode,BackTrack_Flag,Record_Flag;
uint16_t WarnBit,BackTrack_Num,Store_Count,BackTrack_Count,HC_SR04_count,aaaaa,Temp;
uint8_t RxData[1020+16];


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

void  Mode_2()            //观察模式
{
	Servo_Init();
	HC_SR04_Init();
	while(1)
	{
		Servo_SetAngle(90);
	}
}


void Mode_3()  			//跟随模式，这个模式太蠢了，只能直线跟随，随便哪条狗都能拐跑草履车，什么图像识别？不知道。。。
{
	uint8_t Length,Temp,Gap;
	Gap = 20;                   //误差位，不然车太鬼畜了。
	HC_SR04_Init();
	while(1)
	{
		NRF24L01_RxPacket(Data);
		Length = Get_Length();
		if((Keep_Length - Length) == Gap | (Keep_Length - Length) == Gap) Length = Keep_Length; //相差Gap厘米，则不动。
		if(Length > Keep_Length)
		{
			Temp = (Data[0] & 0x03);       //摇杆可以控制跟随速度
			Motor_State(Temp + 0x20);
		}
		else
		{
			Temp = (Data[0] & 0x03);
			Motor_State(Temp + 0xc0);
		}
		if((Data[0] & 0x1c) != 0x08) break;
	}
	
}
	


void Mode_6()   //启动记录
{
	BackTrack_Count = (Store_Data[1] >> 8);
	Store_Count = 1;
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

void Mode_7()          //回溯模式，开始回溯
{
	Timer_Init();
	BackTrack_Count = (Store_Data[1]/2)+1;	//实验才是检测代码的唯一标准，执行对了就行。
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
	uint16_t Mode_7_Count = 0;
	while(1)
	{
		NRF24L01_RxPacket(Data);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断，按理说不该在里面执行大量程序，这会加重丢包（管他呢）
		{
			RxData[Mode_7_Count] = Data[0];
			if(Mode_7_Count == 1)   Buzz_Mode(2);    //计时开始
			Mode_7_Count++;
			if(Mode_7_Count>1020+16 | Data[1] == 0xff) 
			{
				MyDMA_Init((uint32_t)RxData,(uint32_t)Store_Data+2);
				MyDMA_Transfer();	
				Store_Data[1] = Mode_7_Count - 2;    
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



//void Mode_5()       //睡眠模式
//{
//	
//}




void TIM2_IRQHandler(void)               //没资源啊没资源，只能共用定时器中断了，希望不会出啥茬子。
{
	uint16_t State;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if((Data[0] & 0x1c) == 0x18)     //用于执行回溯
		{
			Temp ++;
			if(Temp%2 == 1)
			{
				aaaaa = ~(uint8_t)((Store_Data[BackTrack_Count] >> 8) & 0xe0);
				aaaaa &= 0xe0;
				aaaaa += (uint8_t)((Store_Data[BackTrack_Count] >> 8) & 0x03);
				Motor_State((uint8_t)aaaaa);				
			}
			else
			{
				aaaaa = ~((uint8_t)Store_Data[BackTrack_Count] & 0xe0);
				aaaaa &= 0xe0;
				aaaaa += ((uint8_t)Store_Data[BackTrack_Count] & 0x03);   
				Motor_State((uint8_t)aaaaa);
				BackTrack_Count --;
			}
			if(BackTrack_Count == 1)
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
				aaaaa = ((uint8_t)Store_Data[Store_Count+1]);
				Motor_State((uint8_t)aaaaa);
			}
			else
			{
				aaaaa = (Store_Data[Store_Count+1]>> 8);
				Motor_State((uint8_t)aaaaa);
				Store_Count++;
			}
			if(Temp == (uint8_t)Store_Data[1])
			{
				Motor_State(0);
				Record_Flag = 1;
				Buzz_Mode(2);    
			}
		}
		HC_SR04_count++;    //超超超声波计数用
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
