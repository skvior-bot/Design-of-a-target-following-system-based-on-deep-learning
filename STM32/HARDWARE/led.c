#include "stm32f10x.h"
#include "led.h"

void LED1_2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13);
}

void LED1_Turn_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
}
void LED1_Turn_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12); 
}
void LED2_Turn_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
}
void LED2_Turn_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_13); 
}
