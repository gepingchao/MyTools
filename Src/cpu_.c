#include "cpu_.h"

int Cpu_Use_Rate_Tick ;


osThreadId IDlEhandle = NULL;
osThreadId monitor_handle = NULL;

S_SYS_Task_Info SYS_Task_Info;


void start_monitor_task(void)
{
	monitor_handle = xTaskGetCurrentTaskHandle();
}

void StartTaskMonitor(void)
{
	TaskHandle_t task_handle;
	task_handle = xTaskGetCurrentTaskHandle() ;
    	save_task_start_time(task_handle);
}


void EndTaskMonitor(void)
{
	TaskHandle_t task_handle;
	task_handle = xTaskGetCurrentTaskHandle() ;
	save_task_end_time(task_handle);
}

void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
	if(IDlEhandle == NULL)
		{
			IDlEhandle = xTaskGetCurrentTaskHandle();
			start_monitor_task();
			save_task_info();
		}
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
if(Cpu_Use_Rate_Tick ++ >CALCULATION_PERIOD)  //每CALCULATION_PERIOD个tick,刷新一次CPU使用率   
	{
		Cpu_Use_Rate_Tick = 0;
		count_task_run_time();
	}
}

void save_task_info(void)
{
	unsigned char loop20;
	TaskHandle_t task_handle;
	task_handle = xTaskGetCurrentTaskHandle();
	for(loop20 = 0;loop20 < MAX_SAVE_TASK_INFO_NUM; loop20++ )
		{
			if(task_handle == SYS_Task_Info.task[loop20].task_handle)
				{
					return;
				}
			continue;
		}
	for(loop20 = 0;loop20 < MAX_SAVE_TASK_INFO_NUM; loop20++ )
		{
			if(SYS_Task_Info.task[loop20].task_handle)
				{
				}
			else
				{
					break;
				}
		}
	/*SYS_Task_Info.task[SYS_Task_Info.save_number ].empty_stack = (uxTaskGetStackHighWaterMark(task_handle)*4);
	SYS_Task_Info.task[SYS_Task_Info.save_number ].task_handle = task_handle;
	SYS_Task_Info.task[SYS_Task_Info.save_number ].name = ((My_tskTCB*)task_handle)-> pcTaskName;
	SYS_Task_Info.task[SYS_Task_Info.save_number ].priority = (char)(((My_tskTCB*)task_handle)-> uxPriority);
	SYS_Task_Info.save_number ++;*/

	SYS_Task_Info.task[loop20].empty_stack = (uxTaskGetStackHighWaterMark(task_handle)*4);
	SYS_Task_Info.task[loop20].task_handle = task_handle;
	SYS_Task_Info.task[loop20].name = ((My_tskTCB*)task_handle)-> pcTaskName;
	SYS_Task_Info.task[loop20].priority = (char)(((My_tskTCB*)task_handle)-> uxPriority);
	SYS_Task_Info.save_number ++;

}


void save_task_start_time(osThreadId task_handle)
{
	unsigned loopx = 0;
	if(NULL == task_handle)
		{
			return;
		}
	for(;loopx<MAX_SAVE_TASK_INFO_NUM;loopx++)
		{
			if(task_handle == SYS_Task_Info.task[loopx].task_handle)
				{
					SYS_Task_Info.task[loopx].run_time.start_time=xTaskGetTickCountFromISR();
					return;
				}
		}
}


void save_task_end_time(osThreadId task_handle)
{
	unsigned loopx = 0;
	if(NULL == task_handle)
		{
			return;
		}
	for(;loopx<MAX_SAVE_TASK_INFO_NUM;loopx++)
		{
			if(task_handle == SYS_Task_Info.task[loopx].task_handle)
				{
					SYS_Task_Info.task[loopx].run_time.spent_time = xTaskGetTickCountFromISR() - SYS_Task_Info.task[loopx].run_time.start_time ;
					SYS_Task_Info.task[loopx].run_time.totall_time += SYS_Task_Info.task[loopx].run_time.spent_time;
					return;
				}
		}
}
void delete_task_info(TaskHandle_t target_task_handle)
{
	unsigned char loopx;
	if(NULL == target_task_handle)
		{
			return;
		}
	for(loopx = 0; loopx < MAX_SAVE_TASK_INFO_NUM; loopx ++)
		{
			if(SYS_Task_Info.task[loopx].task_handle == target_task_handle)
				{
					SYS_Task_Info.task[loopx].task_handle = NULL;
					SYS_Task_Info.save_number --;
					break;
				}
		}
}


void count_task_run_time(void)
{
	unsigned loopx = 0;
	for(;loopx<MAX_SAVE_TASK_INFO_NUM;loopx++)
		{
			SYS_Task_Info.task[loopx].run_time.curr_run_time = (SYS_Task_Info.task[loopx].run_time.totall_time > 1000? 1000 : SYS_Task_Info.task[loopx].run_time.totall_time);			
			SYS_Task_Info.task[loopx].cur_task_cpu_use_rate = (SYS_Task_Info.task[loopx].run_time.curr_run_time * 100) /CALCULATION_PERIOD;
			if(SYS_Task_Info.task[loopx].task_handle == IDlEhandle)
				{
					if( SYS_Task_Info.task[loopx].cur_task_cpu_use_rate <=100)
						{
							SYS_Task_Info.cur_cpu_use_rate = 100 - SYS_Task_Info.task[loopx].cur_task_cpu_use_rate;
						}					
				}
			if(!SYS_Task_Info.get_stack_step)
				{
					SYS_Task_Info.task[loopx].empty_stack = (uxTaskGetStackHighWaterMark(SYS_Task_Info.task[loopx].task_handle)*4);
				}
			SYS_Task_Info.task[loopx].run_time.totall_time = 0;
		}
	SYS_Task_Info.get_stack_step++;
}

