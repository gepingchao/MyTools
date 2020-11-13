#include "protocol.h"



const unsigned char CMD_BYTE[4] = {0X7C,0X7D,0X7E,0X7F};
unsigned int TimerCount;
unsigned char UPLOAD_BUFF[3280];//转义完成后的数组，上传的网关的数组
unsigned char PROTOCOL_SEND_BUFF[3280];//将数据摆在这个数组中，然后做转义

//unsigned char graph_buff[3000];//将数据摆在这个数组中，然后做转义

unsigned char PROTOCOL_RECV_BUFF[400];
unsigned char G_Cur_Operat_Addr;
unsigned int G_CFG_ERROR_Flag = 0XFFFFFF;

unsigned int G_COUNT = 0;
extern unsigned char sensor_upload_mod;
#define PRE_TREAT_START_OFFSET	4

unsigned short G_Iap_Delay = 400;
unsigned short G_Iap_STARTUP_Delay = 300;

unsigned char G_EnterIapMod = 0;

unsigned int G_Net_Check_Time = 20;


S_CtrlFrame Rs232frame = {0};
S_CtrlFrame Rs485frame = {0};

S_Cmd RS232_CMD_List[] = 
{
{.function_code = PROTOCOLDEMO,			.cmd_length = 0,.function = CMD_Demo,.help = "protocol explain Demo"},//协议解析例子
{.function_code = FUNCTION_TEST,			.cmd_length = 0,.function = McuFunctionTest,.help = "protocol explain Demo"},//协议解析例子

{.function_code = CTRL_SENSOR_NET_START_UPLOAD,			.cmd_length = 0,.function = CMD_StartUploadSensorNetData,.help = "protocol explain Demo"},//协议解析例子

{.function_code = CTRL_SENSOR_NET,			.cmd_length = 0,.function = CMD_CtrlSensorNet,.help = "protocol explain Demo"},//协议解析例子
{.function_code = GET_SENSOR_NET_INFO,			.cmd_length = 0,.function = CMD_UploadSensorNetInfo,.help = "protocol explain Demo"},//协议解析例子

{.function_code = CTRL_UPLOAD_GRAPH_DATA,			.cmd_length = 0,.function = CMD_CtrlUploadSensorNetInfo,.help = "protocol explain Demo"},//协议解析例子
{.function_code = CTRL_SYSTEM_REBOOT,			.cmd_length = 0,.function = CMD_CtrlSystemReboot,.help = "protocol explain Demo"},//协议解析例子
{.function_code = CTRL_SENSOR_POWER,			.cmd_length = 0,.function = CMD_CtrlSensorPower,.help = "protocol explain Demo"},//协议解析例子

//{.function_code = GET_SENSOR_UID,			.cmd_length = 0,.function = DealUploadDeviceInfo,.help = "protocol explain Demo"},//协议解析例子

{.function_code = TRAN_IAP_CMD,			.cmd_length = 0,.function = TranIapCMD,.help = "protocol explain Demo"},//转发IAP升级命令

};

S_Cmd CAN_CMD_List[] = 
{
{.function_code = PROTOCOLDEMO,			.cmd_length = 0,.function = CMD_Demo,.help = "protocol explain Demo"},//协议解析例子
{.function_code = FUNCTION_TEST,			.cmd_length = 0,.function = McuFunctionTest,.help = "protocol explain Demo"},//协议解析例子

{.function_code = CTRL_SENSOR_NET,			.cmd_length = 0,.function = CMD_CtrlSensorNet,.help = "protocol explain Demo"},//协议解析例子
{.function_code = GET_SENSOR_UID,			.cmd_length = 0,.function = DealUploadDeviceInfo,.help = "protocol explain Demo"},//协议解析例子
{.function_code = GET_SENSOR_INFO,			.cmd_length = 0,.function = DealUploadSensorInfo,.help = "protocol explain Demo"},//协议解析例子
{.function_code = DEVICE_ACK,			.cmd_length = 0,.function = DealDeviceAck,.help = "protocol explain Demo"},//协议解析例子

};

#define RFID_CRC_POLYNOMIAL 0x8408
#define CRC_POLYNOMIAL 0XA001

#define PRESET_VALUE 0xffff

unsigned short GetProtocolCRC16(unsigned char* pData,unsigned int DataLen)
{
	int i;
	int j;
	unsigned	int current_crc_value=PRESET_VALUE;

	for(i=0;i<DataLen;i++)  /*len=number of protocol bytes without CRC*/
		{
		current_crc_value=current_crc_value^((unsigned  int)pData[i]);
		for(j=0;j<8;j++)
			{
			if(current_crc_value&0x0001)
				{
				current_crc_value=(current_crc_value>>1)^CRC_POLYNOMIAL;
				}
			else
				{
				current_crc_value=(current_crc_value>>1);
				}
			}
		}
	return (unsigned short)current_crc_value;
}


unsigned char CMD_Demo(char* arg)
{
	return 1;
}

unsigned char CMD_StartUploadSensorNetData(char* arg)
{
	unsigned char tmp_cmd = *(arg);
	switch(tmp_cmd)
		{
			case CTRL_SENSOR_NET_REBOOT:
				break;
				
			case CTRL_SENSOR_NET_START://触发开关
				//G_SensorNetDownLoadFlag = INVENTORY_PROCESS;
				break;
				
			case CTRL_SENSOR_NET_STOP:
				//G_SensorNetDownLoadFlag = CAN_NET_STOP;
				break;
				
			default :
				break;
		}
	return 1;
}


unsigned char CMD_CtrlSensorNet(char* arg)
{
	unsigned char s_cmd = *(arg);
	U_Short_char tmp;
	/*switch(s_cmd)
		{
			case CTRL_SENSOR_NET_UPLOAD_MOD:
				sensor_upload_mod = *(arg + 1);
				G_SensorNetDownLoadFlag = INVENTORY_PROCESS;
				if(0 == sensor_upload_mod)
				{
					G_SensorNetDownLoadFlag = CAN_NET_STOP;
				}
				
				break;
				
			case CTRL_SENSOR_NET_START_AUTO_SET://自动配置			
				G_SensorNetDownLoadFlag = CMD_PROCESS_START_AUTOSET;
				break;
				
			case CTRL_SENSOR_NET_SET_CFG:
				
				//G_SensorNetDownLoadFlag = CTRL_SENSOR_NET_SET_CFG;
				G_SensorNetDownLoadFlag = CMD_PROCESS_SET_CSD_PARATEMER;
				G_CsdParIdacMin = *(arg + 1);
				G_CsdParIdacMax = *(arg + 2);
				G_CsdParIdac_CMin = *(arg + 3);
				G_CsdParIdac_CMax = *(arg + 4);				
				G_CsdParSclkMin = *(arg + 5);
				G_CsdParSclkMax = *(arg + 6);
				
				tmp.char_.char_h =  *(arg + 7);
				tmp.char_.char_l =  *(arg + 8);
				G_CsdParWindow_Min = tmp.short_;
				tmp.char_.char_h =  *(arg + 9);
				tmp.char_.char_l =  *(arg + 10);
				G_CsdParWindow_Max = tmp.short_;				
				break;
		
				
				
			case CTRL_SENSOR_NET_DEVICE_CFG_SET_DEVICE_ADDR:
				G_Net_Check_Time+=2;//设置次数为2
				SensorNet.SensorBoard[*(arg + 1)].Addr = *(arg + 2);
				SensorNet.SensorBoard[*(arg + 1)].DeviceACK = 0;				
				break;
				
			case CTRL_SENSOR_NET_SET_ALL_BASELINE:
				
				tmp.char_.char_h =  *(arg + 1);
				tmp.char_.char_l =  *(arg + 2);
				if(9999 != tmp.short_)
					{
						G_BASELINE_THRESHOLE = tmp.short_;
					}
				
				tmp.char_.char_h =  *(arg + 3);
				tmp.char_.char_l =  *(arg + 4);
				if(9999 != tmp.short_)
					{
						G_BASELINE_NOISE_THRESHOLE = tmp.short_;
					}
				
				tmp.char_.char_h =  *(arg + 5);
				tmp.char_.char_l =  *(arg + 6);
				G_BASELINE_UPDATA_TIME = tmp.short_;				
				G_SensorNetDownLoadFlag = CMD_PROCESS_SET_BASELINE_PAEATEMER;
				break;						

			case CTRL_SENSOR_NET_LED_FLASH:
				tmp.char_.char_h= (SensorNet.SensorBoard[*(arg+1)].BoardUID >>8) &0XFF;
				tmp.char_.char_l = (SensorNet.SensorBoard[*(arg+1)].BoardUID) &0XFF;
				G_TRANS_VALUE = tmp.short_;
				G_SensorNetDownLoadFlag = CMD_PROCESS_CTRL_LED;
				
				break;
			default :
				break;
		}*/
	return 1;
}


