#include "gui.h"
#include "include.h"


unsigned char G_Normalization_buf[SAVE_POINT_NUM];
unsigned char G_Normalization_buf_2[SAVE_POINT_NUM];

S_LineChartBuf LineChartBuff;
//unsigned char G_Normalization_buf[10];

static unsigned int Step_value = 1;
#define BACKCOLOR	0X0F62
LCDStatus Lcd = {.MenuBackColor.Colour_Value = BACKCOLOR,
				.ImageColor.Colour_Value = 0X05A2,
				.imageBack_Color.Colour_Value = BACKCOLOR,
				.StrColor.Colour_Value = 0X0,
				.StrBack_Color.Colour_Value = BACKCOLOR,
				.StrCursorColor.Colour_Value = 0X00A5,
				.cursor = 7,
				.pre_cursor = 6,
				.CurMenu = {.MenuInit =MainMenuInit,.MenuOperat = MainMenuOperat,.MenuClear = MainMenuClear},
	};



unsigned short GetLcdColor(unsigned short x, unsigned short y,P_LCDStatus lcd)
{
	//unsigned char x_byte,x_bit;
	//x_bit = x/8;
	//x_byte = x%8;
	//if((lcd->Buff[x_byte][y])&(1 << x_bit))
		//{
			//return lcd->ImageColor.Colour_Value;
		//}
	return lcd->imageBack_Color.Colour_Value;
}


void SetLcdColor(unsigned short x, unsigned short y,P_LCDStatus lcd,unsigned char color)
{
	//unsigned char x_byte,x_bit;
	//x_bit = x/8;
	//x_byte = x%8;
	if(color)
		{
			//(lcd->Buff[x_byte][y])|=(1 << x_bit);
		}
	else
		{
			//(lcd->Buff[x_byte][y])&=~(1 << x_bit);
		}
}

void Gui_Circle(unsigned short X,unsigned short Y,unsigned short R,unsigned short fc) 
{
	unsigned short  a,b; 
	int c; 
	a=0; 
	b=R; 
	c=3-2*R; 
	while (a<b) 
	{ 
		Gui_DrawPoint(X+a,Y+b,fc);     //        7 
		Gui_DrawPoint(X-a,Y+b,fc);     //        6 
		Gui_DrawPoint(X+a,Y-b,fc);     //        2 
		Gui_DrawPoint(X-a,Y-b,fc);     //        3 
		Gui_DrawPoint(X+b,Y+a,fc);     //        8 
		Gui_DrawPoint(X-b,Y+a,fc);     //        5 
		Gui_DrawPoint(X+b,Y-a,fc);     //        1 
		Gui_DrawPoint(X-b,Y-a,fc);     //        4 

		if(c<0) c=c+4*a+6; 
		else 
		{ 
			c=c+4*(a-b)+10; 
			b-=1; 
		} 
		a+=1; 
	} 
	if (a==b) 
	{ 
		Gui_DrawPoint(X+a,Y+b,fc); 
		Gui_DrawPoint(X+a,Y+b,fc); 
		Gui_DrawPoint(X+a,Y-b,fc); 
		Gui_DrawPoint(X-a,Y-b,fc); 
		Gui_DrawPoint(X+b,Y+a,fc); 
		Gui_DrawPoint(X-b,Y+a,fc); 
		Gui_DrawPoint(X+b,Y-a,fc); 
		Gui_DrawPoint(X-b,Y-a,fc); 
	} 

} 


void Gui_DrawLine(unsigned short x0, unsigned short  y0,unsigned short  x1, unsigned short  y1,unsigned short Color)   
{
	int dx,             // difference in x's
	dy,             // difference in y's
	dx2,            // dx,dy * 2
	dy2, 
	x_inc,          // amount in pixel space to move during drawing
	y_inc,          // amount in pixel space to move during drawing
	error,          // the discriminant i.e. error i.e. decision variable
	index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;
	dy = y1-y0;

	if (dx>=0)
		{
			x_inc = 1;
		}
	else
		{
			x_inc = -1;
			dx    = -dx;  
		} 

	if (dy>=0)
		{
			y_inc = 1;
		} 
	else
		{
			y_inc = -1;
			dy    = -dy; 
		} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)
		{
			error = dy2 - dx; 
			for (index=0; index <= dx; index++)
				{
				
					Gui_DrawPoint(x0,y0,Color);
					if (error >= 0) 
					{
					error-=dx2;					
					y0+=y_inc;
					} 
					error+=dy2;

					x0+=x_inc;
				} 
		} 
	else
		{
			error = dx2 - dy; 
			for (index=0; index <= dy; index++)
				{
					Gui_DrawPoint(x0,y0,Color);
					if (error >= 0)
						{
							error-=dy2;
							x0+=x_inc;
						} 
					error+=dx2;
					y0+=y_inc;
				}
		} 
}







/////显示文字步骤  1提取字符编码 2根据编码找到点阵数据  3根据点阵信息显示字符

unsigned char GetCodeFromStr(char* str,unsigned short* offset,unsigned short* code)
{
	if(0 == (*(str+*offset)))
		{
			return 0;
		}

	if((*(str+*offset)) < 0X80)
		{
			*code = *(str+*offset);
			*offset += 1;
			return 1;//单字节
		}
	if((*(str+*offset)) > 0XAF)
		{
			*code = 0;
			*code |= (*(str+*offset))<<8;
			*code |= (*(str+*offset+1));
			*offset += 2;//两个字节
			return 2;
		}
	*offset += 1;
	return 3;//不明字节
	
}





/*
void gui_test(void)
{
	unsigned char str[20] = {"1232你好 我叫葛平超"};

	static unsigned char offset = 0;
	static unsigned short code = 0;


	while(GetCodeFromStr(str,&offset,&code));

	offset = 1+ code;

	
	static unsigned num = 0;
	Colour str_clor;
	Colour back_clor;
	str_clor.Colour_Value = 0;
	str_clor.Colour_Com.green = 60;
	back_clor.Colour_Value = 0;
	Lcd_SetRegion(0,0,(23),(17));
	GUI_StrFill(&(Cinfo[num]),(str_clor.Colour_Value),(back_clor.Colour_Value));
	num ++;
	if(num > 7)
		{
			num = 0;
		}
}
*/

