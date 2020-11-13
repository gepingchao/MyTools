
#include "SensorShow.h"

#define SQUARE_X_SIZE	9
#define SQUARE_Y_SIZE	9
unsigned short Square[SQUARE_X_SIZE][SQUARE_Y_SIZE] = {0};

void Fast_Draw_Square(unsigned char x,unsigned char y,unsigned short color)
{
	unsigned char loop_outside,loop_inside;
	for(loop_outside = 0 ;loop_outside < SQUARE_X_SIZE;loop_outside++)
		{			
			for(loop_inside = 0 ;loop_inside < SQUARE_Y_SIZE;loop_inside++)
				{
					Square[loop_outside][loop_inside] = color;
				}
		}
	Lcd_SetRegion(x,y,x+SQUARE_X_SIZE-1,y+SQUARE_Y_SIZE-1);
	Lcd_WriteIndex(0x2C);
	HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
	//__disable_irq();
	HAL_SPI_Transmit(&hspi1, (unsigned char*)&Square, 2*SQUARE_X_SIZE*SQUARE_Y_SIZE, 0XFFF);
	
}

void Draw_Square(unsigned char x,unsigned char y,unsigned short color)
{
	unsigned char loop_outside,loop_inside;
	unsigned short tmpcolor;
	tmpcolor = color;
	Lcd_SetRegion(x,y,x+SQUARE_X_SIZE-1,y+SQUARE_Y_SIZE-1);
	Lcd_WriteIndex(0x2C);
	for(loop_outside = 0 ;loop_outside < SQUARE_X_SIZE;loop_outside++)
		{			
			for(loop_inside = 0 ;loop_inside < SQUARE_X_SIZE;loop_inside++)
				{
					//Square[loop_outside][loop_inside] = color;
					HAL_SPI_Transmit(&hspi1, (unsigned char*)&tmpcolor, 2, 0XF);
				}
		}	
}

void FastLcdClear(unsigned short color)
{

	unsigned char loopx,loopy;
	for(loopx = 0 ; loopx < 160 ; loopx +=(SQUARE_X_SIZE))	
		{
		for(loopy = 0 ; loopy < 128 ; loopy +=(SQUARE_Y_SIZE))
			{
				
				Fast_Draw_Square(loopx,loopy,color);
			}
		}

	
}

void FastLcdClear_SQUARE(unsigned char x_start,unsigned char y_start,unsigned char x_end,unsigned char y_end,unsigned short color)
{

	unsigned char loopx,loopy;
	for(loopx = x_start ; loopx < x_end ; loopx +=(SQUARE_X_SIZE))	
		{
		for(loopy = y_start ; loopy < y_end ; loopy +=(SQUARE_Y_SIZE))
			{
				
				Fast_Draw_Square(loopx,loopy,color);
			}
		}

	
}


/*
rgb
255 0 0
255 255 0
0 255 0
0 255 255 
0 0 255
255 0 255

6块

*/

//unsigned short COLOR_MAP_MAX = 65535;
//unsigned short COLOR_MAP_MIN = 0;



volatile unsigned short DATA_MAP_MAX=65535;
volatile unsigned short DATA_MAP_MIN=20000;

#define COLOR_MAP_MAX	65500
#define COLOR_MAP_MIN	0
#define COLOR_MAP_GAP	(COLOR_MAP_MAX - COLOR_MAP_MIN)

#define COLOR_MAP_BANK	(COLOR_MAP_GAP/5)	

#define COLOR_MAP_BANK0	(COLOR_MAP_BANK)	
#define COLOR_MAP_BANK1	(COLOR_MAP_BANK *2)	
#define COLOR_MAP_BANK2	(COLOR_MAP_BANK *3)	
#define COLOR_MAP_BANK3	(COLOR_MAP_BANK *4)	
#define COLOR_MAP_BANK4	COLOR_MAP_MAX	




unsigned short RED_COLOR_MAX=65535;
unsigned short RED_COLOR_MIN=32768;

unsigned short BLUE_COLOR_MAX=31000;
unsigned short BLUE_COLOR_MIN=0;

unsigned short GREEN_COLOR_MAX=40000;
unsigned short GREEN_COLOR_MIN=20000;

