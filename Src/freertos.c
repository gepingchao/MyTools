/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2020 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "include.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


//unsigned char G_SensorNetDownLoadFlag = CAN_NET_STOP;
unsigned char G_SensorNetDownLoadFlag = INVENTORY_PROCESS;

unsigned char frame_num = 3;
unsigned char slot_gap = 80;
unsigned char sensor_upload_mod = UPLOAD_MOD_RAW_VALUE;
//unsigned char sensor_upload_mod = UPLOAD_MOD_DIFF_VALUE;

unsigned int slot_syn_time = 100;


unsigned char G_CsdParIdacMin = 11;
unsigned char G_CsdParIdacMax = 19;

unsigned char G_CsdParIdac_CMin = 1;
unsigned char G_CsdParIdac_CMax = 129;


unsigned short G_CsdParWindow_Min = 33000;
unsigned short G_CsdParWindow_Max = 45000;

unsigned char G_CsdParSclkMin = 2;
unsigned char G_CsdParSclkMax = 7;

unsigned short G_BASELINE_THRESHOLE = 200;
unsigned short G_BASELINE_NOISE_THRESHOLE = 2;
unsigned short G_BASELINE_UPDATA_TIME = 20;

unsigned short G_TRANS_VALUE = 0;


unsigned char G_UPLOAD_GRAPH_FLAG = 0;

