#include "uart_senddata.h"

#define COM	COM3
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

#define SHOW_ON_LCD	1

#if(SHOW_ON_LCD == 1)

// 0 0 150 110
#define LCD_SHOW_START_X 0
#define LCD_SHOW_START_Y 60
#define LCD_SHOW_END_X 150
#define LCD_SHOW_END_Y 110



#define X_MAX_SIZE 	150
#define Y_MAX_SIZE 	110

#define X_CHAR_LENGTH	6
#define Y_CHAR_LENGTH	10

unsigned char G_Char_x_offset = LCD_SHOW_START_X;
unsigned char G_Char_y_offset = LCD_SHOW_START_Y;

#endif

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&COM, (unsigned char *)&ch, 1, 0xFFFF);

  #if(SHOW_ON_LCD == 1)

	unsigned char buf[2] = {0};
	buf[0]  = ch;
	if(ch == '\n')
		{
			G_Char_y_offset += Y_CHAR_LENGTH;
			G_Char_x_offset =LCD_SHOW_START_X;
			if(G_Char_y_offset > LCD_SHOW_END_Y)
				{
					G_Char_y_offset = LCD_SHOW_START_Y;
					//FastLcdClear(Lcd.StrBack_Color.Colour_Value);
					FastLcdClear_SQUARE(LCD_SHOW_START_X,LCD_SHOW_START_Y,LCD_SHOW_END_X+10,LCD_SHOW_END_Y+10,Lcd.StrBack_Color.Colour_Value);
				}
			return 0;
		}
	GUI_FillStringAt(buf, G_Char_x_offset,G_Char_y_offset, (Lcd.StrColor.Colour_Value), Lcd.StrBack_Color.Colour_Value, &GUI_FontHZ10x10);
	G_Char_x_offset += X_CHAR_LENGTH;
	if(G_Char_x_offset > LCD_SHOW_END_X)
		{
			G_Char_x_offset = LCD_SHOW_START_X;
			G_Char_y_offset +=Y_CHAR_LENGTH;
			if(G_Char_y_offset > LCD_SHOW_END_Y)
				{
					G_Char_y_offset = LCD_SHOW_START_Y;
					//FastLcdClear(Lcd.StrBack_Color.Colour_Value);
					FastLcdClear_SQUARE(LCD_SHOW_START_X,LCD_SHOW_START_Y,LCD_SHOW_END_X+10,LCD_SHOW_END_Y+10,Lcd.StrBack_Color.Colour_Value);
				}
		}
	
  #endif

  return ch;
}

///////////////////////////////////////////////////////////////////
unsigned char string_length(char* string)
{
	char* tmp = string;
	unsigned char length = 0;
	while(*tmp++)
		{
			length ++; 
		}
	return length;
}
unsigned char str_to_hex(char* str,unsigned char* target,unsigned int max_length)//返回值就是转换的长度
{
	char* tmp_str = str;
	unsigned char* tmp_target = target;
	unsigned int loopx,loop_last,tmp = 0;
	CHANGE_TO_HEX change_buff;	
	//unsigned char reslut;
	
	tmp = string_length(str);
	if(tmp > 2*max_length)  //需要转换的长度大于设置的目标最大保存超度时   截去多余的转换长度防止段错误
		{
			tmp =  2*max_length;
		}
	
	loop_last = tmp%2;
	loopx = tmp/2;

	for(tmp = 0; tmp< loopx; tmp++)
		{
			if((*tmp_str) >= '0' && (*tmp_str) <= '9' )
				{
					change_buff.BYTE.byte_h = *tmp_str - '0';
				}
			else if((*tmp_str) >= 'A' && (*tmp_str) <= 'F' )
				{
					change_buff.BYTE.byte_h = *tmp_str - 'A' + 10;
				}
			else if((*tmp_str) >= 'a' && (*tmp_str) <= 'f' )
				{
					change_buff.BYTE.byte_h = *tmp_str - 'a' + 10;
				}
			else
				{
					change_buff.BYTE.byte_h = 0;
					//reslut = 0;
				}
			tmp_str++;
			if((*tmp_str) >= '0' && (*tmp_str) <= '9' )
				{
					change_buff.BYTE.byte_l = *tmp_str - '0';
				}
			else if((*tmp_str) >= 'A' && (*tmp_str) <= 'F' )
				{
					change_buff.BYTE.byte_l = *tmp_str - 'A' +10;
				}
			else if((*tmp_str) >= 'a' && (*tmp_str) <= 'f' )
				{
					change_buff.BYTE.byte_l = *tmp_str - 'a' +10;
				}
			else
				{
					change_buff.BYTE.byte_l = 0;
					//reslut = 0;
				}
			tmp_str++;
			*tmp_target++ = change_buff.reslut;
		}
	if(loop_last)
		{
			if((*tmp_str) >= '0' && (*tmp_str) <= '9' )
				{
					change_buff.BYTE.byte_l = *tmp_str - '0';
				}
			else if((*tmp_str) >= 'A' && (*tmp_str) <= 'F' )
				{
					change_buff.BYTE.byte_l = *tmp_str - 'A' + 10;
				}
			else if((*tmp_str) >= 'a' && (*tmp_str) <= 'f' )
				{
					change_buff.BYTE.byte_l = *tmp_str - 'a' +10;
				}
			else
				{
					change_buff.BYTE.byte_l =0;
					//reslut = 0;
				}
			change_buff.BYTE.byte_h =0;
			*tmp_target = change_buff.reslut;
			loopx++;
		}
	return loopx;
}

void hex_to_str(char* target_str,unsigned char* hex,unsigned int hex_length)
{
	unsigned char* hex_tmp = hex;
	unsigned char loopx;
	CHANGE_TO_HEX change_buff;
	for(loopx = 0; loopx < hex_length ; loopx ++)
		{
			change_buff.reslut = *hex_tmp;
			if(change_buff.BYTE.byte_h <= 9)
				{
					*target_str = '0' + change_buff.BYTE.byte_h;
				}
			else
				{
					*target_str = 'A' + (change_buff.BYTE.byte_h - 10);
				}
			target_str++;
			
			if(change_buff.BYTE.byte_l <= 9)
				{
					*target_str = '0' + change_buff.BYTE.byte_l;
				}
			else 
				{
					*target_str = 'A' + (change_buff.BYTE.byte_l - 10);
				}
			target_str++;
			hex_tmp++;
		}
}


void SendToCOM1(unsigned char* data,unsigned short data_length)
{
	//HAL_UART_Transmit(&COM1, data, data_length, 0xFFFFFF);
	//HAL_UART_Transmit_DMA(&COM1, data, data_length);
	HAL_UART_Transmit(&COM3, data, data_length, 0xFFFFFF);
}

void SendToCOM3(unsigned char* data,unsigned short data_length)
{
	HAL_GPIO_WritePin(RS485_RD_GPIO_Port,RS485_RD_Pin,GPIO_PIN_SET);
	HAL_UART_Transmit(&COM2, data, data_length, 0xFFFFFF);
	HAL_GPIO_WritePin(RS485_RD_GPIO_Port,RS485_RD_Pin,GPIO_PIN_RESET);
	//HAL_UART_Transmit_DMA(&COM3, data, data_length);
}