unsigned short ColorMapRainBow(unsigned short data)
{
	
	Colour TmpColor;
	TmpColor.Colour_Value = 0;
	unsigned char tmp= 0;
	unsigned int tmp_data;
	
	if(data > COLOR_MAP_BANK4)
		{
			return 0XFFFF;
		}
	if(data < COLOR_MAP_MIN)
		{
			return 0;
		}
	///tmp_data = data;
	if(data>DATA_MAP_MIN)
		{
			tmp_data = data - DATA_MAP_MIN;	
		}
	else 
		{
			tmp_data = 0;
		}
	
	if(data>DATA_MAP_MAX)
		{
			tmp_data = DATA_MAP_MAX - DATA_MAP_MIN;	
		}
	
	tmp_data = (tmp_data *(COLOR_MAP_MAX-COLOR_MAP_MIN) /(DATA_MAP_MAX-DATA_MAP_MIN));
	if(tmp_data > COLOR_MAP_BANK3)
		{//增高方向 红满绿减
			TmpColor.Colour_Com.red = 15;
			tmp = 7 - (((tmp_data-COLOR_MAP_BANK3))*7 / COLOR_MAP_BANK);
			TmpColor.Colour_Com.green=  tmp;//((tmp == 0)? 1:tmp);
			return TmpColor.Colour_Value;
		}
	if(tmp_data > COLOR_MAP_BANK2)
		{//增高方向 绿满红增
			TmpColor.Colour_Com.green= 7;
			tmp =(((tmp_data-COLOR_MAP_BANK2))*15 / COLOR_MAP_BANK);
			TmpColor.Colour_Com.red=  tmp;//((tmp == 0)? 1:tmp);
			return TmpColor.Colour_Value;
		}
	if(tmp_data > COLOR_MAP_BANK1)
		{//增高方向 绿满蓝减
			TmpColor.Colour_Com.green= 7;
			tmp =31 - (((tmp_data-COLOR_MAP_BANK1))*31 / COLOR_MAP_BANK);
			TmpColor.Colour_Com.blue= tmp;// ((tmp == 0)? 1:tmp);
			return TmpColor.Colour_Value;
		}
	if(tmp_data > COLOR_MAP_BANK0)
		{//增高方向 蓝满绿增
			TmpColor.Colour_Com.blue= 31;
			tmp = (((tmp_data-COLOR_MAP_BANK0))*7 / COLOR_MAP_BANK);
			TmpColor.Colour_Com.green=  tmp;//((tmp == 0)? 1:tmp);
			return TmpColor.Colour_Value;
		}
	//增高方向 蓝满红减
	TmpColor.Colour_Com.blue= 31;
	tmp =15 - (((tmp_data-0))*15 / COLOR_MAP_BANK);
	TmpColor.Colour_Com.red= tmp;// ((tmp == 0)? 1:tmp);
	return TmpColor.Colour_Value;
}

unsigned short ColorMap(unsigned short data)
{
	Colour TmpColor;
	TmpColor.Colour_Value = 0;
	unsigned char tmp= 0;
	if((data > RED_COLOR_MIN)&&(data < RED_COLOR_MAX))
		{
			tmp = ((((data-RED_COLOR_MIN))*15 / (RED_COLOR_MAX -RED_COLOR_MIN)));
			TmpColor.Colour_Com.red =  ((tmp == 0)? 1:tmp);
		}
	if((data > BLUE_COLOR_MIN)&&(data < BLUE_COLOR_MAX))
		{
			tmp =32 - (((data-BLUE_COLOR_MIN))*32 / (BLUE_COLOR_MAX -BLUE_COLOR_MIN));
			TmpColor.Colour_Com.blue= ((tmp == 0)? 1: tmp);
			//TmpColor.Colour_Com.blue= ((tmp_data-BLUE_COLOR_MIN) / (BLUE_COLOR_MAX -BLUE_COLOR_MIN))*32;
		}
	if((data > GREEN_COLOR_MIN)&&(data < GREEN_COLOR_MAX))
		{
			tmp =8 - (((data-GREEN_COLOR_MIN))*8 / (GREEN_COLOR_MAX -GREEN_COLOR_MIN));
			TmpColor.Colour_Com.green=  ((tmp == 0)? 1:tmp);
			//TmpColor.Colour_Com.green= ((tmp_data-GREEN_COLOR_MIN) / (GREEN_COLOR_MAX -GREEN_COLOR_MIN))*8;
		}
	return TmpColor.Colour_Value;
}

#define SHOW_LINE_SQUARE_NUM 16
void ShowSensorValue(void)
{
	unsigned char line_re;
	unsigned char loop_sensor_board_num,loop_sensor_num,loop_line_num;
	for(loop_sensor_board_num = 0 ; loop_sensor_board_num  < SensorNet.CurSensorNum ;loop_sensor_board_num++)
		{
			line_re=(BOARD_SENSOR_NUM/SHOW_LINE_SQUARE_NUM)-1;
			for(loop_sensor_num = 0 ; loop_sensor_num  < BOARD_SENSOR_NUM;)
				{
					for(loop_line_num = 0 ; loop_line_num  < SHOW_LINE_SQUARE_NUM ;loop_line_num++)
						{							
							//Fast_Draw_Square(loop_line_num*SQUARE_X_SIZE, (loop_sensor_board_num * (BOARD_SENSOR_NUM/SHOW_LINE_SQUARE_NUM) + line_re)*SQUARE_Y_SIZE,ColorMap((SensorNet.SensorBoard[loop_sensor_board_num].sensor[loop_sensor_num].Data)));
							Fast_Draw_Square(loop_line_num*SQUARE_X_SIZE, (loop_sensor_board_num * (BOARD_SENSOR_NUM/SHOW_LINE_SQUARE_NUM) + line_re)*SQUARE_Y_SIZE,ColorMapRainBow((SensorNet.SensorBoard[loop_sensor_board_num].sensor[loop_sensor_num].Data)));
							loop_sensor_num++;
						}
					line_re--;
				}
		}
}

void SQUARE_TEST(void)
{
	static unsigned short color = 0;
	static unsigned short color1 = 0;
	unsigned char loopx,loopy;
	unsigned int tick;
	static unsigned char flag = 0;
	if(flag)
		{
			tick = HAL_GetTick();
			FastLcdClear	(color++);
			osDelay(10);
			tick = HAL_GetTick() - tick;
		}

	else
		{
			for(loopx = 0 ; loopx < 160 ; loopx +=SQUARE_X_SIZE)	
				{
				for(loopy = 0 ; loopy < 128 ; loopy +=SQUARE_Y_SIZE)
					{
						
						Fast_Draw_Square(loopx,loopy,color+=uwTick);
					}
				}
			osDelay(10);
		}

	//Faste_Draw_Square(10,10,color++);
	//Lcd_Clear(color++);
}
