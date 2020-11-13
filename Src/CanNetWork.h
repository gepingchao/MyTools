#ifndef __CANNETWORK_H__
#define __CANNETWORK_H__
#include "include.h"

#define CAN_SINGLE_MSG		1
#define CAN_MULTIP_MSG		2


/**************************************************
PayLoad 定义

       			 	   第一个字节        												 第二个字节      第三个字节      第四个字节...
       			 	 高两位 低六位

单帧					 0		有效字节数															  DATA1					DATA2
多帧首帧			 1		与第二个字节一起表示有效字节数													DATA1
多帧连续帧		 2		帧编号																		DATA1					DATA2
多帧控制帧		 3			
**************************************************/

typedef enum {
	PCI_SINGLE = 0x0,
	PCI_FIRST_FRAME = 0x1,
	PCI_CONSECUTIVE_FRAME = 0x2,
	PCI_FLOW_CONTROL_FRAME = 0x3
} ProtocolControlInformation;


typedef struct{
	unsigned char ExtCtrlTnfo:6;
	unsigned char ProtCtrlInfo:2;
	unsigned char ExtData[7];
}CanPayLoad,*P_CanPayLoad;



#define MAX_RECV_DATA_LENGTH	100
typedef struct{
	unsigned char Recv_OK;//接收完成标志
	unsigned char Recv_ING;//处理完成标志
	unsigned short Data_length;
	unsigned int ID;

	unsigned char CheckByte;
	unsigned int ErrorNum;
	unsigned int RightNum;
	//BitCheck_128 BitCheck;
	unsigned char RecvBuff[MAX_RECV_DATA_LENGTH];
}Can_Recv_Info,*P_Can_Recv_Info;
extern Can_Recv_Info CanRecvData;



#define MAX_CAN_FIFO_LENGTH	4
typedef struct
{
	unsigned char Write_Point:2;
	Can_Recv_Info RECV_DATA[MAX_CAN_FIFO_LENGTH];
}Can_Recv_Info_FIFO,*P_Can_Recv_Info_FIFO;
extern Can_Recv_Info_FIFO CanRecvFifo;

#if (STM32_BOARD == CUR_BOARD)
unsigned char Hal_CanSend(unsigned char* data,unsigned char data_length,unsigned int ID);
#endif

unsigned char CanSend(unsigned char* data,unsigned short data_length,unsigned int ID);
void RecvCanFrame_Stm32(unsigned char* data,CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo);
void SetCanFliter16Bit_IDMask_Mode(unsigned int ID1,unsigned int ID2,unsigned int MASK1,unsigned int MASK2);
void SetCanFliter16Bit_IDList_Mode(unsigned int ID1,unsigned int ID2,unsigned int ID3,unsigned int ID4);




void RecvCanFrame_Cpy(P_CanPayLoad payload,P_Can_Recv_Info recv_info);

#endif

