#include"2dFusedLasso.h"
#include<iostream>
using namespace std;

int main()
{
	MiniEngine2D m;

	m.showWindow();
	m.setTitle("2d fused lasso");

	bool isGameOVer = false;
	MiniImage imageRaw;
	MiniImage imageAim;
	MiniImage imageProcess;
	imageRaw.load("raw.jpg");
	imageAim.load("aim.png");

	imageProcess.makeRectImage(imageRaw.width, imageRaw.height, { 0,0,0 });

	//imageProcess = imageRaw;

	m.setFont("black", 50);


	MiniImage wordRaw = m.makeFontToMiniImage("Raw", 50);
	MiniImage wordAim = m.makeFontToMiniImage("Aim", 50);
	MiniImage wordProcess = m.makeFontToMiniImage("Process", 50);
	
	


	int startBaseX = 30;
	int startBaseY = 50;
	int paddingX = 30;
	int paddingY = 55;
	while (!isGameOVer)
	{
		m.clearScreen();

		m.drawImage(startBaseX, startBaseY, imageRaw);
		m.drawImage(startBaseX, startBaseY- paddingY, wordRaw);

		m.drawImage(startBaseX + imageRaw.width + paddingX, startBaseY, imageProcess);
		m.drawImage(startBaseX + imageRaw.width + paddingX, startBaseY - paddingY, wordProcess);

		m.drawImage(startBaseX+imageRaw.width*2+ paddingX *2, startBaseY, imageAim);
		m.drawImage(startBaseX + imageRaw.width*2+ paddingX *2, startBaseY - paddingY, wordAim);

		
		m.update();
	}

	return 0;
}