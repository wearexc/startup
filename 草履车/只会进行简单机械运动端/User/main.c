#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "nRF24L01.h"
#include "W25Q64.h"
#include "User.h"
#include "Motor.h"
#include "Timer.h"

//uint8_t mode,Speed,Time,Time_Flag,Data[4];
//uint8_t RxData1[256],RxData2[256];

int main(void)
{

	Mode_0();
//	Motor_SetLiftSpeed(40);
	while (1)
	{
		Mode_2();
//		Status(Data[0]);
//		Mode_2();
	}
}

//	W25Q64_Init();
//	W25Q64_SectorErase(0x000000);
//	W25Q64_PageProgram(0x000000, ArrayWrite, 4);
//	
//	W25Q64_ReadData(0x000000, ArrayRead, 4);