const GUI_CHARINFO* GetFontData(unsigned short unicode,const GUI_FONT* font)
{
	const GUI_CHARINFO* RES;
	const GUI_FONT_PROP* tmp;
	unsigned short offset = 0;
	tmp = font->Start;

	CHECK:
		
	if((unicode >= (tmp-> First))&&(unicode <= (tmp-> Last)))
		{
			offset = unicode - (tmp-> First);
			RES =(tmp->paCharInfo) + offset;
			return RES;
		}
	else
		{
			if(0 != tmp->pNext)
				{
					tmp = tmp->pNext;
					goto CHECK;
				}
		}
	return (const GUI_CHARINFO*) 0;
}


void GUI_StrFill(const GUI_CHARINFO  * str_font,unsigned char x_pos,unsigned char y_pos,unsigned char x_size,unsigned char y_line,unsigned short str_color,unsigned short back_color)
{
	unsigned char bit_step = 0;
	unsigned short byte_step = 0;
	
	//Lcd_SetRegion(x_pos,y_pos,(x_pos + x_size - 1),(y_pos+y_line*8 -1));	
	Lcd_SetRegion(x_pos,y_pos,(x_pos+y_line*8 -1),(y_pos+x_size));	
	
	Lcd_WriteIndex(0x2C);
	
	for(byte_step = 0 ; byte_step < (x_size * y_line); byte_step++)
		{
			for(bit_step = 0 ; bit_step < 8 ; bit_step++)
				{
					if(*(str_font->pData +byte_step) & (0X80 >> bit_step))
						{
							LCD_WriteData_16Bit(str_color);
						}
					else
						{
							LCD_WriteData_16Bit(back_color);
						}
				}
		}
}


void GUI_StrDraw(const GUI_CHARINFO  * str_font,unsigned char x_pos,unsigned char y_pos,unsigned char x_size,unsigned char y_line,unsigned short str_color)
{
	unsigned char bit_step = 0;
	unsigned short byte_step = 0;	
	unsigned char line_step = 0;
	
	for(byte_step = 0 ; byte_step < (x_size ); byte_step++)
		{
			for(line_step = 0 ; line_step < y_line ; line_step++)
				{
					for(bit_step = 0 ; bit_step < 8 ; bit_step++)
						{
							if(*(str_font->pData +byte_step*y_line+line_step) & (0X80 >> bit_step))
								{
									Gui_DrawPoint((x_pos+ bit_step + line_step*8),(y_pos+byte_step),str_color);
								}
							else
								{
									
								}
						}
				}
		}
}


void GUI_FillStringAt(char* str,unsigned short x,unsigned short y,unsigned short str_color,unsigned short back_color,const GUI_FONT* font)
{
	//__disable_irq();
	unsigned short code_offset = 0;
	unsigned short code = 0;
	unsigned char disp_offset = 0;
	const GUI_CHARINFO* tmp_info;

	while(GetCodeFromStr(str,&code_offset,&code))
		{
			tmp_info = GetFontData(code,font);
			if(tmp_info)
				{
					GUI_StrFill(tmp_info,(x+disp_offset),y,(font->YSize),(tmp_info->BytesPerLine),str_color,back_color);
					//disp_offset += tmp_info->BytesPerLine*8;
					if(tmp_info->XSize < 4)
						{
							disp_offset += 4;
						}
					else
						{
							disp_offset += tmp_info->XSize;
						}
				}
			
		}
	//__enable_irq();
}


void GUI_DrawStringAt(char* str,unsigned short x,unsigned short y,unsigned short str_color,const GUI_FONT* font)
{
	unsigned short code_offset = 0;
	unsigned short code = 0;

	unsigned char disp_offset = 0;
	const GUI_CHARINFO* tmp_info;
	while(GetCodeFromStr(str,&code_offset,&code))
		{
			tmp_info = GetFontData(code,font);
			if(tmp_info)
				{
					//GUI_StrFill(tmp_info,(x+disp_offset),y,(font->YSize),(tmp_info->BytesPerLine),str_color,back_color);
					GUI_StrDraw(tmp_info,(x+disp_offset),y,(font->YSize),(tmp_info->BytesPerLine),str_color);
					//disp_offset += tmp_info->BytesPerLine*8;
					if(tmp_info->XSize < 4)
						{
							disp_offset += 4;
						}
					else
						{
							disp_offset += tmp_info->XSize;
						}
				}
			
		}
}

#define MIN_RANGE	100

void CalcExtremeInGraph(P_S_Graph graph)
{
	graph->Max = 0;
	graph->Min = 0XFFFF;
	unsigned char loopx;
	if((0 == graph->DrawPointNum) || (graph->DrawPointNum > SAVE_POINT_NUM))
		{
			graph->DrawPointNum = SAVE_POINT_NUM;
		}
	//for(loopx = 0 ; loopx < SAVE_POINT_NUM ; loopx++)
	for(loopx = 0 ; loopx < (graph->DrawPointNum) ; loopx++)
		{
			if((graph->Max) < (graph->buf[loopx]))
				{
					graph->Max = graph->buf[loopx];
				}
			if((graph->Min) > (graph->buf[loopx]))
				{
					graph->Min = graph->buf[loopx];
				}
		}
	(graph->Range) = (graph->Max) - (graph->Min);
	if(0 == graph->Range)
		{
			graph->Range = 1;
		}

}

unsigned char GetNormalizationValue(P_S_Graph graph,unsigned char Point,unsigned char Y_Range)
{
	unsigned char tmp;
	tmp = ((graph->buf[Point]) -( graph->Min)) *Y_Range/ (graph->Range);
	return tmp;
}

