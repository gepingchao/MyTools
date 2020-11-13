#include "key.h"


const unsigned char _pw[8] = {1,2,2,1,2,2,2,1};
const unsigned char _pw_length = 5;

PassWord DevicePW;

key_data_def rtos_key;

unsigned char GetAddrValue(void)
{
	return 0;
}




unsigned char get_key_value(unsigned char key_number)
{
	unsigned char key_status;
	unsigned char key_num = key_number + 1;
	switch(key_num)
		{
			case 1:
				key_status = READ_KEY_STATUS(1);
				break;	
			case 2:
				key_status = READ_KEY_STATUS(2);
				break;
			case 3:
				key_status = READ_KEY_STATUS(3);
				break;
			case 4:
				key_status = READ_KEY_STATUS(4);
				break;
			case 5:
				key_status = READ_KEY_STATUS(5);
				break;
			case 6:
				key_status = READ_KEY_STATUS(6);
				break;
			case 7:
				key_status = READ_KEY_STATUS(7);
				break;
			case 8:
				key_status = READ_KEY_STATUS(8);
				break;
			/*case 9:
				key_status = READ_KEY_STATUS(9);
				break;
			case 10:
				key_status = READ_KEY_STATUS(10);
				break;
			case 11:
				key_status = READ_KEY_STATUS(11);
				break;
			case 12:
				key_status = READ_KEY_STATUS(12);
				break;
			case 13:
				key_status = READ_KEY_STATUS(13);
				break;
			case 14:
				key_status = READ_KEY_STATUS(14);
				break;
			case 15:
				key_status = READ_KEY_STATUS(15);
				break;
			case 16:
				key_status = READ_KEY_STATUS(16);
				break;
			case 17:
				key_status = READ_KEY_STATUS(17);
				break;
			case 18:
				key_status = READ_KEY_STATUS(18);
				break;
			case 19:
				key_status = READ_KEY_STATUS(19);
				break;
			case 20:
				key_status = READ_KEY_STATUS(20);
				break;
			case 21:
				key_status = READ_KEY_STATUS(21);
				break;
			case 22:
				key_status = READ_KEY_STATUS(22);
				break;
			case 23:
				key_status = READ_KEY_STATUS(23);
				break;			
			case 24:
				key_status = READ_KEY_STATUS(24);
				break;*/
			
		}
	return key_status;
}



void read_key(void)
{
unsigned char key_trigger_value;
unsigned int gui_msg_value;

unsigned char loopx = 0;
	for(loopx =0 ;loopx < KEY_NUMBER; loopx++)
	{
		if(GPIO_PIN_RESET == (GPIO_PinState)(get_key_value(loopx)))
			{
				rtos_key.press_time[loopx] = (rtos_key.press_time[loopx] >0XEE) ? 0XEE : rtos_key.press_time[loopx] ;
  				rtos_key.press_time[loopx] ++;
			}
	}
	
	for(rtos_key.key_point = 0; rtos_key.key_point <KEY_NUMBER ;rtos_key.key_point++)
		{
			if((rtos_key.press_time[rtos_key.key_point ]) > 2)
				{
					rtos_key.key_value[rtos_key.key_point ] = key_press(rtos_key.key_point);	
						if((rtos_key.key_ok[rtos_key.key_point ]&0X01) == 0)
									{
										rtos_key.key_ok[rtos_key.key_point ] |= 0X01; 
										key_trigger_value =KEY_TRIGGER(rtos_key.key_point);
										xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
										//xQueueSend(GuiQueueHandle, &(key_trigger_value), 10);//发送给显示任务
									}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0X40)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_press(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X02) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X02; 
									key_trigger_value =KEY_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
									//xQueueSend(GuiQueueHandle, &(key_trigger_value), 10);//发送给显示任务
								}
						}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0XAF)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_long_press(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X04) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X04; 
									key_trigger_value =KEY_LONG_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
									//xQueueSend(GuiQueueHandle, &(key_trigger_value), 10);//发送给显示任务
								}
						}

				}
		}

	for(loopx = 0;loopx < KEY_NUMBER; loopx++)
	{
		if(GPIO_PIN_SET == (GPIO_PinState)(get_key_value(loopx)))
			{
				if(rtos_key.key_ok[loopx])
					{
						rtos_key.key_ok[loopx] = 0;
						gui_msg_value = rtos_key.key_value[loopx];
						xQueueSend(KeyQueueHandle, &(rtos_key.key_value[loopx]), 10);
						xQueueSend(GuiQueueHandle, &(gui_msg_value), 10);//发送给显示任务
					}
				rtos_key.press_time[loopx] = 0;
			}
	}
		
}

