
#ifndef __SENSOR_INFO_H__
#define __SENSOR_INFO_H__

#include "include.h"


#define MAX_SAVE_BOARD_NUM	10
#define SENSOR_ARRARY_FIFO_SIZE MAX_SAVE_BOARD_NUM
#define BOARD_SENSOR_NUM	32




typedef struct
{
	//unsigned short RAW;
	//unsigned short BaseLine;
	unsigned short Data;
	unsigned int CFG;//传感器配置参数包括IDAC补偿IDAC和分频系数
}SenSorData,*P_SenSorData;
/*
typedef struct
{
	unsigned char data_effect;
	unsigned char Sensor_num;
	unsigned char Addr;
	unsigned short board_id;
	unsigned int CFG_ERROR_Flag;
	
	SenSorData sensor[BOARD_SENSOR_NUM];
}BoardInfo,*P_BoardInfo;
extern BoardInfo Board;

typedef struct
{
	unsigned char write_point:7;
	unsigned char data_effect;
	BoardInfo board[SENSOR_ARRARY_FIFO_SIZE];
}InductionNetInfo,*P_InductionNetInfo;
extern InductionNetInfo SensorNetInfo;
*/

typedef struct
{
	unsigned char HeartBeat;
	unsigned char DeviceACK;
	unsigned char BoardSensorNum;
	unsigned char Addr;
	unsigned char SensorNum;
	unsigned char Slot_Error;
	unsigned char Slot_Need_Comp_Ctrl;
	unsigned int BoardUID;	
	unsigned int CFG_ERROR_Flag;
	SenSorData sensor[BOARD_SENSOR_NUM];
}SensorBoard_Info,*P_SensorBoard_Info;
extern SensorBoard_Info Board;

typedef struct
{
	unsigned short CurOperatPoint:7;
	unsigned short CurSensorNum;
	SensorBoard_Info SensorBoard[MAX_SAVE_BOARD_NUM];
}SensorNet_Info,*P_SensorNet_Info;
extern SensorNet_Info SensorNet;

unsigned char MallocAddr(P_SensorNet_Info NetInfo);
unsigned char AddUIDToNet(unsigned int UID,unsigned char Addr,P_SensorNet_Info NetInfo);
unsigned short GetDeviceSpaceByAddr(unsigned short addr,P_SensorNet_Info NetInfo);

unsigned char AddSensorRAWCountTOSensorNet(unsigned char Addr,unsigned char* data,P_SensorNet_Info net);
unsigned char AddSensorDIFFCountTOSensorNet(unsigned char Addr,unsigned char* data,P_SensorNet_Info net);
unsigned char AddSensorBASELINETOSensorNet(unsigned char Addr,unsigned char* data,P_SensorNet_Info net);

unsigned int GetCFGERRORFlag(unsigned char *data);
void GetCSDCFGValueRes(unsigned char *data);

void SENSOR_TEST(unsigned char *data);

void SENSOR_ClearHeartBeat(void);
void SENSOR_SetHeartBeat(unsigned char sensor_num);

#endif
