#include "SensorInfo.h"
SensorBoard_Info Board;
//InductionNetInfo SensorNetInfo;

unsigned int G_ERROR_CAN_FRAME_NUM = 0;
unsigned int G_CURRET_CAN_FRAME_NUM = 0;



SensorNet_Info SensorNet;

unsigned char MallocAddr(P_SensorNet_Info NetInfo)//寻找一个在当前存储地址中没出现过的最小地址
{
	unsigned char loopx = 0;
	unsigned char tmp_addr = 1;

	START_CHECK:
	for(loopx = 0 ; loopx < (NetInfo->CurSensorNum);loopx++)
		{
			if(tmp_addr == (NetInfo->SensorBoard[loopx].Addr))
				{
					tmp_addr++;
					goto START_CHECK;
				}
		}
	return tmp_addr;
}
unsigned char GetAddr(unsigned int UID,P_SensorNet_Info NetInfo,unsigned char* addr)
{
	unsigned char loopx = 0;
	if((NetInfo->CurSensorNum) == MAX_SAVE_BOARD_NUM)
		{
			return 0XFF;//网络满
		}
	if(0 == (NetInfo->CurSensorNum))
		{
			*addr = 0;
			return 0;//新节点
		}
	for(loopx = 0 ; loopx < (NetInfo->CurSensorNum) ; loopx++)
		{
			if(UID == (NetInfo->SensorBoard[loopx].BoardUID))
				{
					*addr = loopx;
					return 1;//节点已经保存
				}
		}
	*addr = NetInfo->CurSensorNum;
	return 0;//新节点
}

unsigned short GetDeviceSpaceByAddr(unsigned short addr,P_SensorNet_Info NetInfo)
{
	unsigned char loopx = 0;
	for(loopx = 0 ; loopx < (NetInfo->CurSensorNum) ; loopx++)
		{
			if(addr== (NetInfo->SensorBoard[loopx].Addr))
				{
					return loopx;
				}
		}
	return 0XFFFF;//没有找到这个UID	
}

unsigned char AddUIDToNet(unsigned int UID,unsigned char Addr,P_SensorNet_Info NetInfo)
{
	unsigned char space = 0;
	unsigned tmp_addr = Addr;
	if(NetInfo->CurSensorNum > MAX_SAVE_BOARD_NUM)
		{
			return 0;//网络满
		}
	/*if(0XF0 == tmp_addr)
		{
			tmp_addr = MallocAddr(NetInfo);
		}*/
	if(0 == GetAddr(UID, NetInfo,&space))
		{
			//NetInfo->SensorBoard[space].DeviceACK = 1;
			NetInfo->SensorBoard[space].HeartBeat = 10;
			NetInfo->SensorBoard[space].Addr = tmp_addr;
			NetInfo->SensorBoard[space].BoardUID = UID;//设备加入网络成功			
			NetInfo->CurSensorNum++;
			return 1;
		}
	return 2;//设备已经加入网络
}

unsigned char CheckAddrArrary(P_SensorNet_Info NetInfo)
{
	unsigned char loop_outside = 0;
	unsigned char loop_inside = 0;
	unsigned char error_num = 0;
	for(loop_outside = 0 ; loop_outside < (NetInfo->CurSensorNum) ; loop_outside++)
		{
			for(loop_inside = (loop_outside+1); loop_inside <  (NetInfo->CurSensorNum) ; loop_inside++)
				{
					if((NetInfo->SensorBoard[loop_outside].Addr )== (NetInfo->SensorBoard[loop_inside].Addr ))
						{
							error_num++;
						}
				}
		}
	return error_num;
}

unsigned char PollingSensorValue(P_SensorNet_Info NetInfo)
{
	unsigned char loopx = 0;
	for(loopx = 0 ; loopx < (NetInfo->CurSensorNum); loopx++)
		{
			NetInfo->SensorBoard[loopx].HeartBeat ++;
			//Pollingsonser(loopx);
			osDelay(1);
		}
		return 1;
}



#define RAW_COUNT_THRESHOLD_MAX   58000
#define RAW_COUNT_THRESHOLD_MIN   30000
unsigned char IsSensorRawCountOK(unsigned short RawValue)
{
	if((RawValue > RAW_COUNT_THRESHOLD_MAX)||((RawValue < RAW_COUNT_THRESHOLD_MIN)))
		{
			return 0;
		}
	return 1;
}

void AddSensorInfo(void* P_SensorInfo,unsigned char* data)
{
	P_SenSorData SensorInfo = NULL;
	SensorInfo = (P_SenSorData) P_SensorInfo;
	SensorInfo->Data = *(data + 0);
	SensorInfo->Data = *(data + 0);
	
	//SensorInfo->BaseLine= *(data + 0);
	//SensorInfo->BaseLine = *(data + 0);
	
	//SensorInfo->Diff= *(data + 0);
	//SensorInfo->Diff= *(data + 0);	
}


