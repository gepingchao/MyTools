#ifndef __CD4052_H__
#define __CD4052_H__
#include "include.h"

#define RS485_PATH_1	1
#define RS485_PATH_2	3
#define CAN_PATH_1	0
#define CAN_PATH_2	2

void Enable_Path(unsigned char Path_Num);
#endif

