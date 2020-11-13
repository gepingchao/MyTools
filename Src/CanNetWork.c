
#include "CanNetWork.h"

#define STM32_BOARD	1
#define CYPRESS_BOARD	2

#define CUR_BOARD	STM32_BOARD
Can_Recv_Info CanRecvData;
Can_Recv_Info_FIFO CanRecvFifo;

#if (STM32_BOARD == CUR_BOARD)
unsigned char Hal_CanSend(unsigned char* data,unsigned char data_length,unsigned int ID)
{	
	if(data_length > 8)
		{
			return 0;
		}
	
	CAN_TxHeaderTypeDef TxMessage1 = {0};
	TxMessage1.DLC=data_length;
	TxMessage1.StdId=ID;	
	TxMessage1.ExtId=0x00002000+ID;
	TxMessage1.IDE=CAN_ID_STD;
	TxMessage1.RTR=CAN_RTR_DATA;
	unsigned int mailbox;

	if(HAL_CAN_AddTxMessage(&hcan,&TxMessage1,data,&mailbox)!=HAL_OK)
		{
		}
	return 1;
}


/*****************
设置can的接收过滤器，
只接收帧ID 为ID1 	ID2	ID3	ID4的帧	
*****************/
void SetCanFliter16Bit_IDList_Mode(unsigned int ID1,unsigned int ID2,unsigned int ID3,unsigned int ID4)
{
	unsigned int id1 = ID1;
	unsigned int id2 = ID2;
	unsigned int id3 = ID3;
	unsigned int id4 = ID4;
	CAN_FilterTypeDef sFilterConfig = {0};
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
	sFilterConfig.FilterIdHigh = id1<<5;
	sFilterConfig.FilterIdLow = id2<<5;
	sFilterConfig.FilterMaskIdHigh = id3<<5;
	sFilterConfig.FilterMaskIdLow = id4<<5;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;  
	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
}

/*****************
设置can的接收过滤器，
例:
ID=0X100	ID2=0X005	MASK1=0XF00		MASK2=0X00F
则帧ID为1XX的和XX5的可以接收    108	505 ...	
*****************/
void SetCanFliter16Bit_IDMask_Mode(unsigned int ID1,unsigned int ID2,unsigned int MASK1,unsigned int MASK2)
{
	unsigned int id1 = ID1;
	unsigned int id2 = ID2;
	unsigned int msk1 = MASK1;
	unsigned int msk2 = MASK2;
	CAN_FilterTypeDef sFilterConfig = {0};
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
	sFilterConfig.FilterIdHigh = id1<<5;	
	sFilterConfig.FilterIdLow = id2<<5;
	
	//sFilterConfig.FilterMaskIdHigh =(msk1<<5)|0X10;//只接收数据帧
	//sFilterConfig.FilterMaskIdLow =(msk2<<5)|0X10;//只接收数据帧
	
	sFilterConfig.FilterMaskIdHigh = msk1<<5;
	sFilterConfig.FilterMaskIdLow = msk2<<5;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
}

void CanFrameDelay(void)
{
	osDelay(1);
	//unsigned int delay = 1500;
	//while(delay--);
}
#endif

#if (CYPRESS_BOARD == CUR_BOARD)
unsigned char Hal_CanSend(unsigned char* data,unsigned char data_length,unsigned int ID)
{	
	if(data_length > 8)
		{
			return 0;
		}
	unsigned char loopx = 0;
	CAN_1_DATA_BYTES_MSG send_data;
	for(loopx = 0 ; loopx < data_length ; loopx ++)
		{
			send_data.byte[loopx] = *(data+loopx);
		}
	CAN_1_TX_MSG MSG_1;
	MSG_1.dlc = data_length;
	MSG_1.id = ID;
	MSG_1.rtr = 0;
	MSG_1.ide = 0;
	MSG_1.msg = &send_data;
	CAN_1_SendMsg(&MSG_1);
    return 1;
}

void CanFrameDelay(void)
{
	//CyDelay(1);
	unsigned int delay = 200;
	while(delay--);
}
#endif


unsigned char CanSingleFrameSend(unsigned char* data,unsigned short data_length,unsigned int ID)
{
	CanPayLoad payload = {0};
	payload.ProtCtrlInfo = PCI_SINGLE;
	payload.ExtCtrlTnfo = data_length;
	memcpy((unsigned char*)&(payload.ExtData[0]),data,data_length);
	Hal_CanSend((unsigned char*)&payload,8,ID);
	return 1;
}

