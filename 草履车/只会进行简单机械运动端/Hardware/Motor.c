#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//因为控制电机驱动芯片的IO都为GPIOB，所以初始化合二为一。
#define RCC_GPIO            RCC_APB2Periph_GPIOB
#define GPIO                GPIOB 
#define Motor_Lift_IN1      GPIO_Pin_5 
#define Motor_Lift_IN2      GPIO_Pin_6 
#define Motor_Right_IN1     GPIO_Pin_10 
#define Motor_Right_IN2     GPIO_Pin_11
#define Motor_Turn          30                 //转向参数设置



/*电机初始化*/
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_GPIO, ENABLE);  
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Motor_Lift_IN1 | Motor_Lift_IN2 | Motor_Right_IN1 | Motor_Right_IN2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO, &GPIO_InitStructure);
	
	PWM_Init();
}



/**
  * @brief    设置小车右路速度
  * @param    Speed 速度
  * @retval 
  */
void Motor_SetRightSpeed(int8_t Speed)   
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIO, Motor_Right_IN1);
		GPIO_ResetBits(GPIO, Motor_Right_IN2);
		PWM_SetRightCompare(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIO, Motor_Right_IN1);
		GPIO_SetBits(GPIO, Motor_Right_IN2);
		PWM_SetRightCompare(-Speed);
	}
}



/**
  * @brief    设置小车左路速度
  * @param    Speed 速度
  * @retval 
  */
void Motor_SetLiftSpeed(int8_t Speed) 
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIO, Motor_Lift_IN1);
		GPIO_ResetBits(GPIO, Motor_Lift_IN2);
		PWM_SetLiftCompare(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIO, Motor_Lift_IN1);
		GPIO_SetBits(GPIO, Motor_Lift_IN2);
		PWM_SetLiftCompare(-Speed);
	}
}



/**
  * @brief  对接收数据进行处理
  * @param  State 蓝牙接收数据。Rocker 摇杆（高3位），Speed 速度（低2位）
  * @retval 
  */
void Motor_State(uint8_t State)
{
	uint8_t Speed,Flag,Rocker;   
	Rocker = State & 0xe0;       //提取摇杆
	Speed = State & 0x03;  //提取速度
	//配置速度
	if(Speed == 0x01) Speed = 40;
		else if(Speed == 0x02) Speed = 70;
		else if(Speed == 0x03) Speed = 100;
		else (Speed = 0);
	//配置马达
	if(Rocker == 0x20)                   // ↑
	{
		Motor_SetRightSpeed(Speed);         
		Motor_SetLiftSpeed (Speed);
	}
	else if(Rocker == 0xc0)             // ↓
	{
		Motor_SetRightSpeed(-Speed);
		Motor_SetLiftSpeed (-Speed);		
	}
	else if(Rocker == 0x40)            //   ↑ + →
	{
		Motor_SetRightSpeed(Speed);
		Motor_SetLiftSpeed (Speed-Motor_Turn);	
	}
	else if(Rocker == 0xa0)            //  ↓ + ←
	{
		Motor_SetRightSpeed(-Speed);
		Motor_SetLiftSpeed (-Speed+Motor_Turn);		
	}
	else if(Rocker == 0x60)           //   ↓ + →
	{
		Motor_SetRightSpeed(-Speed+Motor_Turn);  
		Motor_SetLiftSpeed (-Speed);		
	}
	else if(Rocker == 0x80)           //   ↑ + ←
	{
		Motor_SetRightSpeed(Speed-Motor_Turn);
		Motor_SetLiftSpeed (Speed);		
	}	
	else 
	{
		Motor_SetRightSpeed(0);
		Motor_SetLiftSpeed (0);	
	}
}