void UploadDeviceNum(unsigned char num)
{
	unsigned char data[5];
	data[0] = num;
	SendDataByNewProtocol(GET_SENSOR_NET_INFO,0X1234,data,1);
}

void UploadDeviceInfo(unsigned char num)
{
	/*unsigned char data[5];
	U_Int_char UID;
	UID.int_ = SensorNet.SensorBoard[num].BoardUID;
	data[0] = UID.char_.char_3;
	data[1] = UID.char_.char_2;
	data[2] = UID.char_.char_1;
	data[3] = UID.char_.char_0;
	data[4] = 0XFF;
	if(SensorNet.SensorBoard[num].DeviceACK)
		{
			data[4] = SensorNet.SensorBoard[num].Addr;
		}
	SendDataByNewProtocol(GET_SENSOR_DEVICE_INFO,0X1234,data,5);
	*/
}

unsigned char CMD_UploadSensorNetInfo(char* arg)
{
	/*unsigned char s_cmd = *(arg);
	switch(s_cmd)
		{
			case(GET_SENSOR_NET_INFO_GET_DEVICE_NUM):
				UploadDeviceNum(SensorNet.CurSensorNum);
				break;
				
			case(GET_SENSOR_NET_INFO_GET_DEVICE_INFO):
				UploadDeviceInfo(*(arg+1));
				break;

			default:
				break;
		}
	return 1;*/
}


void EnterIAPMOD(void)
{	
	HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,GPIO_PIN_RESET);
	osDelay(200);
	HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,GPIO_PIN_SET);			
	osDelay(200);
	//EnterIapModCMD();
	return;
}

int ENTER_RECV = 0;
int TRANSFORM_RECV = 0;
int PROGRAM_RECV = 0;
int EXIT_RECV = 0;
int OTHER_RECV = 0;

void IapACK(void)
{
	
	unsigned char data[5];
	data[0] = 0;
	SendDataByNewProtocol(TRAN_IAP_CMD,0X1234,data,0);
}

unsigned char TranIapCMD(char* arg)
{
	unsigned char CMD;
	/*switch(*(arg+1))
		{
			case 1:
			CMD = Bootloader_1_COMMAND_ENTER;
			G_EnterIapMod = 1;
			HAL_GPIO_WritePin(POWER_CTRL_GPIO_Port,POWER_CTRL_Pin,GPIO_PIN_RESET);
			osDelay(G_Iap_Delay);
			HAL_GPIO_WritePin(POWER_CTRL_GPIO_Port,POWER_CTRL_Pin,GPIO_PIN_SET);			
			osDelay(G_Iap_STARTUP_Delay);
			ENTER_RECV++;
			break;
			case 2:
			CMD = Bootloader_1_COMMAND_DATA;
			TRANSFORM_RECV++;
			break;
			case 3:
			CMD = Bootloader_1_COMMAND_PROGRAM;
			PROGRAM_RECV++;
			//*(arg+4) = *(arg+2);//适配原来上位机程序
			break;
			case 4:
			CMD = Bootloader_1_COMMAND_EXIT;
			EXIT_RECV++;
			osDelay(100);			
			G_EnterIapMod = 0;
			break;
			//case 5:
			//CMD = Bootloader_1_COMMAND_ENTER;
			//break;
			default:
				OTHER_RECV++;
				return 0;
		}
	Iap_SendDataToNode(CMD,(unsigned char*)(arg+2), Rs232frame.Frame_Length.short_);
*/
	
	return 1 ;
}

unsigned char CMD_CtrlUploadSensorNetInfo(char* arg)
{
	//G_UPLOAD_GRAPH_FLAG = *arg;	
	return 1;
}
unsigned char CMD_CtrlSystemReboot(char* arg)
{
	HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,GPIO_PIN_RESET);//断电
	osDelay(200);
	NVIC_SystemReset();
}
unsigned char CMD_CtrlSensorPower(char* arg)
{
	if(*arg)
		{
			HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,GPIO_PIN_SET);//
		}
	else
		{
			HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,GPIO_PIN_RESET);//断电
		}
}


unsigned char DealUploadDeviceInfo(char*arg)
{
	U_Int_char TmpUID;
	unsigned char Addr;
	TmpUID.char_.char_3 = *(arg+0);
	TmpUID.char_.char_2 = *(arg+1);
	TmpUID.char_.char_1 = *(arg+2);
	TmpUID.char_.char_0 = *(arg+3);
	Addr = *(arg+4);
	//AddUIDToNet(TmpUID.int_, Addr, &SensorNet);
	return 1;
}

unsigned char DealUploadSensorInfo(char*arg)
{
	unsigned char S_CMD;
	S_CMD = *(arg+0);
	/*switch(S_CMD)
		{
			case GET_SENSOR_INFO_CSD_CFG:
			break;
			
			case GET_SENSOR_INFO_BOARD_CFG:
			break;
			
			case GET_SENSOR_INFO_SENSOR_VALUE:
			AddSensorRAWCountTOSensorNet(G_Cur_Operat_Addr,(unsigned char*)(arg+1),&SensorNet);
			G_COUNT++;
			break;

			
			case GET_SENSOR_INFO_SENSOR_DIFF_VALUE:
			AddSensorDIFFCountTOSensorNet(G_Cur_Operat_Addr,(unsigned char*)(arg+1),&SensorNet);
			break;
			
			case GET_SENSOR_INFO_AUTO_SET_RES:
			G_CFG_ERROR_Flag = GetCFGERRORFlag((unsigned char*)(arg+1));
			break;

			case GET_SENSOR_INFO_AUTO_SET_VALUE_RES:
			GetCSDCFGValueRes((unsigned char*)(arg+1));
			break; 
			
			case GET_SENSOR_INFO_BASE_LINE:
			AddSensorBASELINETOSensorNet(G_Cur_Operat_Addr,(unsigned char*)(arg+1),&SensorNet);
			break;

			
			default:
			break;
		}*/
	return 1;
}

unsigned char DeviceJoinNetSuccess(unsigned short addr)
{
	/*unsigned short res;
	res = GetDeviceSpaceByAddr(addr,&SensorNet);
	if(0XFFFF == res)
		{
			return 0;
		}
	SensorNet.SensorBoard[res].DeviceACK = 1;
	//UpdataSensorBroadcastAddr(addr);
	return 1;*/
}

