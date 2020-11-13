#include "cd4052.h"



void Enable_Path(unsigned char Path_Num)
{
	switch(Path_Num)
		{
			case 0:
				//can a
				HAL_GPIO_WritePin(ADDR_A_GPIO_Port,ADDR_A_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ADDR_B_GPIO_Port,ADDR_B_Pin,GPIO_PIN_RESET);
				break;
			case 1:
				//485 a
				HAL_GPIO_WritePin(ADDR_A_GPIO_Port,ADDR_A_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(ADDR_B_GPIO_Port,ADDR_B_Pin,GPIO_PIN_RESET);
				break;
			case 2:
				//can b
				HAL_GPIO_WritePin(ADDR_A_GPIO_Port,ADDR_A_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ADDR_B_GPIO_Port,ADDR_B_Pin,GPIO_PIN_SET);
				break;
			case 3:
				//485 b
				HAL_GPIO_WritePin(ADDR_A_GPIO_Port,ADDR_A_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(ADDR_B_GPIO_Port,ADDR_B_Pin,GPIO_PIN_SET);
				break;
			default:
				break;
		}
}