#define NET_GUARD_GAP	10
unsigned short G_NetGuardGap = NET_GUARD_GAP;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId DelayStartUpHandle;
osThreadId SysTaskHandle;
osThreadId ScanKeyHandle;
osThreadId DealKeyHandle;
osThreadId DealComHandle;
osThreadId GuiOperatHandle;
osThreadId SendToSensorNetHandle;
osThreadId DealCanHandle;
osMessageQId KeyQueueHandle;
osMessageQId SysQueueHandle;
osMessageQId ComQueueHandle;
osMessageQId GuiQueueHandle;
osMessageQId CanQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void DelayStartUpFunction(void const * argument);
void SysTaskFunction(void const * argument);
void ScanKeyFunction(void const * argument);
void DealKeyFunction(void const * argument);
void DealComTask(void const * argument);
void GuiOperattask(void const * argument);
void SendToSensorNetTask(void const * argument);
void DealCanTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
extern void MX_FATFS_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 800);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of DelayStartUp */
  osThreadDef(DelayStartUp, DelayStartUpFunction, osPriorityNormal, 0, 128);
  DelayStartUpHandle = osThreadCreate(osThread(DelayStartUp), NULL);

  /* definition and creation of SysTask */
  osThreadDef(SysTask, SysTaskFunction, osPriorityNormal, 0, 128);
  SysTaskHandle = osThreadCreate(osThread(SysTask), NULL);

  /* definition and creation of ScanKey */
  osThreadDef(ScanKey, ScanKeyFunction, osPriorityHigh, 0, 128);
  ScanKeyHandle = osThreadCreate(osThread(ScanKey), NULL);

  /* definition and creation of DealKey */
  osThreadDef(DealKey, DealKeyFunction, osPriorityNormal, 0, 128);
  DealKeyHandle = osThreadCreate(osThread(DealKey), NULL);

  /* definition and creation of DealCom */
  osThreadDef(DealCom, DealComTask, osPriorityRealtime, 0, 200);
  DealComHandle = osThreadCreate(osThread(DealCom), NULL);

  /* definition and creation of GuiOperat */
  osThreadDef(GuiOperat, GuiOperattask, osPriorityAboveNormal, 0, 300);
  GuiOperatHandle = osThreadCreate(osThread(GuiOperat), NULL);

  /* definition and creation of SendToSensorNet */
  osThreadDef(SendToSensorNet, SendToSensorNetTask, osPriorityHigh, 0, 200);
  SendToSensorNetHandle = osThreadCreate(osThread(SendToSensorNet), NULL);

  /* definition and creation of DealCan */
  osThreadDef(DealCan, DealCanTask, osPriorityRealtime, 0, 200);
  DealCanHandle = osThreadCreate(osThread(DealCan), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of KeyQueue */
  osMessageQDef(KeyQueue, 16, uint8_t);
  KeyQueueHandle = osMessageCreate(osMessageQ(KeyQueue), NULL);

  /* definition and creation of SysQueue */
  osMessageQDef(SysQueue, 10, uint32_t);
  SysQueueHandle = osMessageCreate(osMessageQ(SysQueue), NULL);

  /* definition and creation of ComQueue */
  osMessageQDef(ComQueue, 16, uint8_t);
  ComQueueHandle = osMessageCreate(osMessageQ(ComQueue), NULL);

  /* definition and creation of GuiQueue */
  osMessageQDef(GuiQueue, 10, uint32_t);
  GuiQueueHandle = osMessageCreate(osMessageQ(GuiQueue), NULL);

  /* definition and creation of CanQueue */
  osMessageQDef(CanQueue, 4, uint8_t);
  CanQueueHandle = osMessageCreate(osMessageQ(CanQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN StartDefaultTask */

  
  osDelay(500);
  FIL TXT_A;
  FRESULT res;
  unsigned int num_write = 0;
  res = f_open(&TXT_A, "message3.txt", FA_WRITE|FA_CREATE_NEW);
  //printf("open function done res = %d\r\n",res);
  res = f_write(&TXT_A, "hello from stm32", 17, &num_write);
  //printf("write function done res = %d , write num = %d \r\n",res,num_write);
  f_close(&TXT_A);
/**/
 
  HAL_GPIO_WritePin(USB_D_UP_GPIO_Port,USB_D_UP_Pin,GPIO_PIN_SET);
	save_task_info();
	InitSerialBus();
	InitTimerCounter();
	InitWatcher();

	
	Load_SystemCFGData();
	G_CsdParIdacMin = SystemCFG.G_CsdParIdacMin;
	G_CsdParIdacMax = SystemCFG.G_CsdParIdacMax;

	G_CsdParIdac_CMin = SystemCFG.G_CsdParIdac_CMin;
	G_CsdParIdac_CMax = SystemCFG.G_CsdParIdac_CMax;


	G_CsdParWindow_Min = SystemCFG.G_CsdParWindow_Min;
	G_CsdParWindow_Max = SystemCFG.G_CsdParWindow_Max;

	G_CsdParSclkMin = SystemCFG.G_CsdParSclkMin;
	G_CsdParSclkMax = SystemCFG.G_CsdParSclkMax;

	G_BASELINE_THRESHOLE = SystemCFG.G_BASELINE_THRESHOLE;
	G_BASELINE_NOISE_THRESHOLE = SystemCFG.G_BASELINE_NOISE_THRESHOLE;
	G_BASELINE_UPDATA_TIME = SystemCFG.G_BASELINE_UPDATA_TIME;
	sensor_upload_mod = SystemCFG.sensor_upload_mod;


	osDelay(2000);
	HAL_GPIO_WritePin(BOOST_EN_GPIO_Port,BOOST_EN_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,GPIO_PIN_SET);
	Enable_Path(CAN_PATH_2);

	osDelay(1500);
		
	osDelay(1000);	
	SensorCallID();
	osDelay(4000);
	//SetAllSensorAddr((void*)&SensorNet);
	
	osDelay(1000);
	
	SetALLSensorIDACScope(G_CsdParIdacMin,G_CsdParIdacMax);
	osDelay(50);
	SetALLSensorIDAC_CScope(G_CsdParIdac_CMin,G_CsdParIdac_CMax);
	osDelay(50);
	
  /* Infinite loop */
  for(;;)
  {
	//UploadSensorInfo( (void*)&Board,1);
	osDelay(200);
	//printf("runing time %d\r\n",HAL_GetTick());

	
	//SD_Test();
	
	if(G_UPLOAD_GRAPH_FLAG)
		{
			//upload_diffvalue_232_all_nod_16sensor();	//上传到上位机工具
			upload_diffvalue_232_all_nod_32sensor();
			#if (DEBUG == 0)
			upload_diffvalue_232_all_nod_32sensor();
			#endif
			
			osDelay(40);
		}
	//GetNextSensorValue();
	//osDelay(10);
	//GetCurSensorDiffValue();
	//osDelay(10);
	//GetSensorCFGRes(tmp_addr);//获取指定节点板的配置结果(是否配置成功)
	
	//osDelay(10);
	//CheckSensorCfg(G_CFG_ERROR_Flag, tmp_addr);
		
		
	//FillUploadBuff(UPLOAD_BUFF,(void*)&Board);
	//HAL_Delay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_DelayStartUpFunction */
/**
* @brief Function implementing the DelayStartUp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DelayStartUpFunction */
void DelayStartUpFunction(void const * argument)
{
  /* USER CODE BEGIN DelayStartUpFunction */
	save_task_info();
  /* Infinite loop */
  for(;;)
  {
	osDelay(2);
	ScanEC11();
  }
  /* USER CODE END DelayStartUpFunction */
}

/* USER CODE BEGIN Header_SysTaskFunction */
/**
* @brief Function implementing the SysTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SysTaskFunction */
void SysTaskFunction(void const * argument)
{
  /* USER CODE BEGIN SysTaskFunction */
	save_task_info();
  /* Infinite loop */
  //test_music();
  	osDelay(1000);
  static unsigned short gate_value = 200;


  for(;;)
  {
	osDelay(10);
	
	PushGraphData(&(Lcd.Graph[0]),(SensorNet.SensorBoard[0].sensor[0].Data));
	PushGraphData(&(Lcd.Graph[1]),(SensorNet.SensorBoard[0].sensor[1].Data));
	if(SensorNet.SensorBoard[0].sensor[0].Data>32700)
		{
			if(gate_value < (SensorNet.SensorBoard[0].sensor[0].Data - 32768))
				{
					PushGraphData(&(Lcd.Graph[2]),10);
				}
			else
				{
					PushGraphData(&(Lcd.Graph[2]),0);
				}
				
		}
	else
		{
			PushGraphData(&(Lcd.Graph[2]),0);
		}	

	if(SensorNet.SensorBoard[0].sensor[1].Data>32700)
		{
			if(gate_value < (SensorNet.SensorBoard[0].sensor[1].Data - 32768))
				{
					PushGraphData(&(Lcd.Graph[3]),10);
				}
			else
				{
					PushGraphData(&(Lcd.Graph[3]),0);
				}
		}
	else
		{
			PushGraphData(&(Lcd.Graph[3]),0);
		}
	
	//PushGraphData(&(Lcd.Graph[3]),Lcd.Graph[1].Range);
	
		
	/*GUI_ReDrawLineChart(&(Lcd.Graph[0]),(Lcd.Graph[0].WritePoint));
	GUI_ReDrawLineChart(&(Lcd.Graph[1]),(Lcd.Graph[1].WritePoint));
	
	GUI_ReDrawLineChart(&(Lcd.Graph[2]),(Lcd.Graph[0].WritePoint));
	GUI_ReDrawLineChart(&(Lcd.Graph[3]),(Lcd.Graph[1].WritePoint));
	*/
	//HAL_UART_Transmit(&huart2,"HELLO WORLD\n",13,0XFFFF);
	
	//osDelay(50);
	//HAL_UART_Transmit(&huart3,"HELLO WORLD\n",13,0XFFFF);

  	//Enable_Path(RS485_PATH_1);
	//HAL_UART_Transmit(&huart2,"HELLO WORLD\n",13,0XFFFF);
	//ChangeBee(power,tone);
  }
  /* USER CODE END SysTaskFunction */
}

/* USER CODE BEGIN Header_ScanKeyFunction */
/**
* @brief Function implementing the ScanKey thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ScanKeyFunction */
void ScanKeyFunction(void const * argument)
{
  /* USER CODE BEGIN ScanKeyFunction */
	save_task_info();
  /* Infinite loop */
  for(;;)
  {		
	read_key();
	osDelay(40);
  }
  /* USER CODE END ScanKeyFunction */
}

/* USER CODE BEGIN Header_DealKeyFunction */
/**
* @brief Function implementing the DealKey thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DealKeyFunction */
void DealKeyFunction(void const * argument)
{
  /* USER CODE BEGIN DealKeyFunction */
	save_task_info();
  /* Infinite loop */
	unsigned char recv_key_value = 0;	
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(KeyQueueHandle, &recv_key_value, portMAX_DELAY);
	deal_key_value(recv_key_value);  
  }
  /* USER CODE END DealKeyFunction */
}

/* USER CODE BEGIN Header_DealComTask */
/**
* @brief Function implementing the DealCom thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DealComTask */
void DealComTask(void const * argument)
{
  /* USER CODE BEGIN DealComTask */
	save_task_info();
	unsigned char serial_value;
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(COM_QUEUE, &serial_value, portMAX_DELAY);
		switch(serial_value)
			{
				case COM1_MSG:
					//HAL_GPIO_TogglePin(POWER_CTRL_GPIO_Port,POWER_CTRL_Pin);
					//Deal_RS232_Data(PTR_COM1_RECV_DATA,COM1_RECV_DATA_LENGTH);
					break;
						
				case COM2_MSG:
					//iap_deal_recv_data(PTR_COM2_RECV_DATA,COM2_RECV_DATA_LENGTH);
					Deal_RS232_Data(PTR_COM2_RECV_DATA,COM2_RECV_DATA_LENGTH);
					break;

				case COM3_MSG:
					Deal_RS232_Data(PTR_COM3_RECV_DATA,COM3_RECV_DATA_LENGTH);					
					break;

				case COM4_MSG:
					
					break;

				case ERROR_MSG:
					break;

				default:
					break;
			}
  }
  /* USER CODE END DealComTask */
}

/* USER CODE BEGIN Header_GuiOperattask */
/**
* @brief Function implementing the GuiOperat thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GuiOperattask */
void GuiOperattask(void const * argument)
{
  /* USER CODE BEGIN GuiOperattask */
	save_task_info();
  	osDelay(500);
  
	Lcd_Init();
	Lcd_Clear(Lcd.imageBack_Color.Colour_Value);
	
	Lcd.CurMenu.MenuInit();

	
 	//Lcd_Clear(Lcd.MenuBackColor.Colour_Value);

	Lcd.Graph[0].Color.Colour_Com.red = 15;
	Lcd.Graph[0].DrawStartPoint = 0;
	Lcd.Graph[0].DrawPointNum = 50;
	Lcd.Graph[0].X_Pos= 30;	
	Lcd.Graph[0].Y_Pos= 47;
	Lcd.Graph[0].X_Size= 100;
	Lcd.Graph[0].Y_Size= 45;
	
	Lcd.Graph[1].Color.Colour_Com.red = 15;
	Lcd.Graph[1].Color.Colour_Com.blue= 15;
	Lcd.Graph[1].DrawStartPoint = 0;
	Lcd.Graph[1].DrawPointNum = 50;	
	Lcd.Graph[1].X_Pos= 30;	
	Lcd.Graph[1].Y_Pos= 94;
	Lcd.Graph[1].X_Size= 100;
	Lcd.Graph[1].Y_Size= 45;

	Lcd.Graph[2].Color.Colour_Com.green= 7;
	Lcd.Graph[2].DrawStartPoint = 0;
	Lcd.Graph[2].DrawPointNum = 100;
	Lcd.Graph[2].X_Pos= 30;	
	Lcd.Graph[2].Y_Pos= 111;
	Lcd.Graph[2].X_Size= 100;
	Lcd.Graph[2].Y_Size= 15;
	
	Lcd.Graph[3].Color.Colour_Com.red = 15;
	Lcd.Graph[3].Color.Colour_Com.green= 7;
	Lcd.Graph[3].DrawStartPoint = 0;
	Lcd.Graph[3].DrawPointNum = 100;
	Lcd.Graph[3].X_Pos= 30;	
	Lcd.Graph[3].Y_Pos= 128;
	Lcd.Graph[3].X_Size= 100;
	Lcd.Graph[3].Y_Size= 15;
	

	
		
/*GUI_InitLinrChart(&(Lcd.Graph[0]));
	GUI_DrawLineChart(&(Lcd.Graph[0]));
	
	GUI_InitLinrChart(&(Lcd.Graph[1]));
	GUI_DrawLineChart(&(Lcd.Graph[1]));
	
	GUI_InitLinrChart(&(Lcd.Graph[2]));
	GUI_DrawLineChart(&(Lcd.Graph[2]));
	
	GUI_InitLinrChart(&(Lcd.Graph[3]));
	GUI_DrawLineChart(&(Lcd.Graph[3]));
*/

	unsigned int GuiMsg = 0;
	unsigned int time_tick = 0;
	char buf[20] = {0};	
	//GUI_MenuChange(ShowGraphMenuInit,ShowGraphMenuClear,ShowGraphMenuOperat);
  /* Infinite loop */
  for(;;)
  {  	
  	//SQUARE_TEST();
	xQueueReceive(GuiQueueHandle, &GuiMsg, portMAX_DELAY);
	Lcd.CurMenu.MenuOperat(GuiMsg);
/*
	if(0XF0 == GuiMsg)
		{	
			sprintf(buf,"SD Size is %dMB",((USERFatFS.csize)*512*(USERFatFS.n_fatent)));
			GUI_FillStringAt(buf, 30,20, (Lcd.StrColor.Colour_Value), Lcd.StrBack_Color.Colour_Value, &GUI_FontHZ10x10);
		}
	if(0XF1 == GuiMsg)
		{		
			sprintf(buf,"SD Card Removed",((USERFatFS.csize)*512*(USERFatFS.n_fatent)));
			GUI_FillStringAt(buf, 30,20, Lcd.StrColor.Colour_Value, Lcd.StrBack_Color.Colour_Value, &GUI_FontHZ10x10);
		}
*/

	///帧率测试
	/*
	time_tick = HAL_GetTick();
	Lcd_Clear(WHITE);
	time_tick = HAL_GetTick() - time_tick;
	sprintf(buf,"%3d",time_tick);
	GUI_FillStringAt(buf,120,110,BLACK,WHITE,&GUI_FontHZ10x10);
	time_tick = HAL_GetTick();
	
	time_tick = HAL_GetTick();
	Lcd_Clear(BLUE);
	time_tick = HAL_GetTick() - time_tick;
	sprintf(buf,"%3d",time_tick);
	GUI_FillStringAt(buf,120,110,BLACK,BLUE,&GUI_FontHZ10x10);
	time_tick = HAL_GetTick();
	
	time_tick = HAL_GetTick();
	Lcd_Clear(YELLOW);
	time_tick = HAL_GetTick() - time_tick;
	sprintf(buf,"%3d",time_tick);
	GUI_FillStringAt(buf,120,110,BLACK,YELLOW,&GUI_FontHZ10x10);
	time_tick = HAL_GetTick();
	
	time_tick = HAL_GetTick();
	Lcd_Clear(GREEN);
	time_tick = HAL_GetTick() - time_tick;
	sprintf(buf,"%3d",time_tick);
	GUI_FillStringAt(buf,120,110,BLACK,GREEN,&GUI_FontHZ10x10);
	time_tick = HAL_GetTick();
	*/

  }
  /* USER CODE END GuiOperattask */
}

/* USER CODE BEGIN Header_SendToSensorNetTask */
/**
* @brief Function implementing the SendToSensorNet thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendToSensorNetTask */
void SendToSensorNetTask(void const * argument)
{
  /* USER CODE BEGIN SendToSensorNetTask */
	save_task_info();
	static unsigned char Polling_Delay = 1;
	unsigned char iap_test = 0;
	static unsigned int last_scan_clk;
	static unsigned int last_inventry_clk;
  /* Infinite loop */
	if(0 == iap_test)
	{
		osDelay(8500);
	}
	else
	{
		EnterIapModCMD();
		osDelay(20);
		IapSendTest();
		osDelay(20);
		IapSendTest();
		osDelay(20);
	}
  sensor_upload_mod = 2;
  /* Infinite loop */
  Polling_Delay = ((SensorNet.CurSensorNum)*CUR_SENSOR_DELAY) > CUR_FREQUENCY? 1:(CUR_FREQUENCY -((SensorNet.CurSensorNum)*CUR_SENSOR_DELAY) );

sensor_upload_mod = UPLOAD_MOD_RAW_VALUE;
  for(;;)
  {
	osDelay(1);
	if(0 == G_EnterIapMod)//iap时不轮询
	{
		switch(G_SensorNetDownLoadFlag)
			{
				case(INVENTORY_PROCESS):
					
				
					GetAllSensorValue(sensor_upload_mod);
					G_SensorNetDownLoadFlag = CMD_PROCESS_IDLE;
				
					if(G_NetGuardGap > NET_GUARD_GAP)//网络维护函数
						{
							G_NetGuardGap = 0;
							G_SensorNetDownLoadFlag = CMD_PROCESS_NET_GUARD;
						}
					G_NetGuardGap++;					

				break;
				
				case(CMD_PROCESS_START_AUTOSET):
					osDelay(slot_syn_time*((SensorNet.CurSensorNum/10)+1));//等待时槽结束
					StartAutoSetAllCSD();
					G_SensorNetDownLoadFlag = CMD_PROCESS_IDLE;
					osDelay(slot_syn_time);
				break;
				
				case(CMD_PROCESS_SET_CSD_PARATEMER):
					osDelay(slot_syn_time*((SensorNet.CurSensorNum/10)+1));//等待时槽结束
					SetALLSensorIDACScope(G_CsdParIdacMin,G_CsdParIdacMax);
					osDelay(10);
					SetALLSensorIDAC_CScope(G_CsdParIdac_CMin,G_CsdParIdac_CMax);
					osDelay(10);
					SetALLSensorCLKScope(G_CsdParSclkMin,G_CsdParSclkMax);
					osDelay(10);
					SetALLSensorWindow_Scope(G_CsdParWindow_Min,G_CsdParWindow_Max);
					osDelay(10);
					G_SensorNetDownLoadFlag = CMD_PROCESS_IDLE;
					osDelay(slot_syn_time);
				break;
				
				
				case CMD_PROCESS_NET_GUARD:
					G_SensorNetDownLoadFlag = CMD_PROCESS_IDLE;
					NetGuard();
					break;
					
				case CMD_PROCESS_SET_BASELINE_PAEATEMER:
					osDelay(slot_syn_time*((SensorNet.CurSensorNum/10)+1));//等待时槽结束
					SetSensorBaseLineThresoldValue(0XFF,G_BASELINE_THRESHOLE);
					osDelay(10);
					SetSensorBaseLineNoiseThresoldValue(0XFF,G_BASELINE_NOISE_THRESHOLE);
					osDelay(10);
					SetSensorBaseLineUpdataTime(0XFF,G_BASELINE_UPDATA_TIME);
					osDelay(10);
					G_SensorNetDownLoadFlag = CMD_PROCESS_IDLE;
					osDelay(slot_syn_time);
				break;

				case CMD_PROCESS_CTRL_LED:
					osDelay(slot_syn_time*((SensorNet.CurSensorNum/10)+1));//等待时槽结束
					CtrlLedFlash(G_TRANS_VALUE, 1);
					G_SensorNetDownLoadFlag = CMD_PROCESS_IDLE;
					osDelay(slot_syn_time);
				break;

				case CMD_PROCESS_IDLE:
					if((HAL_GetTick() - last_scan_clk) > BOARD_SCAN_TIME)
						{
							//StartNextBoardScan();
							last_scan_clk = HAL_GetTick();
							break;
						}

					
					if((HAL_GetTick() - last_inventry_clk)>(Polling_Delay))
						{							
							G_SensorNetDownLoadFlag = INVENTORY_PROCESS;
							last_inventry_clk = HAL_GetTick();
						}
					

					break;
				
				default:
				break;
					
			}
		}/**/
  }
  /* USER CODE END SendToSensorNetTask */
}

/* USER CODE BEGIN Header_DealCanTask */
/**
* @brief Function implementing the DealCan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DealCanTask */
void DealCanTask(void const * argument)
{
  /* USER CODE BEGIN DealCanTask */
	save_task_info();
	unsigned char Can_value;
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(CanQueueHandle, &Can_value, portMAX_DELAY);
	Deal_CAN_Data_Fifo(&CanRecvFifo,Can_value);			
  }
  /* USER CODE END DealCanTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
