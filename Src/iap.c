#include "iap.h"
#include <string.h>

APP_INFORMATION App_info;
S_Recv_Iap_Info IapInfo;
IAP vIAP;

S_Master_Iap_Info MasterIapInfo;

#define PTR_IAP_RECV_DATA	(DataFromCOM1.operat_data)
//#define ARRARY_IAP_RECV_DATA	data_from_RS232.operat_data
#define ARRARY_IAP_RECV_DATA	PTR_IAP_RECV_DATA


#define IAP_RECV_DATA_LENGTH	(DataFromCOM1.data_len)



void IapSendData(unsigned char*data,unsigned short data_length)
{
	SendToCOM1(data,data_length);
}



















typedef  void (*pFunction)(void);
pFunction JumpToApplication;
uint32_t JumpAddress;

void test_jump(void)
{
	HAL_Init();	
	disable_all_irq();
	SystemClock_Config();
	if (((*(__IO uint32_t*)APP_2_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
		{
			/* Jump to user application */
			JumpAddress = *(__IO uint32_t*) (APP_2_ADDRESS + 4);
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) APP_2_ADDRESS);
			JumpToApplication();
		} 
}

void jump_to_app(unsigned int app_addr)
{
	//HAL_Init();
	//HAL_RCC_DeInit();
	//SystemClock_Config();	
	//disable_all_irq();
	if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000 ) == 0x20000000)
		{
			/* Jump to user application */
			JumpAddress = *(__IO uint32_t*) (app_addr + 4);
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) app_addr);
			JumpToApplication();
		} 	
}


void disable_all_irq(void)
{
	unsigned char loopx;
	for(loopx = 0;loopx< 60;loopx ++)
		{			
			HAL_NVIC_DisableIRQ((IRQn_Type)loopx);
			NVIC_ClearPendingIRQ((IRQn_Type)loopx);
		}
}

void try_to_jump_to_next_app(void)
{
	try_to_jump(NEXT_APP);
}
	

void try_to_jump(unsigned char app)
{
	switch(app)
		{
			case APP_BASE:
			App_info.try_to_jump = APP_BASE_ADDRESS;
			App_info.app_offset_table[0] = 0;
			break;

			case APP_1:
			App_info.try_to_jump = APP_1_ADDRESS;	
			App_info.app_offset_table[1] = 0;
			break;

			case APP_2:
			App_info.try_to_jump = APP_2_ADDRESS;	
			App_info.app_offset_table[2] = 0;
			break;

			//case APP_3:
			//App_info.try_to_jump = APP_3_ADDRESS;
			//App_info.app_offset_table[3] = 0;
			//break;
			
			default :
			App_info.try_to_jump = APP_BASE_ADDRESS;
			App_info.app_offset_table[0] = 0;
			break;
		}
	App_info.tem_jump = try_jump_flag;
	write_app_info();
	NVIC_SystemReset();
}

void read_app_info(void)
{
	read_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}

void write_app_info(void)
{
	write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}
