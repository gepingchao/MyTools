#ifndef __TIMER_OPERAT_H__
#define __TIMER_OPERAT_H__
#include "include.h"

#define COUNTER_TIMER	htim15


#define MAX_COUNTVALUE_NUMBER	4

//MAX_COUNTVALUE_NUMBER  �����Գ���32

#define MSEC		1
#define SECOND	(1000*(MSEC))
#define MINUTE	(60*(SECOND))
#define HOUR	(60*(MINUTE))
#define DAY		(24*(HOUR))
#define WEEK	(7*(DAY))

typedef void (*P_TimerCounter_Deal_Function)(void*);

typedef struct 
{
	//unsigned char value_length;//��ʱ���ĵĳ���
	unsigned char repeatability;//��ʱ���Ƿ���ظ�
	unsigned char overflow_flag;//�����־λ

	//unsigned int init_value;//��ʱ��ֵ
	unsigned int target_value;//��ʱ��ֵֹ
	unsigned int count_value;//��ʱֵ

	osMessageQId signal_queue;//������ʱ�����֪ͨ�Ķ���
	P_TimerCounter_Deal_Function timer_counter_callback_function;
}S_Value_In_Timer,*P_S_Value_In_Timer;

typedef struct 
{
	unsigned int space;//32λ����������ָʾ��Щ�ռ䱻ռ����Щ�ռ����
	
	unsigned char* source[MAX_COUNTVALUE_NUMBER];//��ʱ������ı���Щֵ
	unsigned char change_target_value[MAX_COUNTVALUE_NUMBER];//��ʱ��ɺ���Ҫ����Щֵ�ĵ���ֵ
	
	
	S_Value_In_Timer timer[MAX_COUNTVALUE_NUMBER];
	unsigned int timer_tick;
	unsigned int timer_10ms_tick;
	unsigned int second_tick;
}S_Timer_Counter,*P_S_Timer_Counter;
extern S_Timer_Counter timer_counter;

unsigned char register_value_into_timer(unsigned int count_value,unsigned char mod);
unsigned char timer_free(unsigned char space);

void register_queue_into_timer(unsigned char space,osMessageQId queue);
void register_callback_function_into_timer(unsigned char space,P_TimerCounter_Deal_Function function);

void INC_COUNTER_TIMER(void);


void InitTimerCounter(void);


void ChangeCharDataValueAfterTimeUp(unsigned char* source_data,unsigned char target_value,unsigned int time);


unsigned int get_cur_second_tick(void);
unsigned char is_timer_counter_overflow(unsigned char space);
void clear_timer_counter_overflow_flag(unsigned char space);

#endif

