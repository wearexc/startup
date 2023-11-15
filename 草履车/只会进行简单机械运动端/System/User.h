#ifndef __USER_H
#define __USER_H

extern uint8_t mode,Speed,Time,Time_Flag,Data[4];
extern uint8_t RxData1[256],RxData2[256];


void Status(uint8_t Data);  //处理小车状态
void Mode_0();
void Mode_1();
void Mode_2();

#endif