void CalcNormalizationValue(P_S_Graph graph,unsigned char Y_Range)
{
	unsigned char loopx;
	//for(loopx = 0 ;loopx < SAVE_POINT_NUM ; loopx++)
	if((0 == graph->DrawPointNum) || (graph->DrawPointNum > SAVE_POINT_NUM))
		{
			graph->DrawPointNum = SAVE_POINT_NUM;
		}
	for(loopx = 0 ;loopx < (graph->DrawPointNum) ; loopx++)
		{
			graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][loopx] = GetNormalizationValue(graph,loopx,Y_Range);
		}
}

void GUI_InitLinrChart(P_S_Graph graph)
{	
	Gui_DrawLine((graph->X_Pos),(graph->Y_Pos),(graph->X_Pos),((graph->Y_Pos) - (graph->Y_Size)),(graph->Color.Colour_Value));
	Gui_DrawLine((graph->X_Pos),((graph->Y_Pos) - (graph->Y_Size)),((graph->X_Pos) + (graph->X_Size)),((graph->Y_Pos) - (graph->Y_Size)),(graph->Color.Colour_Value));
	Gui_DrawLine(((graph->X_Pos) + (graph->X_Size)),((graph->Y_Pos) - (graph->Y_Size)),((graph->X_Pos) + (graph->X_Size)),(graph->Y_Pos),(graph->Color.Colour_Value));
	Gui_DrawLine(((graph->X_Pos) + (graph->X_Size)),(graph->Y_Pos),(graph->X_Pos),(graph->Y_Pos),(graph->Color.Colour_Value));
}

#define LINEGRRAPH_Y_BLANK	2
void GUI_DrawLineChart(P_S_Graph graph)
{
	unsigned char PointStep = 0;
	unsigned char TmpY_Front,TmpY_Behind;
	unsigned char TmpX = (graph->X_Pos) + 1;
	//unsigned char NormalizationValue = 0;
	unsigned char loopx;
	unsigned char y_axis_offset;
	y_axis_offset = (graph->Y_Pos) -1;

	if(0 == graph->DrawPointNum)
		{
			graph->DrawPointNum = 1;
		}
	
	PointStep = (graph->X_Size) / (graph->DrawPointNum);
	if( 0 == PointStep)
		{
			PointStep = 1;
		}
	CalcExtremeInGraph(graph);
	CalcNormalizationValue(graph,((graph->Y_Size)-LINEGRRAPH_Y_BLANK));
	
	for(loopx = 0 ; loopx < ((graph->DrawPointNum) -2) ; loopx++)
		{
			TmpY_Front = G_Normalization_buf[(graph->DrawStartPoint) + loopx];
			TmpY_Behind = G_Normalization_buf[(graph->DrawStartPoint) + loopx+1];
			Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep),(y_axis_offset - TmpY_Behind),(graph->Color.Colour_Value));
			TmpX += PointStep;
		}	
	TmpY_Front = G_Normalization_buf[(graph->DrawStartPoint) + loopx];
	TmpY_Behind = G_Normalization_buf[(graph->DrawStartPoint) + loopx+1];
	Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),((graph->X_Pos) + (graph->X_Size) - 1),(y_axis_offset - TmpY_Behind),(graph->Color.Colour_Value));
}

void GUI_ReDrawLineChart(P_S_Graph graph,unsigned char ReDrawPoint)
{
	if(0 == Lcd.status)
		{
			return;
		}
	unsigned char PointStep = 0;
	unsigned char TmpY_Front,TmpY_Behind;
	unsigned char TmpX = (graph->X_Pos) + 1;
	//unsigned char NormalizationValue = 0;
	unsigned char loopx;
	
	unsigned char y_axis_offset;
	y_axis_offset = (graph->Y_Pos) -1;

	char buf[10] = {0};

	if(0 == graph->DrawPointNum)
		{
			graph->DrawPointNum = 1;
		}
	
	PointStep = (graph->X_Size) / (graph->DrawPointNum);
	if( 0 == PointStep)
		{
			PointStep = 1;
		}
	CalcExtremeInGraph(graph);
	CalcNormalizationValue(graph,((graph->Y_Size)-LINEGRRAPH_Y_BLANK));
	sprintf(buf,"%5d",graph->Max);
	GUI_FillStringAt(buf,((graph->X_Pos) -30),(y_axis_offset-(graph->Y_Size)), graph->Color.Colour_Value, Lcd.MenuBackColor.Colour_Value,&GUI_FontHZ10x10);
	
	sprintf(buf,"%5d",graph->Range);
	GUI_FillStringAt(buf,((graph->X_Pos) -30),(y_axis_offset-10 -10),graph->Color.Colour_Value, Lcd.MenuBackColor.Colour_Value,&GUI_FontHZ10x10);
	
	sprintf(buf,"%5d",graph->Min);
	GUI_FillStringAt(buf,((graph->X_Pos) -30),(y_axis_offset-10), graph->Color.Colour_Value, Lcd.MenuBackColor.Colour_Value,&GUI_FontHZ10x10);
	for(loopx = 0 ; loopx < ((graph->DrawPointNum) -2) ; loopx++)
		{
			/*TmpY_Front = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx)%SAVE_POINT_NUM];
			TmpY_Behind = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx+1)%SAVE_POINT_NUM];
			Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep),(y_axis_offset - TmpY_Behind),(Lcd.MenuBackColor.Colour_Value));
			
			
			TmpY_Front = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx)%SAVE_POINT_NUM];
			TmpY_Behind = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx+1)%SAVE_POINT_NUM];*/

			TmpY_Front = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx)%(graph->DrawPointNum)];
			TmpY_Behind = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx+1)%(graph->DrawPointNum)];
			Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep),(y_axis_offset - TmpY_Behind),(Lcd.MenuBackColor.Colour_Value));
			
			
			TmpY_Front = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx)%(graph->DrawPointNum)];
			TmpY_Behind = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx+1)%(graph->DrawPointNum)];
			
			Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep),(y_axis_offset - TmpY_Behind),(graph->Color.Colour_Value));
	
			TmpX += PointStep;
		}
	/*TmpY_Front = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx)%SAVE_POINT_NUM];
	TmpY_Behind = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx +1)%SAVE_POINT_NUM];
	Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep-2),(y_axis_offset - TmpY_Behind),(Lcd.MenuBackColor.Colour_Value));
	
	TmpY_Front = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx)%SAVE_POINT_NUM];
	TmpY_Behind = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx +1)%SAVE_POINT_NUM];*/

	TmpY_Front = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx)%(graph->DrawPointNum)];
	TmpY_Behind = graph->ChartaBuf.buf[!(graph->ChartaBuf.CurBuff)][((graph->DrawStartPoint) + loopx +1)%(graph->DrawPointNum)];
	Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep-2),(y_axis_offset - TmpY_Behind),(Lcd.MenuBackColor.Colour_Value));
	
	TmpY_Front = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx)%(graph->DrawPointNum)];
	TmpY_Behind = graph->ChartaBuf.buf[(graph->ChartaBuf.CurBuff)][((ReDrawPoint) + loopx +1)%(graph->DrawPointNum)];

	Gui_DrawLine(TmpX,(y_axis_offset - TmpY_Front),(TmpX+PointStep -2),(y_axis_offset - TmpY_Behind),(graph->Color.Colour_Value));
	graph->ChartaBuf.CurBuff = !graph->ChartaBuf.CurBuff;
	
	graph->DrawStartPoint = ReDrawPoint;
}

