#include"InteriorPointMethod.h"
#include<vector>
#include<string>

using namespace mini;
using namespace std;


void drawGraph(MiniEngine2D& m)
{
	double startX = 100, startY = 50;
	double height = 550, width = 900;
	double maxY = 1,  maxX = 1;
	int scaleCountY = 5,scaleCountX=5;
	double stepSizeY = height / scaleCountY, stepSizeX = width / scaleCountX; //每个刻度之间的像素距离

	pair<double, double>ZeroPoint{ startX,startY + height };
	MiniColor lineColor = { 0,0,0 };

	drawLine(startX, startY, startX, startY + height, lineColor);
	drawLine(startX, startY + height, startX + width, startY + height, lineColor);

	auto wordZeroImg = m.makeFontToMiniImage("0.0", 30);
	m.drawImage(ZeroPoint.first, ZeroPoint.second + 10, wordZeroImg);

	for (int i = 1; i < scaleCountX; i++)
	{
		m.drawLine(startX + i * stepSizeX, ZeroPoint.second, startX + i * stepSizeX, ZeroPoint.second - 10, lineColor);
		char nowScaleSum[30];

		sprintf_s(nowScaleSum, "%.2f", maxX * (double)i / scaleCountX);
		auto wordImg = m.makeFontToMiniImage(nowScaleSum, 30);

		m.drawImage(startX + i * stepSizeX, ZeroPoint.second + 10, wordImg);
	}
	
	for (int i = 1; i < scaleCountY; i++)
	{
		m.drawLine(ZeroPoint.first, ZeroPoint.second - i * stepSizeY, ZeroPoint.first + 10, ZeroPoint.second - i * stepSizeY, lineColor);
		

		char nowScaleSum[30];
		sprintf_s(nowScaleSum, "%.2f", maxY * (double)i / scaleCountY);
		auto wordImg = m.makeFontToMiniImage(nowScaleSum, 30);

		m.drawImage(ZeroPoint.first - 30, ZeroPoint.second - i * stepSizeY, wordImg);
	}




}

void myCleanScreen(MiniEngine2D& m)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			double nowR = 185.0 * (double)y / (double)SCREEN_HEIGHT;
			m.drawPoint(x, y, { (int)nowR,136,125 });
		}
	}
}

int main()
{
	MiniEngine2D m;
	m.showWindow();
	m.setTitle("InteriorPointMethod");


	bool isGameOver = false;
	while (!isGameOver)
	{
		//m.clearScreen();
		myCleanScreen(m);
		drawGraph(m);

		m.update();
	}

	return 0;
}