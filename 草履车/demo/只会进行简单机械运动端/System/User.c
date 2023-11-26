#include "stm32f10x.h" 
#include "Delay.h"
#include "Timer.h"
#include "nRF24L01.h"
#include "Motor.h"
#include "W25Q64.h"
#include "Store.h"
#include "HC_SR04.h"

#define  Comm_WarnBit   60000           //警告位，当一段时间没有收到控制信号，将自动急停。建议取值高些
#define	 Keep_Length      40           //跟随模式，应该保持的距离，取值0~450cm。			
#define	 Avoid_Length     50          //避障，距离障碍物小于xcm将避开行驶。
#define  Observe_Num  	  1          //观察模式，每x轮将接受一次主机信号，决定是否继续观察。
#define  Observe_Send     3			//观察模式，收集的数据重复发送次数（不建议设为1，怕丢包）


//uint8_t mode,Speed,Time,Time_Flag,Data[4],Mode,BackTrack_Flag,Record_Flag;
//uint16_t WarnBit,BackTrack_Num,Store_Count,BackTrack_Count,HC_SR04_count,aaaaa,Temp;
//uint8_t RxData[1020+16];


uint8_t Data[4],BackTrack_Flag,Record_Flag;
uint16_t WarnBit,BackTrack_Num,Store_Count,BackTrack_Count,HC_SR04_count,Temp;
uint8_t RxData[1020+16];

/*通信检查*/
void Comm_Check(void)     
{
	uint8_t i;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断
	{
		WarnBit = Comm_WarnBit;
	}
	else WarnBit--;
	if(WarnBit == 0)
	{
		Motor_State(0);      //车停
		for(i = 0;i<3;i++)
		{
			Buzz_Mode(1);
			Delay_ms(500);
			Buzz_Mode(3); 
		}
	}
}



/*上电初始化*/
void Mode_Init(void)             
{
	Motor_Init();
	NRF24L01_Init();
	NRF24L01_Rx_Mode();
	Buzz_Init();
	Store_Init();
	WarnBit = Comm_WarnBit;
	Buzz_Mode(2);
}



/*实时遥控*/
void Mode_0(void)            
{
	NRF24L01_RxPacket(Data);
	Motor_State(Data[0]);
	Comm_Check();
}



/*观察模式，停车对各方向进行超声波测距，并向主机发送*/
void  Mode_1(void)            
{
	Servo_Init();
	HC_SR04_Init();
	NRF24L01_Tx_Mode();
	uint8_t i,Num,Temp;
	uint16_t Length;
	Servo_SetAngle(0);
	Num = 0;
	Temp = 0;
	Data[0] = 0;
	while(1)
	{	
		if(Temp == 5) 
		{
			Temp = 0;
			Num++;
		}
		Servo_SetAngle(Temp * 45);
		Delay_ms(500);                 //请给舵机一点转向的时间
		Length	= Get_Length();
		Data[1] =  Temp*16;            //Temp存储在高4位。防止与接收检测的数据冲突。
		Data[2] = (Length >> 8);
		Data[3] =  Length;
		for(i = 0;i < Observe_Send;i++)
		{
			NRF24L01_TxPacket(Data);
		}
		Temp++;
		
		if(Num == Observe_Num)
		{
			Num = 0;
			NRF24L01_Rx_Mode();
			for(i = 0;i < 100;i++)     //i值不建议设小，否则容易因为接收不到数据而无法结束
			{
				NRF24L01_RxPacket(Data);
			}
			
			if(Data[0] != 0x00)       
			{
				goto Mode_2_Break;
				
			}
			else  NRF24L01_Tx_Mode();
		}
	}
	Mode_2_Break:
	Servo_SetAngle(90);
	Delay_ms(200);
	Motor_Init();
	NRF24L01_Rx_Mode();
}



