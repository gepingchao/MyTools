#include "flash.h"


/**************
USE_RAM_BUFF  如果这个宏置1则会在ram中创建一块flash操作缓存
可以减少flash擦除次数，但会占用ram2 2K 的空间
当需要频繁操作flash时这里建议置1
注:当USE_RAM_BUFF置1时，数据通过UP_DATA操作同步到flash中

如果这个宏置0 则不会创建ram缓存，此时的读写是直接对flash
操作，如果flash不是经常擦除，并且ram 空间紧张这里建议置0

注:当USE_RAM_BUFF置0时，UP_DATA操作无效
***************/
#define USE_RAM_BUFF	0


#if	USE_RAM_BUFF
FLASH_R_W_BUFF flash_buff;
#endif

/*******
flash_message:
flash进程通信的通信帧，在同一个进程中，不可重复使用同一个通信帧

*********************************************************
WRONG:
void function(void)
{
	flash_operat(READ_FLASH,recv_buff,40,0,30,1,&flash_message_1);
	flash_operat(WRITE_FLASH,test_buff,40,0,30,1,&flash_message_1);
	flash_operat(READ_FLASH,recv_buff,40,0,30,1,&flash_message_1);
}
*********************************************************
CORRECT:
void function(void)
{
	flash_operat(READ_FLASH,recv_buff,40,0,30,1,&flash_message_1);
	flash_operat(WRITE_FLASH,test_buff,40,0,30,1,&flash_message_2);
	flash_operat(READ_FLASH,recv_buff,40,0,30,1,&flash_message_3);
}
*********************************************************

*******/
#define USE_QUEUE 0

#if USE_QUEUE
//flash_message flash_message_1;
//flash_message flash_message_2;
//flash_message flash_message_3;
//flash_message flash_message_4;
//flash_message flash_message_5;
#endif
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


/********
flash 操作时，flash是共享资源，为了确保共享资源的合理操作，
建立一个进程专门操作flash，其他进程无法直接操作flash。
其他进程通过队列与flash进程通信，收到flash操作指令后，flash进程
会做相应动作，FLASH_QUEUE来指定进程间通信的队列
*********/
#define FLASH_QUEUE	FlashQueueHandle


uint32_t Address = 0, PAGEError = 0;
uint32_t PageError = 0;
__IO TestStatus MemoryProgramStatus = PASSED;

static FLASH_EraseInitTypeDef EraseInitStruct;


unsigned char write_flash(unsigned address,char* data,unsigned short len)
{
	__disable_irq();
	erase_flash(address);
	MemoryProgramStatus = PASSED;
	unsigned short count = 0;
	char* data_tmp;
	data_tmp = data;
	HAL_FLASH_Unlock();
	Address = address;
	    while (count < len)
	    {
	      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, *(short*)data_tmp) == HAL_OK)
	      {
	        Address = Address + 2;
		data_tmp= data_tmp+ 2;
		count = count +2;
	      }
	      else
	      {
	        /* Error occurred while writing data in Flash memory. 
	           User can add here some code to deal with this error */	        
	      }
	    }
	
	__enable_irq();
	    /* Check the correctness of written data */
	    Address = address;
		data_tmp = data;
		count = 0;
		while (count < len)				
		    {
		      if((*(__IO uint32_t*) Address) != *((unsigned int*)data_tmp))
		      {
		        MemoryProgramStatus = FAILED;
		      }
		      Address += 4;			  
			data_tmp= data_tmp+ 4;
			count = count +4;
		    }
				
	  HAL_FLASH_Lock();
	   if (MemoryProgramStatus == PASSED)
	    	{
	    		return 1;
	    	}
	   else
	   	{
	   		return 0;
	   	}
 
} 

unsigned char read_flash(unsigned address,char* data,unsigned short len)
{
	  char* data_tmp;
	 data_tmp = data;
	 unsigned short count = 0;
	 MemoryProgramStatus = PASSED;
	 Address = address;
	 while (count < len)				
		    {
		      *data_tmp = (*(__IO uint8_t*) Address);
		      Address += 1;			  
			data_tmp= data_tmp+ 1;
			count = count +1;
		    }
	 
	    /* Check the correctness of read data */
	    Address = address;
		data_tmp = data;
		count = 0;
		while (count < len)				
		    {
		      if((*(__IO uint8_t*) Address) != *data_tmp)
		      {
		        MemoryProgramStatus = FAILED;
		      }
		      Address += 1;			  
			data_tmp= data_tmp+ 1;
			count = count +1;
		    }
		
	  if (MemoryProgramStatus == PASSED)
	    	{
	    		return 1;
	    	}
	   else
	   	{
	   		return 0;
	   	}
}

void erase_flash(unsigned int address)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages     = 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	  {
	    /*
	      Error occurred while page erase.
	      User can add here some code to deal with this error.
	      PAGEError will contain the faulty page and then to know the code error on this page,
	      user can call function 'HAL_FLASH_GetError()'
	    */
	    /* Infinite loop */	   
	  }
	HAL_FLASH_Lock();
}

void memcopy(char* from_addr,char* to_addr,unsigned short length)
{
	char* tmp_from_addr;
	char* tmp_to_addr;
	tmp_from_addr = from_addr;
	tmp_to_addr = to_addr;
	while(length --)
		{
			*(tmp_to_addr ++) = *(tmp_from_addr ++);
		}
}

void memcopy_reverse(char* from_addr,char* to_addr,unsigned short length)
{
	char* tmp_from_addr;
	char* tmp_to_addr;
	unsigned char step;
	step = length;
	
	tmp_from_addr = from_addr;
	tmp_to_addr = to_addr;
	while(length --)
		{
			step--;
			*(tmp_to_addr ++) = *(tmp_from_addr + step);
		}
}