void PushGraphData(P_S_Graph graph,unsigned short data)
{
	if((0 == graph->DrawPointNum) || (graph->DrawPointNum > SAVE_POINT_NUM))
		{
			graph->DrawPointNum = SAVE_POINT_NUM;
		}
	if((graph->WritePoint) > ((graph->DrawPointNum) -1))
		{
			(graph->WritePoint) = 0;
		}
	graph->buf[(graph->WritePoint)] = data;
	graph->WritePoint++;
}
void TestGraph(void)
{
	unsigned step = 3;
	unsigned char loop30;

	for(loop30 = 0 ; loop30 < 30 ; loop30 ++)
		{
			Lcd.Graph[0].buf[loop30 * step] = loop30 *3;
			Lcd.Graph[1].buf[loop30 * step] = loop30 *3+50;
		}
	for(loop30 = 0 ; loop30 < 30 ; loop30 ++)
		{
			Lcd.Graph[0].buf[loop30 * step + 1] = loop30 *2;
			Lcd.Graph[1].buf[loop30 * step + 1] = loop30 *2 + 20;
		}
	for(loop30 = 0 ; loop30 < 30 ; loop30 ++)
		{
			Lcd.Graph[0].buf[loop30 * step + 2] = 180 -loop30 * 5;
			Lcd.Graph[1].buf[loop30 * step + 2] = 180 -loop30 * 2;
		}
	Lcd.Graph[0].Color.Colour_Com.red = 15;
	Lcd.Graph[0].DrawStartPoint = 0;
	Lcd.Graph[0].DrawPointNum = 60;
	
	Lcd.Graph[1].Color.Colour_Com.blue= 30;
	Lcd.Graph[1].DrawStartPoint = 0;
	Lcd.Graph[1].DrawPointNum = 60;
	
	GUI_InitLinrChart(&(Lcd.Graph[0]));
	GUI_DrawLineChart(&(Lcd.Graph[0]));
	
	GUI_InitLinrChart(&(Lcd.Graph[1]));
	GUI_DrawLineChart(&(Lcd.Graph[1]));

	for(loop30 = 0 ; loop30 < 30 ; loop30 ++)
		{			
			/*Lcd.Graph[0].Color.Colour_Value = Lcd.imageBack_Color.Colour_Value;	
			GUI_DrawLineChart(&(Lcd.Graph[0]),30,80,120,70);
			Lcd.Graph[0].DrawStartPoint++;
			Lcd.Graph[0].Color.Colour_Com.red = 15;

			GUI_DrawLineChart(&(Lcd.Graph[0]),30,80,120,70);
			
			Lcd.Graph[1].Color.Colour_Value = Lcd.imageBack_Color.Colour_Value;	
			GUI_DrawLineChart(&(Lcd.Graph[1]),30,126,120,40);
			Lcd.Graph[1].DrawStartPoint++;
			Lcd.Graph[1].Color.Colour_Com.blue= 30;
			GUI_DrawLineChart(&(Lcd.Graph[1]),30,126,120,40);*/
			GUI_ReDrawLineChart(&(Lcd.Graph[0]),(Lcd.Graph[0].DrawStartPoint + 1));
		}
}



#define LCD_X_ 239
#define LCD_Y_ 134
void Gui_DrawChart(unsigned short *ptr, short *data, unsigned char x_off,
                   unsigned char y_off, unsigned y_size,
                   unsigned short data_length, unsigned short color) {                   
  short max_num = -32767;
  short min_num = 32767;
  short gap;
  unsigned char data_flag = 0;
  unsigned char graph_step;
  unsigned short loopx;
  unsigned char tmp_y1, tmp_y2;
  y_size /= 2;
  for (loopx = 0; loopx < data_length; loopx++) {  //找的极值
    if (max_num < *(data + loopx)) {
      max_num = *(data + loopx);
    }
    if (min_num > *(data + loopx)) {
      min_num = *(data + loopx);
    }
  }
  printf("max = %d min = %d", max_num, min_num);

  if (max_num > -(min_num))  //差值
  {
    gap = max_num;
  } else {
    gap = -min_num;
  }
  printf("gap = %d\r\n", gap);

  graph_step = LCD_X_ / data_length;
  if (!graph_step) {
    graph_step = data_length / LCD_X_;
    data_flag = 1;
  }
  printf("graph_step = %d  data_flag = %d\r\n", graph_step, data_flag);

  if (data_flag)  //数据比图像的x轴长
  {
    printf("数据比图像的x轴长\r\n");
    for (loopx = 0; loopx < LCD_X_ - 1; loopx++) {
      if (*(data + loopx * graph_step) > 0) {
        tmp_y1 = y_size - (*(data + loopx * graph_step)) * y_size / gap;
      } else {
        /* code */
        tmp_y1 = y_size + (-*(data + loopx * graph_step)) * y_size / gap;
      }
      if (*(data + (loopx + 1) * graph_step) > 0) {
        tmp_y2 = y_size - (*(data + (loopx + 1) * graph_step)) * y_size / gap;
      } else {
        /* code */
        tmp_y2 = y_size + (-*(data + (loopx + 1) * graph_step)) * y_size / gap;
      }
      // printf("x1 = %d , y1 = %d ******* x2 = %d , y2 = %d \r\n", loopx,
      // tmp_y1,
      // loopx + 1, tmp_y2);
      Gui_DrawLine( loopx, tmp_y1, loopx + 1, tmp_y2, color);
    }
  }

  else  //图像的x轴比数据长
  {
    printf("图像的x轴比数据长\r\n");
    for (loopx = 0; loopx < (data_length - 1); loopx++) {
      if (*(data + loopx) > 0) {
        tmp_y1 = y_size - (*(data + loopx)) * y_size / gap;
      } else {
        /* code */
        tmp_y1 = y_size + (-*(data + loopx)) * y_size / gap;
      }
      if (*(data + loopx + 1) > 0) {
        tmp_y2 = y_size - (*(data + loopx + 1)) * y_size / gap;
      } else {
        /* code */
        tmp_y2 = y_size + (-*(data + loopx + 1)) * y_size / gap;
      }

      // printf("x1 = %d , y1 = %d ******* x2 = %d , y2 = %d \r\n",
      // loopx * graph_step, tmp_y1, loopx * graph_step + 1, tmp_y2);
      Gui_DrawLine( loopx * graph_step, tmp_y1, (loopx + 1) * graph_step,
                   tmp_y2, color);
    }
  }
 // lcd_draw_picture(0, 0, 320, 240, ptr);
}