unsigned char DealDeviceAck(char*arg)
{
	unsigned char S_CMD;
	S_CMD = *(arg+0);
	switch(S_CMD)
		{
			case DEVICE_ACK_JOIN_NET:
				DeviceJoinNetSuccess(*(arg+1));
				break;
		}
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char CheckCanFrame(P_Can_Recv_Info_FIFO FIFO,unsigned char WritePoint)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < ((FIFO->RECV_DATA[WritePoint].Data_length)-1) ; loopx ++)
		{
			if((FIFO->RECV_DATA[WritePoint].RecvBuff[loopx]) == (FIFO->RECV_DATA[WritePoint].RecvBuff[loopx+1]))
				{
					continue;
				}
			else
				{
					FIFO->RECV_DATA[WritePoint].ErrorNum++;
					return 0;//失败
				}
		}
	FIFO->RECV_DATA[WritePoint].RightNum++;	
	return 1;//成功
}


unsigned char CheckFrameHead(unsigned char*Data)
{
//	if((*Data == HEAD_1_CODE)&&(*(Data+1) == HEAD_2_CODE))
//		{
//			return 1;
//		}
	if(*Data == HEAD_CODE)
		{
			return 1;
		}
	return 0;
}


P_S_Cmd find_function(unsigned char function_code,P_S_Cmd cmd_list,unsigned char cmd_num)
{
	unsigned char loopx = 0;
	for(;loopx<cmd_num;loopx++)
		{
			if(function_code == (cmd_list + loopx) ->function_code)
				{
					return (cmd_list + loopx);
				}
		}
	return (P_S_Cmd) 0;
}


/**************************
DecodeCtrlFrame 函数描述


返回值 : 
0数据帧校验失败
1数据帧校验成功，并且数据帧的方向是本台机器
2数据帧校验成功，但数据粘包
****************************/

#define IAP_OTHER_CODE_NUM	9
unsigned char DecodeCtrlFrame(unsigned char* data,unsigned short data_length,P_S_CtrlFrame frame)
{
	unsigned char error = 0;
	static unsigned short tmp_data_length;
	tmp_data_length = data_length;
	unsigned char Escap_Char_Num = 0;
	U_Short_char crc_value;
	crc_value.short_ = 0;	

		
	if(0 == CheckFrameHead(data))
		{
			return 3;//数据头不对
		}
	
	//crc_value.short_ = Bootloader_CalcPacketChecksum(data,(data_length - 3));

	Escap_Char_Num = RecvDataPreTreaded(data,PROTOCOL_RECV_BUFF,data_length);
	
	/*if(0 == frame->PreTreated_flag)
		{
			Escap_Char_Num = RECV_DATA_PreTreated(data, &tmp_data_length);
			frame->PreTreated_flag = 1;
		}*/
	frame->Cur_excape_num = Escap_Char_Num;

	frame->Frame_Length.char_.char_h = *(PROTOCOL_RECV_BUFF+2);
	frame->Frame_Length.char_.char_l = *(PROTOCOL_RECV_BUFF+1);	


	//crc_value.short_ = GetProtocolCRC16((data+1),(frame->Frame_Length.short_)-3);
	
	//if((crc_value.char_.char_l != *(data+(frame->Frame_Length.short_)-2))||(crc_value.char_.char_h != *(data+(frame->Frame_Length.short_)-1)))
	if((crc_value.char_.char_l != *(data+(data_length-3))||(crc_value.char_.char_h != *(data+(data_length-2)))))
		{
			error ++;
			//ReSendCmd();
			//return 0;//校验数据
		}

	
	frame->CMD = *(PROTOCOL_RECV_BUFF+3);
	//frame->Addr = *(data+5);
	frame->data = (PROTOCOL_RECV_BUFF+4);	

	if(tmp_data_length != (frame->Frame_Length.short_))
		{
			//return 3;//协议中的长度与实际长度不符
		}
		
	//if(tmp_data_length > ((frame->Frame_Length.short_) + 2))//去掉本帧的头尾

	
	if(tmp_data_length > ((frame->Frame_Length.short_) + IAP_OTHER_CODE_NUM))//去掉本帧的头尾
	
		{
			frame->Untreated_length = tmp_data_length-(frame->Frame_Length.short_) -IAP_OTHER_CODE_NUM;
			return 2;//协议中的长度与实际长度不符       可能发生粘包现象
		}
	
	return 1;
}



unsigned char ExecuteFrame(P_S_CtrlFrame frame,P_S_Cmd cmd_list,unsigned char cmd_num)
{
	P_S_Cmd curcmd = NULL;
	curcmd = find_function(frame->CMD,cmd_list,cmd_num);
	if(curcmd)
		{
			if((curcmd->cmd_length)&&((curcmd->cmd_length) != (frame->Frame_Length.short_)))
				{
					return 0;//接收数据的长度与初始化数值不匹配，0表示不限制
				}
			return (curcmd->function((char*)(frame->data)));
		}
		return 0;
}
unsigned char Deal_RS232_Data(unsigned char* data, unsigned short data_length)
{
	//S_CtrlFrame frame = {0};
	unsigned char** operat_data = &data;
	unsigned short data_operat_length = data_length;
	unsigned char operat_res = 0;
	//unsigned char res = 0;
	/*res = RecvDataPreTreaded(data,PROTOCOL_RECV_BUFF,data_length);
	if(res)
		{
			res ++;
			res--;
		}*/

	Start_Decode_Data:

	//operat_res = DecodeCtrlFrame(*operat_data,data_operat_length, &Rs232frame);	
	operat_res = DecodeCtrlFrame(data,(data_operat_length), &Rs232frame);
	
	#if(RS232_ALL_RECV_DATA_CHECK)	//是否开启粘包处理
	if(1 == operat_res)
	#else
	if(1)
	#endif
		{
			return(ExecuteFrame(&Rs232frame, RS232_CMD_List, (sizeof(RS232_CMD_List)/(sizeof(RS232_CMD_List[0])))));
		}

	if(2 == operat_res)//粘包
		{
			ExecuteFrame(&Rs232frame, RS232_CMD_List, (sizeof(RS232_CMD_List)/(sizeof(RS232_CMD_List[0]))));			
			(*operat_data) += (Rs232frame.Frame_Length.short_ + 1);
			data_operat_length = (Rs232frame.Untreated_length);
			goto Start_Decode_Data;
		}

	return 0;
}

unsigned char DecodeCanFrame(unsigned char* data,unsigned short data_length,P_S_CtrlFrame frame)
{
	//U_Short_char tmp;
	//frame->Frame_Length.char_.char_h = *(data + 0);
	//frame->Frame_Length.char_.char_l = *(data + 1);
	//frame->CMD = *(data + 2);
	//frame->data = (data+3);
	frame->CMD = *(data + 0);
	frame->data = (data+1);
	
	return 1;
}

unsigned char Deal_CAN_Data(unsigned char* data, unsigned short data_length,unsigned int FrameID)
{
	S_CtrlFrame frame = {0};
	unsigned char** operat_data = &data;
	unsigned short data_operat_length = data_length;
	unsigned char operat_res = 0;
	//frame.Addr = (FrameID - 0X100);
	frame.Addr = (FrameID&0XFF);
	G_Cur_Operat_Addr = frame.Addr;//全局指示变量 指示当前can处理函数正在处理的节点地址

	
	Start_Decode_Data:

	operat_res = DecodeCanFrame(*operat_data,data_operat_length, &frame);
	if(1 == operat_res)
		{
			return(ExecuteFrame(&frame, CAN_CMD_List, (sizeof(CAN_CMD_List)/(sizeof(CAN_CMD_List[0])))));
		}

	if(2 == operat_res)//粘包
		{
			ExecuteFrame(&frame, CAN_CMD_List, (sizeof(CAN_CMD_List)/(sizeof(CAN_CMD_List[0]))));			
			(*operat_data) += (frame.Frame_Length.short_ + 1);
			data_operat_length = (frame.Untreated_length);
			goto Start_Decode_Data;
		}

	return 0;
}