#if USE_RAM_BUFF
void flash_r_w_from_queue(unsigned int message_addr)
{
	flash_message* p_flash_message;
	p_flash_message = (flash_message*)message_addr;
	
	if(p_flash_message ->this_message_effect != MESSAGE_EFFECT)
		{
			return;
		}
	if(p_flash_message ->r_w_flag ==UPDATA_FLASH)
		{
			if(flash_buff.buff_info.is_buff_changed == 1)
				{
					write_flash((0X08000000 + flash_buff.buff_info.page_num *(FLASH_PAGE_SIZE)),(char*)flash_buff.r_w_buff,FLASH_PAGE_SIZE);
					flash_buff.buff_info.is_buff_changed = 0;
				}
			return;
		}
	if((p_flash_message ->page_num == flash_buff.buff_info.page_num)&&(flash_buff.buff_info.data_effect == DATA_EFFECT))
		{
			if(p_flash_message ->r_w_flag == WRITE_BUFF)
				{
					memcopy(p_flash_message ->from_addr,(char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->length);
					flash_buff.buff_info.is_buff_changed = 1;
				}
			if(p_flash_message ->r_w_flag == READ_BUFF)
				{
					memcopy((char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->to_addr,p_flash_message ->length);
				}
			return;
		}

	else
		{
			if((flash_buff.buff_info.data_effect == DATA_EFFECT) && (flash_buff.buff_info.is_buff_changed ==1))
				{
					write_flash((0X08000000 + flash_buff.buff_info.page_num *(FLASH_PAGE_SIZE)),(char*)flash_buff.r_w_buff,FLASH_PAGE_SIZE);
				}
			flash_buff.buff_info.page_num = p_flash_message ->page_num;
			
			read_flash((0X08000000 + flash_buff.buff_info.page_num *(FLASH_PAGE_SIZE)),(char*)flash_buff.r_w_buff,FLASH_PAGE_SIZE);
			flash_buff.buff_info.data_effect = DATA_EFFECT;
			
			if(p_flash_message ->r_w_flag == WRITE_BUFF)
				{
					memcopy(p_flash_message ->from_addr,(char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->length);
					flash_buff.buff_info.is_buff_changed = 1;
				}
			if(p_flash_message ->r_w_flag == READ_BUFF)
				{
					memcopy((char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->to_addr,p_flash_message ->length);
				}
			return;
		}
}
#else

void flash_r_w_from_queue(unsigned int message_addr)
{
	flash_message* p_flash_message;
	p_flash_message = (flash_message*)message_addr;
	
	if(p_flash_message ->this_message_effect != MESSAGE_EFFECT)
		{
			return;
		}
	if(p_flash_message ->r_w_flag ==UPDATA_FLASH)
		{
			return;
		}		
	if(p_flash_message ->r_w_flag == WRITE_BUFF)
		{			
			write_flash((0X08000000 + (p_flash_message->page_num) *(FLASH_PAGE_SIZE)),(char*)(p_flash_message->from_addr),(p_flash_message->length));
		}
	if(p_flash_message ->r_w_flag == READ_BUFF)
		{
			read_flash((0X08000000 + (p_flash_message->page_num) *(FLASH_PAGE_SIZE)),(char*)(p_flash_message->to_addr),(p_flash_message->length));
		}
	return;
}
#endif


#if USE_QUEUE
void flash_operat(FLASH_OPERATION OPEART,unsigned char*addr,unsigned char page_num,unsigned short offset,unsigned short length,unsigned char owner,flash_message* flash_messages)
{
	unsigned int send_to_queue;
	switch(OPEART)
		{
			case(WRITE_FLASH):
				flash_messages ->from_addr = (char*)addr;
				flash_messages ->length =length;
				flash_messages ->offset = offset;
				flash_messages ->owner = owner;
				flash_messages ->page_num = page_num;
				flash_messages ->r_w_flag = WRITE_BUFF;
				flash_messages ->this_message_effect = MESSAGE_EFFECT;
				send_to_queue = (unsigned int)flash_messages;
				xQueueSend(FLASH_QUEUE,&send_to_queue, 50);
				break;

			case(READ_FLASH):
				flash_messages ->to_addr = (char*)addr;
				flash_messages ->length =length;
				flash_messages ->offset = offset;
				flash_messages ->owner = owner;
				flash_messages ->page_num = page_num;
				flash_messages ->r_w_flag = READ_BUFF;
				flash_messages ->this_message_effect = MESSAGE_EFFECT;
				send_to_queue = (unsigned int)flash_messages;
				xQueueSend(FLASH_QUEUE,&send_to_queue, 50);
				break;

			case(UP_DATA):
				flash_messages ->r_w_flag = UPDATA_FLASH;
				flash_messages ->this_message_effect = MESSAGE_EFFECT;
				send_to_queue = (unsigned int)flash_messages;
				xQueueSend(FLASH_QUEUE,&send_to_queue, 50);
				break;
		}
}


//flash_operat(READ_FLASH,(unsigned char*)&time_table_info,26,sizeof(rtc_save_data),sizeof(Time_Table_Info),10,&flash_message_2);


void TestFlashFunction(void)
{
	static unsigned char recv_buff[16] = {0};
	static unsigned int loop128 = 0;

	while(loop128 < (128*16))
		{
			read_flash(FLASH_BASE+(loop128*16),(char*)recv_buff,16);
			loop128 ++;
			Device.Communication.send_to_android_net(recv_buff,16);
		}
	
}
#endif




