#ifndef __MOTOR_H
#define __MOTOR_H


extern uint8_t Motor_Flag;
void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);
void Motor_State(uint8_t State);
void Motor_SetLiftSpeed(int8_t Speed);


#endif
