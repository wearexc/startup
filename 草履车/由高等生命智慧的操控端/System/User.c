#include "stm32f10x.h"
#include "Rocker.h"
#include "Delay.h"
#include "OLED.h"
#include "Rocker.h"
#include "Key.h"
#include "nRF24L01.h"
#include "Timer.h"

#define TimeLimit   60                      //记录操作的时间上限为x秒,参数范围 0~204秒，不建议超过1分钟，否则芯片写入FLASH将耗费大量时间。

uint8_t direction[2];
uint8_t Observe[] = {0x02,0x05,0x08,0x0b,0x0e};
uint8_t demo,Num,abc,Time,Time_Flag,History_Time;
uint16_t i;
uint8_t Data[4],RxData[4],Check[1] = {0xee};



//所有驱动进行初始化
//摇杆

void Mode_Init()           //初始化
{
	OLED_Init();
	NRF24L01_Init();
	Rocker_Init();
	Key_Init();
	NRF24L01_Tx_Mode();
}

void Mode_0()           //实时操控
{
	Data[0] = Rocker_Data();
	NRF24L01_TxPacket(Data);
}

void Mode_1()           //观察模式
{
	uint8_t i;
	NRF24L01_Rx_Mode();
	OLED_Clear();
	uint8_t Angel;
	uint16_t Length;
	OLED_ShowString(1,1,"Mode");
	OLED_ShowString(2,1," 1     xxx      ");
	OLED_ShowString(3,1,"    xxx | xxx   ");
	OLED_ShowString(4,1,"xxx ___\\|/___xxx");   //特殊字符使用“\”需要用“\\”来表示
	while(1)
	{
		Rocker_Data();
		NRF24L01_RxPacket(Data);
		Length =  Data[2] * 256 + Data[3];
		Angel = (Data[1] & 0xf0) >> 4;
		switch(Angel)
		{
			case 0 : OLED_ShowNum(4,14,Length,3); break;
			case 1 : OLED_ShowNum(3,11,Length,3); break;
			case 2 : OLED_ShowNum(2,8, Length,3); break;
			case 3 : OLED_ShowNum(3,5, Length,3); break;
			case 4 : OLED_ShowNum(4,1, Length,3); break;
		}
		if(Host_Mode != 0x01) break;
	}
	NRF24L01_Tx_Mode();
}

void Mode_7()           //记录操作
{
	Timer_Init();
	Time_Flag = 0;
	while(1)
	{
		if(Data[1] == 0xff) break;
	}
	TIM_Cmd(TIM2, DISABLE);
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Data[0] = Rocker_Data();

		NRF24L01_TxPacket(Data);

		if(BackTime_Flag == 1)
		{
//			TxData[1]++;            //TxData[1]数据无关紧要了，如果保留则可以在接收端接收TxData[1]，查看数据丢包情况。
			Time_Flag++;
			if(Time_Flag == 5) 
			{
				Time++;
				Time_Flag = 0;
			}
			if(Time == TimeLimit) 
			{
				Data[1] = 0xff;     //配置上限时间，达到则置标志位。也可以用TxData[0]规定特殊数据，则可完全舍弃TxData[1]
				Time_Flag = 1;      //计时结束
			}    
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