void AddBoardInfo(void* Board,unsigned char* data)
{
	P_SensorBoard_Info BoardInfo = NULL;
	BoardInfo = (P_SensorBoard_Info)Board;
	U_Short_char TMP_Value;

	unsigned char loopx = 0;
	
	BoardInfo->Addr= *(data + 0);
	BoardInfo->BoardSensorNum= *(data + 0);

	for(loopx = 0 ; loopx < BoardInfo->BoardSensorNum; loopx++)
		{
			TMP_Value.char_.char_h = *(data  + loopx*2 + 0);
			TMP_Value.char_.char_h = *(data  + loopx*2 + 1);
			BoardInfo->sensor[loopx].Data = TMP_Value.short_;
		}
	
}

unsigned char FindSensor(unsigned char Addr,P_SensorNet_Info net,unsigned char* space)
{
	unsigned char loopx = 0;
	if(0XF0 == Addr)
		{
			return 0;//新节点
		}
	for(loopx = 0 ; loopx < SENSOR_ARRARY_FIFO_SIZE ; loopx ++)
		{
			if(Addr == (net->SensorBoard[loopx].Addr))
				{
					*space = loopx;
					return 1;//找到就返回
				}
		}
	(net->CurOperatPoint)++;//没找到就返回下一个可用的操作内存节点
	*space = net->CurOperatPoint;
	return 2;//合法的新节点
	//return net->CurOperatPoint;
}

unsigned char CheckData(unsigned char* data,unsigned char sensor_num)
{
	//unsigned char loopx = 0;
	U_Short_char check_data;
	unsigned short calc_crc = 0;
	calc_crc = GetProtocolCRC16(data, (sensor_num*2));
	check_data.char_.char_h = *(data +sensor_num*2 );
	check_data.char_.char_l = *(data +sensor_num*2 + 1);
	if(calc_crc == check_data.short_)
		{
			G_CURRET_CAN_FRAME_NUM++;
			return 1;
		}	
	G_ERROR_CAN_FRAME_NUM++;
	return 0;
}


unsigned char AddSensorRAWCountTOSensorNet(unsigned char Addr,unsigned char* data,P_SensorNet_Info net)
{
	unsigned char WritePoint = 0;
	unsigned char loopx = 0;
	U_Short_char sensor_value;
	if(0 == CheckData(data,BOARD_SENSOR_NUM))
		{
			return 0;
		}
	if(0 == FindSensor(Addr,net,&WritePoint))
		{
			return 0;
		}
	net->SensorBoard[WritePoint].HeartBeat = 10;
	//net->SensorBoard[WritePoint].Addr = Addr;
	
	for(loopx = 0 ; loopx < BOARD_SENSOR_NUM; loopx++)
		{
			sensor_value.char_.char_h = *(data + loopx*2);
			sensor_value.char_.char_l = *(data + loopx*2 + 1);
			//net->SensorBoard[WritePoint].sensor[loopx].RAW = sensor_value.short_;
			net->SensorBoard[WritePoint].sensor[loopx].Data= sensor_value.short_;
		}
	return 1;
}

unsigned char AddSensorDIFFCountTOSensorNet(unsigned char Addr,unsigned char* data,P_SensorNet_Info net)
{
	unsigned char WritePoint = 0;
	unsigned char loopx = 0;
	U_Short_char sensor_value;
	if(0 == CheckData(data,BOARD_SENSOR_NUM))
		{
			return 0;
		}
	//WritePoint = FindSensor(Addr,net);
	if(0 == FindSensor(Addr,net,&WritePoint))
		{
			return 0;
		}
	//net->SensorBoard[WritePoint].Addr = Addr;
	net->SensorBoard[WritePoint].HeartBeat = 10;
	for(loopx = 0 ; loopx < BOARD_SENSOR_NUM ; loopx++)
		{
			sensor_value.char_.char_h = *(data + loopx*2);
			sensor_value.char_.char_l = *(data + loopx*2 + 1);
			net->SensorBoard[WritePoint].sensor[loopx].Data= sensor_value.short_;
			/*if((net->board[WritePoint].sensor[loopx].Diff) > 32768)
				{
					(net->board[WritePoint].sensor[loopx].Diff) -= 32768;
				}
			else
				{
					(net->board[WritePoint].sensor[loopx].Diff) = 32768 - (net->board[WritePoint].sensor[loopx].Diff);
				}*/
		}
	return 1;
}