/*跟随模式，直线跟随并保持一定距离。（凑模式的）*/
void Mode_2(void)  			
{
	uint8_t Length,Temp,Gap;
	Gap = 20;                   //误差位，不然车太鬼畜了。
	HC_SR04_Init();
	while(1)
	{
		NRF24L01_RxPacket(Data);
		Length = Get_Length();
		if((Keep_Length - Length) == Gap | (Keep_Length - Length) == Gap) Length = Keep_Length; //相差Gap厘米，则不动。
		if(Length > Keep_Length)
		{
			Temp = (Data[0] & 0x03);       //摇杆可以控制跟随速度
			Motor_State(Temp + 0x20);
		}
		else
		{
			Temp = (Data[0] & 0x03);
			Motor_State(Temp + 0xc0);
		}
		Comm_Check();
		if((Data[0] & 0x1c) != 0x08) break;
	}
	
}



/*避障模式，它还需要好好调教（凑模式的）*/
void Mode_3(void)
{
	HC_SR04_Init();
	uint8_t   Num,Speed;
	uint16_t  Length;
	while(1)
	{
		NRF24L01_RxPacket(Data);
		Speed = (Data[0] & 0x03);
		Length = Get_Length();
		Num = TIM_GetCounter(TIM3)%2;    //伪随机（AD采样太麻烦了，反正我只要两个随机数。）
		Comm_Check();
		if(Length < 50)
		{
			Avoid:
			Length = Get_Length();
			Motor_State(0xc0+Speed);     //先退退
			Delay_ms(1500);
			if(Num == 1)
			{
				Motor_State(0x80+Speed);     //   ↑ + ←
				Delay_ms(1000);
				Length = Get_Length();
				if(Length < 50)  goto Avoid;
			}
			else
			{
				 Motor_State(0x40+Speed);     //   ↑ + →
		 	     Delay_ms(1000);
		    	 Length = Get_Length();
				 if(Length < 50)  goto Avoid;
			}
		}
		Motor_State(0x20+Speed);
		if((Data[0] & 0x1c) != 0x0c)  break;
	}
}



/*睡眠模式，似乎没啥用（凑模式的）*/
void Mode_4(void)
{
	__WFI();      //懒得关时钟了，请让我摸鱼一下
}




/*读取已记录的操作，并原封不动的执行*/
void Mode_5(void)   
{
	BackTrack_Count = (Store_Data[1] >> 8);
	Store_Count = 1;
	Temp = 0;
	Timer_Init();
	while(1)
	{
		if(Record_Flag == 1)
		{
			Record_Flag = 0;
			TIM_Cmd(TIM2,DISABLE);
			goto Mode_8_Break;			
		}
	}
	Mode_8_Break:
	Buzz_Mode(3);         
	Buzz_Mode(3);         //执行完毕,提醒用户切换模式，嗡鸣结束后操作将再进行一次。	
}



/*读取已记录的操作，并倒着执行（像是回溯或者倒带）*/
void Mode_6(void)          
{
	Timer_Init();
	Temp = 0;
	BackTrack_Count = (Store_Data[1]/2)+1;	//实验才是检测代码的唯一标准，执行对了就行。
	while(1)
	{
		if(BackTrack_Flag == 1)
		{
			BackTrack_Flag = 0;
			TIM_Cmd(TIM2,DISABLE);
			goto Mode_6_Break;
		}
	}
	Mode_6_Break:
	Buzz_Mode(3);         
	Buzz_Mode(3);         //执行完毕,提醒用户切换模式，嗡鸣结束后回溯操作将再进行一次。
	
}	


/*     





void Mode_7(void)            
{
	uint16_t Mode_7_Count = 0;
	while(1)
	{
		NRF24L01_RxPacket(Data);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断，按理说不该在里面执行大量程序，这会加重丢包（管他呢）
		{
			RxData[Mode_7_Count] = Data[0];
			if(Mode_7_Count == 1)   Buzz_Mode(2);    //计时开始
			Mode_7_Count++;
			if(Mode_7_Count>1020+16 | Data[1] == 0xff) 
			{
				MyDMA_Init((uint32_t)RxData,(uint32_t)Store_Data+2);
				MyDMA_Transfer();	
				Store_Data[1] = Mode_7_Count - 2;    
				Store_Save();				
				goto Mode_7_Break;                                     
			}
		}
		if(Mode_7_Count>=15)
		{

			Motor_State(Data[0]);			
		}
	}
	Mode_7_Break:
	Buzz_Mode(2);         //计时结束
}
*/







