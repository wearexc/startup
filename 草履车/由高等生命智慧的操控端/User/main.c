#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Rocker.h"
#include "Key.h"
#include "nRF24L01.h"
#include "Timer.h"
#include "User.h"


int main(void)
{
	Mode_0();
	Mode_8();
	while (1)
	{
		OLED_ShowHexNum(2,1,Rocker_Data(),2);
		OLED_ShowNum(2,5,Speed,3);
		OLED_ShowNum(3,1,Mode,3);
		OLED_ShowNum(3,5,BackTrack,3);
//		OLED_ShowNum(3,9,CountDown,3);
		OLED_ShowNum(4,1,TxData[1],3);
		OLED_ShowNum(4,5,Time,3);
		OLED_ShowNum(4,9,History_Time,3);
		
		
	}
}