void GUI_DrawSmith(void)
{

	Colour str_color,test;
	test.Colour_Com.red = 0;
	test.Colour_Com.green= 0;
	test.Colour_Com.blue= 0;
	Lcd_Clear(test.Colour_Value);
	
	str_color.Colour_Value = 0;
	str_color.Colour_Com.red = 31;
	
	Gui_Circle(100,60,60,str_color.Colour_Value);
	Gui_Circle(110,60,50,str_color.Colour_Value);
	Gui_Circle(120,60,40,str_color.Colour_Value);
	Gui_Circle(130,60,30,str_color.Colour_Value);
	Gui_Circle(140,60,20,str_color.Colour_Value);
	Gui_Circle(150,60,10,str_color.Colour_Value);

	static unsigned char Gx = 15;
	Gui_Circle(160,60+Gx,Gx,str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*2),(Gx*2),str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*4),(Gx*4),str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*8),(Gx*8),str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*16),(Gx*16),str_color.Colour_Value);	

	Y_PIXEL_POLAR = 0;
	Y_PIXEL_OFFSET = 120;
	Gui_Circle(160,60+Gx,Gx,str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*2),(Gx*2),str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*4),(Gx*4),str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*8),(Gx*8),str_color.Colour_Value);
	Gui_Circle(160,60+(Gx*16),(Gx*16),str_color.Colour_Value);

	Y_PIXEL_POLAR = 1;
	Y_PIXEL_OFFSET = 0;

	Gui_DrawLine(0,60,160,60,str_color.Colour_Value);
	Gui_DrawLine(100,0,100,128,str_color.Colour_Value);
}

void test_lcd_g(void)
{
	unsigned char loop32 = 31;
	unsigned char step = 50;
	Colour green;
	green.Colour_Value = 0;
	
	while(loop32)
		{
			green.Colour_Com.green = loop32;
			Gui_DrawLine(10, step, 30, step, green.Colour_Value);
			loop32 --;
			step++;
		}
}

void test_lcd_b(void)
{
	unsigned char loop32 = 63;
	unsigned char step = 50;
	Colour blue;
	blue.Colour_Value = 0;
	
	while(loop32)
		{
			blue.Colour_Com.blue= loop32;
			Gui_DrawLine(31, step, 60, step, blue.Colour_Value);
			loop32 --;
			step++;
		}
}

void test_lcd_r(void)
{
	unsigned char loop32 = 31;
	unsigned char step = 50;
	Colour red;
	red.Colour_Value = 0;
	
	while(loop32)
		{
			red.Colour_Com.red= loop32;
			Gui_DrawLine(61, step, 90, step, red.Colour_Value);
			loop32 --;
			step++;
		}
}

void test_lcd_all_color(void)
{
   unsigned int i,m;
   //unsigned int time_count = 0;
   unsigned char Color = 0;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	LCD_WriteData_16Bit(Color);
		Color++;
    } 

}

void test_lcd_color(void)
{
	test_lcd_g();
	test_lcd_b();
	test_lcd_r();
	test_lcd_all_color();
}

void test_memu(unsigned char label_num,unsigned char isselect)
{
	unsigned short b_color;
	b_color = Lcd.StrBack_Color.Colour_Value;
	if(isselect == 1)
		{
			b_color = Lcd.StrCursorColor.Colour_Value;
		}

	switch(label_num)
		{
			case 1:
				GUI_FillStringAt("设置",0,2,Lcd.StrColor.Colour_Value,b_color,&GUI_FontHZ10x10);
				break;
				
			case 2:
				GUI_FillStringAt("节点",0,60,Lcd.StrColor.Colour_Value,b_color,&GUI_FontHZ10x10);
				break;
				
			case 3:
				GUI_FillStringAt("数据",0,116,Lcd.StrColor.Colour_Value,b_color,&GUI_FontHZ10x10);
				break;
				
			case 4:
				GUI_FillStringAt("通讯",135,2,Lcd.StrColor.Colour_Value,b_color,&GUI_FontHZ10x10);
				break;
				
			case 5:
				GUI_FillStringAt("红外",135,60,Lcd.StrColor.Colour_Value,b_color,&GUI_FontHZ10x10);
				break;
				
			case 6:
				GUI_FillStringAt("扩展",135,116,Lcd.StrColor.Colour_Value,b_color,&GUI_FontHZ10x10);
				break;
				
		}
}