unsigned char CanFirstFrameSend(unsigned char* data,unsigned short data_length,unsigned int ID)
{
	CanPayLoad payload = {0};
	payload.ProtCtrlInfo = PCI_FIRST_FRAME;
	payload.ExtCtrlTnfo = (data_length)/256;
	payload.ExtData[0] = (data_length)%256;	
	memcpy((unsigned char*)&(payload.ExtData[1]),data,6);//第一帧中还包括数据的前6个字节
	Hal_CanSend((unsigned char*)&payload,8,ID);
	return 1;
}
unsigned char CanConsecutiveFrameSend(unsigned char* data,unsigned short data_length,unsigned int ID)//连续帧
{
	static unsigned char** p_p_data = NULL;
	p_p_data = &data;
	*p_p_data += 6;
	unsigned short RemainingDataLength = data_length - 6;
	unsigned char SendTimes = RemainingDataLength/7;
	unsigned char LastSendLength = RemainingDataLength%7;
	
	CanPayLoad payload = {0};
	unsigned char SN = 0;
	payload.ProtCtrlInfo = PCI_CONSECUTIVE_FRAME;
	
	while(SendTimes--)
		{
			payload.ExtCtrlTnfo = SN++;
			memcpy((unsigned char*)&(payload.ExtData[0]),*p_p_data,7);			
			*p_p_data += 7;
			Hal_CanSend((unsigned char*)&payload,8,ID);
			CanFrameDelay();
		}
	if(LastSendLength)
		{
			payload.ExtCtrlTnfo = SN;
			memcpy((unsigned char*)&(payload.ExtData[0]),*p_p_data,LastSendLength);	
			Hal_CanSend((unsigned char*)&payload,(LastSendLength + 1),ID);
		}
	return 1;
}
	
unsigned char CanMultipleFrameSend(unsigned char* data,unsigned short data_length,unsigned int ID)
{
	CanFirstFrameSend(data,data_length,ID);
	CanFrameDelay();
	CanConsecutiveFrameSend(data,data_length,ID);
	return 1;
}
unsigned char CanSend(unsigned char* data,unsigned short data_length,unsigned int ID)
{
	if(data_length < 8)// 单帧发送
		{
			CanSingleFrameSend(data,data_length,ID);		
		}
	else
		{
			CanMultipleFrameSend(data,data_length,ID);
		}
	return 1;
}

/////////////////////////////////////////////////////////RECV///////////////////////////////////////////

#define DEAL_RECV_DATA_IN_ISR	1
#if (STM32_BOARD == CUR_BOARD)

unsigned char GetFifoWritePoint_First(CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo)
{
	unsigned char loopx = 0;
	if(0 == RxHeader->StdId)
		{
			return 0XFF;
		}
	for(loopx = 0 ;loopx < MAX_CAN_FIFO_LENGTH ; loopx ++)
		{
			if((recv_fifo->RECV_DATA[loopx].ID == RxHeader->StdId)&&(0 == recv_fifo->RECV_DATA[loopx].Recv_OK)&&(0 == recv_fifo->RECV_DATA[loopx].Recv_ING))
				{
						return loopx;
				}
		}
	recv_fifo->Write_Point ++;
	return recv_fifo->Write_Point;
}


unsigned char GetFifoWritePoint_Consecutive(CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo)
{
	unsigned char loopx = 0;
	if(0 == RxHeader->StdId)
		{
			return 0XFF;
		}
	for(loopx = 0 ;loopx < MAX_CAN_FIFO_LENGTH ; loopx ++)
		{
			if((recv_fifo->RECV_DATA[loopx].ID == RxHeader->StdId)&&(0 == recv_fifo->RECV_DATA[loopx].Recv_OK)&&(1 == recv_fifo->RECV_DATA[loopx].Recv_ING))
				{
						return loopx;
				}
		}
	return 0XFF;
}

void RecvSingleFrame(P_CanPayLoad payload,CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo)
{
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;	
	unsigned char MSG = CAN_SINGLE_MSG;
	
	unsigned char Write_point = 0;
	P_Can_Recv_Info recv_info = NULL;
	Write_point = GetFifoWritePoint_First(RxHeader, recv_fifo);
	MSG = Write_point;
	if(Write_point > MAX_CAN_FIFO_LENGTH)
		{
			return;
		}
	recv_info = &(recv_fifo->RECV_DATA[Write_point]);
	recv_fifo->RECV_DATA[Write_point].ID = RxHeader->StdId;
	
	recv_info->Data_length = payload->ExtCtrlTnfo;
	recv_info->ID = RxHeader->StdId;
	recv_info->Recv_OK = 1;
	recv_info->Recv_ING= 1;
	
	memcpy((unsigned char *)(recv_info->RecvBuff),payload->ExtData,(recv_info->Data_length));
	//recv end
	
	xQueueSendFromISR((CanQueueHandle),&(MSG),&xHigherPriorityTaskWoken);

}

void RecvFirstFrame(P_CanPayLoad payload,CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo)
{
	unsigned char Write_point = 0;
	P_Can_Recv_Info recv_info = NULL;
	Write_point = GetFifoWritePoint_First(RxHeader, recv_fifo);
	if(Write_point > MAX_CAN_FIFO_LENGTH)
		{
			return;
		}
	recv_info = &(recv_fifo->RECV_DATA[Write_point]);
	recv_info->ID = RxHeader->StdId;	
	recv_info->Recv_ING= 1;
	
	
	recv_info->Data_length = payload->ExtCtrlTnfo*256 + payload->ExtData[0];
		
	memcpy((unsigned char *)(recv_info->RecvBuff),&(payload->ExtData[1]),6);
}

