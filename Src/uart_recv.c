#include "uart_recv.h"

//S_Device_Transfer_Info Device_Transfer_Info;//透传配置


//S_Uart_Info COM1_Info;
S_Uart_Info COM2_Info;
S_Uart_Info COM3_Info;
//S_Uart_Info COM4_Info;

//S_GeneralSeriaRecvData DataFromCOM1;
S_GeneralSeriaRecvData DataFromCOM2;
S_GeneralSeriaRecvData DataFromCOM3;
//S_GeneralSeriaRecvData DataFromCOM4;


void StartRecv(UART_HandleTypeDef* com,unsigned char* recv_buf,unsigned short recv_buf_size)
{
	HAL_UART_Receive_DMA(com, recv_buf, recv_buf_size);  
	__HAL_UART_ENABLE_IT(com, UART_IT_IDLE);
}


unsigned char SetComMsg(UART_HandleTypeDef* com)
{
	//if(com == &COM1)
		//{return COM1_MSG;}
	
	if(com == &COM2)
		{return COM2_MSG;}
	
	if(com == &COM3)
		{return COM3_MSG;}
	return ERROR_MSG;
}


/*void InitCOM1Info(void)
{
	COM1_Info.Com = &COM1;
	COM1_Info.Timer = &COM1_TIMER;	
	COM1_Info.default_Baud = 115200;
	
	COM1_Info.Queue = COM_QUEUE;
	COM1_Info.QueueMsg = SetComMsg(COM1_Info.Com);

	COM1_Info.recv_buf = DataFromCOM1.rx_data;
	COM1_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	COM1_Info.operat_buf = DataFromCOM1.operat_data;
	
	COM1_Info.dma_cndtr = &(DataFromCOM1.dma_cndtr);
	COM1_Info.recv_length = &(DataFromCOM1.data_len);
	COM1_Info.recv_ok = &(DataFromCOM1.recv_ok);

	//COM1_Info.Transfer_Info = &(Device_Transfer_Info.Rfid_Transfer_Info);
	
	StartRecv(&COM1,DataFromCOM1.rx_data,(COM1_Info.recv_buf_size));
}*/

void InitCOM2Info(void)
{
	COM2_Info.Com = &COM2;
	COM2_Info.Timer = &COM2_TIMER;	
	COM2_Info.Queue = COM_QUEUE;
	COM2_Info.QueueMsg = SetComMsg(&COM2);

	COM2_Info.recv_buf = DataFromCOM2.rx_data;
	COM2_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	COM2_Info.operat_buf = DataFromCOM2.operat_data;
	
	COM2_Info.dma_cndtr = &(DataFromCOM2.dma_cndtr);
	COM2_Info.recv_length = &(DataFromCOM2.data_len);
	COM2_Info.recv_ok = &(DataFromCOM2.recv_ok);

	//RFIDInfo.Transfer_Info = &(Device_Transfer_Info.Rfid_Transfer_Info);
	
	StartRecv(&COM2,DataFromCOM2.rx_data,(COM2_Info.recv_buf_size));
}

void InitCOM3Info(void)
{
	COM3_Info.Com = &COM3;
	COM3_Info.Timer = &COM3_TIMER;	
	COM3_Info.Queue = COM_QUEUE;
	COM3_Info.QueueMsg = SetComMsg(&COM3);

	COM3_Info.recv_buf = DataFromCOM3.rx_data;
	COM3_Info.recv_buf_size = GENERAL_RECV_BUFF_SIZE;
	COM3_Info.operat_buf = DataFromCOM3.operat_data;
	
	COM3_Info.dma_cndtr = &(DataFromCOM3.dma_cndtr);
	COM3_Info.recv_length = &(DataFromCOM3.data_len);
	COM3_Info.recv_ok = &(DataFromCOM3.recv_ok);

	//RFIDInfo.Transfer_Info = &(Device_Transfer_Info.Rfid_Transfer_Info);
	
	StartRecv(&COM3,DataFromCOM3.rx_data,(COM3_Info.recv_buf_size));
}


void HUART_ReInit(UART_HandleTypeDef* huart,unsigned int BaudRate)
{

  //huart.Instance = USART1;
  huart->Init.BaudRate = BaudRate;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    Error_Handler();
  }

}


void InitSerialBus(void)
{
	//InitCOM1Info();
	InitCOM2Info();
	InitCOM3Info();
}


void UartTimerOpeart(TIM_HandleTypeDef *htim,S_Uart_Info *Info)
{
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;	
	
	if(htim != (Info->Timer))
		{
			return;
		}
	HAL_TIM_Base_Stop_IT(htim);
	
	if(*(Info->dma_cndtr) ==  (Info->Com->hdmarx->Instance->CNDTR))
		{			
			HAL_UART_DMAStop(Info->Com);   
			*(Info->recv_length)= (Info->recv_buf_size) -*(Info->dma_cndtr);
			*(Info->recv_ok) = 1;

			//memcopy((char*)(Info->recv_buf), (char *)(Info->operat_buf),(*(Info->recv_length)));
			memcpy((char*)(Info->operat_buf), (char *)(Info->recv_buf),(*(Info->recv_length)));
			memset((char*)(Info->recv_buf),0,(*(Info->recv_length)));//数据转移
			
			HAL_UART_Receive_DMA((Info->Com), (Info->recv_buf), (Info->recv_buf_size)); 
			xQueueSendFromISR((Info->Queue),&(Info->QueueMsg),&xHigherPriorityTaskWoken);
		}
	else
		{
			HAL_TIM_Base_Start_IT(htim);
			*(Info->dma_cndtr) =  Info ->Com->hdmarx->Instance->CNDTR;
		}
}

void UartIdleOperat(UART_HandleTypeDef *huart,S_Uart_Info *Info) 
{
	if(Info->Com != huart)
		{
			return;
		}
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
	    {   
	        __HAL_UART_CLEAR_IDLEFLAG(huart);  
		*(Info->dma_cndtr) = huart->hdmarx->Instance->CNDTR;
		HAL_TIM_Base_Start_IT(Info->Timer);
	    } 
}




void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{	
	//UartIdleOperat(huart,&COM1_Info);
	UartIdleOperat(huart,&COM2_Info);
	UartIdleOperat(huart,&COM3_Info);
}


unsigned char CheckSerialBusy(P_S_Uart_Info Info)
{
	unsigned int* check_data = NULL;
	check_data = (unsigned int*)(Info->recv_buf);
	if(*check_data)
		{
			return 1;
		}
	return 0;
}




void test_serial(void)
{
	char buf[50] = {0};
	unsigned char loop6 = 0;
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,loop6);
	HAL_UART_Transmit(&huart2, (unsigned char*)buf, 30, 500);
	osDelay(50);

	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,loop6);
	HAL_UART_Transmit(&huart2, (unsigned char*)buf, 30, 500);
	osDelay(50);
	
	loop6++;
	sprintf(buf,"uart test%d live:%ds",loop6,loop6);
	HAL_UART_Transmit(&huart3, (unsigned char*)buf, 30, 500);
	osDelay(50);

}










