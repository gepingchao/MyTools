#include "include.h"

SystemSave_Info SystemCFG;

void ReinitSystemCFG(void)
{
	SystemCFG.G_CsdParIdacMin = 11;
	SystemCFG.G_CsdParIdacMax = 19;

	SystemCFG.G_CsdParIdac_CMin = 1;
	SystemCFG.G_CsdParIdac_CMax = 129;


	SystemCFG.G_CsdParWindow_Min = 33000;
	SystemCFG.G_CsdParWindow_Max = 45000;

	SystemCFG.G_CsdParSclkMin = 1;
	SystemCFG.G_CsdParSclkMax = 5;

	SystemCFG.G_BASELINE_THRESHOLE = 200;
	SystemCFG.G_BASELINE_NOISE_THRESHOLE = 2;
	SystemCFG.G_BASELINE_UPDATA_TIME = 20;
	
	SystemCFG.sensor_upload_mod = UPLOAD_MOD_RAW_VALUE;
}


void TransAddr(void)
{
	unsigned char loopx;
	SystemCFG.SensorNum = SensorNet.CurOperatPoint;
	for(loopx = 0 ; loopx < SensorNet.CurSensorNum ; loopx++)
		{
			SystemCFG.SensorAddr[loopx] = SensorNet.SensorBoard[loopx].Addr;
		}	
}

unsigned char CheckAddr(unsigned char* error_space_1,unsigned char* error_space_2)
{
	unsigned char loopOutSide;
	unsigned char loopInSide;
		
	for(loopOutSide = 0 ; loopOutSide < (SystemCFG.SensorNum -1) ; loopOutSide++)
		{

			for(loopInSide = loopOutSide+1; loopOutSide < SystemCFG.SensorNum; loopInSide ++)
				{
					if(SystemCFG.SensorAddr[loopOutSide] == SystemCFG.SensorAddr[loopInSide])
						{
							*error_space_1 = loopOutSide;
							*error_space_2 = loopInSide;
							return 0;
						}
				}
		}
	return 1;
}
void Save_SystemCFGData(void)
{	
	TransAddr();
	SystemCFG.data_effect = SAVE_DATA_EFFECT_FLAG;
	SystemCFG.DataCRC = GetProtocolCRC16((unsigned char*)&SystemCFG,(sizeof(SystemCFG) - 8));
	write_flash((0X08000000 + (SYSTEMCFG_SAVE_OFFSET_PAGE) *(FLASH_PAGE_SIZE)),(char*)(&SystemCFG),(sizeof(SystemSave_Info)));
}

unsigned char CheckSysCfg(void)
{
	if(SystemCFG.DataCRC == GetProtocolCRC16((unsigned char*)&SystemCFG,(sizeof(SystemCFG) - 8)))
		{
			return 1;
		}
	return 0;
}

void Load_SystemCFGData(void)
{
	read_flash((0X08000000 + (SYSTEMCFG_SAVE_OFFSET_PAGE) *(FLASH_PAGE_SIZE)),(char*)(&SystemCFG),(sizeof(SystemSave_Info)));
	if((SAVE_DATA_EFFECT_FLAG == SystemCFG.data_effect)&&(CheckSysCfg()))
		{
			
		}
	else
		{
			ReinitSystemCFG();
		}
}


