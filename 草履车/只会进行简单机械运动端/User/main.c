#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "nRF24L01.h"
#include "W25Q64.h"
#include "User.h"
#include "Motor.h"
#include "Timer.h"
#include "HC_SR04.h"
#include "Store.h"

//uint8_t mode,Speed,Time,Time_Flag,Data[4];
//uint8_t RxData1[256],RxData2[256];
uint8_t State,State2,a;

float Distance;

int main(void)          //防丢操作可以丢到主函数。Mode1的防丢因为BUG失效
{
	Mode_Init();
	HC_SR04_Init();
//	HC_SR04_Init();
//	HC_SR04_GPIO_Init();
//	HC_SR_04_TIME_NVIC_Init();
//	State = (Store_Data[0+2]);
//	Motor_State((uint8_t)Store_Data[0+3]);
	while (1)
	{
//		Distance = Get_Length();
		Distance = Get_Length();
//		NRF24L01_RxPacket(Data);
//////		Mode_8();
//		if((Data[0] & 0x1c) == 0x00) Mode_1();     			//实时控制
////		else if((Data[0] & 0x1c) == 0x04) Mode_2();			//观察模式
////		else if((Data[0] & 0x1c) == 0x08) Mode_3();			//跟随模式
////		else if((Data[0] & 0x1c) == 0x0c) Mode_4();			//避障模式
////		else if((Data[0] & 0x1c) == 0x10) Mode_5();			//睡眠模式
//		else if((Data[0] & 0x1c) == 0x14) Mode_6();			//启动记录
//		else if((Data[0] & 0x1c) == 0x18) Mode_7();			//启动回溯
//		else if((Data[0] & 0x1c) == 0x1c) Mode_8();			//记录操作
	}
}

//	W25Q64_Init();
//	W25Q64_SectorErase(0x000000);
//	W25Q64_PageProgram(0x000000, ArrayWrite, 4);
//	
//	W25Q64_ReadData(0x000000, ArrayRead, 4);
