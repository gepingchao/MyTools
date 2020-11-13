#ifndef __INCLUDE_H__

#define __INCLUDE_H__
/**********************************
计时器:
tim1
tim2		红外接收计时器
tim3		lcd	背光PWM
tim4		红外载波PWM
tim6		串口2空闲中断延时计时器
tim7		串口3空闲中断延时计时器
tim8		RTOS系统滴答计时器	
tim15	软件定时器时基
tim16	蜂鸣器频率PWM
tim17	蜂鸣器开启时长

***********************************/

////
typedef int (*Int_Function_Int) (int arg);
typedef void (*Void_Function_char) (char arg);
typedef void (*Void_Function_Uchar) (unsigned char arg);
typedef void (*Void_Function_Int) (int arg);
typedef void (*Void_Function_Void) (void);
typedef void* (*PVoid_Function_PVoid) (void* arg);
typedef unsigned char (*UChar_Function_Void) (void);
typedef unsigned short(*UShort_Function_PUChar_UInt)(unsigned char*  data,unsigned int date_length);

/////

#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "cpu_.h"
#include "Lcd_Driver.h"
#include "key.h"
#include "bee.h"
#include "flash.h"
#include "font.h"
#include "gui.h"
#include "cd4052.h"
#include "CanNetWork.h"
#include "protocol.h"
#include "BitCheck.h"
#include "iap.h"
#include "NodeIap.h"
#include "SensorInfo.h"
#include "sysinfo.h"
#include "timer_operat.h"
#include "uart_recv.h"
#include "Uart_SendData.h"
#include "watcher.h"
#include "image.h"
#include "SensorShow.h"


extern osMessageQId KeyQueueHandle;
extern osMessageQId SysQueueHandle;
extern osMessageQId ComQueueHandle;
extern osMessageQId GuiQueueHandle;
extern osMessageQId CanQueueHandle;

extern unsigned short X_PIXEL_OFFSET;
extern unsigned short Y_PIXEL_OFFSET;
extern unsigned short X_PIXEL_POLAR;
extern unsigned short Y_PIXEL_POLAR;


///
extern unsigned char G_CsdParIdacMin;
extern unsigned char G_CsdParIdacMax;

extern unsigned char G_CsdParIdac_CMin;
extern unsigned char G_CsdParIdac_CMax;

extern unsigned char G_CsdParSclkMin;
extern unsigned char G_CsdParSclkMax;


extern unsigned short G_CsdParWindow_Min;
extern unsigned short G_CsdParWindow_Max;

extern unsigned char G_SensorNetDownLoadFlag;
extern unsigned char G_UPLOAD_GRAPH_FLAG;


extern unsigned short G_BASELINE_THRESHOLE;
extern unsigned short G_BASELINE_NOISE_THRESHOLE;
extern unsigned short G_BASELINE_UPDATA_TIME;

extern unsigned char G_EnterIapMod;
extern unsigned short G_TRANS_VALUE;

#define UPLOAD_MOD_STOP   0
#define UPLOAD_MOD_RAW_VALUE   1
#define UPLOAD_MOD_DIFF_VALUE   2
#define UPLOAD_MOD_BASE_LINE_VALUE   3
#define UPLOAD_MOD_NORMALIZATION_VALUE   4
#define UPLOAD_MOD_NOISE_LEVEL   5
#define UPLOAD_MOD_DEBUG_VALUE   6


#define SENSOR_32_DELAY	10
#define SENSOR_18_DELAY	3
#define SENSOR_8_DELAY	2
#define SENSOR_4_DELAY	1

#define BOARD_SCAN_TIME	20
#define BOARD_CROUP_NUM	4

#define CUR_SENSOR_DELAY	SENSOR_32_DELAY
#define CUR_FREQUENCY	60

#endif
