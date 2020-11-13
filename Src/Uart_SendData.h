#ifndef 	__UART_SEND_DATA_H__
#define __UART_SEND_DATA_H__
#include "include.h"

#define DEBUG_NONE  	0
#define DEBUG_USER	1
#define DEBUG_SYS	2

#define DEBUG DEBUG_NONE

#if (DEBUG > 0)

#define  DEBUG_USR_ErrLog(...)   printf("USR_DEBUG: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\r\n");
#else
#define DEBUG_USR_ErrLog(...)
#endif

#if (DEBUG > 1)

#define  DEBUG_SYS_ErrLog(...)   printf("SYS_DEBUG: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\r\n");
#else
#define DEBUG_SYS_ErrLog(...)
#endif

typedef struct 
{
	unsigned char byte_l:4;  //ʮ�����Ƶ���λ
	unsigned char byte_h:4;//ʮ�����Ƹ���λ
}HEX_BYTE;

#pragma anon_unions 
typedef union  //ת��ʮ���������ݽṹ
{
	HEX_BYTE BYTE;
	unsigned char reslut;	
}CHANGE_TO_HEX;

void SendToCOM1(unsigned char* data,unsigned short data_length);
void SendToCOM3(unsigned char* data,unsigned short data_length);


void ShowAllTagID(void);

#endif