void SendKeyValue(unsigned char value)
{
	/*data_from_KEYBOARD.tx_data[0] = KEY_HEAD_0;
	data_from_KEYBOARD.tx_data[1] = KEY_HEAD_1;	
	data_from_KEYBOARD.tx_data[2] = value;	
	HAL_UART_Transmit(&KEY_BOARD_COM, (unsigned char*)data_from_KEYBOARD.tx_data, 3, 500);*/
}

unsigned char EC11_A_Last = 0;
unsigned char EC11_B_Last = 0;
unsigned char Rotation_F_Start = 0;
unsigned char Rotation_B_Start = 0;

#define GetEC11_A_Status	HAL_GPIO_ReadPin(SIGNAL_A_GPIO_Port,SIGNAL_A_Pin)
#define GetEC11_B_Status	HAL_GPIO_ReadPin(SIGNAL_B_GPIO_Port,SIGNAL_B_Pin)
unsigned char ScanEC11(void)
{
	unsigned char ScanResult = 0;  
	unsigned char EC11_A_Now,EC11_B_Now;
	EC11_A_Now = GetEC11_A_Status;
	EC11_B_Now = GetEC11_B_Status;
	//static int r_count = 0;
	//char buff[40] = {0};

	unsigned int ec11_value = 0;

	
	if(EC11_A_Now != EC11_A_Last)   //以A为时钟，B为数据。正转时AB反相，反转时AB同相
		{
			if(EC11_A_Now == 0)
				{
					if(EC11_B_Now ==1)      //只需要采集A的上升沿或下降沿的任意一个状态，若A下降沿时B为1，正转                    
						{
							Rotation_F_Start = 1; //正转
							ScanResult = 3;     //开始正转
							//Lcd_Clear_Next();
						}

					else                    //反转
						{
							Rotation_B_Start = 1; //反转
							ScanResult = 4;//开始反转
						}
				}
			if(EC11_A_Now == 1)
				{
					if(Rotation_F_Start ==1)      //正转                   
						{
							Rotation_F_Start = 0;
							ScanResult = 1;     //正转
							ec11_value = EC11_TURN_P;
							xQueueSend(GuiQueueHandle, &(ec11_value), 10);
						}
					if(Rotation_B_Start ==1)      //反转                 
						{
							Rotation_B_Start = 0;
							ScanResult = 2;
							ec11_value = EC11_TURN_N;
							xQueueSend(GuiQueueHandle, &(ec11_value), 10);
						}
				}
			EC11_A_Last = EC11_A_Now;   //更新编码器上一个状态暂存变量
			EC11_B_Last = EC11_B_Now;   //更新编码器上一个状态暂存变量
		}
	return ScanResult;      //返回值的取值：   0：无动作；      1：正转；           2：反转；
}

void ScanRFIN(void)
{
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(RF_IN_GPIO_Port,RF_IN_Pin))
		{
			GUI_FillStringAt("H",10,95,WHITE,BLACK,&GUI_FontHZ10x10);
		}
	else
		{
			GUI_FillStringAt("L",10,95,WHITE,BLACK,&GUI_FontHZ10x10);
		}
}

