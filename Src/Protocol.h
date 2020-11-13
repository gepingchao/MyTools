#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "include.h"
#define	HEAD_1_CODE	0X5A
#define	HEAD_2_CODE	0XA5

#define POWER_CTRL_GPIO_Port RELAY_CTRL_GPIO_Port
#define POWER_CTRL_Pin RELAY_CTRL_Pin

#define	HEAD_CODE	0X7E



#define RS232_ALL_RECV_DATA_CHECK		0


#define ULOAD_BUFF_SIZE 512

//////RS232 Porotocol/////////
#define CAN_NET_STOP	0
#define INVENTORY_PROCESS	1
#define CMD_PROCESS_START_AUTOSET	2
#define CMD_PROCESS_SET_CSD_PARATEMER	3
#define CMD_PROCESS_NET_GUARD	4
#define CMD_PROCESS_SET_BASELINE_PAEATEMER	5
#define CMD_PROCESS_CTRL_LED	6
#define CMD_PROCESS_IDLE	0X0F


#define CTRL_SENSOR_NET_START_UPLOAD	0X03

#define CTRL_SENSOR_NET	0X50
#define CTRL_SENSOR_NET_UPLOAD_MOD	0X01

#define CTRL_SENSOR_NET_UPLOAD_MOD_RAW	0X00
#define CTRL_SENSOR_NET_UPLOAD_MOD_DIFF	0X01
#define CTRL_SENSOR_NET_UPLOAD_MOD_BASE_LINE	0X02
#define CTRL_SENSOR_NET_UPLOAD_MOD_NORMAL	0X03
#define CTRL_SENSOR_NET_SET_DEVICE_ADDR	0X04

#define CTRL_SENSOR_NET_START_AUTO_SET	0X02

#define CTRL_SENSOR_NET_SET_CFG	0X03
#define CTRL_SENSOR_NET_SET_CFG_SET_ALL_IDAC_SCOPE	0X01
#define CTRL_SENSOR_NET_SET_CFG_SET_ALL_IDAC_C_SCOPE	0X02
#define CTRL_SENSOR_NET_SET_CFG_SET_ALL_CLK_SCOPE	0X03
#define CTRL_SENSOR_NET_SET_CFG_SET_ALL_WINDOW_SCOPE	0X04

#define CTRL_SENSOR_NET_DEVICE_CFG	0X04
#define CTRL_SENSOR_NET_DEVICE_CFG_SET_DEVICE_ADDR	0X04

#define CTRL_SENSOR_NET_SET_ALL_BASELINE	0X05
#define CTRL_SENSOR_NET_LED_FLASH		0X06

#define GET_SENSOR_NET_INFO	0X20
#define GET_SENSOR_NET_INFO_GET_DEVICE_NUM	0X01
#define GET_SENSOR_NET_INFO_GET_DEVICE_INFO	0X02

#define GET_SENSOR_DEVICE_INFO	0X21

#define CTRL_UPLOAD_GRAPH_DATA	0X55
#define CTRL_SYSTEM_REBOOT		0X54
#define CTRL_SENSOR_POWER		0X53
#define CTRL_SAVE_SYSCFG		0X52

extern unsigned char UPLOAD_BUFF[ULOAD_BUFF_SIZE];//转义完成后的数组，上传的网关的数组
extern unsigned char PROTOCOL_SEND_BUFF[ULOAD_BUFF_SIZE];//将数据摆在这个数组中，然后做转义




///////////转发IAP命令

#define TRAN_IAP_CMD	0X10

///////CAN protocol////////////

//down
#define PROTOCOLDEMO	0XFE
#define FUNCTION_TEST	0XDD


#define CTRL_SENSOR_NET_REBOOT	0X01
#define CTRL_SENSOR_NET_START	0X02
#define CTRL_SENSOR_NET_STOP	0X03

    
#define CTRL_SENSOR_BOARD   0X04
#define CTRL_SENSOR_BOARD_SET_ADDR  0X01
#define CTRL_SENSOR_BOARD_SET_UPLOAD_MOD    0X02
#define CTRL_SENSOR_BOARD_SET_UID   0X03
#define CTRL_SENSOR_BOARD_SET_CSD_CFG   0X04
#define CTRL_SENSOR_BOARD_START_AUTO_SET_CSD	0X05
#define CTRL_SENSOR_BOARD_SET_IDAC_SCOPE	0X06
#define CTRL_SENSOR_BOARD_SET_IDAC_C_SCOPE	0X07
#define CTRL_SENSOR_BOARD_SET_CLK_SCOPE	0X0D
#define CTRL_SENSOR_BOARD_SET_WINDOW_SCOPE	0X0C