void TIM2_IRQHandler(void)               //没资源啊没资源，只能共用定时器中断了，希望不会出啥茬子。
{
	uint16_t State;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if((Data[0] & 0x1c) == 0x18)     //用于执行回溯
		{
			Temp ++;
			if(Temp%2 == 1)
			{
				State = ~(uint8_t)((Store_Data[BackTrack_Count] >> 8) & 0xe0);
				State &= 0xe0;
				State += (uint8_t)((Store_Data[BackTrack_Count] >> 8) & 0x03);
				Motor_State((uint8_t)State);				
			}
			else
			{
				State = ~((uint8_t)Store_Data[BackTrack_Count] & 0xe0);
				State &= 0xe0;
				State += ((uint8_t)Store_Data[BackTrack_Count] & 0x03);   
				Motor_State((uint8_t)State);
				BackTrack_Count --;
			}
			if(BackTrack_Count == 1)
			{
				Motor_State(0);
				BackTrack_Flag = 1;
				Buzz_Mode(2);    
			}
		}
		if((Data[0] & 0x1c) == 0x14)     //用于执行已记录的操作  
		{
			Temp++;
			if(Temp%2 == 1)
			{
				State = ((uint8_t)Store_Data[Store_Count+1]);
				Motor_State((uint8_t)State);
			}
			else
			{
				State = (Store_Data[Store_Count+1]>> 8);
				Motor_State((uint8_t)State);
				Store_Count++;
			}
			if(Temp == (uint8_t)Store_Data[1])
			{
				Motor_State(0);
				Record_Flag = 1;
				Buzz_Mode(2);    
			}
		}
		HC_SR04_count++;    //超超超声波计数用
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


/* 
	Demo1:滴声后将记录操作，记录后将存储至闪存
	方式为：小车收到操控端每200ms一次的数据（数据格式为一个无符号的8位）。通过检查蓝牙中断引脚，边收，边执行，边存储至临时数组。结束后转存至闪存。
	优点：所见即所得，见到的小车的运动都能记录下来
	缺点：因为数据传输不可避免的丢包，操控端会有断触的感觉，体验不太好。
	待优化：蓝牙中断的判断以及在内执行的程序会加重丢包。可以去掉中断判断，直接无脑接收。问题就是无法得知数据是否是新接收的，重复接收同种数据存储
会很严重。除非数据帧引入序号，根据序号判断。即简化版的TCP。更近一步，则干脆解决丢包问题。

	预想：Data[0]为数据，Data[1]为序号。Data[1]若为0xff则表示结束。序号存储上限为255（包括0）。则一共可记录255个数据，即51s。一直接收信号，并进行
序号比对。一致则不管，不一致，则存储至临时数组。最后对数组进行检查，如果数组数据为空，则说明丢包，将请求发送端发送丢包数据。en..................
既然需要发送端存储数据发送。。。。

	预想2：记录操作时，小车仅负责接收并实时运行，操控端存储操控的数据。记录结束时，则操控端发送数据，小车接收并存储，使用简化版的TCP完整保留数据。
缺点：操控小车前进5m，因为丢包只跑了3m，你又操控小车跑了2m。实际：5m。记录：7m。所见不是所得。。。不过可以先整出来玩玩。






void Mode_7(void)            
{
	uint16_t Mode_7_Count = 0;
	while(1)
	{
		NRF24L01_RxPacket(Data);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)  //获取蓝牙中断，按理说不该在里面执行大量程序，这会加重丢包（管他呢）
		{
			RxData[Mode_7_Count] = Data[0];
			if(Mode_7_Count == 1)   Buzz_Mode(2);    //计时开始
			Mode_7_Count++;
			if(Mode_7_Count>1020+16 | Data[1] == 0xff) 
			{
				MyDMA_Init((uint32_t)RxData,(uint32_t)Store_Data+2);
				MyDMA_Transfer();	
				Store_Data[1] = Mode_7_Count - 2;    
				Store_Save();				
				goto Mode_7_Break;                                     
			}
		}
		if(Mode_7_Count>=15)
		{

			Motor_State(Data[0]);			
		}
	}
	Mode_7_Break:
	Buzz_Mode(2);         //计时结束
}

*/