unsigned char Deal_CAN_Data_Fifo(void* can_fifo,unsigned char write_point)
{
	P_Can_Recv_Info_FIFO tmp_can_fifo = NULL;
	tmp_can_fifo = (P_Can_Recv_Info_FIFO)can_fifo;

	
	Deal_CAN_Data((tmp_can_fifo->RECV_DATA[write_point].RecvBuff),(tmp_can_fifo->RECV_DATA[write_point].Data_length),(tmp_can_fifo->RECV_DATA[write_point].ID));
	(tmp_can_fifo->RECV_DATA[write_point].Recv_OK) = 0;


	return 1;
}




unsigned char McuFunctionTest(char*arg)
{
	TestFrame();
	return 1;
}






////////////////////////////////send frame/////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//转义相关函数
//////////////////////////////////////////////////////////////////////////////

unsigned char  DeleteByteInArrary(unsigned char* arrary,unsigned short offset,unsigned short* arrary_length)
{
	unsigned short loopx = offset;
	unsigned short tmp_arrary_length = *arrary_length;
	if((0 == *(arrary_length))||((offset+1) > *(arrary_length)))//参数不合法
		{
			return 0;
		}
	*(arrary_length) = tmp_arrary_length - 1;
	for(loopx = offset ; loopx < (tmp_arrary_length) ; loopx ++)
		{
			*(arrary + loopx) = *(arrary + loopx + 1);
		}
	return 1;  
}


unsigned char AddByteToArrary(unsigned char* arrary,unsigned short offset,unsigned short* arrary_length,unsigned char ADD_DATA)//有风险
{
	unsigned short loopx = 0;	
	unsigned short tmp_arrary_length = *arrary_length;
	if((0 == *(arrary_length))||(offset > *(arrary_length)))//参数不合法
		{
			return 0;
		}		
	*(arrary_length) = tmp_arrary_length + 1;
	for(loopx = 0 ; loopx < (tmp_arrary_length - offset + 1) ; loopx ++)
		{
			*(arrary + tmp_arrary_length - loopx + 1) = *(arrary + tmp_arrary_length - loopx);
		}
	*(arrary  + offset) = ADD_DATA;
	return 1;
}




/***************************
预处理发送数据帧
增加数据帧中的转义字

***************************/
unsigned char SEND_DATA_PreTreated(unsigned char* data,unsigned short* data_length)
{
	unsigned char reslut = 0;
	unsigned short loopx;
	unsigned char loop_cmd_list = 0;
	for(loopx = 0 ; loopx < *data_length ; loopx++)
		{
			for(loop_cmd_list = 0 ; loop_cmd_list < sizeof(CMD_BYTE); loop_cmd_list++)
				{
					if(CMD_BYTE[loop_cmd_list] == *(data+loopx))
						{
							*(data + loopx) ^= 0X50;
							AddByteToArrary(data,loopx,data_length,0X7D);
							reslut ++;
							break;
						}
				}
		}

	return reslut;
}

unsigned short SendDataPreTreated(unsigned char * source,unsigned char *target,unsigned short source_length)
{
	//unsigned short target_length = 0;
	unsigned short loopx = 0;
	unsigned short loop_cmd_list = 0;
	unsigned short target_write_point = 0;
	unsigned char is_cmd_flag = 0;
	for(loopx = 0 ; loopx < source_length ; loopx++)
		{
			is_cmd_flag = 0;
			for(loop_cmd_list = 0 ; loop_cmd_list < sizeof(CMD_BYTE); loop_cmd_list++)
				{
					if(CMD_BYTE[loop_cmd_list] == *(source+loopx))
						{
							*(target + target_write_point++) = 0X7D;
							*(target + target_write_point++) = 0X50 ^ (*(source + loopx)) ;
							is_cmd_flag = 1;
							break;
						}
				}
			if(0 == is_cmd_flag)
				{
					*(target + target_write_point++) = *(source + loopx);
				}
		}
	*target = 0X7E;	
	*(target + target_write_point++) = 0X7F;
	return target_write_point;	
}


unsigned char SendDataByOriginProtocol(unsigned char cmd,unsigned char* data,unsigned short data_length)
{
	U_Short_char tmp;
	unsigned short send_length = 0;
	tmp.short_ = data_length;
	UPLOAD_BUFF[0] = 0X7E;
	UPLOAD_BUFF[1] = tmp.char_.char_l;
	UPLOAD_BUFF[2] = tmp.char_.char_h;
	UPLOAD_BUFF[3] = cmd;
	send_length = SendDataPreTreated((data),&(UPLOAD_BUFF[4]),data_length);
	tmp.short_ = GetProtocolCRC16(data,data_length);
	UPLOAD_BUFF[4+send_length] = tmp.char_.char_l;
	UPLOAD_BUFF[5+send_length] = tmp.char_.char_h;
	UPLOAD_BUFF[6+send_length] = 0X7F;
	send_length += 7;
	//SendToCOM1(UPLOAD_BUFF, send_length);
	return 1;
}


void PROTOCOL_TEST(void)
{
	unsigned short loopx;
	for(loopx = 0 ; loopx < 0XFF ; loopx ++)
		{
			PROTOCOL_SEND_BUFF[loopx] = loopx;
		}
	SendDataByOriginProtocol(0X44,PROTOCOL_SEND_BUFF,0XFF);
}

unsigned char* MallocUploadFrame(unsigned char cmd,unsigned char* data,unsigned short* data_length)
{
	unsigned char* out_data = NULL;
	U_Short_char tmp = {0};
	tmp.short_ = *data_length + 5;	
	static unsigned short TmpLength;
	
	TmpLength = tmp.short_;
	
	out_data = (unsigned char*)malloc(*data_length + 50);//注意回收内存
	
	
	*(out_data + 1) = tmp.char_.char_l;
	*(out_data + 2) = tmp.char_.char_h;
	*(out_data + 3) = cmd;

	
	memcpy((void*)(out_data + 4),(void*)data,*data_length);
	
	tmp.short_  = GetProtocolCRC16((out_data + 1),(*data_length + 2));
	*(out_data + (4 + *data_length)) = tmp.char_.char_l;
	*(out_data + (5 + *data_length)) = tmp.char_.char_h;
	
	TmpLength = 1 + 2 + 1 + *data_length  + 2;//头 长度 命令 数据长度 校验
	SEND_DATA_PreTreated(out_data, &TmpLength);
	
	*(out_data + 0) = 0X7E;	
	*(out_data + TmpLength - 1) = 0X7F;
	
	*data_length = TmpLength;
	
	return out_data;
}



void TestARRARY(void)
{
	static unsigned char buff[20] = {8,8,8,8,8};
	static unsigned short length = 5;
	unsigned char loopx;
	for(loopx = 0 ; loopx < 10 ; loopx++)
		{
			AddByteToArrary(buff,2,&length,loopx);
		}

	for(loopx = 0 ; loopx < 20 ; loopx++)
		{
			DeleteByteInArrary(buff,5,&length);
		}
}