#define CTRL_SENSOR_BOARD_UPDATA_BROADCAST_ADDR	0X08

#define CTRL_SENSOR_BOARD_SET_BASELINE_THRESHOLE_VALUE	0X09
#define CTRL_SENSOR_BOARD_SET_BASELINE_NOISE_THRESHOLE_VALUE	0X0A
#define CTRL_SENSOR_BOARD_SET_BASELINE_UPDATA_TIME	0X0B

#define CTRL_SENSOR_BOARD_CTRL_LED_FLASH		0X0E
#define CTRL_SENSOR_BOARD_CHOOSE_NEXT_SLOT_OFFSET		0X10


#define CTRL_SENSOR_BOARD_CHOOSE_NEXT_SCAN_GROUP_MEMBER		0X11
 
#define CTRL_SENSOR_BOARD_SAVE_CFG  0XFA
#define CTRL_SENSOR_BOARD_SET_ALARM_MOD 0XFC
#define CTRL_SENSOR_BOARD_SET_BAUD  0XFE


#define GET_SENSOR_UID	0X02

#define GET_SENSOR_INFO 0X05
#define GET_SENSOR_INFO_CSD_CFG 0X01
#define GET_SENSOR_INFO_BOARD_CFG   0X02



//获取传感器原始值
#define GET_SENSOR_INFO_SENSOR_VALUE    0X03
//获取传感器变化量
#define GET_SENSOR_INFO_SENSOR_DIFF_VALUE    0X04

//获取自动配置结果 按位表示 1表示此传感器配置失败 0表示此传感器配置成功
#define GET_SENSOR_INFO_AUTO_SET_RES    0X05

//获取自动配置结果 占三个字节分别是IDAC  IDAC_C   CLK
#define GET_SENSOR_INFO_AUTO_SET_VALUE_RES    0X06

//检测通讯帧
#define GET_SENSOR_INFO_CHECK_FREME	0X07

//获取基准线
#define GET_SENSOR_INFO_BASE_LINE	0X08

//获取归一化值
#define GET_SENSOR_INFO_NORMALIZATION	0X09

//获取噪声水平
#define GET_SENSOR_INFO_NOISE_LEVEL	0X0A

//获取调试数据
#define GET_SENSOR_INFO_DEBUG_VALUE	0X0B

#define DEVICE_ACK	0X06
#define DEVICE_ACK_JOIN_NET	0X01

    
#define CTRL_SENSOR_NET_NODELAY 0X07
#define CTRL_SENSOR_NET_NODELAY_SYN_SLOT    0X01 
//up



#define RS485CMD_START_SCAN	0X01
#define RS485CMD_GET_SENSOR_INFO	0X02
/////////////////////////


extern unsigned int TimerCount;
extern unsigned char UPLOAD_BUFF[ULOAD_BUFF_SIZE];

extern unsigned int G_CFG_ERROR_Flag;
typedef struct
{
	unsigned char char_l;
	unsigned char char_h;
}S_ShortSplit;
typedef union
{
	S_ShortSplit char_;
	unsigned short short_;
}U_Short_char;


typedef struct
{
	unsigned char char_0;
	unsigned char char_1;
	unsigned char char_2;
	unsigned char char_3;
}S_IntSplit;
typedef union
{
	S_IntSplit char_;
	unsigned int int_;
}U_Int_char;

typedef struct
{
	unsigned char function_code;
	unsigned short cmd_length;//接收数据的长度与初始化数值是否匹配，0表示不限制
	unsigned char (*function)(char* parameter);
	char* help;
}S_Cmd,*P_S_Cmd;



#define EXTEND_LENGTH	8
typedef struct
{
	unsigned char Head_1;
	unsigned char Head_2;
	U_Short_char Frame_Length;
	unsigned char Cur_excape_num;
	unsigned char PreTreated_flag;
	unsigned char CMD;
	unsigned char Addr;
	unsigned char* data;
	U_Short_char CheckValue;
	unsigned short Untreated_length;
}S_CtrlFrame,*P_S_CtrlFrame;


P_S_Cmd find_function(unsigned char function_code,P_S_Cmd cmd_list,unsigned char cmd_num);
unsigned char Deal_RS232_Data(unsigned char* data, unsigned short data_length);
unsigned char Deal_CAN_Data(unsigned char* data, unsigned short data_length,unsigned int FrameID);

