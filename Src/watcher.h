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
��ʼ���һ����������������ֵ���Ŀ��ֵ��ʱ������з�����Ϣ
�������Ĳ����������Դ������Դ�������ȣ����Ŀ��ֵ��֪ͨ���У�֪ͨ��Ϣ���κ�һ�ͬ���½�һ�������
*************************/
unsigned char register_watcher(void* watcher_source,unsigned char watcher_length,unsigned int target_value,osMessageQId msg_queue,unsigned char msg);



/************************
register_callback_function_into_watcher 
����������ע��ص�����������ص�ֵ���Ŀ��ֵʱ���ûص�����
*************************/
void register_callback_function_into_watcher(unsigned char space, Void_Function_Int function);

/************************
Stop_Watcher 
ֹͣ���
*************************/
void Stop_Watcher(void* watcher_source);
#endif


