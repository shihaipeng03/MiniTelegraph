#include "pwm_output.h"
#include "delay.h"
#include "motor.h"

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */

void TIM4_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* PWM�źŵ�ƽ����ֵ,����ȫ��ȡ��ֵ */
 	u16 CCR1_Val = DOT_DOWN;        
 	u16 CCR2_Val = SERVO_MIN;

	/* ����TIM4CLK Ϊ 72MHZ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	/* GPIOB clock enable */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE); 

	/*GPIOA Configuration: TIM4 channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM4 channel 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Time base configuration */		 
	TIM_TimeBaseStructure.TIM_Period = 20000-1;       //����ʱ����0������19999����Ϊ20000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //����Ԥ��Ƶ:1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //ʹ��ͨ��2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);                   //ʹ�ܶ�ʱ��4
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
void printLine(int b) //����Ǵ�ӡ8*8��ʹ��
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

void printLine16(int b) //����Ǵ�ӡ16*16��ʹ��
{ 
	u8 j;
	PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
	if(b != 0)
	{
		PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
		delay_ms(SERVO_DELAY*3);//�����ʱʱ���Ӱ�쵽��д���겻����*2��̫����������*3�ǱȽϿ���������
		for (j = 0; b != 0; j++)
		{
			dot(b&1);
			b >>= 1;
			PWM_Padding(Servo_Arm,SERVO_MAX - (LINE_TAB+j)*SERVO_STEP);
			delay_ms(SERVO_DELAY);
		} 
		dot(0);
	}
	Motorcw_setp(6); //�������ǰ��
}

void printChar(char c) //��ӡһ���ַ�
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
void printString(char* str) //��ӡһ���ַ�
{
	while(*str != '\0') 
	{
		printChar(*str);
		str+=1;
	}
}
void printChinese(u8 no)//��ӡ����
{
	u8 i;
	u16 b = 0;
	for(i = 0; i < 16; i++)//��Ϊ��16*16�ĵ��� ���i�����Ϊ������16 �����printLine16���������16���㣬���16*16�ĵ���
	{
		b |= Hzk[no][2*i];//��Ϊ�������ɵ�����������8λ�ģ�����Ҫ������8λ�Ķ��������������������ƴ��16����
		b <<= 8;
		b |= Hzk[no][2*i+1];
		if(b != 0)
			printLine16(b);
		else
			Motorcw_setp(5);//�������ǰ���ĳ��ȣ������Լ������
		b = 0;
	}
}