/***************************
预处理接收数据帧
去掉数据帧中的转义字

***************************/
unsigned char RECV_DATA_PreTreated(unsigned char* data,unsigned short* data_length)
{
	unsigned char reslut = 0;
	unsigned short loopx;
	for(loopx = 0 ; loopx < *data_length ; loopx++)
		{
			if(0X7D == *(data+loopx))
				{
					*(data + loopx + 1) = *(data + loopx + 1) ^ 0X50;
					DeleteByteInArrary(data,loopx,data_length);
					reslut ++;
				}
		}

	return reslut;
}

unsigned short RecvDataPreTreaded(unsigned char* source , unsigned char* target, unsigned short source_length)
{
	unsigned short reslut = 0;
	unsigned short loopx;
	unsigned short write_point = 0;
	for(loopx = 0 ; loopx < source_length ; loopx++)
		{
			if(0X7D == *(source+loopx))
				{
					//*(source + loopx + 1) = *(target+ (write_point++)) ^ 0X50;
					*(target+ (write_point++)) = *(source + loopx + 1)^ 0X50;
					loopx++;
					reslut++;
					continue;
				}
			*(target+ (write_point++)) = *(source + loopx);
		}

	return reslut;
}



void TestFrame(void)
{
	unsigned char data[6] = {0X7C,0X7D,0X7E,0X7F,0X00,0XFF};
	unsigned char* SendData = NULL;
	static unsigned short DataLength;
	DataLength = 6;
	SendData = MallocUploadFrame(0x02,data,&DataLength);
	//SendToCOM1(SendData, DataLength);
	free(SendData);
}


/////////////////////////////////////////////////////RS232 UPLOAD///////////////////////////////////////////////


/*
7E  data_length_l  data_length_h 02 01 00  time_ms_1  time_ms_2  time_ms_3  time_ms_4  ctrler_num  frame_id_1  frame_id_2  frame_id_3  frame_id_4  
node_num  00 00  {node1}  {node2}...   crc1  crc2  7F

node addr  sensor_num  data_1_l  data_1_h  data_2_l  data_2_h  

*/

void UploadNodeSensorValue(void)
{
	U_Int_char tmp;
	
	PROTOCOL_SEND_BUFF[0] = 0X01;
	PROTOCOL_SEND_BUFF[1] = 0X00;
	
	tmp.int_ = 0X1234;//run time ms
	PROTOCOL_SEND_BUFF[2] = tmp.char_.char_0;
	PROTOCOL_SEND_BUFF[3] = tmp.char_.char_1;
	PROTOCOL_SEND_BUFF[4] = tmp.char_.char_2;
	PROTOCOL_SEND_BUFF[5] = tmp.char_.char_3;
	
	PROTOCOL_SEND_BUFF[6] = 2;//ctrler num
	
	tmp.int_ = 0X5678;//frame num
	PROTOCOL_SEND_BUFF[7] = tmp.char_.char_0;
	PROTOCOL_SEND_BUFF[8] = tmp.char_.char_1;
	PROTOCOL_SEND_BUFF[9] = tmp.char_.char_2;
	PROTOCOL_SEND_BUFF[10] = tmp.char_.char_3;

	PROTOCOL_SEND_BUFF[11] = 0X90;//node num
	PROTOCOL_SEND_BUFF[12] = 0X00;
	PROTOCOL_SEND_BUFF[13] = 0X00;
	
	SendDataByOriginProtocol(0X02,PROTOCOL_SEND_BUFF,0XFF);
}

void UploadSensorInfo(void* board,unsigned char mod)
{
	unsigned char sned_buf[32] = {0};
	unsigned char loopx = 0;
	unsigned char * SendData = NULL;
	
	U_Short_char tmp;
	
	static unsigned short DataLength;
	
	/*P_SensorBoard_Info tmp_board = (P_SensorBoard_Info)board;
	
	DataLength = 32;

	for(loopx = 0 ; loopx < BOARD_SENSOR_NUM ; loopx ++)
	{
		tmp.short_ = tmp_board->sensor[loopx].Data;
		sned_buf[loopx*2+0] = tmp.char_.char_h;		
		sned_buf[loopx*2+1] = tmp.char_.char_l;
		
	}
	
	SendData = MallocUploadFrame(0x02,sned_buf,&DataLength);
	SendToCOM3(SendData, DataLength);
	free(SendData);
	*/
}



void UploadSensorToGateWay(void)
{
	
}


unsigned short FillUploadBuff(unsigned char*buff,void* Board_Net)
{
	/*U_Int_char Int_value;
	U_Short_char Short_value;
	unsigned char BOARD_Num;
	P_SensorNet_Info tmp_board = (P_SensorNet_Info)Board_Net;
	BOARD_Num = tmp_board->CurSensorNum;
	unsigned char loop_all_sensor = 0;
	
	unsigned char loop_all_board;
	unsigned short send_data_lengh;
	static unsigned int Frame_Count = 0;
	
	*(buff + 0) = 0;
	*(buff + 1) = (BOARD_Num *(BOARD_SENSOR_NUM*2 +2) +19) &0XFF;
	*(buff + 2) = ((BOARD_Num *(BOARD_SENSOR_NUM*2 +2) +19)>>8) &0XFF;;
	*(buff + 3) = 0X02;
	*(buff + 4) = 0X01;	
	*(buff + 5) = 0X00;	
	Int_value.int_ =  TimerCount;	
	*(buff + 6) = Int_value.char_.char_3;
	*(buff + 7) = Int_value.char_.char_2;
	*(buff + 8) = Int_value.char_.char_1;
	*(buff + 9) = Int_value.char_.char_0;
	
	*(buff + 10) = 0X01;
	Int_value.int_ = Frame_Count++;
	*(buff + 11) = Int_value.char_.char_3;
	*(buff + 12) = Int_value.char_.char_2;
	*(buff + 13) = Int_value.char_.char_1;
	*(buff + 14) = Int_value.char_.char_0;
	
	*(buff + 15) = BOARD_Num;
	
	*(buff + 16) = 0;
	*(buff + 17) = 0;
	
	for(loop_all_board = 0 ; loop_all_board < BOARD_Num ; loop_all_board ++)
	{
		*(buff + 18 + loop_all_board*(2+BOARD_SENSOR_NUM*2) ) = tmp_board->SensorBoard[loop_all_board].Addr;
		*(buff + 19 + loop_all_board*(2+BOARD_SENSOR_NUM*2) ) = BOARD_SENSOR_NUM*2;		

		for(loop_all_sensor = 0 ;loop_all_sensor < BOARD_SENSOR_NUM ; loop_all_sensor ++)
			{				
				Short_value.short_ = tmp_board->SensorBoard[loop_all_board].sensor[loop_all_sensor].Data;
				*(buff + 20 + loop_all_board*(2+BOARD_SENSOR_NUM*2) + loop_all_sensor*2) = Short_value.char_.char_l;	
				*(buff + 21 + loop_all_board*(2+BOARD_SENSOR_NUM*2) + loop_all_sensor*2) = Short_value.char_.char_h;	
			}
	}
	Short_value.short_  = GetProtocolCRC16((buff + 1),(17 + BOARD_Num*(BOARD_SENSOR_NUM*2+2)));
	
	*(buff + (18 + BOARD_Num*(BOARD_SENSOR_NUM*2+2))) = Short_value.char_.char_l;
	*(buff + (19 + BOARD_Num*(BOARD_SENSOR_NUM*2+2))) = Short_value.char_.char_h;
	send_data_lengh = (20 + BOARD_Num*(BOARD_SENSOR_NUM*2+2));

	return send_data_lengh;
	
	
	//SEND_DATA_PreTreated(buff, &send_data_lengh);
	//*(buff + 0) = 0X7E;
	//*(buff + send_data_lengh) = 0X7F;

	//SendToCOM3(buff, (send_data_lengh+1));
	*/
}




