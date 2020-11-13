#ifndef __SYSINFO_H__
#define __SYSINFO_H__

#include "include.h"

#define SAVE_DATA_EFFECT_FLAG	0X98

#define SYSTEMCFG_SAVE_OFFSET_PAGE	120

typedef struct
{
	unsigned char CanNetBusy;
	unsigned char RS485NetBusy;		
}NetInfo,*P_NetInfo;

typedef struct
{
	//SensorNet_Info Sensor;
	NetInfo	SysNet;
}System_Info,*P_System_Info;

typedef struct
{	
	unsigned char G_CsdParIdacMin;
	unsigned char G_CsdParIdacMax;
	unsigned char G_CsdParIdac_CMin;
	unsigned char G_CsdParIdac_CMax;
	unsigned short G_CsdParWindow_Min;
	unsigned short G_CsdParWindow_Max;
	unsigned char G_CsdParSclkMin;
	unsigned char G_CsdParSclkMax;

	unsigned char SensorNum;
	unsigned char SensorAddr[100];
	
	
	unsigned char sensor_upload_mod;

	unsigned short G_BASELINE_THRESHOLE;
	unsigned short G_BASELINE_NOISE_THRESHOLE;
	unsigned short G_BASELINE_UPDATA_TIME;

	unsigned char data_effect;
	unsigned short DataCRC;
}SystemSave_Info,*P_SystemSave_Info;

extern SystemSave_Info SystemCFG;

void Save_SystemCFGData(void);
void Load_SystemCFGData(void);


#endif