unsigned int key_to_gui = 0;
void deal_key_value(unsigned char key_value)
{					
	switch(key_value)
  		{
  			case KEY_TRIGGER(0):
				
				break;
  			case KEY_LONG_TRIGGER(0):
				
				break;
  			case KEY_LONG_LONG_TRIGGER(0):
				
				break;
  			case (key_press(0)):
				//G_NeedInventoryFlag = 1;
				//AntInfo.choose_ant_res = EnableNextAnt();
				//StartInventoryProcess(0XFFFF0);
				//MasterGetSlaveAddress();
				//StartMasterGetSlaveTagInfoProcess(4);
				//StartLockProcess(0);
				//write_flash(0x800F000, (char *)&G_TagDataBase,200);
				
				//HAL_GPIO_TogglePin(RF_OUT_GPIO_Port,RF_OUT_Pin);
				//test_rf();
				//Lcd_Init();
				break;
  			case (key_long_press(0)):
				
				//StartMasterGetSlaveTagInfoProcess(2);
				break;
  			case (key_long_long_press(0)):
				
				break;

				
  			case KEY_TRIGGER(1):
				break;
  			case KEY_LONG_TRIGGER(1):
				break;
  			case KEY_LONG_LONG_TRIGGER(1):				
				break;
  			case (key_press(1)):
				

				break;
  			case (key_long_press(1)):
				break;
  			case (key_long_long_press(1)):
				break;


			case KEY_TRIGGER(2):
				break;
  			case KEY_LONG_TRIGGER(2):
				break;
  			case KEY_LONG_LONG_TRIGGER(2):				
				break;
  			case (key_press(2)):

				
				break;
  			case (key_long_press(2)):
				break;
  			case (key_long_long_press(2)):
				break;


			case KEY_TRIGGER(3):
				break;
  			case KEY_LONG_TRIGGER(3):
				break;
  			case KEY_LONG_LONG_TRIGGER(3):				
				break;
  			case (key_press(3)):

				
				break;
  			case (key_long_press(3)):
				break;
  			case (key_long_long_press(3)):
				break;


			case KEY_TRIGGER(4):
				break;
  			case KEY_LONG_TRIGGER(4):
				break;
  			case KEY_LONG_LONG_TRIGGER(4):				
				break;
  			case (key_press(4)):

				
				break;
  			case (key_long_press(4)):
				break;
  			case (key_long_long_press(4)):
				break;


			case KEY_TRIGGER(5):
				break;
  			case KEY_LONG_TRIGGER(5):
				break;
  			case KEY_LONG_LONG_TRIGGER(5):				
				break;
  			case (key_press(5)):
				TEST_485();

				
				break;
  			case (key_long_press(5)):
				break;
  			case (key_long_long_press(5)):
				break;


			case KEY_TRIGGER(6):
				break;
  			case KEY_LONG_TRIGGER(6):
				break;
  			case KEY_LONG_LONG_TRIGGER(6):				
				break;
  			case (key_press(6)):

				
				break;
  			case (key_long_press(6)):
				break;
  			case (key_long_long_press(6)):
				break;


			case KEY_TRIGGER(7):
				break;
  			case KEY_LONG_TRIGGER(7):	
				key_to_gui = 0XF0;
				xQueueSend(GuiQueueHandle, &(key_to_gui), 10);//发送给显示任务
				break;
				
  			case KEY_LONG_LONG_TRIGGER(7):			
				break;
				
  			case (key_press(7)):
				key_to_gui = 0XF1;
				xQueueSend(GuiQueueHandle, &(key_to_gui), 10);//发送给显示任务
				break;
  			case (key_long_press(7)):
				key_to_gui = 0XF1;
				xQueueSend(GuiQueueHandle, &(key_to_gui), 10);//发送给显示任务
				break;
  			case (key_long_long_press(7)):
				key_to_gui = 0XF1;
				xQueueSend(GuiQueueHandle, &(key_to_gui), 10);//发送给显示任务


				break;


			/*case KEY_TRIGGER(8):
				break;
  			case KEY_LONG_TRIGGER(8):
				break;
  			case KEY_LONG_LONG_TRIGGER(8):				
				break;
  			case (key_press(8)):
				SendKeyValue(8);
				break;
  			case (key_long_press(8)):
				break;
  			case (key_long_long_press(8)):
				break;


			case KEY_TRIGGER(9):
				break;
  			case KEY_LONG_TRIGGER(9):
				break;
  			case KEY_LONG_LONG_TRIGGER(9):				
				break;
  			case (key_press(9)):
				SendKeyValue(9);
				break;
  			case (key_long_press(9)):
				break;
  			case (key_long_long_press(9)):
				break;


			case KEY_TRIGGER(10):
				break;
  			case KEY_LONG_TRIGGER(10):
				break;
  			case KEY_LONG_LONG_TRIGGER(10):				
				break;
  			case (key_press(10)):
				SendKeyValue(10);
				break;
  			case (key_long_press(10)):
				break;
  			case (key_long_long_press(10)):
				break;


			case KEY_TRIGGER(11):
				break;
  			case KEY_LONG_TRIGGER(11):
				break;
  			case KEY_LONG_LONG_TRIGGER(11):				
				break;
  			case (key_press(11)):
				SendKeyValue(11);
				break;
  			case (key_long_press(11)):
				break;
  			case (key_long_long_press(11)):
				break;


			case KEY_TRIGGER(12):
				break;
  			case KEY_LONG_TRIGGER(12):
				break;
  			case KEY_LONG_LONG_TRIGGER(12):				
				break;
  			case (key_press(12)):
				SendKeyValue(12);
				break;
  			case (key_long_press(12)):
				break;
  			case (key_long_long_press(12)):
				break;


			case KEY_TRIGGER(13):
				break;
  			case KEY_LONG_TRIGGER(13):
				break;
  			case KEY_LONG_LONG_TRIGGER(13):				
				break;
  			case (key_press(13)):
				SendKeyValue(13);
				break;
  			case (key_long_press(13)):
				break;
  			case (key_long_long_press(13)):
				break;


			case KEY_TRIGGER(14):
				break;
  			case KEY_LONG_TRIGGER(14):
				break;
  			case KEY_LONG_LONG_TRIGGER(14):				
				break;
  			case (key_press(14)):
				SendKeyValue(14);
				break;
  			case (key_long_press(14)):
				break;
  			case (key_long_long_press(14)):
				break;


			case KEY_TRIGGER(15):
				break;
  			case KEY_LONG_TRIGGER(15):
				break;
  			case KEY_LONG_LONG_TRIGGER(15):				
				break;
  			case (key_press(15)):
				SendKeyValue(15);
				break;
  			case (key_long_press(15)):
				break;
  			case (key_long_long_press(15)):
				break;


			case KEY_TRIGGER(16):
				break;
  			case KEY_LONG_TRIGGER(16):
				break;
  			case KEY_LONG_LONG_TRIGGER(16):				
				break;
  			case (key_press(16)):
				SendKeyValue(16);
				break;
  			case (key_long_press(16)):
				break;
  			case (key_long_long_press(16)):
				break;


			case KEY_TRIGGER(17):
				break;
  			case KEY_LONG_TRIGGER(17):
				break;
  			case KEY_LONG_LONG_TRIGGER(17):				
				break;
  			case (key_press(17)):
				SendKeyValue(17);
				break;
  			case (key_long_press(17)):
				break;
  			case (key_long_long_press(17)):
				break;


			case KEY_TRIGGER(18):
				break;
  			case KEY_LONG_TRIGGER(18):
				break;
  			case KEY_LONG_LONG_TRIGGER(18):				
				break;
  			case (key_press(18)):
				SendKeyValue(18);
				break;
  			case (key_long_press(18)):
				break;
  			case (key_long_long_press(18)):
				break;


			case KEY_TRIGGER(19):
				break;
  			case KEY_LONG_TRIGGER(19):
				break;
  			case KEY_LONG_LONG_TRIGGER(19):				
				break;
  			case (key_press(19)):
				SendKeyValue(19);
				break;
  			case (key_long_press(19)):
				break;
  			case (key_long_long_press(19)):
				break;


			case KEY_TRIGGER(20):
				break;
  			case KEY_LONG_TRIGGER(20):
				break;
  			case KEY_LONG_LONG_TRIGGER(20):				
				break;
  			case (key_press(20)):
				LedFlash(21,1, 5000, 100);
				SendKeyValue(20);
				break;
  			case (key_long_press(20)):
				break;
  			case (key_long_long_press(20)):
				break;


			case KEY_TRIGGER(21):
				break;
  			case KEY_LONG_TRIGGER(21):
				break;
  			case KEY_LONG_LONG_TRIGGER(21):				
				break;
  			case (key_press(21)):
				LedFlash(22,1, 5000, 100);
				SendKeyValue(21);
				break;
  			case (key_long_press(21)):
				break;
  			case (key_long_long_press(21)):
				break;


			case KEY_TRIGGER(22):
				break;
  			case KEY_LONG_TRIGGER(22):
				break;
  			case KEY_LONG_LONG_TRIGGER(22):				
				break;
  			case (key_press(22)):
				SendKeyValue(22);
				break;
  			case (key_long_press(22)):
				break;
  			case (key_long_long_press(22)):
				break;


			case KEY_TRIGGER(23):
				break;
  			case KEY_LONG_TRIGGER(23):
				LedStartFlash(24);
				break;
  			case KEY_LONG_LONG_TRIGGER(23):	
				LedStopFlash(24);			
				break;
  			case (key_press(23)):
				break;
  			case (key_long_press(23)):
				LedStopFlash(24);	
				SetLed(24,0);
				SendKeyValue(23);
				NVIC_SystemReset();				
				break;
  			case (key_long_long_press(23)):
				break;
*/
		
  		}
}



void SetPassWord(unsigned char pw)
{
	DevicePW.pw[DevicePW.pw_point] = pw;
	DevicePW.pw_point++;
}

unsigned char CheckPassWord(unsigned char password_length)
{
	unsigned char loopx = 0;
	DevicePW.pw_point = 0;
	if(password_length != _pw_length)
		{
			return 0;
		}
	for(loopx = 0 ; loopx < password_length ; loopx++)
		{
			if(DevicePW.pw[loopx] != _pw[loopx])
				{
					return 0;
				}
		}
	return 1;
}




