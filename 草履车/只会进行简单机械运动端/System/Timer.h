#ifndef __TIMER_H
#define __TIMER_H

void Timer_Init(void);
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB);
void MyDMA_Transfer(void);

void Buzz_Init();
void Buzz_Mode(uint8_t Mode);
#endif
