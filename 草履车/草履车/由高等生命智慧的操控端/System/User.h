#ifndef __USER_H
#define __USER_H


extern uint8_t Time,Time_Flag,History_Time;
extern uint16_t i;
extern uint8_t Data[3];

void Mode_Init(void);
void Mode_0(void);
void Mode_1(void);
void Mode_2(void);
void Mode_4(void);
void Mode_7(void);
#endif
