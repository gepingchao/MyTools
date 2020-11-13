#include "RF.h"

#define RF_Datalength 100
unsigned short FrameHerad_1 = 0;
unsigned short RFdata[RF_Datalength] = {0};
unsigned char RF_Step = 1;
unsigned char RF_START_RECV_FLAG = 0;
unsigned char RF_Head_1_flag = 0;
unsigned char RF_Head_2_flag = 0;

unsigned short counter = 0;
	
#define FRAME_HEARD_1	800
#define FRAME_HEARD_2	400
#define FRAME_HEARD_CONTINUE	200
#define BLINK	15000

void HAL_GPIO_EXTI_Callback(unsigned short GPIO_Pin)
{		
	counter = __HAL_TIM_GET_COUNTER(&htim1);
	if(HAL_GPIO_ReadPin(RF_IN_GPIO_Port,RF_IN_Pin))
		{
			if((counter > FRAME_HEARD_1))
				{
					RF_Head_1_flag = 1;
					FrameHerad_1 = counter;
				}
		}
	else
		{
			if((counter > FRAME_HEARD_CONTINUE))
				{
					
					if((counter > FRAME_HEARD_2))
						{
							if(RF_Head_1_flag)
								{
									RF_START_RECV_FLAG = 1;
									RF_Step = 0;
								}
						}
					if((counter > BLINK))
						{
							RF_START_RECV_FLAG = 0;//²»¿É¿¿
						}

					
					if(RF_Head_1_flag)
						{
							//ÖØ¸´Âë
							
						}
					
				}
			RF_Head_1_flag = 0;
		}
	
	if(RF_START_RECV_FLAG)
		{
			RFdata[RF_Step] = counter;
			RF_Step = RF_Step > (RF_Datalength-1)? 1 : RF_Step+1;
		}

	__HAL_TIM_GET_COUNTER(&htim1) = 0;
	//HAL_GPIO_WritePin(RF_OUT_GPIO_Port,RF_OUT_Pin,GPIO_PIN_RESET);
	
}


void RF_Delay10us(unsigned short time)
{
	__HAL_TIM_GET_COUNTER(&htim2) = 0;
	while(time > __HAL_TIM_GET_COUNTER(&htim2));
}


void RF_SendData(short* data,unsigned short data_length)
{
	unsigned short send_data_length = 0;
	HAL_GPIO_WritePin(RF_OUT_GPIO_Port,RF_OUT_Pin,GPIO_PIN_SET);
	RF_Delay10us(950);
	HAL_GPIO_TogglePin(RF_OUT_GPIO_Port,RF_OUT_Pin);
	for(send_data_length = 0 ; send_data_length < data_length ; send_data_length++)
		{
			RF_Delay10us(*(data + send_data_length));
			HAL_GPIO_TogglePin(RF_OUT_GPIO_Port,RF_OUT_Pin);
		}
	
	RF_Delay10us(9500);
	HAL_GPIO_WritePin(RF_OUT_GPIO_Port,RF_OUT_Pin,GPIO_PIN_RESET);
}

void test_rf(void)
{
	RF_SendData(RFdata,90);
}





