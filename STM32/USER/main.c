#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "motor.h"
#include "usart1.h"
#include "hcsr04.h"

uint16_t distance = 0;
uint16_t x = 0;
uint16_t y = 0;
uint16_t preX = 0;
uint16_t preY = 0;
u8 flag = 0;

void USART1_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1 = 0;
		static u16 RxBuffer1[5] = {0};
		static u8 RxState = 0;	
 
		if( USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  	   //接收中断  
		{
			
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //清除中断标志
			
				com_data = USART_ReceiveData(USART1);
			
				if(RxState == 0 && com_data == 0x2C)  //0x2c帧头
				{
					
					RxState = 1;
					RxBuffer1[RxCounter1++] = com_data;
				}
		
				else if(RxState==1 && com_data == 0x12)  //0x12帧头
				{
					RxState = 2;
					RxBuffer1[RxCounter1++] = com_data;
				}
				
				else if(RxState == 2)
				{
					RxBuffer1[RxCounter1++] = com_data;
 
					if(RxCounter1 == 5 && com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
					{
						x = RxBuffer1[RxCounter1 - 3];
						y = RxBuffer1[RxCounter1 - 2];
						RxCounter1 = 0;
						RxState = 0;	
					}
					
					else if(RxCounter1 > 5)            //接收异常
					{
						RxState = 0;
						RxCounter1 = 0;
						for(i = 0;i < 5; i++)
						{
							RxBuffer1[i] = 0x00;      //将存放数据数组清零
						}
					}
				}
				else   //接收异常
				{
					RxState = 0;
					RxCounter1 = 0;
					for(i = 0; i < 5; i++)
					{
						RxBuffer1[i] = 0x00;      //将存放数据数组清零
					}
				}
		}
}

int main(void)
{
	USART1_Init();
	Motor_Init();
	HCSR04_Init();
	
	while(1)
	{
		if (x >= 20 && y != 0)		//距离到达20cm时跟踪完成（实际距离会短个几cm）
		{
			distance = HCSR04_GetValue(); 	//获取跟踪路径上障碍物的距离(cm)
			
			if(distance <= 12)		//跟踪路径上有障碍物
			{
				//记录下此时的位置信息
				preX = x;
				preY = y;		
				if(distance <= 5)
				{
					Motor_Back();
					Delay_ms(150);
				}
				//移动以规避障碍物
				Motor_Left();
				flag = 1;
				
			}
			else
			{
				if(flag == 1)	//前进一段距离 避开障碍物
				{
					Motor_Advance();
					Delay_ms(700);	
					flag = 2;
				}
				
				if(flag == 2)	//恢复之前的方向
				{
					if(preX != x || preY != y)	//检测到目标就脱离避障模式
					{
						flag = 0;
					}
					else
					{
						Motor_Rigth();
						Delay_ms(250);
						Motor_Stop();	//让小车停下 防止摄像头晃动 无法检测到目标
						Delay_ms(50);
					}
				}
				
				if(flag == 0)	//正常行动
				{
					
					//中间值为112 宽度为48
					if(y > 160 && y < 224)		//目标在右边
					{
						Motor_Rigth();
						Delay_ms(100);
						
						Motor_Stop();
						Delay_ms(50);

					}
					
					else if(y > 0 && y < 64)		//目标在左边
					{
						Motor_Left();
						Delay_ms(100);
						
						Motor_Stop();
						Delay_ms(50);

					}
					
					else if(y >= 64 && y <= 160)	//目标在中间
					{
						Motor_Advance();
						
					}
					else
					{
						y = 0;
					}
					
				}
				
			}
		}
		
		if (x < 20 || y == 0)
		{
			Motor_Stop();
		}
	}
}
