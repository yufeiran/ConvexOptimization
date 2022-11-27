#include<iostream>
#include<random>
#include"../miniEngine2D/MiniEngine2D.h"
#include"../miniMath/miniMath.h"
#include<vector>
#include<fstream>

bool isDebug = true;

using namespace std;
using namespace mini;

void mCleanScreen(MiniEngine2D& m)
{
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++)
		{
			m.drawPoint(j, i, {255,255,255 });
		}
	}
}

void drawGraph(MiniEngine2D& m, Vec& rawData,Vec&fitData)
{
	double startX = 400, startY = 50;
	double height = 550, width = 550;
	double maxY = 10, maxX = 100;
	int scaleCountY = 5, scaleCountX = 5;
	double stepSizeY = height / scaleCountY, stepSizeX = width / scaleCountX; //每个刻度之间的像素距离

	pair<double, double>ZeroPoint{ startX,startY + height };
	MiniColor lineColor = { 0,0,0 };

	drawLine(startX, startY, startX, startY + height, lineColor);
	drawLine(startX, startY + height, startX + width, startY + height, lineColor);

	auto wordZeroImg = m.makeFontToMiniImage("0.0", 30);
	m.drawImage(ZeroPoint.first, ZeroPoint.second + 10, wordZeroImg);

	for (int i = 1; i <= scaleCountX; i++)
	{
		m.drawLine(startX + i * stepSizeX, ZeroPoint.second, startX + i * stepSizeX, ZeroPoint.second - 10, lineColor);
		char nowScaleSum[30];

		sprintf_s(nowScaleSum, "%.2f", maxX * (double)i / scaleCountX);
		auto wordImg = m.makeFontToMiniImage(nowScaleSum, 30);

		m.drawImage(startX + i * stepSizeX, ZeroPoint.second + 10, wordImg);
	}

	for (int i = 1; i <= scaleCountY; i++)
	{
		m.drawLine(ZeroPoint.first, ZeroPoint.second - i * stepSizeY, ZeroPoint.first + 10, ZeroPoint.second - i * stepSizeY, lineColor);


		char nowScaleSum[30];
		sprintf_s(nowScaleSum, "%.2f", maxY * (double)i / scaleCountY);
		auto wordImg = m.makeFontToMiniImage(nowScaleSum, 30);

		m.drawImage(ZeroPoint.first - 30, ZeroPoint.second - i * stepSizeY, wordImg);
	}

	//开始画数据点
	int index = 0;
	for (int i = 0; i < rawData.n; i++)
	{
		double drawDataY = ZeroPoint.second - rawData[i] / maxY * height;
		double drawDataX = ZeroPoint.first + index / maxX * width;

		double nowPart = (double)index / (double)rawData.n;
		MiniColor drawColor = { 255, (int)nowPart * 215, 255 };
		m.drawCircle(drawDataX, drawDataY, 2, { 0,0,0 });
		index++;
	}

	index = 0;
	for (int i = 0; i < fitData.n; i++)
	{
		double drawDataY = ZeroPoint.second - fitData[i] / maxY * height;
		double drawDataX = ZeroPoint.first + index / maxX * width;

		double nowPart = (double)index / (double)fitData.n;
		MiniColor drawColor = { 255, (int)nowPart * 215, 255 };
		m.drawCircle(drawDataX, drawDataY, 2, { 255,0,0 });
		index++;
	}

}



Vec generateData()
{
	Vec dataVec(100);
	default_random_engine e(time(NULL));
	uniform_int_distribution<unsigned> u(2, 8);
	vector<int>baseVec;
	//生成一百个随机数，其中每20个一组，一组中都是正态分布，每组的平均值为[1,9]的随机数,方差为固定值
	int det = 1;
	for (int i = 0; i < 100; i++)
	{
		normal_distribution<double> nu((double)i*0.1, det);
		double nowData = nu(e);
		dataVec[i] = nowData;
	}
	return dataVec;
}


