#ifndef __USER_H
#define __USER_H

extern uint8_t direction[2];

extern uint8_t demo,Num,abc,Time,Time_Flag,History_Time,CountDown;
extern uint16_t i;
extern uint8_t Data[3],RxData[4];

void Mode_Init();
void Mode_0();
void Mode_1();
void Mode_2();
void Mode_4();

void Mode_7();
#endif
