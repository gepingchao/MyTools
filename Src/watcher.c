#include "watcher.h"


S_Watcher_Info Watcher;


/*************************************监控数值改变，如果改变则发出通知************************************************/
unsigned char watcher_malloc(void)
{
	__disable_irq();    // 关闭总中断
	unsigned char loop32 = 0;
	for(;loop32 < MAX_WATCHER_NUM ; loop32++)
		{
			if(0 == Bit_Check_32((Watcher.space),loop32))
				{
					Bit_Set_32(&(Watcher.space),loop32);
					__enable_irq();    // 开启总中断
					return loop32;
				}
		}
	__enable_irq();    // 开启总中断
	return 0XFF;
}

unsigned char register_watcher(void* watcher_source,unsigned char watcher_length,unsigned int target_value,osMessageQId msg_queue,unsigned char msg)
{
	unsigned char watcher_space = 0XFF;
	unsigned char loop32 = 0;
	for(loop32 = 0 ; loop32 < MAX_WATCHER_NUM ; loop32++)
		{
			if(1 == Bit_Check_32((Watcher.space),loop32))
				{
					if((watcher_source == Watcher.member[loop32].value_source )\
						&&(watcher_length == Watcher.member[loop32].watcher_value_length)\
						&&(msg == Watcher.member[loop32].msg)\
						&&(target_value == Watcher.member[loop32].target_value)\
						&&(msg_queue == Watcher.member[loop32].msg_queue))
						{
							return 0XFF;//此监控项已经存在
						}
				}
		}

	
	watcher_space = watcher_malloc();//新建监控项
	if(0XFF == watcher_space)
		{
			return 0XFF;
		}
	Watcher.member[watcher_space].msg = msg;
	Watcher.member[watcher_space].msg_queue = msg_queue;
	Watcher.member[watcher_space].target_value = target_value;
	Watcher.member[watcher_space].value_source = watcher_source;
	Watcher.member[watcher_space].watcher_value_length= watcher_length;
	return watcher_space;
}

void register_callback_function_into_watcher(unsigned char space, Void_Function_Int function)
{
	if(1 != Bit_Check_32((Watcher.space),space))
		{
			return;
		}
	Watcher.member[space].callback_function = function;
}

void Stop_Watcher(void* watcher_source)
{
	__disable_irq();    //  关闭总中断
	unsigned char loop32 = 0;
	for(loop32 = 0; loop32 < MAX_WATCHER_NUM ; loop32++)
		{
			if(Bit_Check_32((Watcher.space),loop32))
				{
					if(watcher_source == Watcher.member[loop32].value_source)
						{
							Bit_Clr_32(&(Watcher.space),loop32);
						}
				}
		}
	__enable_irq();    // 开启总中断	
}

void Watcher_Operat(void)
{
	unsigned char loop32 = 0;
	unsigned int source_value;
	for(loop32 = 0; loop32 < MAX_WATCHER_NUM ; loop32++)
		{
			if(Bit_Check_32((Watcher.space),loop32))
				{
					switch((Watcher.member[loop32].watcher_value_length))
						{
							case 1:
								source_value = *((unsigned char*)(Watcher.member[loop32].value_source));
								break;
								
							case 2:
								source_value = *((unsigned short*)(Watcher.member[loop32].value_source));
								break;
								
							case 4:
								source_value = *((unsigned int*)(Watcher.member[loop32].value_source));
								break;
							default:
								Bit_Clr_32(&(Watcher.space),loop32);
								break;
						}
					if(source_value == (Watcher.member[loop32].target_value))
						{
							Bit_Clr_32(&(Watcher.space),loop32);//清除标志位
							if(Watcher.member[loop32].msg_queue)
								{
									xQueueSend((Watcher.member[loop32].msg_queue),&(Watcher.member[loop32].msg),50);
								}
							if(Watcher.member[loop32].callback_function)
								{
									Watcher.member[loop32].callback_function(loop32);
								}
						}
				}
		}
}

void WatcherTask(void const * argument)
{
	save_task_info();
	while(1)
		{
			Watcher_Operat();
			osDelay(20);//监控数值的轮询周期
		}
}



void StartWatcherProcess(int arg)
{	
  osThreadDef(Watcher, WatcherTask, osPriorityLow, 0, 200);
  Watcher.WatcherHandle = osThreadCreate(osThread(Watcher), (void*)arg);
}

void InitWatcher(void)
{
	StartWatcherProcess(0);
}

void test_watcher(void)
{
	static unsigned char test_watcher_value = 0;
	ChangeCharDataValueAfterTimeUp(&(test_watcher_value), 123, 4*SECOND);

	register_watcher((void*)&test_watcher_value, sizeof(unsigned char),123, 0, 22);
}

