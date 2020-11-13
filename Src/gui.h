#ifndef __GUI_H__
#define __GUI_H__
#include "include.h"
#include "font.h"


#define SAVE_POINT_NUM	100
#define MAX_GRAPH_CHANNAL		4


typedef struct
{
	unsigned char CurBuff;
	unsigned char buf[2][SAVE_POINT_NUM];
}S_LineChartBuf,*P_S_LineChartBuf;


typedef struct
{
	
	unsigned char WritePoint;
	Colour Color;
	Colour Back_Color;

	unsigned char X_Pos;
	unsigned char Y_Pos;
	
	unsigned char X_Size;
	unsigned char Y_Size;

	unsigned char DrawStartPoint;
	unsigned char DrawPointNum;

	unsigned short Max;
	unsigned short Min;
	unsigned short Range;
	
	unsigned short buf[SAVE_POINT_NUM];
	S_LineChartBuf ChartaBuf;
}S_Graph,*P_S_Graph;


typedef struct 
{
	Void_Function_Void MenuInit;
	Void_Function_Int MenuOperat;
	Void_Function_Void MenuClear;
}S_MenuOperatApi,*P_S_MenuOperatApi;

typedef struct
{
	//unsigned char Buff[20][128];
	unsigned char status;

	Colour MenuBackColor;
	
	Colour ImageColor;
	Colour imageBack_Color;
	
	Colour StrColor;
	Colour StrBack_Color;
	Colour StrCursorColor;
	

	unsigned char pre_cursor;
	unsigned char cursor;
	unsigned char cursor_sub_1;
	unsigned char cursor_sub_2;

	S_Graph Graph[MAX_GRAPH_CHANNAL];

	S_MenuOperatApi CurMenu;
}LCDStatus,*P_LCDStatus;
extern LCDStatus Lcd;

void Gui_DrawLine(unsigned short x0, unsigned short  y0,unsigned short  x1, unsigned short  y1,unsigned short Color) ;
void Gui_Circle(unsigned short X,unsigned short Y,unsigned short R,unsigned short fc) ;

void GUI_FillStringAt(char* str,unsigned short x,unsigned short y,unsigned short str_color,unsigned short back_color,const GUI_FONT* font);
void GUI_DrawStringAt(char* str,unsigned short x,unsigned short y,unsigned short str_color,const GUI_FONT* font);

void PushGraphData(P_S_Graph graph,unsigned short data);
void test_memu(unsigned char label_num,unsigned char isselect);
void GUI_MenuChange(Void_Function_Void Menu_Init,Void_Function_Void Menu_Clear,Void_Function_Int Menu_Operat);



void GUI_InitLinrChart(P_S_Graph graph);
void GUI_DrawLineChart(P_S_Graph graph);
void GUI_ReDrawLineChart(P_S_Graph graph,unsigned char ReDrawPoint);


void GUI_MenuOperat(int cmd);
void  MainMenuOperat(int cmd);
void MainMenuInit(void);
void MainMenuClear(void);




void SetBoardMenuInit(void);
void SetBoardMenuClear(void);
void SetBoardMenuOperat(int cmd);


void ShowGraphMenuInit(void);
void ShowGraphMenuClear(void);
void ShowGraphMenuOperat(int cmd);


void NetSetMenuInit(void);
void NetSetMenuClear(void);
void NetSetMenuOperat(int cmd);
#endif