void GUI_MenuChange(Void_Function_Void Menu_Init,Void_Function_Void Menu_Clear,Void_Function_Int Menu_Operat)
{
	Lcd.CurMenu.MenuClear();
	Lcd.CurMenu.MenuClear = Menu_Clear;
	Lcd.CurMenu.MenuInit = Menu_Init;
	Lcd.CurMenu.MenuOperat = Menu_Operat;
	Lcd.CurMenu.MenuInit();
}

void GUI_MenuOperat(int cmd)
{

	switch(cmd)
  		{
  			case (key_press(0)):
				ChangeToIndex(3);
				break;
  			case (key_press(1)):
				ChangeToIndex(2);
				break;
  			case (key_press(2)):
				ChangeToIndex(1);
				break;
  			case (key_press(3)):
				ChangeToIndex(4);
				break;
  			case (key_press(4)):
				ChangeToIndex(5);
				break;
  			case (key_press(5)):
				ChangeToIndex(6);
				break;
		}
	
}



void MainMenuInit(void)
{
	//Lcd_Clear(Lcd.MenuBackColor.Colour_Value);
	GUI_FillStringAt("设置",0,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("节点",0,60,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("数据",0,116,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("通讯",135,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("红外",135,60,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("扩展",135,116,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	//ChangeToIndex(1);
	ChangeToIndex(Lcd.pre_cursor);
}
void MainMenuClear(void)
{
	GUI_FillStringAt("设置",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("节点",0,60,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("数据",0,116,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("通讯",135,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("红外",135,60,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("扩展",135,116,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_ShowGrayImage(MixImage_48_48,58,40,48,48,Lcd.imageBack_Color,Lcd.imageBack_Color);
	Lcd.cursor = 0;
}
void  MainMenuOperat(int cmd)
{
		switch(cmd)
  		{
  			case (key_press(0)):
				ChangeToIndex(3);
				break;
  			case (key_press(1)):
				ChangeToIndex(2);
				break;
  			case (key_press(2)):
				ChangeToIndex(1);
				break;
  			case (key_press(3)):
				ChangeToIndex(4);
				break;
  			case (key_press(4)):
				ChangeToIndex(5);
				break;
  			case (key_press(5)):
				ChangeToIndex(6);
				break;
		}
	
}


void SetBoardCursor(unsigned char NewLable)
{	
	GUI_FillStringAt(">",15,((Lcd.cursor)*10 + 2),Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt(">",15,(NewLable*10 + 2),Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	Lcd.cursor = NewLable;
}

void SetBoardCursorV(unsigned char mod)
{
	if(mod)
		{
			GUI_FillStringAt(">",15,((Lcd.cursor)*10 + 2),Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
		}
	else
		{
			GUI_FillStringAt(">",15,((Lcd.cursor)*10 + 2),Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
		}
}

void SetBoardCursorF(void)
{
	if(Lcd.cursor == 11)
		{
			SetBoardCursor(0);
		}
	else
		{
			SetBoardCursor(Lcd.cursor + 1);
		}
}

void SetBoardCursorB(void)
{
	if(Lcd.cursor == 0)
		{
			SetBoardCursor(11);
		}
	else
		{
			SetBoardCursor(Lcd.cursor -1);
		}
}
void SetBoardCursorOperat(unsigned char cursor)
{

}


static unsigned char IDACmin,IDACmax,IDAC_Cmin,IDAC_Cmax,CLK_min,CLK_max;
static unsigned short WINDOWSmin = 38000,WINDOWSmax = 45000;
void SetBoardMenuInit(void)
{
	unsigned char lable_x_offset = 25;
	unsigned char data_1_x_offset = 80;
	unsigned char data_2_x_offset = 110;
	char BUF[10] = {0};
	GUI_FillStringAt("IDAC:",lable_x_offset,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",IDACmin);
	GUI_FillStringAt(BUF,data_1_x_offset,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",IDACmax);
	GUI_FillStringAt(BUF,data_2_x_offset,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);

	
	GUI_FillStringAt("IDAC_C:",lable_x_offset,12,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",IDAC_Cmin);
	GUI_FillStringAt(BUF,data_1_x_offset,12,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",IDAC_Cmax);
	GUI_FillStringAt(BUF,data_2_x_offset,12,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("CLK_DIV:",lable_x_offset,22,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",CLK_min);
	GUI_FillStringAt(BUF,data_1_x_offset,22,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",CLK_max);
	GUI_FillStringAt(BUF,data_2_x_offset,22,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("Window:",lable_x_offset,32,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%5d",WINDOWSmin);
	GUI_FillStringAt(BUF,data_1_x_offset,32,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%5d",WINDOWSmax);
	GUI_FillStringAt(BUF,data_2_x_offset,32,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("AOTO_SET",lable_x_offset,42,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("BaseLine:",lable_x_offset,52,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",CLK_max);
	GUI_FillStringAt(BUF,data_1_x_offset,52,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("BaseNoize:",lable_x_offset,62,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",CLK_max);
	GUI_FillStringAt(BUF,data_1_x_offset,62,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("UpdateTim:",lable_x_offset,72,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",CLK_max);
	GUI_FillStringAt(BUF,data_1_x_offset,72,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("BaseLineSet",lable_x_offset,82,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("CuruUID:",lable_x_offset,92,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("1A 2C FA CB",data_1_x_offset,92,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("Addr:",lable_x_offset,102,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	sprintf(BUF,"%3d",CLK_max);
	GUI_FillStringAt(BUF,data_1_x_offset,102,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	//GUI_FillStringAt("GetAddr:",30,112,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("SetAddr",lable_x_offset,112,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);

	
	GUI_FillStringAt("...",140,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("+",0,60,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("-",0,116,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("*",0,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
		
	SetBoardCursor(Lcd.cursor);


}

void SetBoardMenuClear(void)
{	
	/*GUI_FillStringAt("IDAC:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("IDAC_C:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("CLK_DIV:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("WINDOW:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("AOTO_SET",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("BaseLne:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("BaseNoize:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("UpdateTim:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("BaseLineSet",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	
	GUI_FillStringAt("CuruUID:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("Addr:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("GetAddr:",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("SetAddr",0,2,Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);*/

	unsigned short str_color = Lcd.StrColor.Colour_Value;
	Lcd.StrColor.Colour_Value = Lcd.StrBack_Color.Colour_Value;
	Lcd.CurMenu.MenuInit();
	GUI_FillStringAt(">",15,((Lcd.cursor)*10 + 2),Lcd.StrBack_Color.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	Lcd.StrColor.Colour_Value = str_color;
}


void ShowBoardSUBCursorValue(unsigned char main_cursor,unsigned char sub_cursor,unsigned char is_choose)
{
	//unsigned char lable_x_offset = 25;
	unsigned char data_1_x_offset = 80;
	unsigned char data_2_x_offset = 110;
	char BUF[10] = {0};

	switch(main_cursor)
		{
			case 0:	
				if(sub_cursor)
					{
						sprintf(BUF,"%3d",IDACmax);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				else
					{
						sprintf(BUF,"%3d",IDACmin);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				
				break;
			case 1:
				if(sub_cursor)
					{
						sprintf(BUF,"%3d",IDAC_Cmax);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				else
					{
						sprintf(BUF,"%3d",IDAC_Cmin);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				break;
			case 2:
				if(sub_cursor)
					{
						sprintf(BUF,"%3d",CLK_max);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				else
					{
						sprintf(BUF,"%3d",CLK_min);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				break;
			case 3:
				if(sub_cursor)
					{
						sprintf(BUF,"%3d",WINDOWSmax);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_2_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				else
					{
						sprintf(BUF,"%3d",WINDOWSmin);
						if(is_choose)
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);								
							}
						else
							{
								GUI_FillStringAt(BUF,data_1_x_offset,2+main_cursor*10,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
							}
					}
				break;
		}
}




void SetValue(unsigned char main_cursor,unsigned char sub_cursor,unsigned char mod)//mod=0 自增 mod=1 不变  mod=2自减 
{
	unsigned char tmp = mod;
	switch(main_cursor)
	{
		case 0:
			if(sub_cursor)
				{
					IDACmax += Step_value;
					IDACmax -= tmp*Step_value;
				}
			else
				{
					IDACmin+= Step_value;
					IDACmin-= tmp*Step_value;
				}
			break;
		case 1:
			if(sub_cursor)
				{
					IDAC_Cmax += Step_value;
					IDAC_Cmax -= tmp*Step_value;
				}
			else
				{
					IDAC_Cmin+= Step_value;
					IDAC_Cmin-= tmp*Step_value;
				}
			break;
		case 2:
			if(sub_cursor)
				{
					CLK_max+= Step_value;
					CLK_max -= tmp*Step_value;
				}
			else
				{
					CLK_min+= Step_value;
					CLK_min-= tmp*Step_value;
				}
			break;
		case 3:
			if(sub_cursor)
				{
					WINDOWSmax+= Step_value;
					WINDOWSmax -= tmp*Step_value;
				}
			else
				{
					WINDOWSmin+= Step_value;
					WINDOWSmin-= tmp*Step_value;
				}
			break;
	}
}

void SetBoardSUBCursorOperat(unsigned char main_cursor,unsigned char sub_cursor,unsigned char mod,unsigned char is_choose)//mod=0 自增 mod=1 不变  mod=2自减 
{
	SetValue(main_cursor,sub_cursor,mod);
	ShowBoardSUBCursorValue(main_cursor,sub_cursor,is_choose);
}

void SetBoardMenuOperat(int cmd)
{
	static unsigned char IsSelect = 0;
	//static unsigned char SUB_Cursor = 0;
	if(key_press(3) == cmd)//...返回
		{
			GUI_MenuChange(MainMenuInit,MainMenuClear,MainMenuOperat);
		}
	
	if(key_press(6) == cmd)
		{
			IsSelect = !IsSelect;
			Step_value=1;
			if(IsSelect)
				{
					SetBoardSUBCursorOperat(Lcd.cursor,Lcd.cursor_sub_1,1,1);//选择菜单
					SetBoardCursorV(0);
				}
			else
				{					
					SetBoardSUBCursorOperat(Lcd.cursor,Lcd.cursor_sub_1,1,0);//选择菜单
					SetBoardCursorV(1);
				}
				
			Lcd.cursor_sub_1 = 0;
			Lcd.cursor_sub_2 = 0;
		}

	
	if(IsSelect)
		{
			switch(cmd)
				{
				case (key_press(0))://-
					//SetValue(Lcd.cursor,Lcd.cursor_sub_1,0,1)
					Step_value/=10;
					if(Step_value < 1)
						{
							Step_value = 1;
						}
					break;
	  			case (key_press(1))://+
	  				Step_value*=10;
					if(Step_value > 1000)
						{
							Step_value = 1000;
						}
					break;
	  			case (key_press(2))://*
					SetBoardSUBCursorOperat(Lcd.cursor,Lcd.cursor_sub_1,1,0);//选择菜单
					Lcd.cursor_sub_1 = !Lcd.cursor_sub_1;
					SetBoardSUBCursorOperat(Lcd.cursor,Lcd.cursor_sub_1,1,1);//选择菜单
					break;
					
				case EC11_TURN_P:
					//SetValue(Lcd.cursor,Lcd.cursor_sub_1,1);
					SetBoardSUBCursorOperat(Lcd.cursor,Lcd.cursor_sub_1,0,1);
					break;
					
				case EC11_TURN_N:					
					SetBoardSUBCursorOperat(Lcd.cursor,Lcd.cursor_sub_1,2,1);
					break;
				}
		}
	else		
		{
			switch(cmd)
				{
					case EC11_TURN_P:
						SetBoardCursorF();//ZHENGZHUAN
						break;
						
					case EC11_TURN_N:
						SetBoardCursorB();//FANZHUAN
						break;
			}
		}
}











///显示波形
void ShowGraphMenuInit(void)
{

	Lcd.Graph[0].Color.Colour_Com.red = 0XF;
	Lcd.Graph[1].Color.Colour_Com.blue= 0XF;
	Lcd.Graph[2].Color.Colour_Com.green= 0X7;
	
	Lcd.Graph[3].Color.Colour_Com.red = 0X7;
	Lcd.Graph[3].Color.Colour_Com.green= 0X3;
	GUI_InitLinrChart(&(Lcd.Graph[0]));
	GUI_DrawLineChart(&(Lcd.Graph[0]));
	
	GUI_InitLinrChart(&(Lcd.Graph[1]));
	GUI_DrawLineChart(&(Lcd.Graph[1]));
	
	GUI_InitLinrChart(&(Lcd.Graph[2]));
	GUI_DrawLineChart(&(Lcd.Graph[2]));
	
	GUI_InitLinrChart(&(Lcd.Graph[3]));
	GUI_DrawLineChart(&(Lcd.Graph[3]));
}

void ShowGraphMenuClear(void)
{
	Lcd.Graph[0].Color.Colour_Value = Lcd.MenuBackColor.Colour_Value;
	Lcd.Graph[1].Color.Colour_Value = Lcd.MenuBackColor.Colour_Value;
	Lcd.Graph[2].Color.Colour_Value = Lcd.MenuBackColor.Colour_Value;
	Lcd.Graph[3].Color.Colour_Value = Lcd.MenuBackColor.Colour_Value;
	
	GUI_InitLinrChart(&(Lcd.Graph[0]));
	GUI_InitLinrChart(&(Lcd.Graph[1]));
	GUI_InitLinrChart(&(Lcd.Graph[2]));
	GUI_InitLinrChart(&(Lcd.Graph[3]));
	
	GUI_ReDrawLineChart(&(Lcd.Graph[0]),(Lcd.Graph[0].WritePoint));
	GUI_ReDrawLineChart(&(Lcd.Graph[1]),(Lcd.Graph[1].WritePoint));
	
	GUI_ReDrawLineChart(&(Lcd.Graph[2]),(Lcd.Graph[0].WritePoint));
	GUI_ReDrawLineChart(&(Lcd.Graph[3]),(Lcd.Graph[1].WritePoint));

}
extern unsigned char sensor_upload_mod;
void ShowGraphMenuOperat(int cmd)
{
	/*GUI_ReDrawLineChart(&(Lcd.Graph[0]),(Lcd.Graph[0].WritePoint));
	GUI_ReDrawLineChart(&(Lcd.Graph[1]),(Lcd.Graph[1].WritePoint));
	
	GUI_ReDrawLineChart(&(Lcd.Graph[2]),(Lcd.Graph[0].WritePoint));
	GUI_ReDrawLineChart(&(Lcd.Graph[3]),(Lcd.Graph[1].WritePoint));*/

	switch(cmd)
		{
			case (key_press(0)):
				sensor_upload_mod = 1;
				break;
			case (key_press(1)):
				sensor_upload_mod = 2;
				break;
			case (key_press(2)):
				sensor_upload_mod = 3;
				break;
			case (key_press(3)):
				sensor_upload_mod = 4;
				break;
			case (key_press(4)):
				sensor_upload_mod = 5;
				break;
			case (key_press(5)):
				break;
			default:
				break;
		}
}





/////////////////网络设置//
//设置can网络 rs485网络的使能 以及极性
static unsigned char CurNet = 0;

#define RS485_PATH_1	1
#define RS485_PATH_2	3
#define CAN_PATH_1	0
#define CAN_PATH_2	2



static unsigned char tmp_cur_net;
void NetSetMenuInit(void)
{
	unsigned char x_offset = 25;
	unsigned char y_offset = 40;

	tmp_cur_net = CurNet;
	switch(CurNet)
		{
			case 0:
				GUI_FillStringAt("NET => CAN Mod A ==x==",x_offset,y_offset,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
				break;
			case 1:
				GUI_FillStringAt("NET => 485 Mod A ==x==",x_offset,y_offset,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
				break;
			case 2:
				GUI_FillStringAt("NET => CAN Mod B =====",x_offset,y_offset,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
				break;
			case 3:
				GUI_FillStringAt("NET => 485 Mod B =====",x_offset,y_offset,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
				break;
			default:
				break;			
		}
	GUI_FillStringAt("Press OK To ChangeMod:",x_offset,y_offset+60,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);

	GUI_FillStringAt("OK",0,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
	GUI_FillStringAt("...",140,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);

}
void NetSetMenuClear(void)
{
	//反向初始化
	unsigned short str_color = Lcd.StrColor.Colour_Value;
	Lcd.StrColor.Colour_Value = Lcd.StrBack_Color.Colour_Value;
	Lcd.CurMenu.MenuInit();
	Lcd.StrColor.Colour_Value = str_color;
}
void NetSetMenuOperat(int cmd)
{
	if(key_press(3) == cmd)//...返回
		{
			GUI_MenuChange(MainMenuInit,MainMenuClear,MainMenuOperat);
		}
	else
		{
			switch(cmd)
				{	
				case(key_press(2))://按下OK
					CurNet = tmp_cur_net;
					Enable_Path(CurNet);
					
					GUI_FillStringAt("OK",0,2,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
					break;
					
				case EC11_TURN_P:
					//正转
					GUI_FillStringAt("OK",0,2,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
					tmp_cur_net++;
					break;
					
				case EC11_TURN_N:		
					//反转
					GUI_FillStringAt("OK",0,2,Lcd.StrCursorColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
					tmp_cur_net--;
					break;
				}
			
			tmp_cur_net = tmp_cur_net%4;
			switch(tmp_cur_net)
				{
					case 0:
						GUI_FillStringAt("NET => CAN Mod A ==x==",25,40,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
						break;
					case 1:
						GUI_FillStringAt("NET => 485 Mod A ==x==",25,40,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
						break;
					case 2:
						GUI_FillStringAt("NET => CAN Mod B =====",25,40,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
						break;
					case 3:
						GUI_FillStringAt("NET => 485 Mod B =====",25,40,Lcd.StrColor.Colour_Value,Lcd.StrBack_Color.Colour_Value,&GUI_FontHZ10x10);
						break;
					default:
						break;
				}
		}
	
	
}


