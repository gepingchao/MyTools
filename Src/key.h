#ifndef __KEY_H__
#define __KEY_H__
#include "include.h"

#define KEY_CHECK_INTERVAL_TIME	40


#define KEY_7_GPIO_Port	SIGNAL_D_GPIO_Port
#define KEY_7_Pin	SIGNAL_D_Pin

#define KEY_8_GPIO_Port	SD_DET_GPIO_Port
#define KEY_8_Pin	SD_DET_Pin

#define READ_KEY_STATUS(number)		HAL_GPIO_ReadPin(KEY_##number##_GPIO_Port, KEY_##number##_Pin)

#define READ_ADDR_S_STATUS(number)	HAL_GPIO_ReadPin(ADDR_S_##number##_GPIO_Port, ADDR_S_##number##_Pin)

/*
������������:


��ѹ��������40ms��ʾ�ɿ����£���ʱ�ᴥ��KEY_TRIGGER ���������ѹ����2.5s
��ʱ��ʾ�������ᴥ��KEY_LONG_TRIGGER�����������ѹ����7s�ᴥ��KEY_LONG_LONG_TRIGGER��ʾ���ѹ


����KEY_TRIGGER֮�󣬴���KEY_LONG_TRIGGER֮ǰ̧�𰴼��ᴥ��key_press
����KEY_LONG_TRIGGER֮�󣬴���KEY_LONG_LONG_TRIGGER֮ǰ̧�𰴼��ᴥ��key_long_press
����KEY_LONG_LONG_TRIGGER֮��̧�𰴼��ᴥ��key_long_long_press


*/



#define KEY_HEAD_0	0X56
#define KEY_HEAD_1	0X89

#define KEY_NUMBER		8



#define KEY_BASE		((KEY_NUMBER)*0)
#define KEY_TRIGGER(X)		(KEY_BASE +(X))

#define KEY_LONG_BASE		((KEY_NUMBER)*1)
#define KEY_LONG_TRIGGER(X)	(KEY_LONG_BASE+(X))	


#define KEY_LONG_LONG_BASE		((KEY_NUMBER)*2)
#define KEY_LONG_LONG_TRIGGER(X)	(KEY_LONG_LONG_BASE+(X))


#define KEY_PERRS_BASE	((KEY_NUMBER)*3)
#define key_press(x)  (KEY_PERRS_BASE + 0X01 +3*(x))
#define key_long_press(x)  (KEY_PERRS_BASE + 0X02 +3*(x))
#define key_long_long_press(x)	(KEY_PERRS_BASE + 0X03+3*(x))

#define EC11_TURN_P	((KEY_NUMBER)*7)
#define EC11_TURN_N	((KEY_NUMBER)*7+1)


typedef struct
{
	unsigned char key_point;
	unsigned char key_ok[KEY_NUMBER];
	unsigned char key_value[KEY_NUMBER];
	unsigned char  press_time[KEY_NUMBER];
}key_data_def;
extern key_data_def rtos_key;


typedef struct
{
	unsigned char pw_point:4;
	unsigned char pw[16];
	unsigned char start_get_pw_flag;
	unsigned char end_get_pw_flag;
}PassWord;

extern osMessageQId KeyQueueHandle;

void read_key(void);
void deal_key_value(unsigned char key_value);
unsigned char GetAddrValue(void);
void Lock_Start_Operat(unsigned char lock_num);

void SetPassWord(unsigned char pw);
unsigned char CheckPassWord(unsigned char password_length);
unsigned char ScanEC11(void);

#endif
