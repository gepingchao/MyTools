//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 1.44��LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103RB
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PB13��SCL��
//              SDA   ��PB15��SDA��
//              RES   ��PB11
//              DC    ��PB10
//              CS    ��PAB12 
//							BL		��PB1
*******************************************************************************/
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/


#include "Lcd_Driver.h"


#include "include.h"
#define USE_SHOW_BUFF 0

#if (USE_SHOW_BUFF == 1)

#define SHOW_BUFF_LENGTH	3096
unsigned short SHOW_BUF[SHOW_BUFF_LENGTH] = {0};

#endif


//Һ��IO��ʼ������

void LCD_Delay(void)
{
	unsigned char delay = 100;
	while(delay--);
}

//��SPI���ߴ���һ��8λ����
void  SPI_WriteData(unsigned char Data)
{
	unsigned char send_data = Data;
	HAL_SPI_Transmit(&hspi1, &Data, 1, 0XFFFF);

	/*unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)
			{
				//LCD_SDA_SET; //�������
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			}
		else 
			{
				//LCD_SDA_CLR;
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			}
		LCD_Delay();
	   
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	      //LCD_SCL_CLR;       
	      //LCD_SCL_SET;
	      Data<<=1; 
	}
	*/
}

//��Һ����дһ��8λָ��
void Lcd_WriteIndex(unsigned char  Index)
{

	unsigned char send_data = Index;
	HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &send_data, 1, 0XFF);
	//SPI_WriteData(Index);
	/*
   //SPI д����ʱ��ʼ
   LCD_CS_CLR;
   LCD_RS_CLR;
	 SPI_WriteData(Index);
   LCD_CS_SET;
   */
}

//��Һ����дһ��8λ����
void Lcd_WriteData(unsigned char Data)
{

	unsigned char send_data = Data;
	HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, &send_data, 1, 0XFF);
	
   	//SPI_WriteData(Data);
/*
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteData(Data);
   LCD_CS_SET; 
*/
   
}
//��Һ����дһ��16λ����
void LCD_WriteData_16Bit(unsigned short Data)
{
unsigned short send_data;
	
send_data = Data;

	HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, (unsigned char*)&send_data, 2, 0XFFFF);
	//HAL_SPI_Transmit_DMA(&hspi1, (unsigned char*)&send_data, 2);
	
	 //SPI_WriteData(Data>>8); 	//д���8λ����
	 //SPI_WriteData(Data); 			//д���8λ����
	
  /* LCD_CS_CLR;
   LCD_RS_SET;
	 SPI_WriteData(Data>>8); 	//д���8λ����
	 SPI_WriteData(Data); 			//д���8λ����
   LCD_CS_SET; 
   */
}

void Lcd_WriteReg(unsigned char Index,unsigned char Data)
{
	Lcd_WriteIndex(Index);
	Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
	
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	
	/*LCD_RST_CLR;
	delay_ms(100);
	LCD_RST_SET;
	delay_ms(50);*/
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
void Lcd_Init(void)
{	
	//LCD_GPIO_Init();
	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	HAL_Delay (120);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
	Lcd_WriteData(0xA0); 
	//Lcd_WriteData(0xA4); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	
	//ST7735R Gamma Sequence
	/*Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  */
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x88);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0xa1);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	
	Lcd_WriteIndex(0x29);//Display on	 
}


/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd_SetRegion(unsigned short x_start,unsigned short y_start,unsigned short x_end,unsigned short y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_start+1);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+1);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x02);
	Lcd_WriteData(y_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+2);
	
	Lcd_WriteIndex(0x2c);
	Lcd.status = 1;
}

