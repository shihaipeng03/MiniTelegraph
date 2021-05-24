#include "pwm_output.h"
#include "delay.h"
#include "motor.h"

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */

void TIM4_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* PWM信号电平跳变值,这里全部取中值 */
 	u16 CCR1_Val = DOT_DOWN;        
 	u16 CCR2_Val = SERVO_MIN;

	/* 设置TIM4CLK 为 72MHZ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	/* GPIOB clock enable */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE); 

	/*GPIOA Configuration: TIM4 channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM4 channel 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Time base configuration */		 
	TIM_TimeBaseStructure.TIM_Period = 20000-1;       //当定时器从0计数到19999，即为20000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //设置预分频:1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //使能通道2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);                   //使能定时器4
}

int pos = 0;
void dot(int m) //????
{
	if(m) {
		pos = 1;
		PWM_Padding(Servo_Dot,DOT_UP);
		delay_ms(DOT_UP_DELAY);
	} else {
		pos = 0;
		PWM_Padding(Servo_Dot,DOT_DOWN);
		//delay_ms(DOT_DOWN_DELAY);
	}
}
void printLine(int b) //这个是打印8*8的使用
{ 
	u8 j;
	PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
	if(b != 0)
	{
		PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
		delay_ms(SERVO_DELAY*20);
		for (j = 0; b != 0; j++)
		{
			dot(b&1);
			b >>= 1;
			PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB+j)*SERVO_STEP);
			delay_ms(SERVO_DELAY);
		} 
		dot(0);
	}
	Motorcw_setp(6); //
}

void printLine16(int b) //这个是打印16*16的使用
{ 
	u8 j;
	PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
	if(b != 0)
	{
		PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
		delay_ms(SERVO_DELAY*3);//这个延时时间会影响到字写得完不完整*2不太完整，所以*3是比较快且完整的
		for (j = 0; b != 0; j++)
		{
			dot(b&1);
			b >>= 1;
			PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB+j)*SERVO_STEP);
			delay_ms(SERVO_DELAY);
		} 
		dot(0);
	}
	Motorcw_setp(6); //步进电机前进
}

void printChar(char c) //打印一个字符
{
	u8 i;
	int n = 0;
	for (i = 0; i < 8; i++)
	{
		if(chars[c][i] != 0)
		{
			printLine(chars[c][i]);
			n++;
		}
		else 
			Motorcw_setp(5);
	}
}
void printString(char* str) //打印一串字符
{
	while(*str != '\0') 
	{
		printChar(*str);
		str+=1;
	}
}
void printChinese(u8 no)//打印中文
{
	u8 i;
	u16 b = 0;
	for(i = 0; i < 16; i++)//因为是16*16的点阵 这个i就理解为长度是16 下面的printLine16就是竖向的16个点，组成16*16的点阵
	{
		b |= Hzk[no][2*i];//因为工具生成的中文数组是8位的，所以要将两个8位的二进制数组合起来，才能拼成16个点
		b <<= 8;
		b |= Hzk[no][2*i+1];
		if(b != 0)
			printLine16(b);
		else
			Motorcw_setp(5);//步进电机前进的长度，可以自己测出来
		b = 0;
	}
}

