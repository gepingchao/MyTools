#include "bitcheck.h"

unsigned char Bit_Check_32(unsigned int check_data,unsigned char bit_num)
{
	if(bit_num > 31)
		{
			return 2;
		}
	if(check_data&(1<<bit_num))
		{
			return 1;
		}
	return 0;
}

unsigned char Bit_Set_32(unsigned int* source,unsigned char bit_num)
{
	if(bit_num > 31)
		{
			return 0;
		}
	*source |= (1<<bit_num);
	return 1;
}

unsigned char Bit_Clr_32(unsigned int* source ,unsigned char bit_num)
{
	if(bit_num > 31)
		{
			return 0;
		}
	*source &= ~(1<<bit_num);
	return 1;
}



unsigned char Bit_Check_128(P_BitCheck_128 check_data,unsigned char bit_num)
{
	unsigned char row = 0;
	unsigned char col = 0;
	if(bit_num > 127)
		{
			return 2;
		}
	row = bit_num / 32;
	col = bit_num % 32;
	return Bit_Check_32((check_data->CheckBuff[row]), col);
}

unsigned char Bit_AllZeroCheck(P_BitCheck_128 check_data)
{
	if((0 == check_data->CheckBuff[0])&&(0 == check_data->CheckBuff[1])&&(0 == check_data->CheckBuff[2])&&(0 == check_data->CheckBuff[3]))
		{
			return 1;
		}
	return 0;
}

unsigned char Bit_Set_128(P_BitCheck_128 source,unsigned char bit_num)
{
	unsigned char row = 0;
	unsigned char col = 0;
	if(bit_num > 127)
		{
			return 2;
		}
	row = bit_num / 32;
	col = bit_num % 32;
	return Bit_Set_32(&((source->CheckBuff[row])), col);
}

unsigned char Bit_Clr_128(P_BitCheck_128 source ,unsigned char bit_num)
{
	unsigned char row = 0;
	unsigned char col = 0;
	if(bit_num > 127)
		{
			return 2;
		}
	row = bit_num / 32;
	col = bit_num % 32;
	return Bit_Clr_32(&((source->CheckBuff[row])), col);
}



