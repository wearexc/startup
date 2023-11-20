#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Rocker.h"
#include "Key.h"
#include "nRF24L01.h"
#include "Timer.h"
#include "User.h"
#include "OLED.h"

uint8_t abbbc;
int main(void)
{
	Mode_0();
//	Mode_8();
//	OLED_Init();
//	OLED_BMP(11);
//	OLED_BMP(11);
	while (1)
	{
//		OLED_BMP(11);
//		OLED_BMP(1);
//		abbbc=Rocker_Data();
		OLED_BMP(Rocker_Data());
//		OLED_ShowHexNum(3,1,Rocker_Data(),2);
//		OLED_ShowNum(2,5,Speed,3);
//		OLED_ShowNum(3,1,Mode,3);
//		OLED_ShowNum(3,5,BackTrack,3);
////		OLED_ShowNum(3,9,CountDown,3);
//		OLED_ShowNum(4,1,TxData[1],3);
//		OLED_ShowNum(4,5,Time,3);
//		OLED_ShowNum(4,9,History_Time,3);
		
		
	}
}