int test_cpu_speed(unsigned int loop_outside,unsigned int loop_inside)
{
	static int loop_out,loop_in,sum;
	
	for(loop_out = 0 ; loop_out < loop_outside ; loop_out++)
		{
			for(loop_in = 0 ; loop_in < loop_inside ; loop_in ++)
				{
					sum ++;
				}
		}
	return sum;
}
/**************
当主频为72MHZ时，1ms可以计算1388次
***************/

#define __SHOW_MEM_INFO__		1

void show_mem(void)
{
	#if __SHOW_MEM_INFO__
	
	unsigned char send_buff[150] ={0};
	char* p_malloc;
	int stack_addr = *((int*)0X08000000);
	int heap_addr;
	
	int stack_set_size = 0X400;
	int heap_set_size = 0X800;
	
	int free_stack_size = stack_set_size;
	heap_addr = stack_addr-stack_set_size-heap_set_size;

	while(!(*((int*)(stack_addr - free_stack_size))))
		{
			free_stack_size -= 4;
		}

	p_malloc = (char*)malloc(1);

	sprintf((char*)send_buff,"\r\n_heap start addr 0X%x _heap total size is %d byte _heap_used size is %d byte free size is %d byte\r\n",heap_addr,heap_set_size,heap_set_size-(stack_addr - stack_set_size-(int)p_malloc),(stack_addr - stack_set_size-(int)p_malloc));

	sprintf((char*)send_buff,"stack start addr 0X%x stack total size is %d byte stack_used size is %d byte free size is %d byte\n\r",stack_addr,stack_set_size,free_stack_size,(stack_set_size - free_stack_size));

	free(p_malloc);
	//free(next_malloc);
		
	#endif
	
}


//////////////////////////事故保存现场


/****
//修改入口函数

HardFault_Handler\
                PROC
			IMPORT  hard_fault_handler_c 
			TST LR, #4  
			ITE EQ     
			MRSEQ R0, MSP 
			MRSNE R0, PSP 
			B  hard_fault_handler_c                
			ENDP 

			EXPORT  HardFault_Handler          [WEAK]
			B       .
			ENDP

****/
void hard_fault_handler_c(unsigned int * hardfault_args) 
{     
  static System_Crash_Info crash_info;
  memset(&crash_info, 0, sizeof(System_Crash_Info));
  
  crash_info.is_crash = 1;
  crash_info.crash_time = (unsigned int)HAL_GetTick();
  
  crash_info.stacked_r0 = ((unsigned long) hardfault_args[0]);  
  crash_info.stacked_r1 = ((unsigned long) hardfault_args[1]);  
  crash_info.stacked_r2 = ((unsigned long) hardfault_args[2]);  
  crash_info.stacked_r3 = ((unsigned long) hardfault_args[3]);  
  crash_info.stacked_r12 = ((unsigned long) hardfault_args[4]);    
  crash_info.stacked_lr = ((unsigned long) hardfault_args[5]);   
  crash_info.stacked_pc = ((unsigned long) hardfault_args[6]);  
  crash_info.stacked_psr = ((unsigned long) hardfault_args[7]); 

  crash_info.MFSR = (*((volatile unsigned char *)(0xE000ED28))); //存储器管理fault状态寄存器   
  crash_info.BFSR = (*((volatile unsigned char *)(0xE000ED29))); //总线fault状态寄存器   
  crash_info.UFSR = (*((volatile unsigned short int *)(0xE000ED2A)));//用法fault状态寄存器    
  crash_info.HFSR = (*((volatile unsigned long *)(0xE000ED2C)));  //硬fault状态寄存器     
  crash_info.DFSR = (*((volatile unsigned long *)(0xE000ED30))); //调试fault状态寄存器  
  crash_info.MMAR = (*((volatile unsigned long *)(0xE000ED34))); //存储管理地址寄存器  
  crash_info.BFAR = (*((volatile unsigned long *)(0xE000ED38))); //总线fault地址寄存器  
  
 // u8 ret = STMFLASH_EraseSector(STMFLASH_GetFlashSector(SYS_CRASH_INFO_ADDR));
  //u8 ret2 = STMFLASH_Write(SYS_CRASH_INFO_ADDR, (u32 *)(&crash_info), (3+sizeof(System_Crash_Info))/4);
write_flash((127*FLASH_PAGE_SIZE + FLASH_BASE), (unsigned char *)(&crash_info), (sizeof(System_Crash_Info)));
  while (1);  
}


void Enter_HardFault(void)
{
	unsigned char buf[10] = {0};
	unsigned char loopx;
	for(loopx = 0 ; loopx < 100 ; loopx++)
		{
			buf[loopx] = loopx;
		}
}


