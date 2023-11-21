#ifndef _HC_SR04_H
#define	_HC_SR04_H
 
uint32_t Get_Timer(void);
static void OpenTIM2();
static void CloseTIM2();
void HC_SR04_Init();
void HC_SR04_GPIO_Init();
void HC_SR_04_TIME_NVIC_Init();
float Get_Length();
 
#endif
