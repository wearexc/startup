 #include	"stm32f10x.h"
#include	"Delay.h"
#include 	"User.h"
 
#define RCC_GPIO	RCC_APB2Periph_GPIOA
#define GPIOx 		GPIOA
#define trigGPIO 	GPIO_Pin_2
#define echoGPIO 	GPIO_Pin_3
 
 
void HC_SR04_Init()     //超声波初始化
{
    RCC_APB2PeriphClockCmd(RCC_GPIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin =  trigGPIO;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_ResetBits(GPIOx,trigGPIO);            		  //设置默认为低电平 高电平会触发HC-SR发射超声波
	GPIO_Init(GPIOx,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin =  echoGPIO;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_ResetBits(GPIOx,echoGPIO); 				  //设置默认低电平
    GPIO_Init(GPIOx,&GPIO_InitStruct);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
 
    TIM_DeInit(TIM2);
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 				
    TIM_TimeBaseInitStructure.TIM_Prescaler =72-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); 
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM2,DISABLE); 		
}
 
void Servo_Init()     //舵机初始化
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //这个定时器和小车驱动一样，所以只能停车观察。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}	

void Servo_SetAngle(float Angle)
{
	TIM_SetCompare3(TIM3, Angle / 180 * 2000 + 500);
}

uint32_t Get_Timer(void)
{
        uint32_t t = 0;
        t = HC_SR04_count*1000;				//此处t为us
        t += TIM_GetCounter(TIM2);
        TIM2->CNT = 0;  					  
        Delay_ms(50);
        return t;
}
 
 float Get_Length()
{
    float t = 0,length = 0;
    GPIO_SetBits(GPIOx,trigGPIO);
    Delay_us(20);
    GPIO_ResetBits(GPIOx,trigGPIO);
    while (GPIO_ReadInputDataBit(GPIOx,echoGPIO) == RESET){};
    TIM_SetCounter(TIM2,0);			
    HC_SR04_count = 0;
    TIM_Cmd(TIM2, ENABLE); 
    while (GPIO_ReadInputDataBit(GPIOx,echoGPIO) == SET);
    TIM_Cmd(TIM2, DISABLE);
    t = Get_Timer() / 1000.0;
    length = t*34.0;
    return length/2;
}