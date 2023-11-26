#ifndef __USER_H
#define __USER_H

extern uint8_t Data[4],RxData0[1020+16];
extern uint16_t HC_SR04_count;

//extern uint8_t mode,Speed,Time,Time_Flag,Data[4];
//extern uint8_t RxData0[1020+16];
//extern uint16_t HC_SR04_count;

void Mode_Init(void);
void Mode_0(void);
void Mode_1(void);
void Mode_2(void);
void Mode_3(void);
void Mode_4(void);
void Mode_5(void);
void Mode_6(void);
void Mode_7(void);
#endif