///////////////////////////////////////////Deal CAN PROTOCOL///////////////////////////////////////////////////

void InsertDataToBuff(unsigned char* buff,unsigned char Board_ID,unsigned char Sensor_ID,U_Short_char Sensor_Value)
{	
	//*(buff + Board_ID*BOARD_SENSOR_NUM + Sensor_ID*2) = Sensor_Value.char_.char_h;
	//*(buff + Board_ID*BOARD_SENSOR_NUM + Sensor_ID*2 + 1) = Sensor_Value.char_.char_l;	
}

void RecordSensorData(unsigned char* data, void* board)
{
	/*P_SensorBoard_Info tmp_board = (P_SensorBoard_Info)board;
	unsigned char loopx = 0;
	U_Short_char tmp;
	tmp.short_ = 0;
	for(loopx = 0 ; loopx < BOARD_SENSOR_NUM ; loopx ++)
		{
			tmp.char_.char_h = *(data + loopx*2);
			tmp.char_.char_l = *(data + loopx*2 + 1);
			tmp_board->sensor[loopx].Data = tmp.short_;
			//InsertDataToBuff(UPLOAD_BUFF,0,loopx,tmp);
		}
		*/
}


/////////////////////////////////////////Can Down to Sensor///////////////////////////////////
void SensorCallID(void)
{		
	U_Short_char SendLength;
	unsigned char CMD;	
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_UID;

	SendLength.short_ = 1;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	
	CanSend(can_send_data,(SendLength.short_), 0X3f0);
}


void SetSensorAddr(unsigned int UID, unsigned char Addr,unsigned char slot_id)
{
	U_Short_char SendLength;
	unsigned char CMD;	
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	
	U_Int_char TmpUID;

	TmpUID.int_ = UID;

	SendLength.short_ = 8;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_ADDR;
	can_send_data[2] = TmpUID.char_.char_3;
	can_send_data[3] = TmpUID.char_.char_2;
	can_send_data[4] = TmpUID.char_.char_1;
	can_send_data[5] = TmpUID.char_.char_0;
	can_send_data[6] = Addr;
	can_send_data[7] = slot_id;
	
	CanSend(can_send_data,(SendLength.short_), 0X3ff);
}


void SetSensorSlotID(unsigned int UID,unsigned char slot_id)//这条命令相当于设置地址命令中的地址为0XFF
{
	U_Short_char SendLength;
	unsigned char CMD;	
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	
	U_Int_char TmpUID;

	TmpUID.int_ = UID;

	SendLength.short_ = 8;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_ADDR;
	can_send_data[2] = TmpUID.char_.char_3;
	can_send_data[3] = TmpUID.char_.char_2;
	can_send_data[4] = TmpUID.char_.char_1;
	can_send_data[5] = TmpUID.char_.char_0;
	can_send_data[6] = 0XFF;//
	can_send_data[7] = slot_id;
	
	CanSend(can_send_data,(SendLength.short_), 0X3ff);
}

void SetAllSensorAddr(void* Net)
{
	/*P_SensorNet_Info tmp_net = NULL;
	tmp_net = (P_SensorNet_Info)Net;
	osDelay(2);
	unsigned char loopx = 0;
	 for(loopx = 0 ; loopx < (tmp_net->CurSensorNum) ; loopx ++)
	 	{
	 		if(0XF0 == tmp_net->SensorBoard[loopx].Addr)//新节点板
	 			{
	 				//tmp_net->SensorBoard[loopx].Addr = loopx+1;
	 				tmp_net->SensorBoard[loopx].Addr = MallocAddr(Net);
			 		SetSensorAddr((tmp_net->SensorBoard[loopx].BoardUID), (tmp_net->SensorBoard[loopx].Addr),loopx);
	 			}
			else//设置时间槽
				{
					SetSensorSlotID((tmp_net->SensorBoard[loopx].BoardUID),loopx);
				}
			osDelay(2);
	 	}*/
}


void CheckNet(void* Net)
{
	/*if(G_Net_Check_Time)
		{
			G_Net_Check_Time--;
			P_SensorNet_Info tmp_net = NULL;
			tmp_net = (P_SensorNet_Info)Net;
			unsigned char loopx = 0;

			//osDelay(10);
			 for(loopx = 0 ; loopx < (tmp_net->CurSensorNum) ; loopx ++)
			 	{
			 		if((0 == tmp_net->SensorBoard[loopx].DeviceACK)||(0 == tmp_net->SensorBoard[loopx].HeartBeat))
			 			{
							osDelay(150*((SensorNet.CurSensorNum/10)+1));//等待时槽结束
							break;
			 			}
			 	}
			 for(loopx = 0 ; loopx < (tmp_net->CurSensorNum) ; loopx ++)
			 	{
			 		if(0 == tmp_net->SensorBoard[loopx].DeviceACK||(0 == tmp_net->SensorBoard[loopx].HeartBeat))
			 			{
			 				//tmp_net->SensorBoard[loopx].Addr = loopx+1;
			 				//tmp_net->SensorBoard[loopx].Addr = loopx+1;	 				
					 		SetSensorAddr((tmp_net->SensorBoard[loopx].BoardUID),(tmp_net->SensorBoard[loopx].Addr),loopx);
							osDelay(10);
			 			}
			 	}
		}*/
}

void CheckSensorCfg(unsigned int errorflag,unsigned char addr)
{	
	/*unsigned char loopx;
	unsigned char Point = 0XFF;
	if(errorflag)
		{
			return;
		}
	for(loopx = 0 ; loopx < SENSOR_ARRARY_FIFO_SIZE ; loopx++)
		{
			if(SensorNet.SensorBoard[loopx].Addr == addr)
				{
					Point = loopx;
					break;
				}
		}
	if(0XFF == Point)
		{
			return;
		}
	for(loopx = 0 ; loopx < BOARD_SENSOR_NUM; loopx++)
		{
			if(0 == SensorNet.SensorBoard[Point].sensor[loopx].CFG)
				{
					GetSensorCFGValueRes(addr,loopx);
					return;
				}
		}*/
}


void NetGuard(void)
{
	//CheckNet((void*)&SensorNet);
}


void RefreshSensorNet(void)
{
	//SensorCallID();	
}

void GetSensorValue(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_SENSOR_VALUE;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}





