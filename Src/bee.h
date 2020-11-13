#ifndef __BEE_H__
#define __BEE_H__
#include "include.h"

#define BEEP_COUNT_TIMER	htim17
#define BEEP_BASE_TIMER	htim16


typedef struct
{
	unsigned short Time_Step;
	unsigned short Music_Step;
	
	unsigned short Music_Length;
	unsigned char* Music;//����
	unsigned char* Beat;//����
	unsigned char*Power;//ǿ��
}S_Music,*P_S_Music;


void ChangeBee(unsigned short pulse,unsigned short period);
void BeeCountTimeCallBack(void);

#endif