void RecvConsecutiveFrame(P_CanPayLoad payload,CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo)
{
	
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;	
	unsigned char MSG = CAN_MULTIP_MSG;
	
	unsigned char Write_point = 0;
	P_Can_Recv_Info recv_info = NULL;
	Write_point = GetFifoWritePoint_Consecutive(RxHeader, recv_fifo);

	
	MSG = Write_point;
	if(Write_point > MAX_CAN_FIFO_LENGTH)
		{
			return;
		}
	recv_info = &(recv_fifo->RECV_DATA[Write_point]);
	
	memcpy((unsigned char *)(recv_info->RecvBuff + 6 + (payload->ExtCtrlTnfo)*7),(payload->ExtData),(RxHeader->DLC-1));
	if(recv_info->Data_length == (6 + (payload->ExtCtrlTnfo)*7 + (RxHeader->DLC-1)))
		{
			//recv_end	
			recv_info->Recv_OK = 1;
			recv_info->Recv_ING= 0;
			//CheckCanFrame(recv_fifo,MSG);
			
			xQueueSendFromISR((CanQueueHandle ),&(MSG),&xHigherPriorityTaskWoken);
			//HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
			//HAL_GPIO_TogglePin(LED_Y_GPIO_Port,LED_Y_Pin);
						
		}
}


void RecvCanFrame_Stm32(unsigned char* data,CAN_RxHeaderTypeDef* RxHeader,P_Can_Recv_Info_FIFO recv_fifo)
{
	CanPayLoad payload = {8,8,8,8,8,8,8,8};
	
	
	memcpy((unsigned char*)&payload,data,RxHeader->DLC);
	switch(payload.ProtCtrlInfo)
		{
			case PCI_SINGLE:
				RecvSingleFrame(&payload,RxHeader,recv_fifo);
				//recv end				
				break;

			case PCI_FIRST_FRAME:
				RecvFirstFrame(&payload,RxHeader,recv_fifo);
				
				break;

			case PCI_CONSECUTIVE_FRAME:
				RecvConsecutiveFrame(&payload,RxHeader,recv_fifo);
				
				break;

			case PCI_FLOW_CONTROL_FRAME:
				break;

			default:
				break;
					
		}
}
#endif


#if (CYPRESS_BOARD == CUR_BOARD)

#define RECV_FRAME_MAIL	CAN_1_RX_DATA_BYTE
#define RECV_FRAME_MAIL_BYTE(n)	CAN_1_RX_DATA_BYTE##n


void RecvSingleFrame(P_CanPayLoad payload,P_Can_Recv_Info recv_info)
{
	recv_info->Data_length = payload->ExtCtrlTnfo;
	memcpy((unsigned char *)(recv_info->RecvBuff),payload->ExtData,(recv_info->Data_length));
	//recv end
	LED_G_Write(1);
	LED_Y_Write(1);	
}

void RecvFirstFrame(P_CanPayLoad payload,P_Can_Recv_Info recv_info)
{
	recv_info->Data_length = payload->ExtCtrlTnfo*256 + payload->ExtData[0];
	memcpy((unsigned char *)(recv_info->RecvBuff),&(payload->ExtData[1]),6);
	
	LED_G_Write(1);
	LED_Y_Write(1);	
}

void RecvConsecutiveFrame(P_CanPayLoad payload,P_Can_Recv_Info recv_info)
{
	memcpy((unsigned char *)(recv_info->RecvBuff + 6 + (payload->ExtCtrlTnfo)*7),(payload->ExtData),7);
	if(recv_info->Data_length <= (6 + (payload->ExtCtrlTnfo)*7 + 7))
		{
			//recv_end				
			LED_G_Write(0);
			LED_Y_Write(0);	
			
		}
}


void RecvCanFrame_Cpy(P_CanPayLoad payload,P_Can_Recv_Info recv_info)
{
	unsigned char TmpData[8] = {0};
	TmpData[0] = RECV_FRAME_MAIL_BYTE(1)[0];
	TmpData[1] = RECV_FRAME_MAIL_BYTE(2)[0];
	TmpData[2] = RECV_FRAME_MAIL_BYTE(3)[0];
	TmpData[3] = RECV_FRAME_MAIL_BYTE(4)[0];
	TmpData[4] = RECV_FRAME_MAIL_BYTE(5)[0];
	TmpData[5] = RECV_FRAME_MAIL_BYTE(6)[0];
	TmpData[6] = RECV_FRAME_MAIL_BYTE(7)[0];
	TmpData[7] = RECV_FRAME_MAIL_BYTE(8)[0];	
	memcpy((unsigned char*)&payload,TmpData,8);
	
	switch(payload.ProtCtrlInfo)
		{
			case PCI_SINGLE:
				RecvSingleFrame(&payload,recv_info);
				//recv end				
				break;

			case PCI_FIRST_FRAME:
				RecvFirstFrame(&payload,recv_info);
				
				break;

			case PCI_CONSECUTIVE_FRAME:
				RecvConsecutiveFrame(&payload,recv_info);
				
				break;

			case PCI_FLOW_CONTROL_FRAME:
				break;

			default:
				break;
					
		}
}
#endif