void GetSensorDiffValue(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_SENSOR_DIFF_VALUE;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void GetSensorBaseLine(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_BASE_LINE;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void GetSensorNormalizationValue(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_NORMALIZATION;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void GetSensorNoiseLevelValue(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_NOISE_LEVEL;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}


void GetSensorDebugValue(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_DEBUG_VALUE;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}
void GetNextSensorValue(void)
{
	//SensorNet.CurOperatPoint = SensorNet.CurOperatPoint < SensorNet.CurSensorNum ? SensorNet.CurOperatPoint : 0;
	//GetSensorValue(SensorNet.SensorBoard[SensorNet.CurOperatPoint++].Addr );
}



void GetNextSensorDIffValue(void)
{
	//SensorNet.CurOperatPoint = SensorNet.CurOperatPoint < SensorNet.CurSensorNum ? SensorNet.CurOperatPoint : 0;
	//GetSensorDiffValue(SensorNet.SensorBoard[SensorNet.CurOperatPoint++].Addr);
}

void GetNextSensorBaseLine(void)
{
	//SensorNet.CurOperatPoint = SensorNet.CurOperatPoint < SensorNet.CurSensorNum ? SensorNet.CurOperatPoint : 0;
	//GetSensorBaseLine(SensorNet.SensorBoard[SensorNet.CurOperatPoint++].Addr);
}
void GetNextSensorNormalizationValue(void)
{
	//SensorNet.CurOperatPoint = SensorNet.CurOperatPoint < SensorNet.CurSensorNum ? SensorNet.CurOperatPoint : 0;
	//GetSensorNormalizationValue(SensorNet.SensorBoard[SensorNet.CurOperatPoint++].Addr);
}
void GetNextSensorNoiseLevelValue(void)
{
	//SensorNet.CurOperatPoint = SensorNet.CurOperatPoint < SensorNet.CurSensorNum ? SensorNet.CurOperatPoint : 0;
	//GetSensorNoiseLevelValue(SensorNet.SensorBoard[SensorNet.CurOperatPoint++].Addr);
}
void GetNextSensorDebugValue(void)
{
	//SensorNet.CurOperatPoint = SensorNet.CurOperatPoint < SensorNet.CurSensorNum ? SensorNet.CurOperatPoint : 0;
	//GetSensorDebugValue(SensorNet.SensorBoard[SensorNet.CurOperatPoint++].Addr);
}
void GetCurSensorDiffValue(void)
{
	//GetSensorDiffValue(SensorNet.SensorBoard[SensorNet.CurOperatPoint].Addr );
}

unsigned int GetSensorCFGRes(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_AUTO_SET_RES;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
	return 1;
}

unsigned int GetSensorCFGValueRes(unsigned char Addr,unsigned char sensor_num)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 4;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = GET_SENSOR_INFO_AUTO_SET_VALUE_RES;
	can_send_data[2] = Addr;
	can_send_data[3] = sensor_num;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));	
	return 1;
}


void GetAllSensorValue(unsigned char data_type)
{

	/*unsigned char loop_all_sensor;
	for(loop_all_sensor = 0 ; loop_all_sensor < SensorNet.CurSensorNum ; loop_all_sensor++)
		{
			switch(data_type)
				{
					case UPLOAD_MOD_STOP:				
						GetNextSensorDIffValue();
					break;
					case UPLOAD_MOD_RAW_VALUE:						
						GetNextSensorValue();
					break;
					case UPLOAD_MOD_DIFF_VALUE:
						GetNextSensorDIffValue();
					break;
					case UPLOAD_MOD_BASE_LINE_VALUE:
						GetNextSensorBaseLine();
					break;
					case UPLOAD_MOD_NORMALIZATION_VALUE:
						GetNextSensorNormalizationValue();
					break;
					case UPLOAD_MOD_NOISE_LEVEL:
						GetNextSensorNoiseLevelValue();
					break;
					case UPLOAD_MOD_DEBUG_VALUE:
						GetNextSensorDebugValue();
					break;

					default:
					break;
				}
			osDelay(CUR_SENSOR_DELAY);
		}*/
}


void StartAutoSetCSD(unsigned char Addr,unsigned short sensor)
{
	U_Short_char SendLength;
	U_Short_char Tmp_Sensor;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	Tmp_Sensor.short_ = sensor;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_START_AUTO_SET_CSD;
	can_send_data[2] = Addr;
	can_send_data[3] = Tmp_Sensor.char_.char_h;
	can_send_data[4] = Tmp_Sensor.char_.char_l;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void StartAutoSetAllCSD(void)
{
	unsigned char can_send_data[10] = {0};	
	CanSend(can_send_data,(5), (0X642));
}

void SetSensorIDACScope(unsigned char Addr,unsigned char min,unsigned char max)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_IDAC_SCOPE;
	can_send_data[2] = Addr;
	can_send_data[3] = min;
	can_send_data[4] = max;	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void SetSensorIDAC_CScope(unsigned char Addr,unsigned char min,unsigned char max)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_IDAC_C_SCOPE;
	can_send_data[2] = Addr;
	can_send_data[3] = min;
	can_send_data[4] = max;	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void SetSensorBaseLineThresoldValue(unsigned char Addr,unsigned short thresold)
{
	U_Short_char SendLength;
	U_Short_char tmp_thresold;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	tmp_thresold.short_ = thresold;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_BASELINE_THRESHOLE_VALUE;
	can_send_data[2] = Addr;
	can_send_data[3] = tmp_thresold.char_.char_h;
	can_send_data[4] = tmp_thresold.char_.char_l;	
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

/*
void SetSensorBaseLineNoiseThresoldValue(unsigned char Addr,unsigned short thresold)
{
	U_Short_char SendLength;
	U_Short_char tmp_thresold;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	tmp_thresold.short_ = thresold;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_BASELINE_NOISE_THRESHOLE_VALUE;
	can_send_data[2] = Addr;
	can_send_data[3] = tmp_thresold.char_.char_h;
	can_send_data[4] = tmp_thresold.char_.char_l;	
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

*/
void SetSensorBaseLineUpdataTime(unsigned char Addr,unsigned short thresold)
{
	U_Short_char SendLength;
	U_Short_char tmp_thresold;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	tmp_thresold.short_ = thresold;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_BASELINE_UPDATA_TIME;
	can_send_data[2] = Addr;
	can_send_data[3] = tmp_thresold.char_.char_h;
	can_send_data[4] = tmp_thresold.char_.char_l;	
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}

void SetSensorBaseLineNoiseThresoldValue(unsigned char Addr,unsigned short thresold)
{
	U_Short_char SendLength;
	U_Short_char tmp_thresold;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	tmp_thresold.short_ = thresold;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_BASELINE_NOISE_THRESHOLE_VALUE;
	can_send_data[2] = Addr;
	can_send_data[3] = tmp_thresold.char_.char_h;
	can_send_data[4] = tmp_thresold.char_.char_l;	
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}



void CtrlLedFlash(unsigned short UID,unsigned short mod)
{
	U_Short_char SendLength;
	U_Short_char UID_TMP;
	UID_TMP.short_ = UID;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_CTRL_LED_FLASH;
	can_send_data[2] = UID_TMP.char_.char_h;
	can_send_data[3] = UID_TMP.char_.char_l;
	can_send_data[4] = mod;
	
	CanSend(can_send_data,(SendLength.short_), (0X3ff));	
}

void CtrlNextSlotOffset(unsigned char SensorNum)
{
	/*U_Short_char SendLength;
	U_Short_char UID_TMP;
	UID_TMP.short_ = (SensorNet.SensorBoard[SensorNum].BoardUID) & 0XFFFF;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;

	SendLength.short_ = 4;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_CHOOSE_NEXT_SLOT_OFFSET;
	can_send_data[2] = UID_TMP.char_.char_h;
	can_send_data[3] = UID_TMP.char_.char_l;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + SensorNet.SensorBoard[SensorNum].Addr));*/	
}

void SetSensorSynSlot(unsigned short Slot_Num,unsigned char Frame_num,unsigned char Slot_Gap,unsigned char mod)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_NET_NODELAY;

	SendLength.short_ = 6;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_NET_NODELAY_SYN_SLOT;
	can_send_data[2] = Frame_num;//每个时槽同步帧能带给节点的发送次数
	can_send_data[3] = Slot_Num;
	can_send_data[4] = Slot_Gap;
	can_send_data[5] = mod;
	//SENSOR_ClearHeartBeat();
	
	CanSend(can_send_data,(SendLength.short_), (0X3FF));
}



void UpdataSensorBroadcastAddr(unsigned char Addr)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = GET_SENSOR_INFO;

	SendLength.short_ = 3;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_UPDATA_BROADCAST_ADDR;
	can_send_data[2] = Addr;
	
	CanSend(can_send_data,(SendLength.short_), (0X300 + Addr));
}


