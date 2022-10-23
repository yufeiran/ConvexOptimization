#include"InteriorPointMethod.h"
#include<vector>
#include<string>
#include<random>

using namespace mini;
using namespace std;


struct Point
{

	double x, y;
	int index;
	Point() :index(0), x(0), y(0) {};
	Point(double X, double Y) :x(X), y(Y), index(0) {};
	Point(double X, double Y, int Index) :x(X), y(Y), index(Index) {};
};


Mat CalHessian(const Vec& x, const Mat& A, const Vec& b)
{

	auto AT = transposeMat(A);

	Mat D(A.m, A.m);

	for (int i = 0; i < A.m; i++)
	{
		Mat ARol(1,A.n); //获取A中第i行组成的矩阵
		for (int j = 0; j < A.n; j++)
		{
			ARol.d(0, j) = A.d(i, j);
		}

		D.d(i, i) = 1.0 / pow(((ARol * x).value() - b[i]), 2);
	}

	//cout << "AT=" << endl;
	//cout << AT << endl;


	//cout << "D=" << endl;
	//cout << D << endl;

	//cout << "A=" << endl;
	//cout << A << endl;

	Mat Hessian = AT * D * A;
	//cout << "Hessian=" << endl;
	//cout << Hessian << endl;

	return Hessian;

}

Vec CalOneTime(const Vec& oldX, const Mat& A, const Vec& b,const Vec&dx)
{
	// 迭代公式为
	//  (n+1)     (n)               (n)  -1      (n)
	// x       = x      -  Hessian(x   )   * df(x    ) ,n >=0
	//  
	//（*这里面的x是向量，比如二维坐标x和y都是这里的x向量

	//                                T
	//其中这里的Hessian矩阵可以分解为A * D *A
	// A是约束矩阵，D是一个对角矩阵,具体推导请看以下链接
	// https://yufeiran.com/zui-you-hua-suan-fa-shi-xian-nei-dian-fa/

	Vec newX(2);

	auto Hessian = CalHessian(oldX, A, b);
	auto HessianInv = inverseMat(Hessian);


	//cout << "HessianInv=" << endl;
	//cout << HessianInv << endl;

	newX = oldX - HessianInv * dx;


	return newX;
}

vector<Point>runNTimes(int n, const Vec& initPoint, const Mat& A, const Vec& b, const Vec& dx)
{
	vector<Point>ansList;
	Vec oldX = initPoint;
	int nowPointIndex = 0;
	ansList.push_back({ oldX[0],oldX[1],nowPointIndex });
	nowPointIndex++;
	for (int i = 0; i < n; i++)
	{
		oldX = CalOneTime(oldX, A, b, dx);
		ansList.push_back({ oldX[0],oldX[1],nowPointIndex });
		nowPointIndex++;
	}
	return ansList;
}

void drawGraph(MiniEngine2D& m, const vector<Point>& ansList)
{
	double startX = 400, startY = 50;
	double height = 550, width = 550;
	double maxY = 0.5,  maxX = 0.5;
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

	//开始画数据点
	for (const auto& p : ansList)
	{

		double drawDataY = ZeroPoint.second - p.y / maxY * height;
		double drawDataX = ZeroPoint.first + p.x / maxX * width;

		double nowPart = (double)p.index / (double)ansList.size();
		MiniColor drawColor = { 255, (int)nowPart * 215, 255 };
		m.drawPoint(drawDataX, drawDataY, drawColor);
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

Vec generateRandomStartPos(const Mat&A,const Vec&b)
{
	default_random_engine e(time(0));
	uniform_real_distribution<double> u(0, 1);
	Vec nowPos(2);
	while (true)
	{
		nowPos[0] = u(e); nowPos[1] = u(e);//随机生成一个点
		//判断是否在约束面内
		auto ans= A* nowPos - b;
		bool allNeg = true;
		for (int i = 0; i < ans.n; i++)
		{
			if (ans[i] > 0) {
				allNeg = false;
				break;
			}
		}
		if (allNeg == true)break;
		
	}
	return nowPos;
}

int main()
{


	Vec v = { 7,8,9,10 };
	Mat m1(4, 4);
	m1.d(0, 0) = 1; m1.d(1, 1) = 2; m1.d(2, 2) = 3; m1.d(3, 3) = 4;

	cout << "v ="<<endl << v << endl;
	cout << "m ="<<endl << m1 << endl;
	auto ansV = m1 * v;
	cout << "m*v = "<<endl << ansV << endl;

	Mat A(4, 2); Vec b(4);

	A.d(0, 0) = 1; A.d(0, 1) = 2;
	A.d(1, 0) = 2; A.d(1, 1) = 1;
	A.d(2, 0) = -1; A.d(2, 1) = 0;
	A.d(3, 0) = 0; A.d(3, 1) = -1;
	b[0] = 1; b[1] = 1; b[2] = 0; b[3] = 0;



	//Vec oldX = { 0.3,0.2 };
	Vec oldX = generateRandomStartPos(A, b);
	Vec dx = { .1,.1 };
	auto ansList = runNTimes(3000, oldX, A, b, dx);
	//cout << "==========now point Pos=============" << endl;
	//cout << oldX << endl;

	//int runTime = 1;
	//while (true)
	//{
	//	oldX = CalOneTime(oldX, A, b, dx);

	//	runTime--;
	//	if (runTime == 0) {
	//		char c;
	//		cin >> c;
	//		if (c == 'e')break;
	//		if (c == 's') {
	//			runTime = 10;
	//		}
	//		if (c == 'b') {
	//			runTime = 100;
	//		}
	//		else {
	//			runTime = 1;
	//		}
	//	}

	//	cout << "==========now point Pos=============" << endl;
	//	cout << oldX << endl;

	//}

	MiniEngine2D m;
	m.showWindow();
	m.setTitle("InteriorPointMethod");




	bool isGameOver = false;
	while (!isGameOver)
	{
		//m.clearScreen();
		myCleanScreen(m);
		drawGraph(m,ansList);

		m.update();
	}

	return 0;
}