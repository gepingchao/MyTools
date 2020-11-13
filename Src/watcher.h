#ifndef __WATCHER_H__
#define __WATCHER_H__
#include "include.h"


#define MAX_WATCHER_NUM	4


typedef struct
{
	unsigned char watcher_value_length;
	unsigned int target_value;
	unsigned char msg;
	osMessageQId msg_queue;
	void* value_source;
	Void_Function_Int callback_function;
}S_Watcher_Value_Info,*P_S_Watcher_Value_Info;

typedef struct
{
	unsigned int space;
	osThreadId WatcherHandle;
	S_Watcher_Value_Info member[MAX_WATCHER_NUM];
}S_Watcher_Info,*P_S_Watcher_Info;
extern S_Watcher_Info Watcher;


void InitWatcher(void);

/************************
register_watcher 
开始监控一个变量，当变量的值变成目标值的时候向队列发送消息
当监控项的参数包括监控源变量，源变量长度，监控目标值，通知队列，通知消息有任何一项不同则新建一个监控项
*************************/
unsigned char register_watcher(void* watcher_source,unsigned char watcher_length,unsigned int target_value,osMessageQId msg_queue,unsigned char msg);



/************************
register_callback_function_into_watcher 
向监控任务中注册回调函数，当监控的值变成目标值时调用回调函数
*************************/
void register_callback_function_into_watcher(unsigned char space, Void_Function_Int function);

/************************
Stop_Watcher 
停止监控
*************************/
void Stop_Watcher(void* watcher_source);
#endif