void SetALLSensorIDACScope(unsigned char min,unsigned char max)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_IDAC_SCOPE;
	can_send_data[2] = 0XFF;
	can_send_data[3] = min;
	can_send_data[4] = max;	
	if(min>max)
		{
			can_send_data[3] = max;//参数不合法时取两个小的值
		}
	CanSend(can_send_data,(SendLength.short_), (0X3FF));
}

void SetALLSensorIDAC_CScope(unsigned char min,unsigned char max)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_IDAC_C_SCOPE;
	can_send_data[2] = 0XFF;
	can_send_data[3] = min;
	can_send_data[4] = max;	
	if(min>max)
		{
			can_send_data[3] = max;//参数不合法时取两个小的值
		}
	CanSend(can_send_data,(SendLength.short_), (0X3FF));
}

void SetALLSensorCLKScope(unsigned char min,unsigned char max)
{
	U_Short_char SendLength;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;

	SendLength.short_ = 5;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_CLK_SCOPE;
	can_send_data[2] = 0XFF;
	can_send_data[3] = min;
	can_send_data[4] = max;	
	if(min>max)
		{
			can_send_data[3] = max;//参数不合法时取两个小的值
		}
	CanSend(can_send_data,(SendLength.short_), (0X3FF));
}



void SetALLSensorWindow_Scope(unsigned short min,unsigned short max)
{
	U_Short_char SendLength;
	U_Short_char Sendmin;
	U_Short_char Sendmax;
	unsigned char CMD;
	unsigned char can_send_data[10] = {0};
	CMD = CTRL_SENSOR_BOARD;
	
	Sendmin.short_ = min;
	Sendmax.short_ = max;
	
	SendLength.short_ = 7;
	//can_send_data[0] = SendLength.char_.char_h;
	//can_send_data[1] = SendLength.char_.char_l;
	can_send_data[0] = CMD;
	can_send_data[1] = CTRL_SENSOR_BOARD_SET_WINDOW_SCOPE;
	can_send_data[2] = 0XFF;
	can_send_data[3] = Sendmin.char_.char_h;
	can_send_data[4] = Sendmin.char_.char_l;	
	can_send_data[5] = Sendmax.char_.char_h;
	can_send_data[6] = Sendmax.char_.char_l;	
	CanSend(can_send_data,(SendLength.short_), (0X3FF));
}
//////////////////////////////new  protocol//////////////////////////////////////////

//5A  A5  L_1  L_2  CMD  FRAME_NUM_1  FRAME_NUM_2  FRAME_NUM_3  FRAME_NUM_4  DA....TA  CRC_1  CRC_2

unsigned char SendDataByNewProtocol(unsigned char CMD,unsigned int Frame_num,unsigned char*data,unsigned short data_length)
{
	U_Short_char tmp_short;
	U_Int_char tmp_int;
	tmp_int.int_ = Frame_num;
	tmp_short.short_ = data_length;
	unsigned char* send_buff = NULL;
	unsigned short loopx;
	tmp_int.int_ =  TimerCount;
	send_buff = (unsigned char*)pvPortMalloc((11 + data_length));
	*(send_buff+0) = HEAD_1_CODE;
	*(send_buff+1) = HEAD_2_CODE;
	*(send_buff+2) = tmp_short.char_.char_h;
	*(send_buff+3) = tmp_short.char_.char_l;
	*(send_buff+4) = CMD;
	*(send_buff+5) = tmp_int.char_.char_3;
	*(send_buff+6) = tmp_int.char_.char_2;
	*(send_buff+7) = tmp_int.char_.char_1;
	*(send_buff+8) = tmp_int.char_.char_0;
	
	for(loopx = 0 ; loopx < data_length ; loopx ++)
		{
			*(send_buff+9+ loopx) = *(data + loopx);
		}
	tmp_short.short_ = GetProtocolCRC16(send_buff,(9 + data_length));
	
	*(send_buff+9+ loopx) = tmp_short.char_.char_h;
	*(send_buff+10+ loopx) = tmp_short.char_.char_l;

	//SendToCOM1(send_buff, (11 + data_length));
	vPortFree(send_buff);
	return 1;
}
void upload_diffvalue_232(void)
{
	/*unsigned char data[32];
	unsigned char loopx;
	unsigned char loop4;
	U_Short_char sensor_value;
	static int frame = 0;
	for(loopx = 0 ; loopx < 4; loopx ++)
		{
			for(loop4 = 0 ; loop4 < 4 ;loop4 ++)
				{
					sensor_value.short_ = SensorNet.SensorBoard[loopx].sensor[loop4].Data;
					data[loopx*8+loop4*2 ]= sensor_value.char_.char_h;
					data[loopx*8+loop4*2 +1]= sensor_value.char_.char_l;
				}
		}

	SendDataByNewProtocol(0x55,frame++,data,32);*/
}

void upload_diffvalue_232_1nod_16sensor(void)
{
	/*unsigned char data[32];
	unsigned char loopx;
	//unsigned char loop4;
	U_Short_char sensor_value;
	static int frame = 0;
	for(loopx = 0 ; loopx < 16; loopx ++)
		{
			sensor_value.short_ = SensorNet.SensorBoard[0].sensor[loopx].Data;
			data[loopx*2]= sensor_value.char_.char_h;
			data[loopx*2+1]= sensor_value.char_.char_l;				
		}

	SendDataByNewProtocol(0x55,frame++,data,32);*/
}

void upload_diffvalue_232_all_nod_16sensor(void)
{
	/*unsigned char data[128] = {0};
	unsigned char loopx;
	unsigned char loop4;
	U_Short_char sensor_value;
	static int frame = 0;
	for(loop4 = 0 ; loop4 < 4 ; loop4 ++)
		{
			for(loopx = 0 ; loopx < 16; loopx ++)
				{
					sensor_value.short_ = SensorNet.SensorBoard[loop4].sensor[loopx].Data;
					data[loop4*32+loopx*2]= sensor_value.char_.char_h;
					data[loop4*32+loopx*2+1]= sensor_value.char_.char_l;				
				}
		}

	SendDataByNewProtocol(0x55,frame++,data,128);*/
}


void upload_diffvalue_232_all_nod_32sensor(void)
{
	/*//unsigned char data[2560] = {0};
	unsigned char loopx;
	unsigned char loop4;
	U_Short_char sensor_value;
	static int frame = 0;
	for(loop4 = 0 ; loop4 < (SensorNet.CurSensorNum + 1) ; loop4 ++)
		{
			for(loopx = 0 ; loopx < BOARD_SENSOR_NUM; loopx ++)
				{
					sensor_value.short_ = SensorNet.SensorBoard[loop4].sensor[loopx].Data;
					UPLOAD_BUFF[loop4*(BOARD_SENSOR_NUM*2)+loopx*2]= sensor_value.char_.char_h;
					UPLOAD_BUFF[loop4*(BOARD_SENSOR_NUM*2)+loopx*2+1]= sensor_value.char_.char_l;				
				}
		}

	SendDataByNewProtocol(0x55,frame++,UPLOAD_BUFF,((BOARD_SENSOR_NUM*2)*(SensorNet.CurSensorNum+1)));*/
}
