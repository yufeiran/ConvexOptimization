#include"../miniEngine2D/MiniEngine2D.h"
#include"../miniMath/miniMath.h"
#include<random>
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
			m.drawPoint(j, i, { 255,255,255 });
		}
	}
}

struct Graph
{
	double startX, startY;
	double height, width;
	double maxY, maxX;
	int scaleCountY, scaleCountX;
	Graph(double StartX=0, double StartY=200, double Height=200, double Width=300, double MaxY=10, double MaxX=10, int ScaleCountY=5, int ScaleCountX=5) :
		startX(StartX), startY(StartY), height(Height), width(Width), maxY(MaxY), maxX(MaxX), scaleCountY(ScaleCountY), scaleCountX(ScaleCountX) {};

};

void drawGraphBase(MiniEngine2D& m,const Graph& g)
{
	double stepSizeY = g.height / g.scaleCountY, stepSizeX = g.width / g.scaleCountX; //每个刻度之间的像素距离

	pair<double, double>ZeroPoint{ g.startX,g.startY + g.height };
	MiniColor lineColor = { 0,0,0 };

	drawLine(g.startX, g.startY, g.startX, g.startY + g.height, lineColor);
	drawLine(g.startX, g.startY + g.height, g.startX + g.width, g.startY + g.height, lineColor);

	auto wordZeroImg = m.makeFontToMiniImage("0.0", 30);
	m.drawImage(ZeroPoint.first, ZeroPoint.second + 10, wordZeroImg);

	for (int i = 1; i <= g.scaleCountX; i++)
	{
		m.drawLine(g.startX + i * stepSizeX, ZeroPoint.second, g.startX + i * stepSizeX, ZeroPoint.second - 10, lineColor);
		char nowScaleSum[30];

		sprintf_s(nowScaleSum, "%.2f", g.maxX * (double)i / g.scaleCountX);
		auto wordImg = m.makeFontToMiniImage(nowScaleSum, 30);

		m.drawImage(g.startX + i * stepSizeX, ZeroPoint.second + 10, wordImg);
	}

	for (int i = 1; i <= g.scaleCountY; i++)
	{
		m.drawLine(ZeroPoint.first, ZeroPoint.second - i * stepSizeY, ZeroPoint.first + 10, ZeroPoint.second - i * stepSizeY, lineColor);


		char nowScaleSum[30];
		sprintf_s(nowScaleSum, "%.2f", g.maxY * (double)i / g.scaleCountY);
		auto wordImg = m.makeFontToMiniImage(nowScaleSum, 30);

		m.drawImage(ZeroPoint.first - 30, ZeroPoint.second - i * stepSizeY, wordImg);
	}
}


void drawGraphData(MiniEngine2D& m, const Graph& g,Vec& data, MiniColor color={0,0,0})
{
	pair<double, double>ZeroPoint{ g.startX,g.startY + g.height };
	//开始画数据点
	int index = 0;
	for (int i = 0; i < data.n; i++)
	{
		double drawDataY = ZeroPoint.second - data[i] / g.maxY * g.height;
		double drawDataX = ZeroPoint.first + index / g.maxX * g.width;

		double nowPart = (double)index / (double)data.n;
		m.drawCircle(drawDataX, drawDataY, 2, color);
		index++;
	}
}

void drawGraphDataLine(MiniEngine2D& m, const Graph& g, Vec& data, MiniColor lineColor = { 0,0,0 })
{
	pair<double, double>ZeroPoint{ g.startX,g.startY + g.height };
	//开始画数据点
	int index = 0;
	for (int i = 0; i < data.n-1; i++)
	{
		double drawDataY0 = ZeroPoint.second - data[i] / g.maxY * g.height;
		double drawDataX0 = ZeroPoint.first + index / g.maxX * g.width;

		double drawDataY1 = ZeroPoint.second - data[i+1] / g.maxY * g.height;
		double drawDataX1 = ZeroPoint.first + (index+1) / g.maxX * g.width;

		m.drawLine(drawDataX0, drawDataY0, drawDataX1, drawDataY1, lineColor);
		index++;
	}
}


