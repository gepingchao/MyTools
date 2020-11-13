#ifndef __UART_RECV_H__
#define __UART_RECV_H__
#include "include.h"

//#define COM1	huart1
#define COM2	huart2
#define COM3	huart3
//#define COM4	huart4


extern osMessageQId SysQueueHandle;
extern osMessageQId UartQueueHandle;
extern osMessageQId CanQueueHandle;

#define COM_QUEUE	ComQueueHandle


#define 	DUMMY_COM	NULL


#define COM1_TIMER		htim6
#define COM2_TIMER		htim6
#define COM3_TIMER		htim7
//#define COM4_TIMER		htim5





typedef struct
{
	unsigned char Enable_Transfer;
	
	unsigned int Transfer_Target;
	unsigned int Transfer_Baud;
}S_Net_Transfer,*P_S_Net_Transfer;

typedef struct
{
	S_Net_Transfer Rs232_Transfer_Info;
	S_Net_Transfer Rs485_1_Transfer_Info;
	S_Net_Transfer Rs485_2_Transfer_Info;
	S_Net_Transfer Rfid_Transfer_Info;
}S_Device_Transfer_Info,*P_S_Device_Transfer_Info;
extern S_Device_Transfer_Info Device_Transfer_Info;


//#define RS232_COM		COM1
//#define RFID_COM		COM2

//#define RS232_TIMER		htim4
//#define RFID_TIMER		htim5

#define COM1_MSG		1
#define COM2_MSG		2
#define COM3_MSG		3
#define COM4_MSG		4
#define COM5_MSG		5
#define ERROR_MSG		0XFF


#define GENERAL_RECV_BUFF_SIZE	100
#define GENERAL_SEND_BUFF_SIZE	100


typedef struct
{
	UART_HandleTypeDef* Com;
	TIM_HandleTypeDef* Timer;
	unsigned int default_Baud;
	QueueHandle_t Queue;
	unsigned char QueueMsg;
	
	unsigned char* recv_buf;
	unsigned short recv_buf_size;
	unsigned char* operat_buf;
	unsigned short* dma_cndtr;
	unsigned short* recv_length;
	unsigned char* recv_ok;
	

	P_S_Net_Transfer Transfer_Info;
}S_Uart_Info,*P_S_Uart_Info;
extern S_Uart_Info COM1_Info;
extern S_Uart_Info COM2_Info;
extern S_Uart_Info COM3_Info;




typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[GENERAL_RECV_BUFF_SIZE];
	unsigned char operat_data[GENERAL_RECV_BUFF_SIZE];
	
	unsigned char recv_ok;	
	unsigned short point;
	unsigned char enable_rxdata;
	unsigned char vBigRxMax;
	PVoid_Function_PVoid Action;
}S_GeneralSeriaRecvData,*P_S_GeneralSeriaRecvData;


extern S_GeneralSeriaRecvData DataFromCOM1;
extern S_GeneralSeriaRecvData DataFromCOM2;
extern S_GeneralSeriaRecvData DataFromCOM3;

#define PTR_COM1_RECV_DATA			(DataFromCOM1.operat_data)
#define PTR_COM1_SEND_DATA			(DataFromCOM1.tx_data)
#define COM1_RECV_DATA_LENGTH		(DataFromCOM1.data_len)


#define PTR_COM2_RECV_DATA			(DataFromCOM2.operat_data)
#define PTR_COM2_SEND_DATA			(DataFromCOM2.tx_data)
#define COM2_RECV_DATA_LENGTH		(DataFromCOM2.data_len)

#define PTR_COM3_RECV_DATA			(DataFromCOM3.operat_data)
#define PTR_COM3_SEND_DATA			(DataFromCOM3.tx_data)
#define COM3_RECV_DATA_LENGTH		(DataFromCOM3.data_len)

#define PTR_COM4_RECV_DATA			(DataFromCOM4.operat_data)
#define PTR_COM4_SEND_DATA			(DataFromCOM4.tx_data)
#define COM4_RECV_DATA_LENGTH		(DataFromCOM4.data_len)



void InitSerialBus(void);
void UsartReceive_IDLE(UART_HandleTypeDef *huart);


unsigned char Transfer(unsigned char Serial_msg);//Í¸´«ÖÐ¼Ìº¯Êý

void StartRecv(UART_HandleTypeDef* com,unsigned char* recv_buf,unsigned short recv_buf_size);

void UartTimerOpeart(TIM_HandleTypeDef *htim,S_Uart_Info *Info);
void UartIdleOperat(UART_HandleTypeDef *huart,S_Uart_Info *Info) ;
void test_serial(void);
void HUART_ReInit(UART_HandleTypeDef* huart,unsigned int BaudRate);
unsigned char CheckSerialBusy(P_S_Uart_Info Info);

#endif

