#pragma once
#include"MiniEngine2D.h"
void showWindow();





extern MiniColor BUFFER[SCREEN_HEIGHT* SCREEN_WIDTH];



void update();

void putBufferToScreen();

void clearScreen();

void drawPoint(int x,int y,const MiniColor &);
void drawLine(int x0, int y0, int x1, int y1, const MiniColor&);
void drawRect(int x0, int y0, int width, int height, const MiniColor&);
void drawCircle(int x, int y, double r, const MiniColor&);
void drawImage(int x, int y, const MiniImage&);
MiniImage makeFontToMiniImage(std::string str,int size);
void setFont(const std::string & fontName, int size);
void setTitle(const char* title);