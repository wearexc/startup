#include "stm32f10x.h"                  // Device header
#include "nRF24L01.h"
#include "User.h"




int main(void)          
{
	Mode_Init();
	while (1)
	{
		NRF24L01_RxPacket(Data);
		if((Data[0] & 0x1c) == 0x00) Mode_0();     			//实时控制
		else if((Data[0] & 0x1c) == 0x04) Mode_1();			//观察模式
		else if((Data[0] & 0x1c) == 0x08) Mode_2();			//跟随模式
		else if((Data[0] & 0x1c) == 0x0c) Mode_3();			//避障模式
		else if((Data[0] & 0x1c) == 0x10) Mode_4();			//睡眠模式
		else if((Data[0] & 0x1c) == 0x14) Mode_5();			//启动记录
		else if((Data[0] & 0x1c) == 0x18) Mode_6();			//启动回溯
		else if((Data[0] & 0x1c) == 0x1c) Mode_7();			//记录操作
	}
}

