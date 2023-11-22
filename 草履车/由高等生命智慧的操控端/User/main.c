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
	Mode_Init();
	while (1)
	{
		Data[0] = Rocker_Data();
		NRF24L01_TxPacket(Data);

			 if(Host_Mode == 0x00) Mode_0();     		//实时控制
		else if(Host_Mode == 0x01) Mode_1();			//观察模式
//		else if(Host_Mode == 0x02) Mode_2();			//跟随模式
//		else if(Host_Mode == 0x03) Mode_3();			//避障模式
//		else if(Host_Mode == 0x04) Mode_4();			//睡眠模式
//		else if(Host_Mode == 0x05) Mode_5();			//启动记录
//		else if(Host_Mode == 0x06) Mode_6();			//启动回溯
//		else if(Host_Mode == 0x07) Mode_7();			//记录操作

		OLED_BMP(Rocker_Data());
		OLED_ShowString(1,11,"Mode");
		OLED_ShowNum(2,13,(Rocker_Data() >> 2) & 0x07,1);
		OLED_ShowString(3,11,"Speed");
		OLED_ShowNum(4,12,Speed,3);
		OLED_ShowHexNum(1,5,Rocker_Data(),2);
		

		

//		OLED_ShowNum(3,5,BackTrack,3);
////		OLED_ShowNum(3,9,CountDown,3);
//		OLED_ShowNum(4,1,TxData[1],3);
//		OLED_ShowNum(4,5,Time,3);
//		OLED_ShowNum(4,9,History_Time,3);
		
		
	}
}