///232Porotocol
unsigned char CMD_StartUploadSensorNetData(char* arg);
unsigned char CMD_UploadSensorNetInfo(char* arg);
unsigned char CMD_CtrlUploadSensorNetInfo(char* arg);
unsigned char CMD_CtrlSystemReboot(char* arg);
unsigned char CMD_CtrlSensorPower(char* arg);
unsigned char CMD_CtrlSystemSaveCFG(char* arg);


//IAP
unsigned char TranIapCMD(char* arg);


//unsigned char Deal_CAN_Data_Fifo(void* can_fifo);

unsigned char Deal_CAN_Data_Fifo(void* can_fifo,unsigned char write_point);

unsigned short GetProtocolCRC16(unsigned char* pData,unsigned int DataLen);
unsigned char RECV_DATA_PreTreated(unsigned char* data,unsigned short* data_length);
unsigned char SendDataByOriginProtocol(unsigned char cmd,unsigned char* data,unsigned short data_length);
unsigned short RecvDataPreTreaded(unsigned char* source , unsigned char* target, unsigned short source_length);


///rs485
unsigned char Deal_RS485_Data(unsigned char* data, unsigned short data_length);


//////////////////////deal function//////////
unsigned char CMD_Demo(char* arg);
unsigned char McuFunctionTest(char*arg);
unsigned char CMD_CtrlSensorNet(char* arg);
unsigned char DealUploadDeviceInfo(char*arg);
unsigned char DealUploadSensorInfo(char*arg);
unsigned char DealDeviceAck(char*arg);


///////////////UPLOAD///////////////////////////

unsigned char CmdACK(char* arg);
void TestFrame(void);
void UploadSensorInfo(void* board,unsigned char mod);
unsigned short FillUploadBuff(unsigned char*buff,void* Board_Net);


///////////////////////////////////////////////////can protocol///////////////////////////
void RecordSensorData(unsigned char* data, void* board);

/////////////////////////////////////////Can Down to Sensor///////////////////////////////////
void SensorCallID(void);
void SetSensorAddr(unsigned int UID, unsigned char Addr,unsigned char slot_id);

void NetGuard(void);
void CheckSensorCfg(unsigned int errorflag,unsigned char addr);

void StartAutoSetAllCSD(void);

void SetAllSensorAddr(void* Net);
void GetNextSensorValue(void);
void GetAllSensorValue(unsigned char data_type);

void StartAutoSetCSD(unsigned char Addr,unsigned short sensor);
void SetSensorIDACScope(unsigned char Addr,unsigned char min,unsigned char max);
void SetSensorIDAC_CScope(unsigned char Addr,unsigned char min,unsigned char max);

void UpdataSensorBroadcastAddr(unsigned char Addr);

void SetALLSensorIDACScope(unsigned char min,unsigned char max);
void SetALLSensorIDAC_CScope(unsigned char min,unsigned char max);
void SetALLSensorWindow_Scope(unsigned short min,unsigned short max);
void SetALLSensorCLKScope(unsigned char min,unsigned char max);

void SetSensorBaseLineNoiseThresoldValue(unsigned char Addr,unsigned short thresold);
void SetSensorBaseLineUpdataTime(unsigned char Addr,unsigned short thresold);
void SetSensorBaseLineThresoldValue(unsigned char Addr,unsigned short thresold);

void CtrlLedFlash(unsigned short UID,unsigned short mod);

void CtrlNextSlotOffset(unsigned char SensorNum);//进行时槽补偿

unsigned int GetSensorCFGRes(unsigned char Addr);
unsigned int GetSensorCFGValueRes(unsigned char Addr,unsigned char sensor_num);


void StartNextBoardScan(void);


void SetSensorSynSlot(unsigned short Slot_Num,unsigned char Frame_num,unsigned char Slot_Gap,unsigned char mod);//同步时槽
unsigned short SendDataPreTreated(unsigned char * source,unsigned char *target,unsigned short source_length);
//unsigned char CheckCanFrame(P_Can_Recv_Info_FIFO FIFO,unsigned char WritePoint);






//////////////////////////////new  protocol//////////////////////////////////////////

//A5  5A  L_1  L_2  CMD  DA....TA  CRC

typedef struct 
{
	unsigned char Head_1;
	unsigned char Head_2;
	U_Short_char DataLength;
	unsigned char CMD;
	unsigned char* Data;
	U_Short_char FrameCrc;
}Protocol_Frame,*P_Protocol_Frame;

unsigned char SendDataByNewProtocol(unsigned char CMD,unsigned int Frame_num,unsigned char*data,unsigned short data_length);
void upload_diffvalue_232_all_nod_32sensor(void);

#endif