/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(unsigned short x,unsigned short y)
{
  	Lcd_SetRegion(x,y,x,y);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/

unsigned short X_PIXEL_OFFSET = 0;
unsigned short Y_PIXEL_OFFSET = 0;
unsigned short X_PIXEL_POLAR = 1;
unsigned short Y_PIXEL_POLAR = 1;

void Gui_DrawPoint(unsigned short x,unsigned short y,unsigned short Data)
{
	if(X_PIXEL_POLAR)
		{
			x+=X_PIXEL_OFFSET;
		}
	else
		{
			x=X_PIXEL_OFFSET-x;
		}	

	if(Y_PIXEL_POLAR)
		{
			y+=Y_PIXEL_OFFSET;
		}
	else
		{
			y=Y_PIXEL_OFFSET-y;
		}
	if((x > X_MAX_PIXEL)||(y > Y_MAX_PIXEL))
		{
			return;
		}
	Lcd_SetRegion(x,y,x+1,y+1);
	LCD_WriteData_16Bit(Data);

}    





/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
unsigned int Lcd_ReadPoint(unsigned short x,unsigned short y)
{
  unsigned int Data;
  Lcd_SetXY(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  Lcd_WriteData(Data);
  return Data;
}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/

HAL_StatusTypeDef MY_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  HAL_StatusTypeDef errorcode = HAL_OK;



  __HAL_LOCK(hspi);


  /* Set the transaction information */
  hspi->pTxBuffPtr  = (uint8_t *)pData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;
  /* Configure communication direction : 1Line */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    SPI_1LINE_TX(hspi);
  }


  /* Transmit data in 16 Bit mode */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)
  {
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (hspi->TxXferCount == 0x01U))
    {
      hspi->Instance->DR = *((uint16_t *)pData);
      pData += sizeof(uint16_t);
      hspi->TxXferCount--;
    }
    /* Transmit data in 16 Bit mode */
    while (hspi->TxXferCount > 0U)
    {
      /* Wait until TXE flag is set to send data */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
        hspi->Instance->DR = *((uint16_t *)pData);
        pData += sizeof(uint16_t);
        hspi->TxXferCount--;
      }
      else
      {
        /* Timeout management */
        if ((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >=  Timeout)))
        {
          errorcode = HAL_TIMEOUT;
          goto error;
        }
      }
    }
  }
  /* Transmit data in 8 Bit mode */


  /*Check the end of the transaction
  if (SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK)
  {
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
  } */ 
__IO uint8_t tmpreg;
while ((hspi->Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY)
  {
    if ((SPI_FLAG_FTLVL == SPI_SR_FRLVL) && (SPI_FTLVL_EMPTY == SPI_FRLVL_EMPTY))
    {
      tmpreg = *((__IO uint8_t *)&hspi->Instance->DR);
      /* To avoid GCC warning */
      UNUSED(tmpreg);
    }
}
	
  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }

  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    errorcode = HAL_ERROR;
  }

error:
  hspi->State = HAL_SPI_STATE_READY;
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);
  return errorcode;
}

void Lcd_Clear(unsigned short Color)               
{	
   unsigned int i,m;
   static unsigned short tmp_color = 0;
   tmp_color = Color;
   //unsigned int time_count = 0;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
   //__disable_irq();
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	//LCD_WriteData_16Bit(Color);
		HAL_SPI_Transmit(&hspi1, (unsigned char*)&Color, 2, 0XF);
		//HAL_SPI_Transmit_DMA(&hspi1, (unsigned char*)&tmp_color, 2);	
		
    } 
//__enable_irq();
}

void Lcd_Clear_Next(unsigned char MOD)
{
	static unsigned char next = 1;
	if(MOD)
		{
			next ++;
		}
	else
		{
			next--;
		}
	if (next == 0)
		{
			next = 5;
		}
	if (next == 6)
		{
			next = 1;
		}
	switch(next)
		{
			case 1:
				Lcd_Clear(RED);
				break;
			case 2:
				Lcd_Clear(GREEN);
				break;
			case 3:
				Lcd_Clear(YELLOW);
				break;
			case 4:
				Lcd_Clear(BLUE);
				break;
			case 5:
				Lcd_Clear(GRAY0);
				break;
		}

}

