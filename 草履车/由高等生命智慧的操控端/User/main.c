#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Rocker.h"
#include "Key.h"
#include "nRF24L01.h"
#include "Timer.h"
#include "User.h"
#include "OLED.h"
#include "Rocker.h"

int main(void)
{
	Mode_0();
//	Mode_8();
//	OLED_Init();
//	OLED_BMP(11);
//	OLED_BMP(11);
	Mode_8();
	while (1)
	{
//		OLED_BMP(11);
//		OLED_BMP(1);
//		abbbc=Rocker_Data();
//			 if(Host_Mode == 0x00) Mode_1();     			//实时控制
//		else if(Host_Mode == 0x04) Mode_2();			//观察模式
//		else if(Host_Mode == 0x08) Mode_3();			//跟随模式
//		else if(Host_Mode == 0x0c) Mode_4();			//避障模式
//		else if(Host_Mode == 0x10) Mode_5();			//睡眠模式
//		else if(Host_Mode == 0x14) Mode_6();			//启动记录
//		else if(Host_Mode == 0x18) Mode_7();			//启动回溯
//		else if(Host_Mode == 0x1c) Mode_8();			//记录操作

		OLED_BMP(Rocker_Data());
		OLED_ShowString(1,8,"Speed");
		OLED_ShowHexNum(1,5,Rocker_Data(),2);
		OLED_ShowNum(2,8,Speed,3);
		OLED_ShowNum(2,6,(Rocker_Data() >> 2) & 0x07,1);
//		OLED_ShowNum(3,5,BackTrack,3);
////		OLED_ShowNum(3,9,CountDown,3);
//		OLED_ShowNum(4,1,TxData[1],3);
//		OLED_ShowNum(4,5,Time,3);
//		OLED_ShowNum(4,9,History_Time,3);
		
		
	}
}
