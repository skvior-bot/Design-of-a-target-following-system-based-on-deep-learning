#include "stm32f10x.h"                  // Device header
#include "motor.h"
#include "pwm.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
	
	PWM_Init();
}

void Motor_Advance(void)
{
	TIM_SetCompare2(TIM4, 18);	
	TIM_SetCompare1(TIM4, 15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

void Motor_Back(void)
{
	TIM_SetCompare2(TIM4, 18);	
	TIM_SetCompare1(TIM4, 15);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void Motor_Left(void)
{
	TIM_SetCompare2(TIM4, 18);
	TIM_SetCompare1(TIM4, 0);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

void Motor_Rigth(void)
{
	TIM_SetCompare2(TIM4, 0);	
	TIM_SetCompare1(TIM4, 15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

void Motor_Stop(void)
{
	TIM_SetCompare2(TIM4, 0);	
	TIM_SetCompare1(TIM4, 0);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