void reset_iap_buff(void)
{
	unsigned short loop;
	for(loop = 0;loop<FLASH_PAGE_SIZE;loop++)
		{
			vIAP.Page_buffer[loop] = 0XFF;
		}
}
void choose_app(void)
{
	if((App_info.tem_jump == try_jump_flag)||(App_info.tem_jump == 0XFF))
		{
			App_info.tem_jump = no_jump;
			if(App_info.try_to_jump == THIS_APP_ADDR)
				{
					App_info.jump_to_app = App_info.try_to_jump;
					App_info.try_to_jump = 0X01234567;
					write_app_info();
					return;
				}
			else
				{					
					write_app_info();
					/////jump to try_to_jump app
					jump_to_app(App_info.try_to_jump);
				}
		}
	else
		{
			if((App_info.try_to_jump == APP_BASE_ADDRESS)||(App_info.try_to_jump == APP_1_ADDRESS)||(App_info.try_to_jump == APP_2_ADDRESS)||(App_info.try_to_jump == APP_3_ADDRESS))
				{
					if(App_info.try_to_jump == THIS_APP_ADDR)   //跳转成功
						{
							App_info.jump_to_app = App_info.try_to_jump;
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							return;
						}
					else		//跳转失败
						{
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							jump_to_app(App_info.jump_to_app);
						}
				}
			if(App_info.jump_to_app == THIS_APP_ADDR)
				{
					if(App_info.jump_to_app == APP_BASE_ADDRESS)
						{
							if(App_info.app_offset_table[0] != App_info.jump_to_app )
								{
									App_info.app_offset_table[0] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == APP_1_ADDRESS)
						{
							if(App_info.app_offset_table[1] != App_info.jump_to_app )
								{
									App_info.app_offset_table[1] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == APP_2_ADDRESS)
						{
							if(App_info.app_offset_table[2] != App_info.jump_to_app )
								{
									App_info.app_offset_table[2] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == APP_3_ADDRESS)
						{
							if(App_info.app_offset_table[3] != App_info.jump_to_app )
								{
									App_info.app_offset_table[3] = App_info.jump_to_app;
									write_app_info();
								}
						}
					
					return;
				}
			if((App_info.jump_to_app != APP_BASE_ADDRESS)&&(App_info.jump_to_app != APP_1_ADDRESS)&&(App_info.jump_to_app != APP_2_ADDRESS)&&(App_info.jump_to_app != APP_3_ADDRESS)) //修改跳转bug

				{	
					App_info.jump_to_app = THIS_APP_ADDR;
					write_app_info();
					return;
				}
			jump_to_app(App_info.jump_to_app);
		}
	return;
}






void ClearRetryFlag(void)
{
	IapInfo.need_retry = 0;
	IapInfo.timer_set_flag = 0;
}

void IapStopRetryFunction(void)
{
	if(1 == IapInfo.start_retry)
		{
			IapInfo.start_retry = 0;//关闭超时重传
			timer_free(IapInfo.retry_timer);
		}
}



void RetryFunction(void* arg)
{
	if(1 == IapInfo.timer_set_flag)
		{
			if(1 == IapInfo.need_retry)
				{
					if(0 == IapInfo.retry_times)
						{
							IapStopRetryFunction();
							return;
						}
					IapInfo.retry_times --;
					IapRequestPage(IapInfo.request_page);
				}
			IapInfo.need_retry = 1;
		}
	IapInfo.timer_set_flag = 1;
}

void IapStartRetryFunction(unsigned short retry_interval,unsigned char retry_time)
{
	IapInfo.retry_times = retry_time;
	IapInfo.retry_timer = register_value_into_timer(retry_interval,1);
	if(0XFF != IapInfo.retry_timer)
		{
			IapInfo.start_retry = 1;//启动超时重传标志位
			register_callback_function_into_timer(IapInfo.retry_timer,RetryFunction);
		}
}


void init_save_data(void)
{
	//reset_iap_buff();
	read_app_info();
	if(App_info.data_effect != 0X88)//0X88作为标志位,表示之前保存过数据,数据有效
		{	
			App_info.data_effect = 0X88;
			App_info.tem_jump= no_jump;
			App_info.jump_to_app = THIS_APP_ADDR;
			
			App_info.app_offset_table[0] = 0;
			App_info.app_offset_table[1] = 0;
			App_info.app_offset_table[2] = 0;
			App_info.app_offset_table[3] = 0;
			App_info.cur_app_offset= App_info.app_offset_table[App_info.app_point] ;
			write_app_info();
		}
	
}

void my_gpio_init(void)
{
  //GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin */
  //GPIO_InitStruct.Pin = KEY_1_Pin;
  //GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //HAL_GPIO_Init(KEY_1_GPIO_Port, &GPIO_InitStruct);

}

#define JUMP_TO_BASE	(0 == READ_KEY_STATUS(1))

void start_app(void)//在工程最开始时调用
{
	my_gpio_init();
	init_save_data();
	//if(JUMP_TO_BASE)//key1  按下
	if(0)//key1  按下
		{
			//while(JUMP_TO_BASE);//等待按键松开
			while(0);//等待按键松开
			try_to_jump(APP_BASE);
		}
	choose_app();
	IapInfo.IapCalCrc = GetProtocolCRC16;
}








////////////////////////////////////////////////////////////////////////////////new protocol//////////////////






void RetryFunction_(void* arg)
{
	if(1 == IapInfo.timer_set_flag)
		{
			if(1 == IapInfo.need_retry)
				{
					if(0 == IapInfo.retry_times)
						{
							IapStopRetryFunction();
							return;
						}
					IapInfo.retry_times --;
					IapRequestPage(IapInfo.request_page);
				}
			IapInfo.need_retry = 1;
		}
	IapInfo.timer_set_flag = 1;
}
void IapStartRetryFunction_(unsigned short retry_interval,unsigned char retry_time)
{
	IapInfo.retry_times = retry_time;
	IapInfo.retry_timer = register_value_into_timer(retry_interval,1);
	if(0XFF != IapInfo.retry_timer)
		{
			IapInfo.start_retry = 1;//启动超时重传标志位
			register_callback_function_into_timer(IapInfo.retry_timer,RetryFunction_);
		}
}




void IapRequestPage(unsigned short page)
{
//A5  5A  00  0D  F2  00  02  00  00  00  06  CRC16
	
	U_Short_char tmp_short;
	tmp_short.short_ = page;
	unsigned char Tx_data[20] = {0};
	
	//__SKIP_FUNCTION_IF_NEQU__((Device.Me.role),role_master);
	
	ClearRetryFlag();
	
	Tx_data[0] = 0X5A;
	Tx_data[1] = 0XA5;
	Tx_data[2] = 0X00;
	Tx_data[3] = 0X0D;
	Tx_data[4] = 0XF2;
	Tx_data[5] = 0X00;
	Tx_data[6] = 0X02;
	Tx_data[7] = 0X00;
	Tx_data[8] = 0X00;
	
	Tx_data[9] = tmp_short.char_.char_h;
	Tx_data[10] = tmp_short.char_.char_l;
	
	tmp_short.short_ = 0;
	
	if(IapInfo.IapCalCrc)
		{
			tmp_short.short_ = IapInfo.IapCalCrc(Tx_data,11);
		}
	Tx_data[11] = tmp_short.char_.char_h;
	Tx_data[12] = tmp_short.char_.char_l;	
	
	IapSendData(Tx_data, 13);
}



void IapSendReadyCode(void)
{
//A5  5A  00  0D  F0  00  02  00  00  00  02  CRC16
	U_Short_char crc;
	crc.short_ = 0;
	unsigned char Tx_data[20] = {0};
	
	//__SKIP_FUNCTION_IF_NEQU__((Device.Me.role),role_master);
	
	Tx_data[0] = 0X5A;
	Tx_data[1] = 0XA5;
	Tx_data[2] = 0X00;
	Tx_data[3] = 0X0D;
	Tx_data[4] = 0XF0;
	Tx_data[5] = 0X00;
	Tx_data[6] = 0X02;
	Tx_data[7] = 0X00;
	Tx_data[8] = 0X00;
	
	Tx_data[9] = (DEVICE_SOFT_VER>>8)&0XFF;
	Tx_data[10] = (DEVICE_SOFT_VER)&0XFF;
	
	if(IapInfo.IapCalCrc)
		{
			crc.short_ = IapInfo.IapCalCrc(Tx_data,11);
		}
	Tx_data[11] = crc.char_.char_h;
	Tx_data[12] = crc.char_.char_l;	
	
	IapSendData(Tx_data, 13);
}


void IapStartOperat(P_S_IapRecvFrame frame)
{
	U_Short_char tmp_short;
	U_Int_char tmp_int;
	
	tmp_short.short_ = 0;
	tmp_int.int_ = 0;
	
	IapInfo.total_size = 0;
	IapInfo.recved_page_sum = 0;
	IapInfo.request_page = 0;
	IapInfo.block = 0;
	IapInfo.page = 0;
	IapInfo.correct_time = 0;

	tmp_short.char_.char_h = *((frame->cmd_arg)+0);
	tmp_short.char_.char_l = *((frame->cmd_arg)+1);
	IapInfo.target_version = tmp_short.short_;

	tmp_int.char_.char_3 = *((frame->cmd_arg)+2);
	tmp_int.char_.char_2 = *((frame->cmd_arg)+3);
	tmp_int.char_.char_1 = *((frame->cmd_arg)+4);
	tmp_int.char_.char_0 = *((frame->cmd_arg)+5);
	IapInfo.total_size = tmp_int.int_;

	IapInfo.total_page = (IapInfo.total_size)/IAP_PAGE_SIZE;
	IapInfo.last_block = (IapInfo.total_page - 1)/(FLASH_PAGE_SIZE / IAP_PAGE_SIZE);
	
	if((IapInfo.total_size)%IAP_PAGE_SIZE)
		{
			IapInfo.total_page ++;
		}
	
	//__SKIP_FUNCTION_IF_NEQU__((Device.Me.role),role_master);
	
	IapRequestPage(0);
	IapStartRetryFunction_(1000,15);
}

void IapSendEndCode(void)
{
	//A5  5A  00  0B  F3  00  00  00  00  CRC16
	U_Short_char crc;
	crc.short_ = 0;
	
	unsigned char Tx_data[20] = {0};
	
	//__SKIP_FUNCTION_IF_NEQU__((Device.Me.role),role_master);
	
	Tx_data[0] = 0X5A;
	Tx_data[1] = 0XA5;
	Tx_data[2] = 0X00;
	Tx_data[3] = 0X0B;
	Tx_data[4] = 0XF3;
	Tx_data[5] = 0X00;
	Tx_data[6] = 0X00;
	Tx_data[7] = 0X00;
	Tx_data[8] = 0X00;

	
	if(IapInfo.IapCalCrc)
		{
			crc.short_ = IapInfo.IapCalCrc(Tx_data,9);
		}
	Tx_data[9] = crc.char_.char_h;
	Tx_data[10] = crc.char_.char_l;	
	
	IapSendData(Tx_data, 11);
	IapStopRetryFunction();
}


void SlaveIapSendEndCode(void)
{
	//A5  5A  00  0B  F4  00  00  00  00  CRC16
	U_Short_char crc;
	crc.short_ = 0;
	
	unsigned char Tx_data[20] = {0};
	
	//__SKIP_FUNCTION_IF_NEQU__((Device.Me.role),role_master);
	
	Tx_data[0] = 0X5A;
	Tx_data[1] = 0XA5;
	Tx_data[2] = 0X00;
	Tx_data[3] = 0X0B;
	Tx_data[4] = 0XF4;
	Tx_data[5] = 0X00;
	Tx_data[6] = 0X00;
	Tx_data[7] = 0X00;
	Tx_data[8] = 0X00;

	
	if(IapInfo.IapCalCrc)
		{
			crc.short_ = IapInfo.IapCalCrc(Tx_data,9);
		}
	Tx_data[9] = crc.char_.char_h;
	Tx_data[10] = crc.char_.char_l;	
	
	IapSendData(Tx_data, 11);
	//IapStopRetryFunction();
}


void SystemIapCompleteSendEndCodeCBFunction(int arg)
{
	IapSendEndCode();
}
void SystemIapCompleteSendEndCode(void)
{
	IapInfo.SystemIapComplete = 0;
	register_callback_function_into_watcher((register_watcher((void *)&(IapInfo.SystemIapComplete), sizeof(unsigned char),1, NULL,0)),SystemIapCompleteSendEndCodeCBFunction);
}


void IapCopyToFlash(P_S_IapRecvFrame frame)
{	
	if(IapInfo.recved_page < IapInfo.total_page -1)
		{
			if(IapInfo.cur_page_size < IAP_PAGE_SIZE)
				{
					IapInfo.request_page = IapInfo.recved_page;
					return;
				}
		}
	IapInfo.correct_time++;
	memcopy((char*)(frame->iap_data),(char*)&vIAP.Page_buffer[IapInfo.page * IAP_PAGE_SIZE],(IapInfo.cur_page_size));
	if(IapInfo.page == ((FLASH_PAGE_SIZE / IAP_PAGE_SIZE)-1))
		{
			write_flash(WRITE_TO_ADDR+(IapInfo.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);
			//if(IapInfo.recved_page == IapInfo.total_page- 1)//如果正好没有多余的页，则不再这里清空iap buff
			if(IapInfo.block == IapInfo.last_block)//最后一块 需要将iapbuff重置
				{
					reset_iap_buff();
				}
		}
	else
		{
			//memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[IapInfo.page *128],(IapInfo.cur_page_size));
		}
}

void IapWriteToFlash(P_S_IapRecvFrame frame)
{
	U_Short_char tmp_short;
	tmp_short.short_ = 0;
	
	vIAP.owner = OWNER_WIFI_IAP_BUFFER;

	IapInfo.recved_page_sum++;
	IapInfo.recved_page =0;
	tmp_short.char_.char_h = *((frame->cmd_arg)+0);
	tmp_short.char_.char_l = *((frame->cmd_arg)+1);
	IapInfo.recved_page = tmp_short.short_;

	/*if(IapInfo.recved_page ==  IapInfo.request_page)//这一页就是需求的页 还没有操作
		{
		}else{
		return;//这一页操作过了
		}*/
	
	IapInfo.request_page = IapInfo.recved_page+1;
	IapInfo.cur_page_size = frame->iap_data_length;
	
	IapInfo.page = IapInfo.recved_page % (FLASH_PAGE_SIZE / IAP_PAGE_SIZE);
	IapInfo.block = IapInfo.recved_page /(FLASH_PAGE_SIZE / IAP_PAGE_SIZE);

	IapCopyToFlash(frame);
	
	if(IapInfo.recved_page == IapInfo.total_page- 1)//接收完成
		{
			if(IapInfo.page == ((FLASH_PAGE_SIZE / IAP_PAGE_SIZE)-1))//没有多余的页  
				{}
			else
				{
					if(IapInfo.page)
						{
							//IapInfo.block ++;
							memcopy((char*)(frame->iap_data),(char*)&vIAP.Page_buffer[IapInfo.page *IAP_PAGE_SIZE],(IapInfo.cur_page_size));
							write_flash(WRITE_TO_ADDR+(IapInfo.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);
							if(IapInfo.block == IapInfo.last_block)//最后一块 需要将iapbuff重置
								{
									reset_iap_buff();
								}
						}
				}
			
			vIAP.owner = OWNER_ON_ONE;
			IapSendEndCode();
		}
	else
		{
			IapRequestPage(IapInfo.request_page);
		}
}






void IapJumpToApp(P_S_IapRecvFrame frame)
{
	switch(*(frame->cmd_arg))
		{
			case 0:
				try_to_jump(APP_BASE);
				break;
			case 1:
				try_to_jump(APP_1);
				break;
			case 2:
				try_to_jump(APP_2);
				break;
				
		}
}

/*
解码iap数据
最后的参数是指定crc16校验方法
*/
unsigned char DecodeIapRecvData(unsigned char* data,unsigned short data_length,P_S_IapRecvFrame frame,UShort_Function_PUChar_UInt calcrc)
{
	U_Short_char tmp;	
	tmp.short_ = 0;
	
	frame->head_code_1 = *(data +0);
	frame->head_code_2 = *(data +1);
	
	tmp.char_.char_h= *(data +2);
	tmp.char_.char_l= *(data +3);
	frame->frame_length = tmp.short_;

	frame->cmd = *(data +4);

	frame->addr = *(data +5);

	frame->cmd_arg_length = *(data +6);

	tmp.short_ = 0;
	tmp.char_.char_h = *(data +7);
	tmp.char_.char_l = *(data +8);
	frame->iap_data_length = tmp.short_;

	frame->cmd_arg = (data +9);
	frame->iap_data = (data+9+(frame->cmd_arg_length));

	tmp.short_ = 0;
	//tmp.char_.char_h = *(data +9+(frame->cmd_arg_length)+(frame->iap_data_length));
	//tmp.char_.char_l = *(data +10+(frame->cmd_arg_length)+(frame->iap_data_length));
	tmp.char_.char_h = *(data + data_length -2);
	tmp.char_.char_l = *(data + data_length -1);
	frame->crc= tmp.short_;
	
	if(calcrc)//如果指定校验函数则校验否则不校验
		{
			if((frame->crc) == calcrc(data,(frame->frame_length - 2)))
				{
					return 1;
				}
			return 0;
		}
	return 1;
}

unsigned char iap_deal_recv_data(unsigned char* data,unsigned short data_length)
{
	S_IapRecvFrame frame = {0};	
	
	if(DecodeIapRecvData(data,data_length,&frame,(IapInfo.IapCalCrc)))
		{
			IapInfo.is_cur_frame_correct = 1;
		}else{
			IapInfo.is_cur_frame_correct = 0;
			return 0;
		}
//////////////////////////////////////new protocol/////////////////////////////////////////
	switch(frame.cmd)
		{
			case IAP_CMD_START:
				IapSendReadyCode();
				break;
				
			case IAP_CMD_FIRMWARE_INFO:
				IapStartOperat(&frame);
				break;
				
			case IAP_CMD_GET_PAGE_DATA:				
				IapWriteToFlash(&frame);
				break;
				
			case IAP_CMD_END:
				//收到最后一条就会发送
				//IapSendEndCode();
				break;
				
			case IAP_CMD_JUMP_TO:
				IapJumpToApp(&frame);
				break;

			default:
				break;
				
		}
	
//////////////////////////////////old protocol//////////////////////////////////////

/*
	if(*(data+7)==0XF0)//START
		{
			iap_send_ready_code();
		}	

	//if(IAP_RECV_DATA.operat_data[7]==0XF1)//READY
	
	if(*(data+7)==0XF1)//READY
		{
			IapInfo.total_size = 0;
			IapInfo.recved_page_sum = 0;
			IapInfo.request_page = 0;
			IapInfo.block = 0;
			IapInfo.page = 0;

			IapInfo.target_version |= ARRARY_IAP_RECV_DATA[8];
			IapInfo.target_version <<= 8;
			IapInfo.target_version |= ARRARY_IAP_RECV_DATA[9];

			IapInfo.total_size |= ARRARY_IAP_RECV_DATA[10];
			IapInfo.total_size <<= 8;
			IapInfo.total_size |= ARRARY_IAP_RECV_DATA[11];
			IapInfo.total_size <<= 8;
			IapInfo.total_size |= ARRARY_IAP_RECV_DATA[12];
			IapInfo.total_size <<= 8;
			IapInfo.total_size |= ARRARY_IAP_RECV_DATA[13];
			IapInfo.total_page = (IapInfo.total_size)/IAP_PAGE_SIZE;
			if((IapInfo.total_size)%IAP_PAGE_SIZE)
				{
					IapInfo.total_page ++;
				}

			iap_request_page(0);
			IapStartRetryFunction(1000,5);
		}

	if(*(data+7)==0XF2)//transfer
		{
			iap_write_to_flash();
		}

	if(*(data+7)==0XF8)//jump
		{
			if(0 == *(data+8))
				{					
					try_to_jump(APP_BASE);
				}
			if(1 == *(data+8))
				{
					try_to_jump(APP_1);				
				}
			if(2 == *(data+8))
				{
					try_to_jump(APP_2);				
				}
		}*/
		
		
		
	return 1;
}







