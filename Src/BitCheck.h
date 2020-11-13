#ifndef __BITCHECK_H__
#define __BITCHECK_H__

#include "include.h"


typedef struct{
	unsigned int CheckBuff[4];
}BitCheck_128,*P_BitCheck_128;

unsigned char Bit_Check_32(unsigned int check_data,unsigned char bit_num);
unsigned char Bit_Set_32(unsigned int* source,unsigned char bit_num);
unsigned char Bit_Clr_32(unsigned int* source ,unsigned char bit_num);
unsigned char Bit_Check_128(P_BitCheck_128 check_data,unsigned char bit_num);
unsigned char Bit_AllZeroCheck(P_BitCheck_128 check_data);
unsigned char Bit_Set_128(P_BitCheck_128 source,unsigned char bit_num);
unsigned char Bit_Clr_128(P_BitCheck_128 source ,unsigned char bit_num);

#endif

