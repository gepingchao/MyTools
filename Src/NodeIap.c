#include "NodeIap.h"

#define NODE_CAN_RECV_ID	0X333

unsigned char IAPSendData[300] = {0};


void IapSendDelay(void)
{
	//unsigned short delay = 1300;//500K ==> 1600  1M ==> 800
	unsigned short delay = 2500;//500K ==> 1600  1M ==> 800
	while(delay--);
}

unsigned short Bootloader_CalcPacketChecksum(unsigned char* buffer, unsigned short size)
{
        unsigned short  sum = 0u;

        while (size > 0u)
        {
            sum += buffer[size - 1u];
            size--;
        }

        return(( unsigned short )1u + ( unsigned short )(~sum));
}


void NodeIapSendData(unsigned char* data , unsigned short data_length)
{
	unsigned char loop_send_times = 0;	
	unsigned char send_frame = 0;
	loop_send_times = (data_length%8)? (data_length/8+1):(data_length/8);
	while(loop_send_times--)
		{
			//Hal_CanSend((data + send_frame*8),8,NODE_CAN_RECV_ID);
			//send_frame++;
			//IapSendDelay();
			//osDelay(1);

			//while(HAL_CAN_GetTxMailboxesFreeLevel( &hcan ) == 0);
			/*
			{
				IapSendDelay();
			}
			*/
			Hal_CanSend((data + send_frame*8),8,NODE_CAN_RECV_ID);
			send_frame++;
			IapSendDelay();
		}
}


void Iap_SendDataToNode(unsigned char CMD,unsigned char*data,unsigned short data_length)
{
	unsigned char loop_send_data = 0;
	U_Short_char CheckSum;
	memset(IAPSendData,0,data_length);

	IAPSendData[0] = SOF_CODE;
	IAPSendData[1] = CMD;
	IAPSendData[2] = (data_length);
	IAPSendData[3] = (data_length)>>8;
	
	for(loop_send_data = 0 ; loop_send_data < data_length ; loop_send_data++)
		{
			IAPSendData[loop_send_data +4] = *(data + loop_send_data) ;
		}
	
	CheckSum.short_ = Bootloader_CalcPacketChecksum(IAPSendData,(data_length + 4));
	IAPSendData[4 + data_length] = CheckSum.char_.char_l;
	IAPSendData[5 + data_length] = CheckSum.char_.char_h;
	
	IAPSendData[6 + data_length] = EOF_CODE;

	//SendToCOM1(IAPSendData,(data_length + OTHER_CODE));
	NodeIapSendData(IAPSendData,(data_length + OTHER_CODE));
}



void EnterIapModCMD(void)
{	
	unsigned char can_send_data[10] = {0};
	/*can_send_data[0] = SOF_CODE;
	can_send_data[1] = Bootloader_1_COMMAND_ENTER;
	can_send_data[2] = 0X00;
	can_send_data[3] = 0X00;
	can_send_data[4] = 0XC7;
	can_send_data[5] = 0XFF;
	can_send_data[6] = 0X17;
	Hal_CanSend(can_send_data,7,NODE_CAN_RECV_ID);*/

	Iap_SendDataToNode(Bootloader_1_COMMAND_ENTER,can_send_data,0);
}

void IapSendTest(void)
{
	unsigned char Send_Data[40] = {0};
	unsigned char loop_send_data = 0;
	unsigned short send_data_length = 40;
	U_Short_char CheckSum;

	for(loop_send_data = 4 ; loop_send_data < 40 ; loop_send_data++)
		{
			Send_Data[loop_send_data] = loop_send_data ;
		}
	Send_Data[0] = SOF_CODE;
	Send_Data[1] = Bootloader_1_COMMAND_DATA;
	Send_Data[2] = (send_data_length - OTHER_CODE);
	Send_Data[3] = (send_data_length - OTHER_CODE)>>8;
	
	CheckSum.short_ = Bootloader_CalcPacketChecksum(Send_Data,(send_data_length-3));
	Send_Data[37] = CheckSum.char_.char_l;
	Send_Data[38] = CheckSum.char_.char_h;
	
	Send_Data[39] = EOF_CODE;
	
	NodeIapSendData(Send_Data,send_data_length);
}