Vec generateData1()
{
	Vec dataVec(100);
	default_random_engine e(time(NULL));
	uniform_int_distribution<unsigned> u(2, 8);
	vector<int>baseVec;
	//生成一百个随机数，其中每20个一组，一组中都是正态分布，每组的平均值为[1,9]的随机数,方差为固定值
	int det = 1;
	for (int i = 0; i < 5; i++)
	{
		int nowPos = u(e);
		while (find(baseVec.begin(), baseVec.end(), nowPos) != baseVec.end())
		{
			nowPos = u(e);
		}
		baseVec.push_back(nowPos);

		normal_distribution<double> nu(nowPos, det);
		cout << "======================================================" << endl;
		cout << "base:" << nowPos << endl;
		for (int j = 0; j < 20; j++)
		{
			double nowData = nu(e);
			cout << nowData << endl;
			dataVec[i * 20 + j] = nowData;
		}
		cout << "======================================================" << endl;
	}
	return dataVec;
}



//=============================================================================
// 算法实现部分


double SoftThresholdingOne(double bi, double lambda)
{
	if (bi > lambda)
	{
		return bi - lambda;
	}
	else if (bi<= lambda&&bi>=-lambda)
	{
		return 0;
	}
	else if (bi < -lambda)
	{
		return bi + lambda;
	}
}

Vec SoftThresholding(Vec& input, double lambda)
{
	Vec output(input.n);
	for (int i = 0; i < input.n; i++)
	{
		double ans = SoftThresholdingOne(input[i], lambda);
		output[i] = ans;
	}
	return output;
}


//算法迭代部分
Vec iterOne(Vec &b,double t,Mat X,Vec y,double lambda)
{

	if (isDebug)
	{
		cout << "X:" << endl;
		cout << X << endl;
		auto part1 = (y - X * b);
		cout << "(y - X * b) = " << endl << part1 << endl;
		auto part2 = t * transposeMat(X) * part1;
		cout << "t * transposeMat(X) * (y - X * b) = " << endl << part2 << endl;
		auto part3 = b + part2;
		cout << "b + t * transposeMat(X) * (y - X * b) = " << endl << part3 << endl;

	}

	auto iter = b + t * transposeMat(X) * (y - X * b);


	Vec newb = SoftThresholding(iter,lambda);

	if (isDebug)
	{
		cout << "SoftThresholding(b + t * transposeMat(X) * (y - X * b) = " << endl << newb << endl;
	}


	return newb;
}




//=============================================================================

int main()
{
	MiniEngine2D m;
	

	cout << time << endl;

	m.showWindow();
	m.setTitle("ProximalGradientDescent");

	bool isGameOver = false;

	auto dataVec = generateData();

	ofstream os("data.txt");

	for (int i = 0; i < dataVec.n; i++)
	{
		os << dataVec[i] << endl;
	}
	os.close();


	Mat X(100, 1);
	for (int i = 0; i < 100; i++)
	{
		//X.d(i, i) = i + 1;
		X.d(i, 0) = i+1;
	}

	Vec y = dataVec;
	Vec b(1);
	b[0] = 0.002;

	double lambda = 0.002;
	double t = 0.000005;

	int runtime = 0;
	int stepTime = 0;
	Vec ansData(100);
	while (isGameOver != true) {
		mCleanScreen(m);

		for (int i = 0; i < 100; i++) {
			ansData[i] = b[0] * (double)(i + 1);
		}

		drawGraph(m, dataVec, ansData);
		if (m.isKeyDown('S') == true||m.isKeyDown(VK_UP))
		{
			runtime++;
			
		}
		if (m.isKeyDown('M') == true||m.isKeyDown(VK_DOWN))
		{
			runtime += 10;
		}
		if (m.isKeyDown('B') == true||m.isKeyDown(VK_RIGHT))
		{
			runtime += 100;
		}

		if (runtime > 0)
		{
			b = iterOne(b, t, X, y, lambda);
			runtime--;
			stepTime++;
			cout << "stepTime " << stepTime << endl;
		}
		m.update();
	}

	return 0;
}