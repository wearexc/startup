// #include	"stm32f10x.h"
//#include	"Delay.h"
//#include 	"OLED.h"
// 
//#define RCC_GPIO	RCC_APB2Periph_GPIOA
//#define GPIOx 		GPIOA
//#define trigGPIO 	GPIO_Pin_2
//#define echoGPIO 	GPIO_Pin_3
// 
// 
////给毫秒ms计数
//uint16_t count = 0;
// 
////GPIO口的初始化
//void HC_SR04_GPIO_Init(){
//    //定义结构体
//    GPIO_InitTypeDef GPIO_InitStruct;
//    //开启GPIOB RCC时钟
//    RCC_APB2PeriphClockCmd(RCC_GPIO,ENABLE);
// 
//    //对结构体成员赋值
//    //发送GPIOB6 模式为推挽输出 trig
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Pin =  trigGPIO;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_ResetBits(GPIOx,trigGPIO);   //设置默认为低电平 高电平会触发HC-SR发射超声波
// 
//	GPIO_Init(GPIOx,&GPIO_InitStruct);
//	
//    //返回GPIOB7 模式设置为浮空输入(因为该GPIO口需要被外设改变电平)
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStruct.GPIO_Pin =  echoGPIO;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_ResetBits(GPIOx,echoGPIO);   //设置默认低电平
// 
//    //初始化GPIO外围设备    echo
//    GPIO_Init(GPIOx,&GPIO_InitStruct);
//}
// 
////初始化定时器
//void HC_SR_04_TIME_NVIC_Init(){
// 
//    //定义定时器的结构体
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
// 
//    //NVIC结构体
//    NVIC_InitTypeDef NVIC_InitStructure;
// 
//    //开启TIM2
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
// 
//    //将TIMx外围寄存器反初始化为其默认重置值。
//    TIM_DeInit(TIM2);
// 
//    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到1000为1ms
//    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	
// 
//    //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数				
//    TIM_TimeBaseInitStructure.TIM_Prescaler =72-1;
// 
//    //不分频					
//    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
// 
//    //TIM向上计数模式		
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
// 
//    //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx TimeBase Unit外设  	
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); 
// 
//    //打开定时器的更新中断
//    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
// 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//    //选择串口1中断
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           	  
// 
//    //抢占式中断优先级设置为1
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     
// 
//    //响应式中断优先级设置为1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            
// 
//    //使能中断
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               
//    NVIC_Init(&NVIC_InitStructure);
// 
//    //先关掉定时器，先不让它计时
//    TIM_Cmd(TIM2,DISABLE); 
//}
// 
//void HC_SR04_Init()
//{
//    RCC_APB2PeriphClockCmd(RCC_GPIO,ENABLE); 
//	GPIO_InitTypeDef GPIO_InitStruct;

//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Pin =  trigGPIO;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_ResetBits(GPIOx,trigGPIO);   //设置默认为低电平 高电平会触发HC-SR发射超声波
//	GPIO_Init(GPIOx,&GPIO_InitStruct);
//	
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStruct.GPIO_Pin =  echoGPIO;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_ResetBits(GPIOx,echoGPIO);   //设置默认低电平
//    GPIO_Init(GPIOx,&GPIO_InitStruct);
//	
//    //定义定时器的结构体
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
// 
//    //NVIC结构体
//    NVIC_InitTypeDef NVIC_InitStructure;
// 
//    //开启TIM2
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
// 
//    //将TIMx外围寄存器反初始化为其默认重置值。
//    TIM_DeInit(TIM2);
// 
//    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到1000为1ms
//    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	
// 
//    //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数				
//    TIM_TimeBaseInitStructure.TIM_Prescaler =72-1;
// 
//    //不分频					
//    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
// 
//    //TIM向上计数模式		
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
// 
//    //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx TimeBase Unit外设  	
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); 
// 
//    //打开定时器的更新中断
//    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
// 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//    //选择串口1中断
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           	  
// 
//    //抢占式中断优先级设置为1
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     
// 
//    //响应式中断优先级设置为1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            
// 
//    //使能中断
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               
//    NVIC_Init(&NVIC_InitStructure);
// 
//    //先关掉定时器，先不让它计时
//    TIM_Cmd(TIM2,DISABLE); 	
//	
//	
//}
// 
////打开定时器
//static void OpenTIM2()        
//{
//        TIM_SetCounter(TIM2,0);			//清除计数
//        count = 0;
//        TIM_Cmd(TIM2, ENABLE);  		//使能TIM2外设
//}
// 
////关闭定时器
//static void CloseTIM2()        
//{
//        TIM_Cmd(TIM2, DISABLE);  //使能TIM2外设
//}
// 
// 
// 
////TIM2定时器的中断函数
//void TIM2_IRQHandler(void)   
//{
//        //检查是否是更新中断位
//        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
//        {
//            //清除标志位	清除TIMx的中断挂起位
//            TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
//            count++;
//        }
//}
// 
// 
////获取定时器时间
//uint32_t Get_Timer(void)
//{
//        uint32_t t = 0;
// 
//        //此处t为us
//        t = count*1000;	
// 
//        //TIM_GetCounter 获取TIM2计数器值
//        t += TIM_GetCounter(TIM2);
//        //将TIM2计数寄存器的计数值清零  
//        TIM2->CNT = 0;  					  
//        Delay_ms(50);
//        return t;
//}
// 
// float Get_Length()
//{
//    
//	 
//    //存储时间
//    float t = 0,length = 0;
//    
//    // 1. 先将trig置位(设置为高电平)
//    GPIO_SetBits(GPIOx,trigGPIO);
// 
//    // 2. 持续时间至少要10us    这里我就给20了
//    Delay_us(20);
// 
//    // 3. 然后需要将trig位清零(也就是设置为低电平)
//    GPIO_ResetBits(GPIOx,trigGPIO);
//	 
//    // 4. 等待 HC-SR04 将echo置位(也就是高电平),如果置位了就开始发出声波了，我们要开始计时
//    //      这里用GPIO_ReadInputDataBit 是读取GPIO的输入
//    while (GPIO_ReadInputDataBit(GPIOx,echoGPIO) == RESET){
//	
//	};
//    // 5. 打开定时器 开始计时
//    OpenTIM2();
//	
//	
//    // 6. 等待 HC-SR04 将echo清零(也就是低电平)，如果清零了就是收到了声波
//    while (GPIO_ReadInputDataBit(GPIOx,echoGPIO) == SET);
//    
//	
//    // 7. 关闭定时器 停止计时
//    CloseTIM2();
// 
//    // 8. 获取时间,因为是us(微秒)，我们要转化为ms(毫秒)所以除1000
//    t = Get_Timer() / 1000.0;
// 
//    // 9. 计算长度cm(厘米)
//    //      声音在空气中传播的速度是340m/s,我们转换一下就是 34cm/ms
//    length = t*34.0;
// 
//    // 10. 返回长度，因为是一个往返我们要除2
//    return length/2;
//}
