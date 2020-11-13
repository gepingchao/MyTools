#ifndef __IMAGE_H__
#define __IMAGE_H__
#include "include.h"
void ChangeIndexICO(unsigned char curICO,unsigned char nextICO,unsigned char mod);
void ChangeToIndex(unsigned char index);
void GUI_ShowGrayImage(const unsigned char *p,unsigned short x,unsigned char y,unsigned short x_size,unsigned short y_size,Colour image_color,Colour back_color);

extern unsigned char MixImage_48_48[288];

#endif

