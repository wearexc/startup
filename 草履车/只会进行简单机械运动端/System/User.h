#ifndef __USER_H
#define __USER_H

extern uint8_t mode,Speed,Time,Time_Flag,Data[4];
extern uint8_t RxData1[256],RxData2[256];


void Status(uint8_t Data);  //处理小车状态
void Mode_Init();
void Mode_0();
void Mode_1();
void Mode_2();
void Mode_4();
void Mode_5();
void Mode_6();
void Mode_7();
void Mode_8();
#endif