Vec generateData(int n,double max)
{
	int groupCount = n / 20;
	Vec dataVec(groupCount*20);
	default_random_engine e(time(NULL));
	uniform_int_distribution<unsigned> u(max*0.2, max*0.8);
	vector<int>baseVec;
	//生成一百个随机数，其中每20个一组，一组中都是正态分布，每组的平均值为[1,9]的随机数,方差为固定值
	int det = 1;

	for (int i = 0; i < groupCount; i++)
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


double SoftThresholdingOne(double bi, double lambda)
{
	if (bi > lambda)
	{
		return bi - lambda;
	}
	else if (bi <= lambda && bi >= -lambda)
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


//=================================================================
//ADMM算法实现部分
int n = 100;

Mat A;
Vec x;
Vec b;
Mat B;
Vec z;
Vec w;
double lambda=5;
double rho=10;

Mat AT;
Mat BT;

//这个part1 是 迭代x时用的:iterX()
Mat part1Inv;

void init1DFussedLassoData()
{
	A = MakeIdentityMat(n);
	int dataType = 1;
	if (dataType==0)
	{
		b = generateData(n,1);
	}
	else if (dataType == 1)
	{
		b = Vec(n);
		ifstream infile("data.txt");
		for (int i = 0; i < n; i++)
		{
			double nowData;
			infile >> nowData;
			nowData *= 10;
			b[i] = nowData;
		}
		cout<<"b is \n" << b << endl;
	}

	B = Mat(n, n);
	for (int i = 0; i < n-1; i++)
	{
		B.d(i, i) = -1;
		int nextI = (i + 1) % n;
		B.d(i, nextI) = 1;
	}
	x = b;

	z = Vec(n);
	for (int i = 0; i < n; i++)
	{
		z[i] = 0;
	}
	w = Vec(n);
	for (int i = 0; i < n; i++)
	{
		w[i] = 0;
	}


	AT = transposeMat(A);
	BT = transposeMat(B);
	auto part1 = AT * A + (rho * BT * B);
	//part1Inv = inverseMat(part1);
	part1Inv = inverseMatLU_decomposition(part1);

}

/*
	公式推导请参考:https://yufeiran.com/zui-you-hua-suan-fa-c-shi-xian-2/
	 t+1	T     T  -1   T      T   t     t
	X    =(A A+ρB B)   [A b+ρ B (Z   - w  )
	   part1              part2

	ρ=rho
*/



Vec iterX()
{
	Vec newX(x.n);

	auto part2 = AT * b + rho * B * (z - w);
	newX = part1Inv * part2;
	return newX;
}

/*
     t+1         t+1    t
	z    =soft(Bx    + w , λ/ρ)
	             part1

*/

Vec iterZ()
{
	Vec newZ(z.n);

	auto part1 = B * x + w;
	auto part2 = lambda / rho;

	newZ = SoftThresholding(part1, part2);

	return newZ;
}

Vec iterW(const Vec& w)
{
	Vec newW(w.n);
	newW = w + B * x - z;
	return newW;
}


//总体的迭代一次的函数
void iterOne()
{
	x = iterX();
	z = iterZ();
	w = iterW(w);
}

//=================================================================

int main()
{
	MiniEngine2D m;

	m.showWindow();
	m.setTitle("1dFussedLassoUseADMM");

	bool isGameOver = false;
	init1DFussedLassoData();


	cout << b << endl;

	Graph g(400, 50, 550, 550, 10, 100, 5, 5);

	int iterTime = 0;
	int iterCount = 0;

	while (!isGameOver)
	{
		mCleanScreen(m);


		drawGraphBase(m, g);
		drawGraphData(m, g, b);
		drawGraphDataLine(m, g, x, { 0,0,255 });

		if (m.isKeyDown('S'))
		{
			iterTime++;
		}
		if (m.isKeyDown('D'))
		{
			iterTime += 10;
		}
		if (m.isKeyDown('Z'))
		{
			iterTime += 1000;
		}
		if (m.isKeyDown('X'))
		{
			iterTime += 10000;
		}
		if (iterTime > 0)
		{
			//cout << "iterOne!" << endl;
			iterOne();
			iterTime--;
			iterCount++;
		}
		char title[20];
		sprintf_s(title, "%d", iterCount);

		m.setTitle(title);
		m.update();

	}

	return 0;
}