unsigned char AddSensorBASELINETOSensorNet(unsigned char Addr,unsigned char* data,P_SensorNet_Info net)
{
	unsigned char WritePoint = 0;
	unsigned char loopx = 0;
	U_Short_char sensor_value;
	if(0 == CheckData(data,BOARD_SENSOR_NUM))
		{
			return 0;
		}
	//WritePoint = FindSensor(Addr,net);
	if(0 == FindSensor(Addr,net,&WritePoint))
		{
			return 0;
		}
	net->SensorBoard[WritePoint].HeartBeat = 10;
	//net->SensorBoard[WritePoint].Addr = Addr;
	
	for(loopx = 0 ; loopx < BOARD_SENSOR_NUM ; loopx++)
		{
			sensor_value.char_.char_h = *(data + loopx*2);
			sensor_value.char_.char_l = *(data + loopx*2 + 1);
			net->SensorBoard[WritePoint].sensor[loopx].Data= sensor_value.short_;
			/*if((net->board[WritePoint].sensor[loopx].Diff) > 32768)
				{
					(net->board[WritePoint].sensor[loopx].Diff) -= 32768;
				}
			else
				{
					(net->board[WritePoint].sensor[loopx].Diff) = 32768 - (net->board[WritePoint].sensor[loopx].Diff);
				}*/
		}
	return 1;
}
unsigned int GetCFGERRORFlag(unsigned char *data)
{
	U_Int_char tmp;
	tmp.char_.char_3 = *(data+0);
	tmp.char_.char_2 = *(data+1);
	tmp.char_.char_1 = *(data+2);
	tmp.char_.char_0 = *(data+3);
	return tmp.int_;
}

void GetCSDCFGValueRes(unsigned char *data)
{
	
	unsigned char loopx;
	unsigned char Point = 0XFF;
	U_Int_char tmp;

	
	unsigned char addr = *data;
	unsigned char sensor_num = *(data+1);
	
	tmp.char_.char_3 = *(data+2);
	tmp.char_.char_2 = *(data+3);
	tmp.char_.char_1 = *(data+4);
	tmp.char_.char_0 = *(data+5);
	
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
	SensorNet.SensorBoard[Point].sensor[sensor_num].CFG = tmp.int_;
	SensorNet.SensorBoard[Point].HeartBeat = 10;
}

void CTRL_Slot_Comp(void)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < SensorNet.CurSensorNum ; loopx++)
		{
			if((1 == SensorNet.SensorBoard[loopx].Slot_Error)&&(1 == SensorNet.SensorBoard[loopx+1].Slot_Error))
				{
					SensorNet.SensorBoard[loopx+1].Slot_Need_Comp_Ctrl = 1;
					loopx++;
				}
		}
}
void SENSOR_ClearHeartBeat(void)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < SensorNet.CurSensorNum ; loopx++)
		{
			if(SensorNet.SensorBoard[loopx].HeartBeat)
				{
					SensorNet.SensorBoard[loopx].HeartBeat--;
				}
			if(SensorNet.SensorBoard[loopx].HeartBeat < 5)
				{
					SensorNet.SensorBoard[loopx].Slot_Error = 1;
				}
		}
	CTRL_Slot_Comp();
}



void SENSOR_SetHeartBeat(unsigned char sensor_num)
{
	if(sensor_num > (SensorNet.CurSensorNum - 1))
		{
			return;
		}
	SensorNet.SensorBoard[sensor_num].HeartBeat = 1;
}


void SENSOR_TEST(unsigned char *data)
{
	unsigned char loop16;
	unsigned char loop3;
	U_Short_char sensor_raw_value;

	for(loop3 = 0 ; loop3 < 4 ; loop3++)
		{
			*(data + 33*loop3 ) = loop3+1;
			for(loop16 = 0 ; loop16 < 16 ;loop16 ++)
				{
					sensor_raw_value.short_= SensorNet.SensorBoard[loop3 + 1].sensor[loop16].Data;
					*(data + 33*loop3 +1 + loop16*2) = sensor_raw_value.char_.char_h;
					*(data + 33*loop3 +1 + loop16*2 +1) = sensor_raw_value.char_.char_l;
				}
		}
}


void SENSOR_DIFF_TEST(unsigned char *data)
{
	unsigned char loop16;
	unsigned char loop3;
	U_Short_char sensor_raw_value;

	for(loop3 = 0 ; loop3 < 4 ; loop3++)
		{
			*(data + 33*loop3 ) = loop3+1;
			for(loop16 = 0 ; loop16 < 16 ;loop16 ++)
				{
					sensor_raw_value.short_=  SensorNet.SensorBoard[loop3 + 1].sensor[loop16].Data;
					*(data + 33*loop3 +1 + loop16*2) = sensor_raw_value.char_.char_h;
					*(data + 33*loop3 +1 + loop16*2 +1) = sensor_raw_value.char_.char_l;
				}
		}
}

